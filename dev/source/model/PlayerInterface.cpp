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

#include "PlayerInterface.h"

CPlayerInterface::CPlayerInterface()
{
    Create();
}

CPlayerInterface::CPlayerInterface(const CPlayerInterface& playerInterface)
{
    Copy(playerInterface);
}

void CPlayerInterface::operator=(const CPlayerInterface& playerInterface)
{
    Copy(playerInterface);
}

CPlayerInterface::STATE CPlayerInterface::GetState() const
{
    return m_state;
}

double CPlayerInterface::GetMillisecondsByFrame() const
{
    return m_millisecondsByFrame;
}

void CPlayerInterface::SetMillisecondsByFrame(double millisecondsByFrame)
{
    m_millisecondsByFrame = millisecondsByFrame;
}

my::int64 CPlayerInterface::GetPositionInMilliseconds() const
{
    return m_positionInMilliseconds;
}


void CPlayerInterface::SetPositionInMilliseconds(my::int64 positionInMilliseconds)
{
    m_positionInMilliseconds = positionInMilliseconds;
}

my::int64 CPlayerInterface::GetDurationInMilliseconds() const
{
    return m_durationInMilliseconds;
}

void CPlayerInterface::SetDurationInMilliseconds(my::int64 durationInMilliseconds)
{
    m_durationInMilliseconds = durationInMilliseconds;
}

void CPlayerInterface::Create()
{
    // Default framerate (29.97002997)
    m_millisecondsByFrame = 1000.0 / (30.0 / 1.001);

    m_positionInMilliseconds = 0;

    m_durationInMilliseconds = 0;

    m_state = STOPPED_STATE;
}

void CPlayerInterface::Copy(const CPlayerInterface& playerInterface)
{
    m_millisecondsByFrame = playerInterface.m_millisecondsByFrame;

    m_positionInMilliseconds = playerInterface.m_positionInMilliseconds;

    m_durationInMilliseconds = playerInterface.m_durationInMilliseconds;

    m_state = playerInterface.m_state;
}

