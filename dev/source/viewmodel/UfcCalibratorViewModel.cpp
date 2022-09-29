/*
* COPYRIGHT NOTICE, DISCLAIMER, and LICENSE:
*
* 
* For the purposes of this copyright and license, "Contributing Authors"
* is defined as the following set of individuals:
*
*    Carlos Augusto Dietrich (cadietrich@gmail.com)
*
* This library is supplied "AS IS".  The Contributing Authors disclaim 
* all warranties, expressed or implied, including, without limitation, 
* the warranties of merchantability and of fitness for any purpose. 
* The Contributing Authors assume no liability for direct, indirect, 
* incidental, special, exemplary, or consequential damages, which may 
* result from the use of the this library, even if advised of the 
* possibility of such damage.
*
* Permission is hereby granted to use, copy, modify, and distribute this
* source code, or portions hereof, for any purpose, without fee, subject
* to the following restrictions:
*
* 1. The origin of this source code must not be misrepresented.
*
* 2. Altered versions must be plainly marked as such and must not be 
*    misrepresented as being the original source.
*
* 3. This Copyright notice may not be removed or altered from any source 
*    or altered source distribution.
*
* The Contributing Authors specifically permit, without fee, and 
* encourage the use of this source code as a component in commercial 
* products. If you use this source code in a product, acknowledgment 
* is not required but would be appreciated.
*
* 
* "Software is a process, it's never finished, it's always evolving. 
* That's its nature. We know our software sucks. But it's shipping! 
* Next time we'll do better, but even then it will be shitty. 
* The only software that's perfect is one you're dreaming about. 
* Real software crashes, loses data, is hard to learn and hard to use. 
* But it's a process. We'll make it less shitty. Just watch!"
*/

// boost/asio.hpp
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <future>

#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/error/en.h>
#include <rapidjson/stringbuffer.h>
#include <boost/lexical_cast.hpp>
#include <boost/filesystem.hpp>
#include <boost/circular_buffer.hpp>
#include <boost/asio.hpp>

#include <Timestamp.h>
#include <FileHelper.h>
#include <UnitConversion.h>
#include <PngWrapper.h>
#include <CurveResampling.h>

#include "../StringResource.h"
#include "../ObjectType.h"

#include "UfcCalibratorViewModel.h"

SINGLETON_DEFINITION(CUfcCalibratorViewModel)

template < >
bool CUfcCalibratorViewModel::GetAttribute(OBJECT_TYPE objectType) const
{
    bool attribute = false;

    switch (objectType) {
    case EXTRINSIC_CALIBRATION_EDITING:
    case TRACKING_DATA_TAB_ITEM:
    case GUI_EXPERT_MODE:
    {
        std::map<int, boost::any>::const_iterator objectTypeToValueIterator = m_objectTypeToValueMap.find(objectType);

        if (objectTypeToValueIterator != m_objectTypeToValueMap.end())
        {
            HEALTH_CHECK(objectTypeToValueIterator->second.type() != typeid(bool), false);

            attribute = boost::any_cast<bool>(objectTypeToValueIterator->second);
        }
    }
    break;

    default:
        LOG_ERROR();
    }

    return attribute;
}

template < >
int CUfcCalibratorViewModel::GetAttribute(OBJECT_TYPE objectType) const
{
    int attribute = my::Null<int>();

    switch (objectType) {
    case GUI_FIELD_LAYOUT_LINE_THICKNESS_ITEM:
    case GUI_LABEL_POSITION_ITEM:
    case GUI_REFRESH_RATE:
    case GUI_CONTENTS_REFRESH_RATE:
    case GUI_SETTINGS_DOCK_MINIMUM_SIZE:
    case GUI_PLAY_KEY:
    case GUI_PREVIOUS_FRAME_KEY:
    case GUI_NEXT_FRAME_KEY:
    case GUI_OCTAGON_SIZE:
    case GUI_TEXT_SIZE_ITEM:
    {
        std::map<int, boost::any>::const_iterator objectTypeToValueIterator = m_objectTypeToValueMap.find(objectType);

        if (objectTypeToValueIterator != m_objectTypeToValueMap.end())
        {
            HEALTH_CHECK(objectTypeToValueIterator->second.type() != typeid(int), my::Null<int>());

            attribute = boost::any_cast<int>(objectTypeToValueIterator->second);
        }
    }
    break;
    default:
        LOG_ERROR();
    }

    return attribute;
}

template < >
std::string CUfcCalibratorViewModel::GetAttribute(OBJECT_TYPE objectType) const
{
    std::string attribute = my::Null<std::string>();

    switch (objectType) {
    case DATABASE_SETTINGS_CAMERA_ID_FORMAT:
    {
        std::map<int, boost::any>::const_iterator objectTypeToValueIterator = m_objectTypeToValueMap.find(objectType);

        if (objectTypeToValueIterator != m_objectTypeToValueMap.end())
        {
            if (objectTypeToValueIterator->second.type() != typeid(std::string))
            {
                LOG_ERROR();

                return my::Null<std::string>();
            }

            attribute = boost::any_cast<std::string>(objectTypeToValueIterator->second);
        }
    }
    break;

    default:
        LOG_ERROR();
    }

    return attribute;
}

/**
*/
CUfcCalibratorViewModel::CUfcCalibratorViewModel()
{
    Create();
}

/**
*/
CUfcCalibratorViewModel::~CUfcCalibratorViewModel()
{
    // BUG: (15-Jul-2017) IT SHOULD BE THE LAST ONE TO BE CALLED, BUT NOT FROM THE CLASS DESTRUCTOR, SINCE IT'S A SINGLETON AND MAY BE DEAD BEFORE OTHER SIGLETONS, LIKE LOGGER
    //if (!SaveUserSettings())
    //{
    //    // TRICKY: (04-AUG-2016) DO NOT CALL LOGGER (IT IS A SINGLETON, IT MIGHT BE DEAD)
    //    //LOG_ERROR();

    //    return /*false*/;
    //}

    //if (!CloseGame())
    //{
    //    // TRICKY: (04-AUG-2016) DO NOT CALL LOGGER (IT IS A SINGLETON, IT MIGHT BE DEAD)
    //    //LOG_ERROR();

    //    return /*false*/;
    //}

    ClosePlay();
}

