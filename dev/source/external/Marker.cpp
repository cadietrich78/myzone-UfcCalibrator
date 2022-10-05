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

#include <Logger.h>

#include <Marker.h>

/**
*/
my::video::CMarker::CMarker()
    : CObject(),
    my::CPropertyObject()
{
    Create();
}

/**
*/
my::video::CMarker::CMarker(const CMarker& marker)
    : CObject(marker),
    my::CPropertyObject(marker)
{
    Copy(marker);
}

/**
*/
my::video::CMarker::~CMarker()
{
}

/**
*/
void my::video::CMarker::operator=(const CMarker& marker)
{
    CObject::operator=(marker);
    my::CPropertyObject::operator=(marker);

    Copy(marker);
}

/**
*/
std::string my::video::CMarker::GetName() const
{
    return m_name;
}

/**
*/
void my::video::CMarker::SetName(std::string name)
{
    m_name = name;
}

/**
*/
std::string my::video::CMarker::GetIcon() const
{
    return m_icon;
}

/**
*/
void my::video::CMarker::SetIcon(std::string icon)
{
    m_icon = icon;
}

/**
*/
const double *my::video::CMarker::GetWorldCoord() const
{
    return m_worldCoord;
}

/**
*/
void my::video::CMarker::SetWorldCoord(const double* worldCoord)
{
    HEALTH_CHECK(!worldCoord, /*false*/);

    m_worldCoord[0] = worldCoord[0];
    m_worldCoord[1] = worldCoord[1];
    m_worldCoord[2] = worldCoord[2];
}

// (WORLD) POSITION WHERE THE REAL-WORLD MARKER IS (3D)
void my::video::CMarker::SetWorldCoord(double x, double y, double z)
{
    m_worldCoord[0] = x;
    m_worldCoord[1] = y;
    m_worldCoord[2] = z;
}

/**
*/
const double *my::video::CMarker::GetWorldCoordError() const
{
	return m_worldCoordError;
}

/**
*/
void my::video::CMarker::SetWorldCoordError(const double* worldCoordError)
{
	HEALTH_CHECK(!worldCoordError, /*false*/);

	m_worldCoordError[0] = worldCoordError[0];
	m_worldCoordError[1] = worldCoordError[1];
	m_worldCoordError[2] = worldCoordError[2];
}

/**
*/
void my::video::CMarker::SetWorldCoordError(double x, double y, double z)
{
	m_worldCoordError[0] = x;
	m_worldCoordError[1] = y;
	m_worldCoordError[1] = z;
}

// (SCREEN) POSITION WHERE THE MARKER SHOULD BE PROJECTED (2D) [-1, 1]
const double *my::video::CMarker::GetScreenCoord() const
{
    return m_screenCoord;
}

// (SCREEN) POSITION WHERE THE MARKER SHOULD BE PROJECTED (2D) [-1, 1]
void my::video::CMarker::SetScreenCoord(const double* screenCoord)
{
    HEALTH_CHECK(!screenCoord, /*false*/);

    SetScreenCoord(screenCoord[0], screenCoord[1]);
}

// (SCREEN) POSITION WHERE THE MARKER SHOULD BE PROJECTED (2D) [-1, 1]
void my::video::CMarker::SetScreenCoord(double x, double y)
{
    // X

    if (!my::IsNull(m_screenCoord[0]) &&
        (m_screenCoord[0] != x) &&
        !my::IsNull(x))
    {
        m_hasChanged = true;
    }

    if ((m_constraint != Y_CONSTRAINT) ||
        (m_screenCoord[1] == my::Null<double>()))
    {
        m_screenCoord[0] = x;
    }

    // Y

    if ((m_screenCoord[1] != my::Null<double>()) &&
        (m_screenCoord[1] != y) &&
        !my::IsNull(y))
    {
        m_hasChanged = true;
    }

    m_screenCoord[1] = y;
}

/**
*/
const double *my::video::CMarker::GetScreenCoordError() const
{
	return m_screenCoordError;
}

