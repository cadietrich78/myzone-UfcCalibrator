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

#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/error/en.h>
#include <rapidjson/stringbuffer.h>

#include <Logger.h>
#include <FileHelper.h>

#include "CalibratedPinholeCamera.h"

CCalibratedPinholeCamera::CCalibratedPinholeCamera()
    : CPinholeCamera2(),
    m_timestamp(my::Null<my::CTimestamp>()),
    m_cameraMatrix(),
    m_distortionCoefficientArray()
{
}

CCalibratedPinholeCamera::CCalibratedPinholeCamera(const CCalibratedPinholeCamera& calibratedPinholeCamera)
    : CPinholeCamera2(calibratedPinholeCamera),
    m_timestamp(calibratedPinholeCamera.m_timestamp)
{
    Copy(calibratedPinholeCamera);
}

CCalibratedPinholeCamera::CCalibratedPinholeCamera(const CPinholeCamera2& pinholeCamera, const cv::Mat& cameraMatrix, const cv::Mat& distortionCoefficientArray)
    : CPinholeCamera2(pinholeCamera),
    m_timestamp(my::CTimestamp::Now()),
    m_cameraMatrix(cameraMatrix),
    m_distortionCoefficientArray(distortionCoefficientArray)
{
}

CCalibratedPinholeCamera& CCalibratedPinholeCamera::operator=(const CCalibratedPinholeCamera& calibratedPinholeCamera)
{
    Copy(calibratedPinholeCamera);
    
    return (*this);
}