// DEPRECATED: (23-Oct-2016) REPLACE BY DEPENDENCY INJECTION
void CUfcCalibratorViewModel::SetMessageQueue(boost::shared_ptr<CMessageQueue> messageQueue)
{
    m_messageQueue = messageQueue;
}

/**
*/
boost::shared_ptr<CUfcCalibratorModel> CUfcCalibratorViewModel::GetUfcCalibratorModel()
{
    return m_ufcCalibratorModel;
}

// OPEN PLAY FROM IMAGE, IMAGE SEQUENCE OR VIDEO
// 1. SAVE SETTINGS OF THE CURRENT PLAY
// 2. CLEAR CURRENT PLAY (METADATA AND UNDERLYING DATA)
// 3. STOP CONTINUOUS CALIBRATION
// 4. LOAD VIDEO
// 5. LOAD CURRENT PLAY SETTINGS
bool CUfcCalibratorViewModel::OpenFromUrl(std::string url, bool createPlay)
{
    HEALTH_CHECK(url.empty(), false);

    HEALTH_CHECK(!m_ufcCalibratorModel, false);

    // DEBUG ONLY! (11-Mar-2015) ERROR REPORTING
    LOG_MESSAGE("LOADING PLAY: " + url);

    if (!createPlay)
    {
        boost::shared_ptr<CFootage> footage = m_ufcCalibratorModel->GetFootage();

        HEALTH_CHECK(!footage, false);

        if (!footage->OpenUrl(url))
        {
            LOG_ERROR();

            return false;
        }
    }
    else
    {
        if (!ClosePlay())
        {
            LOG_ERROR();

            return false;
        }

        if (!m_ufcCalibratorModel->SetFootage(url))
        {
            LOG_ERROR();

            return false;
        }

        // (BEGIN OF) DEBUG ONLY! (20-Nov-2015) DEFAULT CAMERA?
        boost::shared_ptr<CPinholeCamera2> pinholeCamera(new CPinholeCamera2);

        HEALTH_CHECK(!pinholeCamera, false);

        double opticalCenter[3] = { 0.0, 400.0, 30.0 },
            center[3] = { 0, 0, 0 },
            up[3] = { 0.0, 0.0, 1.0 };

        // TRICKY: (21-Nov-2015) IS THERE A WAY TO GET THE VIEWPORT PARAMETERS?
        pinholeCamera->Create(opticalCenter, center, up, 2.0, 1.0, 1000.0, 1280, 720);
        // (END OF) DEBUG ONLY! (20-Nov-2015) DEFAULT CAMERA?

        boost::shared_ptr<CFootage> footage = m_ufcCalibratorModel->GetFootage();

        HEALTH_CHECK(!footage, false);

        footage->SetPinholeCamera(pinholeCamera);

        std::vector<std::string> directoryNameArray = my::GetDirectoryNameArray(url);

        std::string auditingId;

        for (auto directoryName : directoryNameArray)
            auditingId += directoryName + "_";

        auditingId += my::GetFileName(url);

        auditingId = my::ReplaceKeyword(auditingId, ":", "_");

        if (auditingId.size() > 80)
            auditingId = auditingId.substr(auditingId.size() - 80, 80);

        m_ufcCalibratorModel->SetAuditingId(auditingId);

        if (!OpenSettings())
        {
            LOG_ERROR();

            return false;
        }
    }

    return CHAOS_MONKEY_TESTING(true, false);
}

bool CUfcCalibratorViewModel::ClosePlay()
{
    if (!SaveSettings())
    {
        LOG_ERROR();

        return false;
    }

    // BUG: (22-Jul-2016)
    m_ufcCalibratorModel->Clear();

    return true;
}

void CUfcCalibratorViewModel::SetAttribute(OBJECT_TYPE objectType, boost::any value)
{
    m_objectTypeToValueMap[objectType] = value;
}

// 3D VIEW CAMERA
boost::shared_ptr<CPinholeCamera2> CUfcCalibratorViewModel::GetPinholeCamera()
{
    return m_pinholeCamera;
}

// 3D VIEW CAMERA
bool CUfcCalibratorViewModel::SetPinholeCamera(boost::shared_ptr<CPinholeCamera2> pinholeCamera)
{
    m_pinholeCamera.reset(new CPinholeCamera2(*pinholeCamera));

    HEALTH_CHECK(!m_pinholeCamera, false);

    return true;
}

// SELECT 3D OR FOOTAGE PLAYER INTERFACE AUTOMATICALLY (BASED ON THE CURRENT TAB)
boost::shared_ptr<CPlayerInterface> CUfcCalibratorViewModel::GetPlayerInterface()
{
    boost::shared_ptr<CPlayerInterface> emptyObject;

    if (!m_ufcCalibratorModel)
        return emptyObject;

    boost::shared_ptr<CFootage> footage = m_ufcCalibratorModel->GetFootage();

    if (footage)
    {
        boost::shared_ptr<CMediaPlayerInterface> mediaPlayerInterface = footage->GetMediaPlayerInterface();

        HEALTH_CHECK(!mediaPlayerInterface, emptyObject);

        return mediaPlayerInterface;
    }

    return emptyObject;
}

/**
*/
void CUfcCalibratorViewModel::RegisterUpdateCallback(boost::function<void()> updateCallback)
{
    m_updateCallback = updateCallback;
}

// TESTING: (07-Nov-2016) CHECK WHETHER THE PLAY DATA IS READY OR NOT
bool CUfcCalibratorViewModel::IsReady(std::string playId) const
{
    return true;
}