/**
*/
void my::video::CMarker::SetScreenCoordError(const double* screenCoordError)
{
	HEALTH_CHECK(!screenCoordError, /*false*/);

	m_screenCoordError[0] = screenCoordError[0];
	m_screenCoordError[1] = screenCoordError[1];
}

/**
*/
void my::video::CMarker::SetScreenCoordError(double x, double y)
{
	m_screenCoordError[0] = x;
	m_screenCoordError[1] = y;
}

/**
*/
my::CVector3<double> my::video::CMarker::GetWindowCoordError() const
{
    return m_windowCoordError;
}

/**
*/
void my::video::CMarker::SetWindowCoordError(my::CVector3<double> windowCoordError)
{
    m_windowCoordError = windowCoordError;
}

/**
*/
void my::video::CMarker::SetWindowCoordError(double x, double y)
{
    m_windowCoordError.Set(x, y, 0.0);
}

//// TRICKY: (21-Sep-2016) ERROR ON SCREEN COORDINATES
//double my::video::CMarker::GetFitness(my::CViewProjectionMatrix<double>& viewProjectionMatrix)
//{
//    return my::Null<double>();
//}

// TRICKY: (21-Sep-2016) ERROR ON SCREEN COORDINATES
double my::video::CMarker::GetFitness(boost::shared_ptr<CPinholeCamera2>& pinholeCamera)
{
    return my::Null<double>();
}

// IT IS NECESSARY TO COMPUTE THE SIZE OF THE MARKER ON SCREEN COORDINATES
double my::video::CMarker::GetScreenSize() const
{
    return m_screenSize;
}

// IT IS NECESSARY TO COMPUTE THE SIZE OF THE MARKER ON SCREEN COORDINATES
void my::video::CMarker::SetScreenSize(double screenSize)
{
    m_screenSize = screenSize;
}

// IT IS NECESSARY TO COMPUTE THE SIZE OF THE MARKER ON SCREEN COORDINATES
double my::video::CMarker::GetZoomFactor() const
{
    return m_zoomFactor;
}

// IT IS NECESSARY TO COMPUTE THE SIZE OF THE MARKER ON SCREEN COORDINATES
void my::video::CMarker::SetZoomFactor(double screenZoom)
{
    m_zoomFactor = screenZoom;
}

/**
*/
my::video::CMarker::CONSTRAINT my::video::CMarker::GetConstraint() const
{
    return m_constraint;
}

/**
*/
void my::video::CMarker::SetConstraint(CONSTRAINT constraint)
{
    m_constraint = constraint;
}

/**
*/
bool my::video::CMarker::SetConstraint(INT32 constraintIndex)
{
    switch (constraintIndex) {
    case UNCONSTRAINED:
        m_constraint = UNCONSTRAINED;
        break;
    case Y_CONSTRAINT:
        m_constraint = Y_CONSTRAINT;
        break;
    default:
        LOG_ERROR();

        return false;
    }

    return true;
}

// DEPRECATED: (27-Jan-2016)
double my::video::CMarker::GetScreenSpeed() const
{
    return m_screenSpeed;
}

// DEPRECATED: (27-Jan-2016)
void my::video::CMarker::SetScreenSpeed(double speed)
{
    m_screenSpeed = speed;
}

/**
*/
bool my::video::CMarker::IsEnabled() const
{
    return m_enabled;
}

/**
*/
void my::video::CMarker::SetEnabled(bool enabled)
{
    m_enabled = enabled;
}

// IT REFERS TO ANY CHANGES TO THE MARKER SINCE INITIALIZATION
bool my::video::CMarker::HasChanged() const
{
    return m_hasChanged;
}

void my::video::CMarker::SetHasChanged(bool hasChanged)
{
    m_hasChanged = hasChanged;
}

