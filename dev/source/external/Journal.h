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

#if !defined(JOURNAL_INCLUDED)
#define JOURNAL_INCLUDED

#include <string>
#include <vector>

#include "Common.h"

#include <boost/utility.hpp>
#include <boost/any.hpp>

namespace my
{
    class CJournal
        : public boost::noncopyable
    {
    public:
        CJournal();

        void Log(std::string function);

        // ARGn MAY BE MY::INT64, MY::INT32, DOUBLE, FLOAT, STD::STRING, BOOL, CHAR*
        void Log(std::string message, boost::any arg0);
        // ARGn MAY BE MY::INT64, MY::INT32, DOUBLE, FLOAT, STD::STRING, BOOL, CHAR*
        void Log(std::string message, boost::any arg0, boost::any arg1);
        // ARGn MAY BE MY::INT64, MY::INT32, DOUBLE, FLOAT, STD::STRING, BOOL, CHAR*
        void Log(std::string message, boost::any arg0, boost::any arg1, boost::any arg2);

        // COPY OF INTERNAL BUFFER
        std::vector<std::string> GetMessageArray() const;

        std::string GetMessageDirectoryName() const;
        bool SetMessageDirectoryName(std::string messageDirectoryName);

        std::string GetMessageFileName() const;
        bool SetMessageFileName(std::string messageFileName);

    private:
        std::string ToString(boost::any arg) const;

        void AddMessage(std::string message);

        void Create();

    private:
        std::vector<std::string> m_messageArray;

        std::string m_messageDirectoryName;

        std::string m_messageFileName;

        // TRICKY: (??-???-????) THIS COUNTER IS INCREASED EVERY TIME THE BUFFER HITS ITS MAXIMUM SIZE.
        my::int32 m_messageFileNumber;
    };
}

#endif // #if !defined(JOURNAL_INCLUDED)

