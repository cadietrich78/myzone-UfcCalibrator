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

#if !defined(UFC_CALIBRATOR_MODEL_INCLUDED)
#define UFC_CALIBRATOR_MODEL_INCLUDED

#include <boost/utility.hpp>

#include <vector>
#include <string>

#include <SceneryLayout.h>

#include "Footage.h"

// Model-View-ViewModel (MVVM) - The model is the component of the application that defines and handles all the basic entities of the application. The goal of this layer is to remove any dependency from the way the data is represented. Ideally, you should be able to take the classes that belong to this component and use them in another application without applying any change. For example, if you're working on an application to handle orders and customers of a company, the model could be defined by all the classes which define the base entities, like a customer, an order, a product, etc.
class CUfcCalibratorModel
    : public boost::noncopyable
{
public:
    CUfcCalibratorModel();

    boost::shared_ptr<CFootage> GetFootage();
    bool SetFootage(std::string url);
    
    std::string GetAuditingId() const;
    void SetAuditingId(std::string auditingId);
    
    boost::shared_ptr<my::CSceneryLayout> GetSceneryLayout();

    void Clear();

    bool IsValid() const;

private:
    void Create();
    
protected:
    boost::shared_ptr<CFootage> m_footage;

    std::string m_auditingId;

    boost::shared_ptr<my::CSceneryLayout> m_sceneryLayout;
};

#endif // #if !defined(UFC_CALIBRATOR_MODEL_INCLUDED)

