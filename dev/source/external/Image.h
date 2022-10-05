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

#if !defined(IMAGE_INCLUDED)
#define IMAGE_INCLUDED

#include <vector>

#include <boost/noncopyable.hpp>
#include <boost/shared_array.hpp>

#include <Common.h>
#include <Color.h>

namespace my
{
    namespace image
    {
        // Gamma-corrected sRGB or sRGBA (device dependent).
        class CImage
            : boost::noncopyable
        {
        public:
            enum CHANNEL {
                RED,
                GREEN,
                BLUE,
                // (https://en.wikipedia.org/wiki/Relative_luminance) The formula reflects the luminosity function: green light contributes the most to the intensity perceived by humans, and blue light the least.
                LUMINANCE
            };

            CImage();
            CImage(my::int32 width, my::int32 height, my::int32 channelCount, unsigned char value);
            // TRICKY: (27-Jan-2016) THE BUFFER IS COPIED!
            CImage(my::int32 width, my::int32 height, my::int32 channelCount, boost::shared_array<unsigned char> buffer);

            virtual ~CImage();

            bool Set(my::int32 width, my::int32 height, my::int32 channelCount, unsigned char value);
            // TRICKY: (27-Jan-2016) THE BUFFER IS COPIED!
            bool Set(my::int32 width, my::int32 height, my::int32 channelCount, boost::shared_array<unsigned char> buffer);

            my::int32 GetWidth() const;
            void SetWidth(my::int32 width);

            my::int32 GetHeight() const;
            void SetHeight(my::int32 height);

            my::int32 GetChannelCount() const;
            void SetChannelCount(my::int32 channelCount);

            boost::shared_array<unsigned char> GetBuffer() const;
            // TRICKY: (27-Jan-2016) THE BUFFER IS COPIED!
            bool SetBuffer(boost::shared_array<unsigned char> buffer);

            std::vector<double> GetChannel(CHANNEL channel);

            template < typename ValueType >
            MyColor::CRGB< ValueType > GetPixelRgb(my::int32 x, my::int32 y) const;
            template < typename ValueType >
            bool SetPixelRgb(my::int32 x, my::int32 y, MyColor::CRGB<ValueType> rgb);

            template < typename ValueType >
            ValueType GetPixelLuminance(my::int32 x, my::int32 y) const;
            template < typename ValueType >
            ValueType GetPixelInterpolatedLuminance(double x, double y);

            // LEFT < RIGHT, BOTTOM < TOP
            bool SubImage(my::int32 left, my::int32 bottom, my::int32 right, my::int32 top, boost::shared_ptr<my::image::CImage>& subImage) const;
            // ALL COORDINATES ON IMAGE SPACE ([0, width), [0, height))
            bool SubImage(double left, double bottom, double right, double top, my::int32 subImageWidth, my::int32 subImageHeight, boost::shared_ptr<my::image::CImage>& subImage);

            bool IsValid() const;

            virtual std::string ToJson() const;

        private:
            bool UpdateChannel(CHANNEL channel);

            void Create();

        protected:
            my::int32 m_width;
            my::int32 m_height;

            my::int32 m_channelCount;

            boost::shared_array<unsigned char> m_buffer;

            // TRICKY: (28-Jan-2016) ON-DEMAND ATTRIBUTE.
            std::vector<double> m_luminanceArray;
        };
    }; // image
}; // my

#endif //#if !defined(IMAGE_INCLUDED)

