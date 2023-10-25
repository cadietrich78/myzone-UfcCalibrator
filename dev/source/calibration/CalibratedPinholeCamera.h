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

#if !defined(CALIBRATED_PINHOLE_CAMERA_INCLUDED)
#define CALIBRATED_PINHOLE_CAMERA_INCLUDED

#include <opencv2/opencv.hpp>

#include <PinholeCamera2.h>
#include <Timestamp.h>

class CCalibratedPinholeCamera
    : public CPinholeCamera2
{
public:
    CCalibratedPinholeCamera();
    CCalibratedPinholeCamera(const CCalibratedPinholeCamera& calibratedPinholeCamera);
    CCalibratedPinholeCamera(const CPinholeCamera2& pinholeCamera, const cv::Mat& cameraMatrix, const cv::Mat& distortionCoefficientArray);

    CCalibratedPinholeCamera& operator=(const CCalibratedPinholeCamera& calibratedPinholeCamera);

    bool FromOpenCvFile(std::string openCvCameraFileName);

    bool ToPinholeCameraFile(std::string pinholeCameraFileName);
    bool FromPinholeCameraFile(std::string pinholeCameraFileName);

    my::CTimestamp GetTimestamp() const;
    void SetTimestamp(my::CTimestamp timestamp);

    cv::Mat GetCameraMatrix() const;
    void SetCameraMatrix(const cv::Mat& cameraMatrix);

    cv::Mat GetDistortionCoefficientArray() const;
    void SetDistortionCoefficientArray(const cv::Mat& distortionCoefficientArray);

private:
    void Copy(const CCalibratedPinholeCamera& calibratedPinholeCamera);

protected:
    my::CTimestamp m_timestamp;

    /// <summary>
    /// (0,0) focal length (f_x)
    /// (0,2) optical center expressed in pixel coordinates (c_x)
    /// (1,1) focal length (f_y)
    /// (1,2) optical center expressed in pixel coordinates (c_y)
    /// </summary>
    cv::Mat m_cameraMatrix;

    cv::Mat m_distortionCoefficientArray;
};

#endif //#if !defined(CALIBRATED_PINHOLE_CAMERA_INCLUDED)