bool CUfcCalibratorViewModel::SaveCameraSettings(std::string cameraFileName)
{
    HEALTH_CHECK(!m_ufcCalibratorModel, false);

    boost::shared_ptr<CFootage> footage = m_ufcCalibratorModel->GetFootage();

    // BUG: (15-Sep-2016) THERE ARE VALID SITUATIONS, LIKE INITIALIZATION, WHERE THERE IS NO VALID FOOTAGE SET.
    if (!footage ||
        !footage->IsValid())
    {
        return true;
    }

    boost::shared_ptr<CPinholeCamera2> pinholeCamera = footage->GetPinholeCamera();

    if (!pinholeCamera)
    {
        LOG_ERROR();

        return false;
    }

    // (BEGIN OF) TODO: (08-Jul-2016) REPLACE PINHOLECAMERA BY VIEWPROJECTIONMATRIX
    const int* viewport = pinholeCamera->GetViewport();

    HEALTH_CHECK(!viewport, false);

    const double* viewMatrix = pinholeCamera->GetViewMatrix(),
        * projectionMatrix = pinholeCamera->GetProjectionMatrix();

    HEALTH_CHECK(!viewMatrix, false);
    HEALTH_CHECK(!projectionMatrix, false);

    my::CVector3<double> opticalCenter = pinholeCamera->GetOpticalCenter(),
        opticalAxis = pinholeCamera->GetOpticalAxis(),
        upAxis = pinholeCamera->GetUpAxis();

    opticalAxis.Normalize();
    upAxis.Normalize();

    double fieldOfView = pinholeCamera->GetFieldOfView(),
        nearClippingPlane = pinholeCamera->GetNearPlaneDepth(),
        farClippingPlane = pinholeCamera->GetFarPlaneDepth(),
        aspectRatio = (double)(viewport[2] - viewport[0]) / (viewport[3] - viewport[1]);
    // (END OF) TODO: (08-Jul-2016) REPLACE PINHOLECAMERA BY VIEWPROJECTIONMATRIX

    rapidjson::StringBuffer jsonString;
    rapidjson::Writer<rapidjson::StringBuffer> jsonWriter(jsonString);

    jsonWriter.StartObject();

    // VERSION

    jsonWriter.String("version");
    jsonWriter.String("my_zone_camera_calibration_4");

    // TIMESTAMP

    jsonWriter.String("timestamp");
    jsonWriter.String(my::CTimestamp::Now().ToString().c_str());

    // CAMERA INDEX

    jsonWriter.String("camera_index");

    int cameraIndex = pinholeCamera->GetIndex();

    jsonWriter.Int(my::IsNull(cameraIndex) ? -1 : cameraIndex);

    // VIEWPORT

    jsonWriter.String("viewport");

    jsonWriter.StartArray();

    jsonWriter.Int(viewport[0]);
    jsonWriter.Int(viewport[1]);
    jsonWriter.Int(viewport[2]);
    jsonWriter.Int(viewport[3]);

    jsonWriter.EndArray();

    // OPTICAL CENTER

    jsonWriter.String("optical_center");

    jsonWriter.StartArray();

    jsonWriter.Double(opticalCenter.x());
    jsonWriter.Double(opticalCenter.y());
    jsonWriter.Double(opticalCenter.z());

    jsonWriter.EndArray();

    // OPTICAL AXIS

    jsonWriter.String("optical_axis");

    jsonWriter.StartArray();

    jsonWriter.Double(opticalAxis.x());
    jsonWriter.Double(opticalAxis.y());
    jsonWriter.Double(opticalAxis.z());

    jsonWriter.EndArray();

    // UP

    jsonWriter.String("up_vector");

    jsonWriter.StartArray();

    jsonWriter.Double(upAxis.x());
    jsonWriter.Double(upAxis.y());
    jsonWriter.Double(upAxis.z());

    jsonWriter.EndArray();

    // FIELD OF VIEW

    jsonWriter.String("field_of_view");
    jsonWriter.Double(fieldOfView);

    // NEAR CLIPPING PLANE

    jsonWriter.String("near_clipping_plane");
    jsonWriter.Double(nearClippingPlane);

    // FAR CLIPPING PLANE

    jsonWriter.String("far_clipping_plane");
    jsonWriter.Double(farClippingPlane);

    // ASPECT RATIO

    jsonWriter.String("aspect_ratio");
    jsonWriter.Double(aspectRatio);

    jsonWriter.EndObject();

    std::ofstream cameraFileStream(cameraFileName);

    HEALTH_CHECK(!cameraFileStream.is_open(), false);

    cameraFileStream << jsonString.GetString();

    cameraFileStream.close();

    return true;
}

/**
*/
bool CUfcCalibratorViewModel::OpenSettings()
{
#if defined(TIMER_INCLUDED)
    // DEBUG ONLY! (01-Nov-2016) PERFORMANCE MEASUREMENT
    CTimer timer;
#endif // #if defined(TIMER_INCLUDED)

    if (!OpenExtrinsicSettings())
    {
        LOG_ERROR();

        // THE PIPELINE SHOULD NOT BE BROKEN HERE
        //return false;
    }

    if (!OpenCameraSettings())
    {
        LOG_ERROR();

        // THE PIPELINE SHOULD NOT BE BROKEN HERE
        //return false;
    }

    //// DEBUG ONLY! (17-Feb-2017) DISABLED ON PRODUCTION ENVIRONMENT
    //if (!OpenTrackingDataSettings())
    //{
    //    LOG_ERROR();

    //    // THE PIPELINE SHOULD NOT BE BROKEN HERE
    //    //return false;
    //}

#if defined(TIMER_INCLUDED)
    // DEBUG ONLY! (01-Nov-2016) PERFORMANCE MEASUREMENT
    m_stepNameAndTimeArray.push_back(std::make_pair(__FUNCTION__, timer.GetElapsed()));
#endif // #if defined(TIMER_INCLUDED)

    return true;
}

