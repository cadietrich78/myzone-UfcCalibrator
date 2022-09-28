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

#if !defined(PLAYER_INTERFACE_INCLUDED)
#define PLAYER_INTERFACE_INCLUDED

#include <string>

#include <boost/shared_array.hpp>
#include <boost/function.hpp>

#include <Logger.h>
#include <Common.h>

class CPlayerInterface
{
public:
    enum STATE {
        STOPPED_STATE,
        PLAYING_STATE,
        PAUSED_STATE
    };

    CPlayerInterface();
    CPlayerInterface(const CPlayerInterface& playerInterface);

    virtual ~CPlayerInterface() {}

    void operator=(const CPlayerInterface& playerInterface);

    virtual bool IsPlayable() const = 0;

    virtual bool IsReady() const = 0;

    virtual void PreviousFrame() = 0;

    virtual void TogglePlay() = 0;

    virtual void NextFrame() = 0;

    virtual void Stop() = 0;

    STATE GetState() const;

    double GetMillisecondsByFrame() const;
    void SetMillisecondsByFrame(double millisecondsByFrame);

    my::int64 GetPositionInMilliseconds() const;
    virtual void SetPositionInMilliseconds(my::int64 positionInMilliseconds) = 0;

    my::int64 GetDurationInMilliseconds() const;
    void SetDurationInMilliseconds(my::int64 durationInMilliseconds);

    virtual void RegisterUpdateCallback(boost::function<void()> updateCallback) { }

private:
    void Create();
    void Copy(const CPlayerInterface& playerInterface);

protected:
    double m_millisecondsByFrame;

    my::int64 m_positionInMilliseconds;

    my::int64 m_durationInMilliseconds;

    STATE m_state;
};

#endif // #if !defined(PLAYER_INTERFACE_INCLUDED)

