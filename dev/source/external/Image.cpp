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
 * is not re7quired but would be appreciated.
 *
 * 
 * "Software is a process, it's never finished, it's always evolving. 
 * That's its nature. We know our software sucks. But it's shipping! 
 * Next time we'll do better, but even then it will be shitty. 
 * The only software that's perfect is one you're dreaming about. 
 * Real software crashes, loses data, is hard to learn and hard to use. 
 * But it's a process. We'll make it less shitty. Just watch!"
 */

#include <algorithm>

#include <Logger.h>
#include <PngWrapper.h>
#include <MyMath.h>

#include <Image.h>

// ALL COORDINATES ON IMAGE SPACE ([0, width), [0, height))
bool BilinearSampling(double s, double t, my::int32 width, my::int32 height, my::int32 channelCount, boost::shared_array<unsigned char>& buffer, unsigned char *color)
{
    HEALTH_CHECK(s < 0.0, false);
    HEALTH_CHECK(s >(width - 1), false);
    HEALTH_CHECK(t < 0.0, false);
    HEALTH_CHECK(t >(height - 1), false);
    HEALTH_CHECK(width < 0, false);
    HEALTH_CHECK(height < 0, false);
    HEALTH_CHECK(channelCount < 1, false);
    HEALTH_CHECK(channelCount > 4, false);
    HEALTH_CHECK(!buffer, false);
    HEALTH_CHECK(!color, false);

    my::int32 sInt = (my::int32)s,
        tInt = (my::int32)t,
        sIntNext = sInt + 1,
        tIntNext = tInt + 1;

    sIntNext = std::min(sIntNext, width - 1);
    tIntNext = std::min(tIntNext, height - 1);

    double ts = s - sInt,
        tt = t - tInt;

    // tIntNext p2------top------p3
    //                   |
    //                   |
    //                center
    //                   |
    //                   |
    //     tInt p0----bottom-----p1
    //        sInt            sIntNext

    const unsigned char *p = buffer.get(),
        *p0 = p + channelCount * (tInt * width + sInt),
        *p1 = p + channelCount * (tInt * width + sIntNext),
        *p2 = p + channelCount * (tIntNext * width + sInt),
        *p3 = p + channelCount * (tIntNext * width + sIntNext);

    double bottomColor[4] = { 0 },
        topColor[4] = { 0 },
        centerColor[4] = { 0 };

    for (my::int32 channelIndex = 0; channelIndex < channelCount; ++channelIndex)
    {
        // HORIZONTAL

        bottomColor[channelIndex] = (1.0 - ts) * p0[channelIndex] + ts * p1[channelIndex];

        topColor[channelIndex] = (1.0 - ts) * p2[channelIndex] + ts * p3[channelIndex];

        // VERTICAL

        centerColor[channelIndex] = (1.0 - tt) * bottomColor[channelIndex] + tt * topColor[channelIndex];

        color[channelIndex] = (unsigned char)MyMath::Clamp((my::int32)(centerColor[channelIndex] + 0.5), 0, 255);
    }

    return true;
}

/**
*/
my::image::CImage::CImage()
{
    Create();
}

/**
*/
my::image::CImage::CImage(my::int32 width, my::int32 height, my::int32 channelCount, unsigned char value)
{
    if (!Set(width, height, channelCount, value))
        LOG_ERROR();
}

/**
*/
my::image::CImage::CImage(my::int32 width, my::int32 height, my::int32 channelCount, boost::shared_array<unsigned char> buffer)
{
    if (!Set(width, height, channelCount, buffer))
        LOG_ERROR();
}

/**
*/
my::image::CImage::~CImage()
{
}

/**
*/
bool my::image::CImage::Set(my::int32 width, my::int32 height, my::int32 channelCount, unsigned char value)
{
    HEALTH_CHECK(my::IsNull(width), false);
    HEALTH_CHECK(width <= 0, false);
    HEALTH_CHECK(my::IsNull(height), false);
    HEALTH_CHECK(height <= 0, false);
    HEALTH_CHECK(my::IsNull(channelCount), false);
    HEALTH_CHECK(channelCount < 3, false);

    m_width = width;
    m_height = height;
    
    m_channelCount = channelCount;
    
    m_buffer.reset(new unsigned char[channelCount * width * height]);

    HEALTH_CHECK(!m_buffer, false);

    std::fill_n(m_buffer.get(), channelCount * width * height, value);

    return true;
}