bool CCalibratedPinholeCamera::FromOpenCvFile(std::string openCvCameraFileName)
{
    std::string openCvCameraAsString;

    if (!my::file::GetFileAsString(openCvCameraFileName, openCvCameraAsString))
    {
        LOG_ERROR();

        return false;
    }

    rapidjson::Document document;

    HEALTH_CHECK(document.Parse<0>(openCvCameraAsString.c_str()).HasParseError(), false);

    std::string timestampAsString;

    if (document.HasMember("timestamp"))
    {
        const rapidjson::Value& timestampHandle = document["timestamp"];

        HEALTH_CHECK(!timestampHandle.IsString(), false);

        timestampAsString = timestampHandle.GetString();
    }

    HEALTH_CHECK(!document.HasMember("viewport_width"), false);

    const rapidjson::Value& viewportWidthHandle = document["viewport_width"];

    HEALTH_CHECK(!viewportWidthHandle.IsInt(), false);

    int viewportWidth = viewportWidthHandle.GetInt();

    HEALTH_CHECK(!document.HasMember("viewport_height"), false);

    const rapidjson::Value& viewportHeightHandle = document["viewport_height"];

    HEALTH_CHECK(!viewportHeightHandle.IsInt(), false);

    int viewportHeight = viewportHeightHandle.GetInt();

    HEALTH_CHECK(!document.HasMember("camera_matrix"), false);

    const rapidjson::Value& cameraMatrixHandle = document["camera_matrix"];

    HEALTH_CHECK(!cameraMatrixHandle.IsObject(), false);

    HEALTH_CHECK(!cameraMatrixHandle.HasMember("data"), false);

    const rapidjson::Value& cameraMatrixDataHandle = cameraMatrixHandle["data"];

    HEALTH_CHECK(!cameraMatrixDataHandle.IsArray(), false);
    HEALTH_CHECK(cameraMatrixDataHandle.Size() != 9, false);

    cv::Mat cameraMatrix(3, 3, CV_64FC1);

    try
    {
        std::vector<double> cameraMatrixAsArray;

        for (int i = 0; i < 9; ++i)
            cameraMatrixAsArray.push_back(cameraMatrixDataHandle[i].GetDouble());

        std::memcpy(cameraMatrix.data, cameraMatrixAsArray.data(), cameraMatrixAsArray.size() * sizeof(cameraMatrixAsArray[0]));
    }
    catch (std::exception& e)
    {
        LOG_MESSAGE(e.what());
    }

    HEALTH_CHECK(!document.HasMember("distortion_coefficients"), false);

    const rapidjson::Value& distortionCoefficientsHandle = document["distortion_coefficients"];

    HEALTH_CHECK(!distortionCoefficientsHandle.IsObject(), false);

    HEALTH_CHECK(!distortionCoefficientsHandle.HasMember("data"), false);

    const rapidjson::Value& distortionCoefficientsDataHandle = distortionCoefficientsHandle["data"];

    HEALTH_CHECK(!distortionCoefficientsDataHandle.IsArray(), false);
    HEALTH_CHECK(distortionCoefficientsDataHandle.Size() != 5, false);

    cv::Mat distortionCoefficientArray(1, 5, CV_64FC1);

    try
    {
        std::vector<double> distortionCoefficientsAsArray;

        for (int i = 0; i < 5; ++i)
            distortionCoefficientsAsArray.push_back(distortionCoefficientsDataHandle[i].GetDouble());

        std::memcpy(distortionCoefficientArray.data, distortionCoefficientsAsArray.data(), distortionCoefficientsAsArray.size() * sizeof(distortionCoefficientsAsArray[0]));
    }
    catch (std::exception& e)
    {
        LOG_MESSAGE(e.what());
    }

    cv::Vec3d rotationVector;

    HEALTH_CHECK(!document.HasMember("rotation_vector"), false);

    const rapidjson::Value& rotationVectorHandle = document["rotation_vector"];

    if (rotationVectorHandle.IsObject())
    {
        HEALTH_CHECK(!rotationVectorHandle.HasMember("data"), false);

        const rapidjson::Value& rotationVectorDataHandle = rotationVectorHandle["data"];

        HEALTH_CHECK(!rotationVectorDataHandle.IsArray(), false);
        HEALTH_CHECK(rotationVectorDataHandle.Size() != 3, false);

        try
        {
            std::vector<double> rotationVectorAsArray;

            for (int i = 0; i < 3; i++)
                rotationVectorAsArray.push_back(rotationVectorDataHandle[i].GetDouble());

            rotationVector = cv::Vec3d(rotationVectorAsArray.data());
        }
        catch (std::exception& e)
        {
            LOG_MESSAGE(e.what());
        }
    }
    else if (rotationVectorHandle.IsArray())
    {
        HEALTH_CHECK(rotationVectorHandle.Size() != 3, false);

        rotationVector[0] = rotationVectorHandle[0].GetDouble();
        rotationVector[1] = rotationVectorHandle[1].GetDouble();
        rotationVector[2] = rotationVectorHandle[2].GetDouble();
    }
    else
    {
        LOG_ERROR();

        return false;
    }

    cv::Vec3d translationVector;

    HEALTH_CHECK(!document.HasMember("translation_vector"), false);

    const rapidjson::Value& translationVectorHandle = document["translation_vector"];

    if (translationVectorHandle.IsObject())
    {
        HEALTH_CHECK(!translationVectorHandle.HasMember("data"), false);

        const rapidjson::Value& translationVectorDataHandle = translationVectorHandle["data"];

        HEALTH_CHECK(!translationVectorDataHandle.IsArray(), false);
        HEALTH_CHECK(translationVectorDataHandle.Size() != 3, false);

        try
        {
            std::vector<double> translationVectorAsArray;

            for (int i = 0; i < 3; i++)
                translationVectorAsArray.push_back(translationVectorDataHandle[i].GetDouble());

            translationVector = cv::Vec3d(translationVectorAsArray.data());
        }
        catch (std::exception& e)
        {
            LOG_MESSAGE(e.what());
        }
    }
    else if (translationVectorHandle.IsArray())
    {
        HEALTH_CHECK(translationVectorHandle.Size() != 3, false);

        translationVector[0] = translationVectorHandle[0].GetDouble();
        translationVector[1] = translationVectorHandle[1].GetDouble();
        translationVector[2] = translationVectorHandle[2].GetDouble();
    }
    else
    {
        LOG_ERROR();

        return false;
    }

    HEALTH_CHECK(viewportWidth == DBL_MAX, false);
    HEALTH_CHECK(viewportHeight == DBL_MAX, false);

    // (1,1) focal length (f_y)
    double yFocalLength = cameraMatrix.at<double>(1, 1),
        fovY = MyMath::RadiansToDegrees(2.0 * atan(viewportHeight / (2.0 * yFocalLength))),
        nearPlaneDepth = 1.0,
        farPlaneDepth = 1000.0;

    try
    {
        Create(fovY, nearPlaneDepth, farPlaneDepth, (int)(viewportWidth + 0.5), (int)(viewportHeight + 0.5));

        if (!timestampAsString.empty())
        {
            my::CTimestamp calibrationTimestamp(timestampAsString);

            SetTimestamp(calibrationTimestamp);
        }

        cv::Mat rMat3x3(3, 3, CV_64F);

        cv::Rodrigues(rotationVector, rMat3x3);

        // BUG: (21-Nov-2021) FIGHTFLOW CALIBRATIONS ARE STORED IN INCHES!
        double translationVectorInFeet[3] = { 0.0833333333333333 * translationVector[0], 0.0833333333333333 * translationVector[1] , 0.0833333333333333 * translationVector[2] };

        SetCameraMatrix(cameraMatrix);
        SetDistortionCoefficientArray(distortionCoefficientArray);

        // (BEGIN OF) TRICKY: (01-Sep-2022) THE CONVERSION FROM THE OPENCV CAMERA MATRIX (ROTATION + TRANSLATION) TO THE OPENGL PINHOLE CAMERA IS NOT WORKING, SO I'LL TAKE A DETOUR USING THE PinholeCamera TYPE!
        double viewMatrix[16] = { -rMat3x3.at<double>(0, 0), -rMat3x3.at<double>(1, 0), -rMat3x3.at<double>(2, 0), 0.0, -rMat3x3.at<double>(0, 1), -rMat3x3.at<double>(1, 1), -rMat3x3.at<double>(2, 1), 0.0, -rMat3x3.at<double>(0, 2), -rMat3x3.at<double>(1, 2), -rMat3x3.at<double>(2, 2), 0.0, -translationVectorInFeet[0], -translationVectorInFeet[1], -translationVectorInFeet[2], 1.0 };

        SetViewMatrix(viewMatrix);

        my::CVector3<double> opticalCenter = GetOpticalCenter(),
            opticalAxis = GetOpticalAxis(),
            upVector = GetUpAxis();

        double opticalCenterPointer[3] = { opticalCenter.x(), opticalCenter.y(), opticalCenter.z() },
            centerPointer[3] = { opticalCenterPointer[0] + opticalAxis[0], opticalCenterPointer[1] + opticalAxis[1], opticalCenterPointer[2] + opticalAxis[2] },
            upVectorPointer[3] = { upVector.x(), upVector.y(), upVector.z() };

        if (!Create(opticalCenterPointer, centerPointer, upVectorPointer, fovY, nearPlaneDepth, farPlaneDepth, (int)(viewportWidth + 0.5), (int)(viewportHeight + 0.5)))
        {
            LOG_ERROR();

            return false;
        }
        // (END OF) TRICKY: (01-Sep-2022) THE CONVERSION FROM THE OPENCV CAMERA MATRIX (ROTATION + TRANSLATION) TO THE OPENGL PINHOLE CAMERA IS NOT WORKING, SO I'LL TAKE A DETOUR USING THE PinholeCamera TYPE!
    }
    catch (std::exception& e)
    {
        LOG_MESSAGE(e.what());

        return false;
    }

    return true;
}

