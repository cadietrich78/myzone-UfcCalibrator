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

#if !defined(PNG_WRAPPER_INCLUDED)
#define PNG_WRAPPER_INCLUDED

#include <string>

#include <boost/noncopyable.hpp>
#include <boost/shared_array.hpp>

#include <Common.h>
#include <Image.h>

class CPngWrapper
    : boost::noncopyable
{
public:
    CPngWrapper();

    bool Open(const std::string& fileName);

    bool Save(std::string fileName, unsigned int width, unsigned int height, unsigned char *rgbArray) const;
    bool Save(std::string fileName, boost::shared_ptr<my::image::CImage> image) const;

    bool Encode(my::int32 width, my::int32 height, int channelCount, const unsigned char *rgbArray, std::vector<unsigned char> *byteArray) const;

    boost::shared_array<unsigned char> GetBuffer() const;
    void SetBuffer(boost::shared_array<unsigned char> buffer);
    
    unsigned int GetWidth() const;
    void SetWidth(unsigned int width);

    unsigned int GetHeight() const;
    void SetHeight(unsigned int height);
    
	INT32 GetChannelCount() const;
	void SetChannelCount(INT32 channelCount);

protected:
    boost::shared_array<unsigned char> m_buffer;
    
    UINT32 m_width;
    UINT32 m_height;
    
    UINT32 m_channelCount;
};

#endif // #if !defined(PNG_WRAPPER_INCLUDED)