/**
*/
bool my::image::CImage::Set(my::int32 width, my::int32 height, my::int32 channelCount, boost::shared_array<unsigned char> buffer)
{
    HEALTH_CHECK(my::IsNull(width), false);
    HEALTH_CHECK(width <= 0, false);
    HEALTH_CHECK(my::IsNull(height), false);
    HEALTH_CHECK(height <= 0, false);
    HEALTH_CHECK(my::IsNull(channelCount), false);
    HEALTH_CHECK(channelCount < 3, false);
    HEALTH_CHECK(!buffer, false);

    m_width = width;
    m_height = height;

    m_channelCount = channelCount;

    if (!SetBuffer(buffer))
    {
        LOG_ERROR();

        return false;
    }

    return true;
}

/**
*/
my::int32 my::image::CImage::GetWidth() const
{
    return m_width;
}

/**
*/
void my::image::CImage::SetWidth(my::int32 width)
{
    HEALTH_CHECK(my::IsNull(width), /*false*/);
    HEALTH_CHECK(width <= 0, /*false*/);

    m_width = width;
}

/**
*/
my::int32 my::image::CImage::GetHeight() const
{
    return m_height;
}

/**
*/
void my::image::CImage::SetHeight(my::int32 height)
{
    HEALTH_CHECK(my::IsNull(height), /*false*/);
    HEALTH_CHECK(height <= 0, /*false*/);

    m_height = height;
}

/**
*/
my::int32 my::image::CImage::GetChannelCount() const
{
    return m_channelCount;
}

/**
*/
void my::image::CImage::SetChannelCount(my::int32 channelCount)
{
    HEALTH_CHECK(my::IsNull(channelCount), /*false*/);
    HEALTH_CHECK(channelCount < 3, /*false*/);

    m_channelCount = channelCount;
}

/**
*/
boost::shared_array<unsigned char> my::image::CImage::GetBuffer() const
{
    return m_buffer;
}

// TRICKY: (27-Jan-2016) THE BUFFER IS COPIED!
bool my::image::CImage::SetBuffer(boost::shared_array<unsigned char> buffer)
{
    HEALTH_CHECK(my::IsNull(m_width), false);
    HEALTH_CHECK(m_width <= 0, false);
    HEALTH_CHECK(my::IsNull(m_height), false);
    HEALTH_CHECK(m_height <= 0, false);
    HEALTH_CHECK(my::IsNull(m_channelCount), false);
    HEALTH_CHECK(m_channelCount < 3, false);
    HEALTH_CHECK(!buffer, false);

    m_buffer.reset(new unsigned char[m_channelCount * m_width * m_height]);

    HEALTH_CHECK(!m_buffer, false);

    std::copy_n(buffer.get(), m_channelCount * m_width * m_height, m_buffer.get());

    // TRICKY: (28-Jan-2016) ON-DEMAND ATTRIBUTE.
    m_luminanceArray.clear();

    return true;
}

/**
*/
std::vector<double> my::image::CImage::GetChannel(CHANNEL channel)
{
    std::vector<double> pixelArray;

    switch (channel) {
    case LUMINANCE:
        if (m_luminanceArray.empty())
        {
            if (!UpdateChannel(LUMINANCE))
                LOG_ERROR();
        }

        pixelArray = m_luminanceArray;
        break;

    default:
        LOG_ERROR();
    }

    return pixelArray;
}

/**
*/
template <>
MyColor::CRGB<unsigned char> my::image::CImage::GetPixelRgb(my::int32 x, my::int32 y) const
{
    MyColor::CRGB<unsigned char> emptyObject;

    HEALTH_CHECK(!IsValid(), emptyObject);
    HEALTH_CHECK(x < 0, emptyObject);
    HEALTH_CHECK(x >= m_width, emptyObject);
    HEALTH_CHECK(y < 0, emptyObject);
    HEALTH_CHECK(y >= m_height, emptyObject);

    my::int32 index = m_channelCount * (y * m_width + x);

    const unsigned char *pixel = &m_buffer[0] + index;

    // TRICKY: (31-Mar-2016) ONLY 3 OR 4 CHANNELS ARE ALLOWED

    return MyColor::CRGB<unsigned char>(pixel[0], pixel[1], pixel[2]);
}