bool CCalibratedPinholeCamera::ToPinholeCameraFile(std::string pinholeCameraFileName)
{
    HEALTH_CHECK(m_cameraMatrix.empty(), false);
    HEALTH_CHECK(m_cameraMatrix.size() != cv::Size(3, 3), false);
    HEALTH_CHECK(m_distortionCoefficientArray.empty(), false);
    HEALTH_CHECK(m_distortionCoefficientArray.size() != cv::Size(5, 1), false);

    my::CVector3<double> opticalCenter = GetOpticalCenter(),
        opticalAxis = GetOpticalAxis(),
        up = GetUpAxis();

    opticalAxis.Normalize();
    up.Normalize();

    double aspectRatio = (double)(m_viewport[2] - m_viewport[0]) / (m_viewport[3] - m_viewport[1]);

    rapidjson::StringBuffer jsonString;
    rapidjson::Writer<rapidjson::StringBuffer> jsonWriter(jsonString);

    jsonWriter.StartObject();

    // "viewport"

    jsonWriter.String("viewport");

    jsonWriter.StartArray();

    jsonWriter.Int(m_viewport[0]);
    jsonWriter.Int(m_viewport[1]);
    jsonWriter.Int(m_viewport[2]);
    jsonWriter.Int(m_viewport[3]);

    jsonWriter.EndArray();

    // "optical_center"

    jsonWriter.String("optical_center");

    jsonWriter.StartArray();

    jsonWriter.Double(opticalCenter.x());
    jsonWriter.Double(opticalCenter.y());
    jsonWriter.Double(opticalCenter.z());

    jsonWriter.EndArray();

    // "optical_axis"

    jsonWriter.String("optical_axis");

    jsonWriter.StartArray();

    jsonWriter.Double(opticalAxis.x());
    jsonWriter.Double(opticalAxis.y());
    jsonWriter.Double(opticalAxis.z());

    jsonWriter.EndArray();

    // "up_vector"

    jsonWriter.String("up_vector");

    jsonWriter.StartArray();

    jsonWriter.Double(up.x());
    jsonWriter.Double(up.y());
    jsonWriter.Double(up.z());

    jsonWriter.EndArray();

    // "field_of_view"

    jsonWriter.String("field_of_view");
    jsonWriter.Double(m_fieldOfView);

    // "near_clipping_plane"

    jsonWriter.String("near_clipping_plane");
    jsonWriter.Double(m_nearPlaneDepth);

    // "far_clipping_plane"

    jsonWriter.String("far_clipping_plane");
    jsonWriter.Double(m_farPlaneDepth);

    // "aspect_ratio"

    jsonWriter.String("aspect_ratio");
    jsonWriter.Double(aspectRatio);

    // "camera_matrix"

    jsonWriter.String("camera_matrix");

    jsonWriter.StartObject(); // camera_matrix

    jsonWriter.String("type_id");
    jsonWriter.String("opencv-matrix");

    jsonWriter.String("rows");
    jsonWriter.Int(3);

    jsonWriter.String("cols");
    jsonWriter.Int(3);

    jsonWriter.String("dt");
    jsonWriter.String("d");

    jsonWriter.String("data");

    jsonWriter.StartArray(); // camera_matrix.data

    for (int j = 0; j < 3; ++j)
    {
        for (int i = 0; i < 3; ++i)
            jsonWriter.Double(m_cameraMatrix.at<double>(j, i));
    }

    jsonWriter.EndArray(); // camera_matrix.data

    jsonWriter.EndObject(); // camera_matrix

    // "distortion_coefficients"

    jsonWriter.String("distortion_coefficients");

    jsonWriter.StartObject(); // distortion_coefficients

    jsonWriter.String("type_id");
    jsonWriter.String("opencv-matrix");

    jsonWriter.String("rows");
    jsonWriter.Int(5);

    jsonWriter.String("cols");
    jsonWriter.Int(1);

    jsonWriter.String("dt");
    jsonWriter.String("d");

    jsonWriter.String("data");

    jsonWriter.StartArray(); // distortion_coefficients.data

    for (int i = 0; i < 5; ++i)
        jsonWriter.Double(m_distortionCoefficientArray.at<double>(0, i));

    jsonWriter.EndArray(); // distortion_coefficients.data

    jsonWriter.EndObject(); // distortion_coefficients

    jsonWriter.EndObject();

    jsonString.Flush();

    std::ofstream jsonFileStream(pinholeCameraFileName);

    HEALTH_CHECK(!jsonFileStream.is_open(), false);

    jsonFileStream << jsonString.GetString();

    jsonFileStream.close();

    return true;
}

