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

#if !defined(MEDIA_PLAYER_INTERFACE_INCLUDED)
#define MEDIA_PLAYER_INTERFACE_INCLUDED

#include <string>

#include <boost/shared_array.hpp>
#include <boost/function.hpp>

#include <Logger.h>
#include <Common.h>

#include "../model/PlayerInterface.h"

class CMediaPlayerInterface
    : public CPlayerInterface
{
public:
    CMediaPlayerInterface();
    CMediaPlayerInterface(const CMediaPlayerInterface& mediaPlayerInterface);

    virtual ~CMediaPlayerInterface() {}

    void operator=(const CMediaPlayerInterface& mediaPlayerInterface);

    virtual bool OpenUrl(const std::string& url);
    virtual bool OpenUrl(std::vector<std::string> urlArray);

    virtual int GetFrameCount() const;

    virtual bool GetFrame(boost::shared_array<unsigned char>& bits, int& width, int& height, unsigned int& format, unsigned int& type) const;

    virtual bool GetFrame(int index, boost::shared_array<unsigned char>& bits, int& width, int& height, unsigned int& format, unsigned int& type) const;

    virtual int GetWidth() const;
    virtual bool SetWidth(int width);

    virtual int GetHeight() const;
    virtual bool SetHeight(int height);

private:
    void Create();
    void Copy(const CMediaPlayerInterface& mediaPlayerInterface);

protected:
    int m_width;
    int m_height;
};

#endif // #if !defined(MEDIA_PLAYER_INTERFACE_INCLUDED)