/**
*/
template <>
bool my::image::CImage::SetPixelRgb<double>(my::int32 x, my::int32 y, MyColor::CRGB<double> rgb)
{
    HEALTH_CHECK(!IsValid(), false);
    HEALTH_CHECK(x < 0, false);
    HEALTH_CHECK(x >= m_width, false);
    HEALTH_CHECK(y < 0, false);
    HEALTH_CHECK(y >= m_height, false);

    my::int32 index = m_channelCount * (y * m_width + x);

    unsigned char *pixel = &m_buffer[0] + index;

    // TRICKY: (31-Mar-2016) ONLY 3 OR 4 CHANNELS ARE ALLOWED

    pixel[0] = (unsigned char)((my::int32)(255.0 * rgb.GetR()));
    pixel[1] = (unsigned char)((my::int32)(255.0 * rgb.GetG()));
    pixel[2] = (unsigned char)((my::int32)(255.0 * rgb.GetB()));

    return true;
}

/**
*/
template <>
bool my::image::CImage::SetPixelRgb<unsigned char>(my::int32 x, my::int32 y, MyColor::CRGB<unsigned char> rgb)
{
    HEALTH_CHECK(!IsValid(), false);
    HEALTH_CHECK(x < 0, false);
    HEALTH_CHECK(x >= m_width, false);
    HEALTH_CHECK(y < 0, false);
    HEALTH_CHECK(y >= m_height, false);

    my::int32 index = m_channelCount * (y * m_width + x);

    unsigned char *pixel = &m_buffer[0] + index;

    // TRICKY: (31-Mar-2016) ONLY 3 OR 4 CHANNELS ARE ALLOWED

    pixel[0] = rgb.GetR();
    pixel[1] = rgb.GetG();
    pixel[2] = rgb.GetB();

    if (m_channelCount == 4)
        pixel[3] = 255;

    return true;
}

/**
*/
template <>
double my::image::CImage::GetPixelLuminance(my::int32 x, my::int32 y) const
{
    HEALTH_CHECK(!IsValid(), my::Null<double>());
    HEALTH_CHECK(x < 0, my::Null<double>());
    HEALTH_CHECK(x >= m_width, my::Null<double>());
    HEALTH_CHECK(y < 0, my::Null<double>());
    HEALTH_CHECK(y >= m_height, my::Null<double>());
    HEALTH_CHECK(m_channelCount < 3, my::Null<double>());

    my::int32 index = m_channelCount * (y * m_width + x);

    const unsigned char *pixel = &m_buffer[0] + index;

    MyColor::CRGB<double> rgb(pixel[0], pixel[1], pixel[2]);

    return rgb.GetLuminance();
}

/**
*/
template <>
double my::image::CImage::GetPixelInterpolatedLuminance(double x, double y)
{
    HEALTH_CHECK(!IsValid(), my::Null<double>());
    HEALTH_CHECK(x < 0, my::Null<double>());
    HEALTH_CHECK(x >= m_width, my::Null<double>());
    HEALTH_CHECK(y < 0, my::Null<double>());
    HEALTH_CHECK(y >= m_height, my::Null<double>());
    HEALTH_CHECK(m_channelCount < 3, my::Null<double>());

    unsigned char pixel[4] = { 0 };

    if (!BilinearSampling(x, y, m_width, m_height, m_channelCount, m_buffer, pixel))
        return my::Null<double>();

    MyColor::CRGB<double> rgb(pixel[0], pixel[1], pixel[2]);

    return rgb.GetLuminance();
}

/**
*/
bool my::image::CImage::SubImage(my::int32 left, my::int32 bottom, my::int32 right, my::int32 top, boost::shared_ptr<my::image::CImage>& subImage) const
{
    HEALTH_CHECK(!IsValid(), false);

    HEALTH_CHECK(right <= left, false);
    HEALTH_CHECK(top <= bottom, false);

    my::int32 subImageWidth = right - left,
        subImageHeight = top - bottom;

    my::int32 subImageSize = m_channelCount * subImageWidth * subImageHeight;

    boost::shared_array<unsigned char> subImageBuffer(new unsigned char[subImageSize]);

    HEALTH_CHECK(!subImageBuffer, false);

    std::fill_n(subImageBuffer.get(), subImageSize, 0);

    unsigned char *inBuffer = &m_buffer[0],
        *outBuffer = subImageBuffer.get();

    HEALTH_CHECK(!inBuffer, false);
    HEALTH_CHECK(!outBuffer, false);

    for (my::int32 y = bottom; y < top; ++y)
    {
        for (my::int32 x = left; x < right; ++x)
        {
            if ((y >= 0) &&
                (y < m_height) &&
                (x >= 0) &&
                (x < m_width))
            {
                unsigned char *inPixel = inBuffer + m_channelCount * (y * m_width + x),
                    *outPixel = outBuffer + m_channelCount * ((y - bottom) * subImageWidth + (x - left));

                HEALTH_CHECK(!inPixel, false);
                HEALTH_CHECK(!outPixel, false);

                for (my::int32 channelIndex = 0; channelIndex < m_channelCount; ++channelIndex)
                    (*outPixel++) = (*inPixel++);
            }
        }
    }

    subImage.reset(new CImage(subImageWidth, subImageHeight, m_channelCount, subImageBuffer));

    HEALTH_CHECK(!subImage, false);

    return true;
}