std::string my::video::CMarker::ToJson() const
{
    // (BEGIN OF) DEPRECATED: (17-Mar-2020)
    //std::string jsonString;

    //jsonString += "{";

    //if (!m_name.empty())
    //    jsonString += JsonHelper::StringToJson("name", m_name) + ",";

    //if (!m_icon.empty())
    //    jsonString += JsonHelper::StringToJson("icon", m_icon) + ",";

    //if (MyMath::IsValid(m_worldCoord[0]) ||
    //    MyMath::IsValid(m_worldCoord[1]) ||
    //    MyMath::IsValid(m_worldCoord[2]))
    //{
    //    jsonString += JsonHelper::ArrayToJson("world_coord", m_worldCoord, 3) + ",";
    //}

    //if (MyMath::IsValid(m_worldCoordError[0]) ||
    //    MyMath::IsValid(m_worldCoordError[1]) ||
    //    MyMath::IsValid(m_worldCoordError[2]))
    //{
    //    jsonString += JsonHelper::ArrayToJson("world_coord_error", m_worldCoordError, 3) + ",";
    //}

    //if (MyMath::IsValid(m_screenCoord[0]) ||
    //    MyMath::IsValid(m_screenCoord[1]))
    //{
    //    jsonString += JsonHelper::ArrayToJson("screen_coord", m_screenCoord, 2) + ",";
    //}

    //if (MyMath::IsValid(m_screenCoordError[0]) ||
    //    MyMath::IsValid(m_screenCoordError[1]))
    //{
    //    jsonString += JsonHelper::ArrayToJson("screen_coord_error", m_screenCoordError, 2) + ",";
    //}

    //if (m_windowCoordError.IsValid())
    //    jsonString += JsonHelper::ArrayToJson("window_coord_error", m_windowCoordError, 2) + ",";

    //jsonString += JsonHelper::NumberToJson("screen_size", m_screenSize) + ",";
    //jsonString += JsonHelper::NumberToJson("zoom_factor", m_zoomFactor) + ",";

    //jsonString += JsonHelper::NumberToJson("constraint", (my::int32)m_constraint) + ",";

    //jsonString += JsonHelper::BooleanToJson("enabled", m_enabled) + ",";

    //if (!jsonString.empty() &&
    //    (jsonString.back() == ','))
    //{ 
    //    jsonString.pop_back();
    //}

    //jsonString += "}";

    //return jsonString;

    LOG_ERROR();

    return std::string();
    // (BEGIN OF) DEPRECATED: (17-Mar-2020)
}

/**
*/
void my::video::CMarker::Create()
{
    m_name.clear();
    m_icon.clear();
	m_worldCoord[0] = 0;
	m_worldCoord[1] = 0;
	m_worldCoord[2] = 0;
	m_worldCoordError[0] = 0;
	m_worldCoordError[1] = 0;
	m_worldCoordError[2] = 0;
    m_screenCoord[0] = my::Null<double>();
    m_screenCoord[1] = my::Null<double>();
	m_screenCoordError[0] = 0;
	m_screenCoordError[1] = 0;
    m_windowCoordError.Set(my::Null<double>(), my::Null<double>(), my::Null<double>());
    m_screenSize = 0.1;
    m_zoomFactor = 0.025;
    m_constraint = UNCONSTRAINED;
	m_enabled = true;
    m_hasChanged = false;
    m_screenSpeed = 1.0;
}

/**
*/
void my::video::CMarker::Copy(const CMarker& marker)
{
    m_name = marker.m_name;
    m_icon = marker.m_icon;
	m_worldCoord[0] = marker.m_worldCoord[0];
	m_worldCoord[1] = marker.m_worldCoord[1];
	m_worldCoord[2] = marker.m_worldCoord[2];
	m_worldCoordError[0] = marker.m_worldCoordError[0];
	m_worldCoordError[1] = marker.m_worldCoordError[1];
	m_worldCoordError[2] = marker.m_worldCoordError[2];
	m_screenCoord[0] = marker.m_screenCoord[0];
	m_screenCoord[1] = marker.m_screenCoord[1];
	m_screenCoordError[0] = marker.m_screenCoordError[0];
	m_screenCoordError[1] = marker.m_screenCoordError[1];
    m_windowCoordError = marker.m_windowCoordError;
    m_screenSize = marker.m_screenSize;
    m_zoomFactor = marker.m_zoomFactor;
    m_constraint = marker.m_constraint;
	m_enabled = marker.m_enabled;
    m_hasChanged = marker.m_hasChanged;
    m_screenSpeed = marker.m_screenSpeed;
}