/**
*/
bool CUfcCalibratorViewModel::SaveSettings()
{
#if defined(TIMER_INCLUDED)
    // DEBUG ONLY! (01-Nov-2016) PERFORMANCE MEASUREMENT
    CTimer timer;
#endif // #if defined(TIMER_INCLUDED)

    if (!SaveExtrinsicSettings())
    {
        LOG_ERROR();

        // BUG: (??-???-2016) OTHER SETTINGS MAY STILL BE SAVED, THE PIPELINE SHOULD NOT BE BROKEN HERE
        //return false;
    }

    if (!SaveCameraSettings())
    {
        LOG_ERROR();

        // BUG: (??-???-2016) OTHER SETTINGS MAY STILL BE SAVED, THE PIPELINE SHOULD NOT BE BROKEN HERE
        //return false;
    }

    //// DEBUG ONLY! (17-Feb-2017) DISABLED ON PRODUCTION ENVIRONMENT
    //if (!SaveTrackingDataSettings())
    //{
    //    LOG_ERROR();

    //    // BUG: (??-???-2016) OTHER SETTINGS MAY STILL BE SAVED, THE PIPELINE SHOULD NOT BE BROKEN HERE
    //    //return false;
    //}

#if defined(TIMER_INCLUDED)
    // DEBUG ONLY! (01-Nov-2016) PERFORMANCE MEASUREMENT
    m_stepNameAndTimeArray.push_back(std::make_pair(__FUNCTION__, timer.GetElapsed()));
#endif // #if defined(TIMER_INCLUDED)

    return true;
}

// HELPER FOR INTRINSIC / EXTRINSIC MARKER SETTINGS IO
template <typename T>
bool ArrayFromJson(const rapidjson::Value& valueArrayHandle, T *valueArray, rapidjson::SizeType valueCount)
{
    HEALTH_CHECK(!valueArrayHandle.IsArray(), false);
    HEALTH_CHECK(!valueArray, false);
    HEALTH_CHECK(valueArrayHandle.Size() < valueCount, false);

    for (rapidjson::SizeType valueIndex = 0; valueIndex < valueCount; ++valueIndex)
    {
        const rapidjson::Value& valueHandle = valueArrayHandle[valueIndex];

        HEALTH_CHECK(!valueHandle.IsNumber(), false);

        valueArray[valueIndex] = valueHandle.GetDouble();
    }

    return true;
}

bool CUfcCalibratorViewModel::OpenExtrinsicSettings()
{
    HEALTH_CHECK(!m_ufcCalibratorModel, false);

    std::string auditingId = m_ufcCalibratorModel->GetAuditingId(),
        extrinsicSettingsFileName = my::ReplaceKeyword(UFC_STRING_RESOURCE_0433, "CAMERA_ID", auditingId),
        jsonString;

    my::file::GetFileAsString(extrinsicSettingsFileName, jsonString);

    rapidjson::Document document;

    if (document.Parse<0>(jsonString.c_str()).HasParseError())
    {
        LOG_MESSAGE("JSON error (offset " + my::NumberToString((unsigned)document.GetErrorOffset()) + "): " + rapidjson::GetParseError_En(document.GetParseError()));

        return false;
    }

    HEALTH_CHECK(!document.IsArray(), false);

    boost::shared_ptr<CFootage> footage = m_ufcCalibratorModel->GetFootage();

    HEALTH_CHECK(!footage, false);

    // BUG: (08-Jun-2017) IT SEEMS THAT INVALID SETTINGS ARE BEING LOADED, SO LET'S MAKE THE USER AWARE OF THAT
    bool areMarkersUpdated = false;

    for (rapidjson::SizeType markerIndex = 0; markerIndex < document.Size(); ++markerIndex)
    {
        const rapidjson::Value& markerIterator = document[markerIndex];

        HEALTH_CHECK(!markerIterator.IsObject(), false);

        std::string name = my::Null<std::string>();

        if (markerIterator.HasMember("name") &&
            markerIterator["name"].IsString())
        {
            name = markerIterator["name"].GetString();
        }

        boost::shared_ptr<my::video::CMarker> marker = footage->GetMarker(name);

        if (marker)
        {
            if (markerIterator.HasMember("world_coord"))
            {
                const rapidjson::Value& worldCoordHandle = markerIterator["world_coord"];

                double worldCoord[3] = { 0 };

                if (!ArrayFromJson(worldCoordHandle, worldCoord, 3))
                {
                    LOG_ERROR();

                    return false;
                }

                marker->SetWorldCoord(worldCoord);
            }

            if (markerIterator.HasMember("world_coord_error"))
            {
                const rapidjson::Value& worldCoordErrorHandle = markerIterator["world_coord_error"];

                double worldCoordError[3] = { 0 };

                if (!ArrayFromJson(worldCoordErrorHandle, worldCoordError, 3))
                {
                    LOG_ERROR();

                    return false;
                }

                marker->SetWorldCoordError(worldCoordError);
            }

            if (markerIterator.HasMember("window_coord_error"))
            {
                const rapidjson::Value& windowCoordErrorHandle = markerIterator["window_coord_error"];

                double windowCoordError[2] = { 0 };

                if (!ArrayFromJson(windowCoordErrorHandle, windowCoordError, 2))
                {
                    LOG_ERROR();

                    return false;
                }

                marker->SetWindowCoordError(windowCoordError[0], windowCoordError[1]);
            }

            const rapidjson::Value& screenCoordHandle = markerIterator["screen_coord"];

            double screenCoord[2] = { 0 };

            if (!ArrayFromJson(screenCoordHandle, screenCoord, 2))
            {
                LOG_ERROR();

                return false;
            }

            marker->SetScreenCoord(screenCoord);

            // BUG: (08-Jun-2017) IT SEEMS THAT INVALID SETTINGS ARE BEING LOADED, SO LET'S MAKE THE USER AWARE OF THAT
            areMarkersUpdated = true;

            if (markerIterator.HasMember("screen_size") &&
                markerIterator["screen_size"].IsNumber())
            {
                marker->SetScreenSize(markerIterator["screen_size"].GetDouble());
            }

            if (markerIterator.HasMember("zoom_factor") &&
                markerIterator["zoom_factor"].IsNumber())
            {
                marker->SetZoomFactor(markerIterator["zoom_factor"].GetDouble());
            }

            if (markerIterator.HasMember("enabled") &&
                markerIterator["enabled"].IsBool())
            {
                marker->SetEnabled(markerIterator["enabled"].GetBool());
            }

            // TRICKY: (07-Jul-2016) ONLY USER-DEFINED CHANGES ARE TAKEN INTO CONSIDERATION
            marker->SetHasChanged(false);
        }
    }

    // BUG: (08-Jun-2017) IT SEEMS THAT INVALID SETTINGS ARE BEING LOADED, SO LET'S MAKE THE USER AWARE OF THAT
    if (!areMarkersUpdated &&
        m_messageQueue)
    {
        m_messageQueue->PushMessageDialog(UFC_STRING_RESOURCE_0468);
    }

    return true;
}

