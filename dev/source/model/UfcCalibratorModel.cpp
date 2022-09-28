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

#include <UnitConversion.h>
#include <UfcSceneryLayout.h>

#include "UfcCalibratorModel.h"

CUfcCalibratorModel::CUfcCalibratorModel()
{
    Create();
}

boost::shared_ptr<CFootage> CUfcCalibratorModel::GetFootage()
{
    return m_footage;
}

bool CUfcCalibratorModel::SetFootage(std::string url)
{
    //TRACE_FUNCTION(my::CTrace::MUST_HAVE, "(url): %s", url.empty() ? "" : url.c_str());

    HEALTH_CHECK(url.empty(), false);

    try
    {
        m_footage.reset(new CFootage);
    }
    catch (std::exception& e)
    {
        LOG_MESSAGE(e.what());

        return false;
    }

    if (!m_footage->OpenUrl(url))
    {
        LOG_ERROR();

        return false;
    }

    // (BEGIN OF) DEBUG ONLY! (20-Nov-2015) DEFAULT STRIKE ZONE CAMERA?
    boost::shared_ptr<CPinholeCamera2> pinholeCamera(new CPinholeCamera2);

    HEALTH_CHECK(!pinholeCamera, false);

    // CENTER FIELD CAMERA!
    //double opticalCenter[3] = { 0.0, 400.0, 30.0 },
    // FIGHTFLOW CAMERA!
    double opticalCenter[3] = { 19.928911271784585, 42.006903157967180, 23.005910657377818 },
        center[3] = { 0, 0, 0 },
        up[3] = { 0.0, 0.0, 1.0 };

    // TRICKY: (21-Nov-2015) VIEWPORT PARAMETERS?
    // CENTER FIELD CAMERA!
    //pinholeCamera->Create(opticalCenter, center, up, 2.0, 1.0, 1000.0, 1280, 720);
    // FIGHTFLOW CAMERA!
    pinholeCamera->Create(opticalCenter, center, up, 23.0, 1.0, 1000.0, 1280, 720);

    m_footage->SetPinholeCamera(pinholeCamera);
    // (END OF) DEBUG ONLY! (20-Nov-2015) STRIKE ZONE CAMERA?

    SetAuditingId(my::GetFileName(url));

    return true;
}

std::string CUfcCalibratorModel::GetAuditingId() const
{
    return m_auditingId;
}

void CUfcCalibratorModel::SetAuditingId(std::string auditingId)
{
    m_auditingId = auditingId;
}

boost::shared_ptr<my::CSceneryLayout> CUfcCalibratorModel::GetSceneryLayout()
{
    return m_sceneryLayout;
}

void CUfcCalibratorModel::Clear()
{
    // TRICKY: (09-Jun-20165) SHOULD WE KEEP THE MODE?
    //m_mode
    m_footage.reset();
    m_auditingId = my::Null<std::string>();
    // TRICKY: (09-Jun-20165) SHOULD WE KEEP THE MODE, AND, AS CONSEQUENCE, THE SCENERY LAYOUT?
    //m_sceneryLayout
}

bool CUfcCalibratorModel::IsValid() const
{
    return !(!m_footage);
}

void CUfcCalibratorModel::Create()
{
    m_footage.reset();
    m_auditingId = my::Null<std::string>();

    try
    {
        m_sceneryLayout.reset(new my::sport::CUfcSceneryLayout());
    }
    catch (std::exception& e)
    {
        LOG_MESSAGE(e.what());

        return /*false*/;
    }
}