// ALL COORDINATES ON IMAGE SPACE ([0, width), [0, height))
bool my::image::CImage::SubImage(double left, double bottom, double right, double top, my::int32 subImageWidth, my::int32 subImageHeight, boost::shared_ptr<my::image::CImage>& subImage)
{
    HEALTH_CHECK(left < 0.0, false);
    HEALTH_CHECK(left >= right, false);
    HEALTH_CHECK(bottom < 0.0, false);
    HEALTH_CHECK(bottom >= top, false);

    HEALTH_CHECK(!IsValid(), false);
    
    my::int32 subImageSize = m_channelCount * subImageWidth * subImageHeight;

    boost::shared_array<unsigned char> buffer(new unsigned char[subImageSize]);

    HEALTH_CHECK(!buffer, false);

    std::fill_n(buffer.get(), subImageSize, 0);

    unsigned char inputPixel[4] = { 0 };

    for (my::int32 y = 0; y < subImageHeight; ++y)
    {
        // TRICKY: (12-Oct-2016) WINDOW VS. IMAGE COORDINATES!
        double t = bottom + ((double)y / subImageHeight) * (top - bottom);

        if ((t >= 0) &&
            (t <= (m_height - 1)))
        {
            for (my::int32 x = 0; x < subImageWidth; ++x)
            {
                // TRICKY: (12-Oct-2016) WINDOW VS. IMAGE COORDINATES!
                double s = left + ((double)x / subImageWidth) * (right - left);

                if ((s >= 0) &&
                    (s <= (m_width - 1)))
                {
                    if (BilinearSampling(s, t, m_width, m_height, m_channelCount, m_buffer, inputPixel))
                    {
                        unsigned char *outPixel = buffer.get() + m_channelCount * (y * subImageWidth + x);

                        for (my::int32 channelIndex = 0; channelIndex < m_channelCount; ++channelIndex)
                            (*outPixel++) = inputPixel[channelIndex];
                    }
                }
            }
        }
    }

    subImage.reset(new CImage(subImageWidth, subImageHeight, m_channelCount, buffer));

    HEALTH_CHECK(!subImage, false);

    return true;
}

std::string my::image::CImage::ToJson() const
{
    return "{}";
}

/**
*/
bool my::image::CImage::IsValid() const
{
    if (my::IsNull(m_width))
        return false;

    if (m_width <= 0)
        return false;

    if (my::IsNull(m_height))
        return false;

    if (m_height <= 0)
        return false;

    if (my::IsNull(m_channelCount))
        return false;

    if (m_channelCount < 3)
        return false;

    if (!m_buffer)
        return false;

    return true;
}

/**
*/
bool my::image::CImage::UpdateChannel(CHANNEL channel)
{
    HEALTH_CHECK(my::IsNull(m_width), false);
    HEALTH_CHECK(m_width <= 0, false);
    HEALTH_CHECK(my::IsNull(m_height), false);
    HEALTH_CHECK(m_height <= 0, false);
    HEALTH_CHECK(my::IsNull(m_channelCount), false);
    HEALTH_CHECK(m_channelCount < 3, false);
    HEALTH_CHECK(!m_buffer, false);

    switch (channel) {
    case LUMINANCE:
    {
        m_luminanceArray.clear();

        const unsigned char *pixel = m_buffer.get();
        my::int32 pixelCount = m_width * m_height;

        for (my::int32 pixelIndex = 0; pixelIndex < pixelCount; ++pixelIndex)
        {
            MyColor::CRGB<double> rgb(pixel[0], pixel[1], pixel[2]);

            // (https://en.wikipedia.org/wiki/Relative_luminance) The formula reflects the luminosity function: green light contributes the most to the intensity perceived by humans, and blue light the least.
            m_luminanceArray.push_back(rgb.GetLuminance());

            pixel += m_channelCount;
        }
    }
    break;

    default:
        LOG_ERROR();

        return false;
    }

    return true;
}

/**
*/
void my::image::CImage::Create()
{
    m_width = my::Null<my::int32>();
    m_height = my::Null<my::int32>();

    m_channelCount = my::Null<my::int32>();

    m_buffer.reset();

    m_luminanceArray.clear();
}