bool CUfcCalibratorViewModel::SaveExtrinsicSettings()
{
    HEALTH_CHECK(!m_ufcCalibratorModel, false);

    boost::shared_ptr<CFootage> footage = m_ufcCalibratorModel->GetFootage();

    // BUG: (15-Sep-2016) THERE ARE VALID SITUATIONS, LIKE INITIALIZATION, WHERE THERE IS NO VALID FOOTAGE SET.
    if (!footage ||
        !footage->IsValid())
    {
        return true;
    }

    std::string auditingId = m_ufcCalibratorModel->GetAuditingId(),
        extrinsicSettingsFileName = UFC_STRING_RESOURCE_0422 + auditingId + ".json";

    rapidjson::StringBuffer jsonString;
    rapidjson::Writer<rapidjson::StringBuffer> jsonWriter(jsonString);

    jsonWriter.StartArray();

    for (const auto& marker : footage->GetExtrinsicCalibrationMarkerArray())
    {
        jsonWriter.StartObject();

        if (!marker->GetName().empty())
        {
            jsonWriter.String("name");
            jsonWriter.String(marker->GetName().c_str());
        }

        if (!marker->GetIcon().empty())
        {
            jsonWriter.String("icon");
            jsonWriter.String(marker->GetIcon().c_str());
        }

        const double* worldCoord = marker->GetWorldCoord();

        if (MyMath::IsValid(worldCoord[0]) ||
            MyMath::IsValid(worldCoord[1]) ||
            MyMath::IsValid(worldCoord[2]))
        {
            jsonWriter.String("world_coord");

            jsonWriter.StartArray();

            jsonWriter.Double(worldCoord[0]);
            jsonWriter.Double(worldCoord[1]);
            jsonWriter.Double(worldCoord[2]);

            jsonWriter.EndArray(); // world_coord
        }

        const double* worldCoordError = marker->GetWorldCoordError();

        if (MyMath::IsValid(worldCoordError[0]) ||
            MyMath::IsValid(worldCoordError[1]) ||
            MyMath::IsValid(worldCoordError[2]))
        {
            jsonWriter.String("world_coord_error");

            jsonWriter.StartArray();

            jsonWriter.Double(worldCoordError[0]);
            jsonWriter.Double(worldCoordError[1]);
            jsonWriter.Double(worldCoordError[2]);

            jsonWriter.EndArray(); // world_coord_error
        }

        const double* screenCoord = marker->GetScreenCoord();

        if (MyMath::IsValid(screenCoord[0]) ||
            MyMath::IsValid(screenCoord[1]))
        {
            jsonWriter.String("screen_coord");

            jsonWriter.StartArray();

            jsonWriter.Double(screenCoord[0]);
            jsonWriter.Double(screenCoord[1]);

            jsonWriter.EndArray(); // screen_coord
        }

        const double* screenCoordError = marker->GetScreenCoordError();

        if (MyMath::IsValid(screenCoordError[0]) ||
            MyMath::IsValid(screenCoordError[1]))
        {
            jsonWriter.String("screen_coord_error");

            jsonWriter.StartArray();

            jsonWriter.Double(screenCoordError[0]);
            jsonWriter.Double(screenCoordError[1]);

            jsonWriter.EndArray(); // screen_coord
        }

        my::CVector3<double> windowCoordError = marker->GetWindowCoordError();

        if (windowCoordError.IsValid())
        {
            jsonWriter.String("window_coord_error");

            jsonWriter.StartArray();

            jsonWriter.Double(windowCoordError.x());
            jsonWriter.Double(windowCoordError.y());
            jsonWriter.Double(windowCoordError.z());

            jsonWriter.EndArray(); // window_coord_error
        }

        jsonWriter.String("screen_size");
        jsonWriter.Double(marker->GetScreenSize());

        jsonWriter.String("zoom_factor");
        jsonWriter.Double(marker->GetZoomFactor());

        jsonWriter.String("constraint");
        jsonWriter.Double(marker->GetConstraint());

        jsonWriter.String("enabled");
        jsonWriter.Bool(marker->IsEnabled());

        jsonWriter.EndObject();
    }

    jsonWriter.EndArray();

    std::ofstream extrinsicSettingsFileStream(extrinsicSettingsFileName);

    HEALTH_CHECK(!extrinsicSettingsFileStream.is_open(), false);

    extrinsicSettingsFileStream << jsonString.GetString();

    extrinsicSettingsFileStream.close();

    return true;
}

