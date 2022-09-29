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

#if !defined(UFC_CALIBRATOR_VIEW_MODEL_INCLUDED)
#define UFC_CALIBRATOR_VIEW_MODEL_INCLUDED

#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp> 

#include <Common.h>
#include <PinholeCamera2.h>
#include <JournalObject.h>

#include "../ObjectType.h"
#include "../model/UfcCalibratorModel.h"
#include "../model/PlayerInterface.h"
#include "MessageQueue.h"

class CUfcCalibratorViewModel
    : public my::CJournalObject
{
    SINGLETON_DECLARATION(CUfcCalibratorViewModel)

public:
    ~CUfcCalibratorViewModel();

    void SetMessageQueue(boost::shared_ptr<CMessageQueue> messageQueue);

    boost::shared_ptr<CUfcCalibratorModel> GetUfcCalibratorModel();

    bool OpenFromUrl(std::string url, bool createPlay = true);

    bool ClosePlay();

    template < typename T >
    T GetAttribute(OBJECT_TYPE objectType) const;
    void SetAttribute(OBJECT_TYPE objectType, boost::any value);

    boost::shared_ptr<CPinholeCamera2> GetPinholeCamera();
    bool SetPinholeCamera(boost::shared_ptr<CPinholeCamera2> pinholeCamera);

    boost::shared_ptr<CPlayerInterface> GetPlayerInterface();

    void RegisterUpdateCallback(boost::function<void()> updateCallback = 0);

    bool IsReady(std::string playId) const;

    bool SaveCameraSettings(std::string cameraFileName);

private:
    bool OpenSettings();
    bool SaveSettings();

    bool OpenExtrinsicSettings();
    bool SaveExtrinsicSettings();

    bool OpenCameraSettings() const;
    bool SaveCameraSettings();

    bool OpenUserSettings();
    bool OpenUserSettings(std::string userSettingsJsonString);

public:
    bool SaveUserSettings() const;

private:
    void Create();
    
protected:
    boost::shared_ptr<CMessageQueue> m_messageQueue;

    boost::shared_ptr<CUfcCalibratorModel> m_ufcCalibratorModel;

    boost::shared_ptr<CPinholeCamera2> m_pinholeCamera;

    std::map<int, boost::any> m_objectTypeToValueMap;

    boost::function<void()> m_updateCallback;
};

#endif // #if !defined(UFC_CALIBRATOR_VIEW_MODEL_INCLUDED)