bool CCalibratedPinholeCamera::FromPinholeCameraFile(std::string pinholeCameraFileName)
{
    if (!CPinholeCamera2::FromFile(pinholeCameraFileName))
    {
        LOG_ERROR();

        return false;
    }

    std::string pinholeCameraAsString;

    if (!my::file::GetFileAsString(pinholeCameraFileName, pinholeCameraAsString))
    {
        LOG_ERROR();

        return false;
    }

    rapidjson::Document document;

    HEALTH_CHECK(document.Parse<0>(pinholeCameraAsString.c_str()).HasParseError(), false);

    HEALTH_CHECK(!document.HasMember("camera_matrix"), false);

    const rapidjson::Value& cameraMatrixHandle = document["camera_matrix"];

    HEALTH_CHECK(!cameraMatrixHandle.IsObject(), false);

    HEALTH_CHECK(!cameraMatrixHandle.HasMember("data"), false);

    const rapidjson::Value& cameraMatrixDataHandle = cameraMatrixHandle["data"];

    HEALTH_CHECK(!cameraMatrixDataHandle.IsArray(), false);
    HEALTH_CHECK(cameraMatrixDataHandle.Size() != 9, false);

    cv::Mat cameraMatrix(3, 3, CV_64FC1);

    try
    {
        std::vector<double> cameraMatrixAsArray;

        for (int i = 0; i < 9; ++i)
            cameraMatrixAsArray.push_back(cameraMatrixDataHandle[i].GetDouble());

        std::memcpy(cameraMatrix.data, cameraMatrixAsArray.data(), cameraMatrixAsArray.size() * sizeof(cameraMatrixAsArray[0]));
    }
    catch (std::exception& e)
    {
        LOG_MESSAGE(e.what());
    }

    HEALTH_CHECK(!document.HasMember("distortion_coefficients"), false);

    const rapidjson::Value& distortionCoefficientsHandle = document["distortion_coefficients"];

    HEALTH_CHECK(!distortionCoefficientsHandle.IsObject(), false);

    HEALTH_CHECK(!distortionCoefficientsHandle.HasMember("data"), false);

    const rapidjson::Value& distortionCoefficientsDataHandle = distortionCoefficientsHandle["data"];

    HEALTH_CHECK(!distortionCoefficientsDataHandle.IsArray(), false);
    HEALTH_CHECK(distortionCoefficientsDataHandle.Size() != 5, false);

    cv::Mat distortionCoefficientArray(1, 5, CV_64FC1);

    try
    {
        std::vector<double> distortionCoefficientsAsArray;

        for (int i = 0; i < 5; ++i)
            distortionCoefficientsAsArray.push_back(distortionCoefficientsDataHandle[i].GetDouble());

        std::memcpy(distortionCoefficientArray.data, distortionCoefficientsAsArray.data(), distortionCoefficientsAsArray.size() * sizeof(distortionCoefficientsAsArray[0]));
    }
    catch (std::exception& e)
    {
        LOG_MESSAGE(e.what());
    }

    return true;
}

my::CTimestamp CCalibratedPinholeCamera::GetTimestamp() const
{
    return m_timestamp;
}

void CCalibratedPinholeCamera::SetTimestamp(my::CTimestamp timestamp)
{
    m_timestamp = timestamp;
}

cv::Mat CCalibratedPinholeCamera::GetCameraMatrix() const
{
    return m_cameraMatrix;
}

void CCalibratedPinholeCamera::SetCameraMatrix(const cv::Mat& cameraMatrix)
{
    m_cameraMatrix = cameraMatrix;
}

cv::Mat CCalibratedPinholeCamera::GetDistortionCoefficientArray() const
{
    return m_distortionCoefficientArray;
}

void CCalibratedPinholeCamera::SetDistortionCoefficientArray(const cv::Mat& distortionCoefficientArray)
{
    m_distortionCoefficientArray = distortionCoefficientArray;
}

void CCalibratedPinholeCamera::Copy(const CCalibratedPinholeCamera& calibratedPinholeCamera)
{
    m_timestamp = calibratedPinholeCamera.m_timestamp;
    m_cameraMatrix = calibratedPinholeCamera.m_cameraMatrix;
    m_distortionCoefficientArray = calibratedPinholeCamera.m_distortionCoefficientArray;
}