/**
*/
bool CUfcCalibratorViewModel::OpenCameraSettings() const
{
    HEALTH_CHECK(!m_ufcCalibratorModel, false);

    // (BEGIN OF) BUG: (21-Mar-2017) EACH VIDEO, PLAY OR VIDEO/PLAY PAIR SHOULD HAVE AN AUDITING ID. THE AUDITING ID WAS DESIGNED TO BE A CUSTOMIZABLE STRING COMPOSED OF GAME_PRIMARY_KEY AND PLAY_ID, AT LEAST. THE AUDITING ID WILL SET THE NAME OF THE FILE THAT WILL HOLD THE PLAY SETTINGS. HOWEVER, THE CHANGES ON THE TOOL THAT WOULD BE NECESSARY TO SUPPORT A CUSTOMIZABLE STRING FOUND NO TIME TO BE IMPLEMENTED, AND, FOR NOW ON, THE AUDITING ID WILL BE GIVEN IN THE FORMAT GAME_PRIMARY_KEY-PLAY_ID.
    std::string auditingIdFormat = GetAttribute<std::string>(DATABASE_SETTINGS_CAMERA_ID_FORMAT);

    // TRICKY: (18-Feb-2017) SANITY CHECK - EXPECTED AUDITING ID FORMAT
    if (auditingIdFormat != "GAME_PRIMARY_KEY-PLAY_ID")
    {
        LOG_ERROR();

        return false;
    }
    // (END OF) BUG: (21-Mar-2017) EACH VIDEO, PLAY OR VIDEO/PLAY PAIR SHOULD HAVE AN AUDITING ID. THE AUDITING ID WAS DESIGNED TO BE A CUSTOMIZABLE STRING COMPOSED OF GAME_PRIMARY_KEY AND PLAY_ID, AT LEAST. THE AUDITING ID WILL SET THE NAME OF THE FILE THAT WILL HOLD THE PLAY SETTINGS. HOWEVER, THE CHANGES ON THE TOOL THAT WOULD BE NECESSARY TO SUPPORT A CUSTOMIZABLE STRING FOUND NO TIME TO BE IMPLEMENTED, AND, FOR NOW ON, THE AUDITING ID WILL BE GIVEN IN THE FORMAT GAME_PRIMARY_KEY-PLAY_ID.

    std::string playAuditingId = m_ufcCalibratorModel->GetAuditingId();

    // THE FOLLOWING IS AN ATTEMPT TO LOCATE THE CAMERA SETTINGS FOR THIS PLAY

    std::string jsonString;

    // LOCAL SETTINGS DATABASE - (1) LOCAL FILE, (2) PRELOADED FILE AND (3) PREVIOUS SETTINGS FOR THIS VENUE (USER OR REFERENCE SETTINGS)

    // (1) LOOK FOR A LOCAL FILE THAT CONTAINS THE SETTINGS FOR THIS PLAY

    std::string localFileName = my::ReplaceKeyword(UFC_STRING_RESOURCE_0431, "CAMERA_ID", playAuditingId);

    if (!my::file::GetFileAsString(localFileName, jsonString))
        jsonString.clear();

    if (jsonString.empty())
    {
        if (m_messageQueue)
            m_messageQueue->PushMessageDialog(UFC_STRING_RESOURCE_0414);

        LOG_ERROR();

        return false;
    }

    rapidjson::Document document;

    HEALTH_CHECK(document.Parse<0>(jsonString.c_str()).HasParseError(), false);

    HEALTH_CHECK(!document.IsObject(), false);

    int viewport[4] = { 0 };

    double opticalCenter[3] = { 0 },
        opticalAxis[3] = { 0 },
        upVector[3] = { 0 };

    double fieldOfView = my::Null<double>(),
        nearClippingPlane = my::Null<double>(),
        farClippingPlane = my::Null<double>(),
        aspectRatio = my::Null<double>();

    // viewport

    HEALTH_CHECK(!document.HasMember("viewport"), false);

    const rapidjson::Value& viewportParameterArrayHandle = document["viewport"];

    HEALTH_CHECK(!viewportParameterArrayHandle.IsArray(), false);
    HEALTH_CHECK(viewportParameterArrayHandle.Size() != 4, false);

    for (rapidjson::SizeType viewportParameterIndex = 0; viewportParameterIndex < viewportParameterArrayHandle.Size(); ++viewportParameterIndex)
    {
        const rapidjson::Value& viewportParameterHandle = viewportParameterArrayHandle[viewportParameterIndex];

        HEALTH_CHECK(!viewportParameterHandle.IsInt(), false);

        viewport[viewportParameterIndex] = viewportParameterHandle.GetInt();
    }

    // optical_center

    HEALTH_CHECK(!document.HasMember("optical_center"), false);

    const rapidjson::Value& opticalCenterHandle = document["optical_center"];

    HEALTH_CHECK(!opticalCenterHandle.IsArray(), false);
    HEALTH_CHECK(opticalCenterHandle.Size() != 3, false);

    for (rapidjson::SizeType opticalCenterParameterIndex = 0; opticalCenterParameterIndex < opticalCenterHandle.Size(); ++opticalCenterParameterIndex)
    {
        const rapidjson::Value& opticalCenterParameterHandle = opticalCenterHandle[opticalCenterParameterIndex];

        HEALTH_CHECK(!opticalCenterParameterHandle.IsDouble(), false);

        opticalCenter[opticalCenterParameterIndex] = opticalCenterParameterHandle.GetDouble();
    }

    // optical_axis

    HEALTH_CHECK(!document.HasMember("optical_axis"), false);

    const rapidjson::Value& opticalAxisHandle = document["optical_axis"];

    HEALTH_CHECK(!opticalAxisHandle.IsArray(), false);
    HEALTH_CHECK(opticalAxisHandle.Size() != 3, false);

    for (rapidjson::SizeType opticalAxisParameterIndex = 0; opticalAxisParameterIndex < opticalAxisHandle.Size(); ++opticalAxisParameterIndex)
    {
        const rapidjson::Value& opticalAxisParameterHandle = opticalAxisHandle[opticalAxisParameterIndex];

        HEALTH_CHECK(!opticalAxisParameterHandle.IsDouble(), false);

        opticalAxis[opticalAxisParameterIndex] = opticalAxisParameterHandle.GetDouble();
    }

    // up

    HEALTH_CHECK(!document.HasMember("up_vector"), false);

    const rapidjson::Value& upVectorHandle = document["up_vector"];

    HEALTH_CHECK(!upVectorHandle.IsArray(), false);
    HEALTH_CHECK(upVectorHandle.Size() != 3, false);

    for (rapidjson::SizeType upVectorParameterIndex = 0; upVectorParameterIndex < upVectorHandle.Size(); ++upVectorParameterIndex)
    {
        const rapidjson::Value& upVectorParameterHandle = upVectorHandle[upVectorParameterIndex];

        HEALTH_CHECK(!upVectorParameterHandle.IsDouble(), false);

        upVector[upVectorParameterIndex] = upVectorParameterHandle.GetDouble();
    }

    // field_of_view

    HEALTH_CHECK(!document.HasMember("field_of_view"), false);

    const rapidjson::Value& fieldOfViewHandle = document["field_of_view"];

    HEALTH_CHECK(!fieldOfViewHandle.IsDouble(), false);

    fieldOfView = fieldOfViewHandle.GetDouble();

    // near_clipping_plane

    HEALTH_CHECK(!document.HasMember("near_clipping_plane"), false);

    const rapidjson::Value& nearClippingPlaneHandle = document["near_clipping_plane"];

    HEALTH_CHECK(!nearClippingPlaneHandle.IsNumber(), false);

    nearClippingPlane = nearClippingPlaneHandle.GetDouble();

    // far_clipping_plane

    HEALTH_CHECK(!document.HasMember("far_clipping_plane"), false);

    const rapidjson::Value& farClippingPlaneHandle = document["far_clipping_plane"];

    HEALTH_CHECK(!farClippingPlaneHandle.IsNumber(), false);

    farClippingPlane = farClippingPlaneHandle.GetDouble();

    // aspect_ratio

    HEALTH_CHECK(!document.HasMember("aspect_ratio"), false);

    const rapidjson::Value& aspectRatioHandle = document["aspect_ratio"];

    HEALTH_CHECK(!aspectRatioHandle.IsDouble(), false);

    aspectRatio = aspectRatioHandle.GetDouble();

    boost::shared_ptr<CFootage> footage = m_ufcCalibratorModel->GetFootage();

    HEALTH_CHECK(!footage, false);

    boost::shared_ptr<CPinholeCamera2> pinholeCamera(new CPinholeCamera2);

    HEALTH_CHECK(!pinholeCamera, false);

    // ADAPTOR FOR GLULOOKAT
    double center[3] = { opticalCenter[0] + opticalAxis[0], opticalCenter[1] + opticalAxis[1], opticalCenter[2] + opticalAxis[2] };

    if (!pinholeCamera->Create(opticalCenter, center, upVector, fieldOfView, nearClippingPlane, farClippingPlane, viewport[2] - viewport[0], viewport[3] - viewport[1]))
    {
        LOG_ERROR();

        return false;
    }

    footage->SetPinholeCamera(pinholeCamera);

    return true;
}

