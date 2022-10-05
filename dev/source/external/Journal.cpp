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

#include <Timestamp.h>

#include <Journal.h>

my::CJournal::CJournal()
{
    Create();
}

void my::CJournal::Log(std::string message)
{
    AddMessage(message);
}

// ARGn MAY BE MY::INT64, MY::INT32, DOUBLE, FLOAT, STD::STRING, BOOL, CHAR*
void my::CJournal::Log(std::string message, boost::any arg0)
{
    message += " ARG0: "
        + ToString(arg0);

    AddMessage(message);
}

// ARGn MAY BE MY::INT64, MY::INT32, DOUBLE, FLOAT, STD::STRING, BOOL, CHAR*
void my::CJournal::Log(std::string message, boost::any arg0, boost::any arg1)
{
    message += " ARG0: "
        + ToString(arg0)
        + " ARG1: "
        + ToString(arg1);

    AddMessage(message);
}

// ARGn MAY BE MY::INT64, MY::INT32, DOUBLE, FLOAT, STD::STRING, BOOL, CHAR*
void my::CJournal::Log(std::string message, boost::any arg0, boost::any arg1, boost::any arg2)
{
    message += " ARG0: "
        + ToString(arg0)
        + " ARG1: "
        + ToString(arg1)
        + " ARG2: "
        + ToString(arg2);

    AddMessage(message);
}

// COPY OF INTERNAL BUFFER
std::vector<std::string> my::CJournal::GetMessageArray() const
{
    return m_messageArray;
}

std::string my::CJournal::GetMessageDirectoryName() const
{
    return m_messageDirectoryName;
}

bool my::CJournal::SetMessageDirectoryName(std::string messageDirectoryName)
{
    if (!messageDirectoryName.empty() &&
        (m_messageFileNumber != 0))
    {
        return false;
    }

    if (!messageDirectoryName.empty())
        m_messageDirectoryName = my::AddTrailingSlash(messageDirectoryName);

    return true;
}

std::string my::CJournal::GetMessageFileName() const
{
    if (my::IsNull(m_messageFileName))
    {
        std::string currentTimeAsString = my::CTimestamp::Now().ToString();

        // REMOVE INVALID CHARACTERS FROM FILE NAME
        currentTimeAsString.replace(currentTimeAsString.find(":"), 1, "h");
        // REMOVE INVALID CHARACTERS FROM FILE NAME
        currentTimeAsString.replace(currentTimeAsString.find(":"), 1, "m");

        std::string messageFileName = m_messageDirectoryName
            + "LOG "
            + currentTimeAsString
            + "-"
            + my::NumberToString(m_messageFileNumber)
            + ".txt";

        return messageFileName;
    }

    return m_messageFileName;
}

bool my::CJournal::SetMessageFileName(std::string messageFileName)
{
    if (messageFileName.empty())
        return false;

    m_messageFileName = messageFileName;

    return true;
}

std::string my::CJournal::ToString(boost::any arg) const
{
    std::string valueAsString = my::Null<std::string>();

    try
    {
        if (arg.type() == typeid(my::int64))
        {
            my::int64 value = boost::any_cast<my::int64>(arg);

            valueAsString = boost::lexical_cast<std::string>(value);
        }
        else if (arg.type() == typeid(my::int32))
        {
            my::int32 value = boost::any_cast<my::int32>(arg);

            valueAsString = boost::lexical_cast<std::string>(value);
        }
        else if (arg.type() == typeid(double))
        {
            double value = boost::any_cast<double>(arg);

            valueAsString = boost::lexical_cast<std::string>(value);
        }
        else if (arg.type() == typeid(float))
        {
            float value = boost::any_cast<float>(arg);

            valueAsString = boost::lexical_cast<std::string>(value);
        }
        else if (arg.type() == typeid(std::string))
            valueAsString = boost::any_cast<std::string>(arg);
        else if (arg.type() == typeid(bool))
        {
            if (boost::any_cast<bool>(arg))
                valueAsString = "true";
            else
                valueAsString = "false";
        }
        else if (arg.type() == typeid(char *))
        {
            char * value = boost::any_cast<char *>(arg);

            valueAsString = boost::lexical_cast<std::string>(value);
        }
    }
    catch (...)
    {
        // TRICKY: (04-AUG-2016) DO NOT CALL LOGGER (IT IS A SINGLETON, IT MIGHT BE DEAD)
    }

    return valueAsString;
}

/**
*/
void my::CJournal::AddMessage(std::string message)
{
    if (message.empty())
    {
        // TRICKY: (04-AUG-2016) DO NOT CALL LOGGER (IT IS A SINGLETON, IT MIGHT BE DEAD)
        //LOG_ERROR();

        return /*false*/;
    }

    m_messageArray.push_back(message);

    std::string messageFileName = GetMessageFileName();

    std::ofstream messageFileStream(messageFileName, std::ofstream::out | std::ofstream::app);

    if (messageFileStream)
        messageFileStream << message << std::endl;

    // TODO: (14-Feb-2016) SET AN APPROPRIATE VALUE
    if (m_messageArray.size() >= 512)
    {
        ++m_messageFileNumber;

        m_messageArray.clear();
    }
}

void my::CJournal::Create()
{
    // TODO: (14-Feb-2016) SET AN APPROPRIATE VALUE
    m_messageArray.reserve(512);
    m_messageDirectoryName.clear();
    m_messageFileName.clear();
    m_messageFileNumber = 0;
}

