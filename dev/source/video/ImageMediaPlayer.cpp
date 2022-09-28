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

#include <boost/lexical_cast.hpp>
#include <opencv2/highgui.hpp>

#include <Logger.h>
#include <Texture.h>

#include "ImageMediaPlayer.h"

CImageMediaPlayer::CImageMediaPlayer()
    : CMediaPlayerInterface()
{
    Create();
}

bool CImageMediaPlayer::OpenUrl(const std::string& url)
{
    HEALTH_CHECK(url.empty(), false);

    cv::Mat frame = cv::imread(url, cv::IMREAD_COLOR);

    HEALTH_CHECK(frame.empty(), false);

    HEALTH_CHECK(frame.type() != CV_8UC3, false);

    boost::shared_array<unsigned char> pixelArray;

    try
    {
        pixelArray.reset(new unsigned char[3 * frame.cols * frame.rows]);
    }
    catch (std::exception& e)
    {
        LOG_MESSAGE(e.what());

        return false;
    }

    unsigned char* pixel = pixelArray.get();

    for (int r = 0; r < frame.rows; ++r) 
    {
        cv::Vec3b* ptr = frame.ptr<cv::Vec3b>(r);

        for (int c = 0; c < frame.cols; ++c) 
        {
            *pixel++ = ptr[c][2];
            *pixel++ = ptr[c][1];
            *pixel++ = ptr[c][0];
        }
    }

    if (!SetWidth(frame.cols))
    {
        LOG_ERROR();

        return false;
    }

    if (!SetHeight(frame.rows))
    {
        LOG_ERROR();

        return false;
    }

    m_pixelArray = pixelArray;

    return true;
}

void CImageMediaPlayer::PreviousFrame()
{
}

void CImageMediaPlayer::TogglePlay()
{
}

bool CImageMediaPlayer::IsPlayable() const
{
    return false;
}

// IS THE INSTANCE CAPABLE OF RETURNING A FRAME RIGHT NOW?
bool CImageMediaPlayer::IsReady() const
{
    return !(!m_pixelArray);
}

void CImageMediaPlayer::NextFrame()
{
}

void CImageMediaPlayer::Stop()
{
}

void CImageMediaPlayer::SetPositionInMilliseconds(my::int64 positionInMilliseconds)
{
    m_positionInMilliseconds = 0;
}

bool CImageMediaPlayer::GetFrame(boost::shared_array<unsigned char>& pixelArray, int& width, int& height, unsigned int& format, unsigned int& type) const
{
    pixelArray = m_pixelArray;

    width = m_width;
    height = m_height;

    format = CTexture::RGB;
    type = CTexture::UNSIGNED_BYTE;

    return true;
}

bool CImageMediaPlayer::SetWidth(int width)
{
    if ((m_width != my::Null<int>()) &&
        (m_width != width))
    {
        LOG_ERROR();

        return false;
    }
    else if (m_width == my::Null<int>())
        m_width = width;

    return true;
}

bool CImageMediaPlayer::SetHeight(int height)
{
    if ((m_height != my::Null<int>()) &&
        (m_height != height))
    {
        LOG_ERROR();

        return false;
    }
    else if (m_height == my::Null<int>())
        m_height = height;

    return true;
}

void CImageMediaPlayer::Create()
{
    m_pixelArray.reset();

    m_width = my::Null<int>();
    m_height = my::Null<int>();
}