/**
*/
bool CUfcCalibratorViewModel::SaveCameraSettings()
{
    HEALTH_CHECK(!m_ufcCalibratorModel, false);

    boost::shared_ptr<CFootage> footage = m_ufcCalibratorModel->GetFootage();

    // BUG: (15-Sep-2016) THERE ARE VALID SITUATIONS, LIKE INITIALIZATION, WHERE THERE IS NO VALID FOOTAGE SET.
    if (!footage ||
        !footage->IsValid())
    {
        return true;
    }

    std::string playAuditingId = m_ufcCalibratorModel->GetAuditingId();

    std::string cameraFileName = UFC_STRING_RESOURCE_0423
        + playAuditingId
        + ".json";

    if (!SaveCameraSettings(cameraFileName))
    {
        LOG_ERROR();

        return true;
    }

    return true;
}

/**
*/
bool CUfcCalibratorViewModel::OpenUserSettings()
{
    std::string userSettingsFileName = "./data/user/"
        // TODO: (22-Aug-2016) USER-BASED OR APP-BASED?
        + std::string("settings")
        + ".json";

    std::string userSettingsJsonString;

    if (my::file::GetFileAsString(userSettingsFileName, userSettingsJsonString) == 0)
    {
        LOG_ERROR();

        return false;
    }

    if (!OpenUserSettings(userSettingsJsonString))
    {
        LOG_ERROR();

        return false;
    }

    return true;
}

/**
*/
bool CUfcCalibratorViewModel::OpenUserSettings(std::string userSettingsJsonString)
{
    HEALTH_CHECK(userSettingsJsonString.empty(), false);

    rapidjson::Document document;
    document.Parse<0>(userSettingsJsonString.c_str());

    HEALTH_CHECK(!document.IsObject(), false);

    // (BEGIN OF) DATABASE
    if (document.HasMember("database"))
    {
        const rapidjson::Value& databaseHandle = document["database"];

        HEALTH_CHECK(!databaseHandle.IsObject(), false);

        if (databaseHandle.HasMember("auditing_id_format") &&
            databaseHandle["auditing_id_format"].IsString())
        {
            SetAttribute(DATABASE_SETTINGS_CAMERA_ID_FORMAT, std::string(databaseHandle["auditing_id_format"].GetString()));
        }
    }
    // (END OF) DATABASE

    // (BEGIN OF) GUI
    if (document.HasMember("gui"))
    {
        const rapidjson::Value& guiHandle = document["gui"];

        HEALTH_CHECK(!guiHandle.IsObject(), false);

        if (guiHandle.HasMember("gui_text_size") &&
            guiHandle["gui_text_size"].IsNumber())
        {
            SetAttribute(GUI_TEXT_SIZE_ITEM, guiHandle["gui_text_size"].GetInt());
        }

        if (guiHandle.HasMember("gui_field_layout_line_thickness") &&
            guiHandle["gui_field_layout_line_thickness"].IsNumber())
        {
            SetAttribute(GUI_FIELD_LAYOUT_LINE_THICKNESS_ITEM, guiHandle["gui_field_layout_line_thickness"].GetInt());
        }

        if (guiHandle.HasMember("gui_label_position") &&
            guiHandle["gui_label_position"].IsNumber())
        {
            SetAttribute(GUI_LABEL_POSITION_ITEM, guiHandle["gui_label_position"].GetInt());
        }

        if (guiHandle.HasMember("tracking_data_tab_item") &&
            guiHandle["tracking_data_tab_item"].IsBool())
        {
            SetAttribute(TRACKING_DATA_TAB_ITEM, guiHandle["tracking_data_tab_item"].GetBool());
        }

        if (guiHandle.HasMember("gui_refresh_rate") &&
            guiHandle["gui_refresh_rate"].IsNumber())
        {
            SetAttribute(GUI_REFRESH_RATE, guiHandle["gui_refresh_rate"].GetInt());
        }

        if (guiHandle.HasMember("gui_contents_refresh_rate") &&
            guiHandle["gui_contents_refresh_rate"].IsNumber())
        {
            SetAttribute(GUI_CONTENTS_REFRESH_RATE, guiHandle["gui_contents_refresh_rate"].GetInt());
        }

        if (guiHandle.HasMember("gui_settings_dock_minimum_size") &&
            guiHandle["gui_settings_dock_minimum_size"].IsNumber())
        {
            SetAttribute(GUI_SETTINGS_DOCK_MINIMUM_SIZE, guiHandle["gui_settings_dock_minimum_size"].GetInt());
        }

        if (guiHandle.HasMember("gui_play_key") &&
            guiHandle["gui_play_key"].IsNumber())
        {
            SetAttribute(GUI_PLAY_KEY, guiHandle["gui_play_key"].GetInt());
        }

        if (guiHandle.HasMember("gui_previous_frame_key") &&
            guiHandle["gui_previous_frame_key"].IsNumber())
        {
            SetAttribute(GUI_PREVIOUS_FRAME_KEY, guiHandle["gui_previous_frame_key"].GetInt());
        }

        if (guiHandle.HasMember("gui_next_frame_key") &&
            guiHandle["gui_next_frame_key"].IsNumber())
        {
            SetAttribute(GUI_NEXT_FRAME_KEY, guiHandle["gui_next_frame_key"].GetInt());
        }

        if (guiHandle.HasMember("gui_expert_mode") &&
            guiHandle["gui_expert_mode"].IsBool())
        {
            SetAttribute(GUI_EXPERT_MODE, guiHandle["gui_expert_mode"].GetBool());
        }

        if (guiHandle.HasMember("gui_octagon_size") &&
            guiHandle["gui_octagon_size"].IsInt())
        {
            SetAttribute(GUI_OCTAGON_SIZE, guiHandle["gui_octagon_size"].GetInt());
        }
    }
    // (END OF) GUI

    return true;
}

/**
*/
bool CUfcCalibratorViewModel::SaveUserSettings() const
{
    std::string userSettingsFileName = "./data/user/"
        // TODO: (22-Aug-2016) USER-BASED OR APP-BASED?
        + std::string("settings")
        + ".json";

    rapidjson::StringBuffer jsonString;
    rapidjson::Writer<rapidjson::StringBuffer> jsonWriter(jsonString);

    jsonWriter.StartObject();

    // (BEGIN OF) DATABASE
    jsonWriter.String("database");

    jsonWriter.StartObject();

    jsonWriter.String("auditing_id_format");
    jsonWriter.String(GetAttribute<std::string>(DATABASE_SETTINGS_CAMERA_ID_FORMAT).c_str());

    jsonWriter.EndObject();
    // (END OF) DATABASE

    // (BEGIN OF) GUI
    jsonWriter.String("gui");

    jsonWriter.StartObject();

    int guiTextSize = GetAttribute<int>(GUI_TEXT_SIZE_ITEM);

    if (!my::IsNull(guiTextSize))
    {
        jsonWriter.String("gui_text_size");
        jsonWriter.Int(guiTextSize);
    }

    int fieldLayoutLineThickness = GetAttribute<int>(GUI_FIELD_LAYOUT_LINE_THICKNESS_ITEM);

    if (!my::IsNull(fieldLayoutLineThickness))
    {
        jsonWriter.String("gui_field_layout_line_thickness");
        jsonWriter.Int(fieldLayoutLineThickness);
    }

    jsonWriter.String("gui_label_position");
    jsonWriter.Int(GetAttribute<int>(GUI_LABEL_POSITION_ITEM));

    jsonWriter.String("tracking_data_tab_item");
    jsonWriter.Bool(GetAttribute<bool>(TRACKING_DATA_TAB_ITEM));

    jsonWriter.String("gui_refresh_rate");
    jsonWriter.Int(GetAttribute<int>(GUI_REFRESH_RATE));

    jsonWriter.String("gui_contents_refresh_rate");
    jsonWriter.Int(GetAttribute<int>(GUI_CONTENTS_REFRESH_RATE));

    jsonWriter.String("gui_settings_dock_minimum_size");
    jsonWriter.Int(GetAttribute<int>(GUI_SETTINGS_DOCK_MINIMUM_SIZE));

    jsonWriter.String("gui_play_key");
    jsonWriter.Int(GetAttribute<int>(GUI_PLAY_KEY));

    jsonWriter.String("gui_previous_frame_key");
    jsonWriter.Int(GetAttribute<int>(GUI_PREVIOUS_FRAME_KEY));

    jsonWriter.String("gui_next_frame_key");
    jsonWriter.Int(GetAttribute<int>(GUI_NEXT_FRAME_KEY));

    jsonWriter.String("gui_expert_mode");
    jsonWriter.Bool(GetAttribute<bool>(GUI_EXPERT_MODE));

    jsonWriter.String("gui_octagon_size");
    jsonWriter.Int(GetAttribute<int>(GUI_OCTAGON_SIZE));

    jsonWriter.EndObject();
    // (END OF) GUI

    jsonWriter.EndObject();

    // LOCAL USER SETTINGS

    std::ofstream fileStream(userSettingsFileName);

    HEALTH_CHECK(!fileStream.is_open(), false);

    fileStream << jsonString.GetString();

    fileStream.close();

    return true;
}

/**
*/
void CUfcCalibratorViewModel::Create()
{
    boost::shared_ptr<my::CJournal> journal(new my::CJournal);

    if (journal)
    {
        journal->SetMessageDirectoryName("./data/trace/");

        SetJournal(journal);
    }

    m_messageQueue.reset();

    m_ufcCalibratorModel.reset(new CUfcCalibratorModel);

    HEALTH_CHECK(!m_ufcCalibratorModel, /*false*/);

    m_pinholeCamera.reset(new CPinholeCamera2);

    HEALTH_CHECK(!m_pinholeCamera, /*false*/);

    m_objectTypeToValueMap.clear();

    if (!OpenUserSettings())
        LOG_ERROR();

    boost::shared_ptr<my::CSceneryLayout> sceneryLayout = m_ufcCalibratorModel->GetSceneryLayout();

    if (sceneryLayout)
        sceneryLayout->SetMode(GetAttribute<int>(GUI_OCTAGON_SIZE));
}

