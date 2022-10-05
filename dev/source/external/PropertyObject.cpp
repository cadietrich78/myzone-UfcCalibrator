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

#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <boost/lexical_cast.hpp>

#include <Common.h>
#include <Logger.h>

#include <PropertyObject.h>

/**
*/
my::CPropertyObject::CPropertyObject()
{
	Create();
}

/**
*/
my::CPropertyObject::CPropertyObject(const CPropertyObject& propertyObject)
{
	Copy(propertyObject);
}

/**
*/
void my::CPropertyObject::operator=(const CPropertyObject& propertyObject)
{
	Copy(propertyObject);
}


/**
 */
bool my::CPropertyObject::HasProperty(const std::string& name) const
{
    return m_nameToValueMap.find(name) != m_nameToValueMap.end();
}

/**
*/
void my::CPropertyObject::AddPropertyByName(const std::string& name, boost::any value)
{
    m_nameToValueMap[name] = value;
}

/**
 */
boost::any my::CPropertyObject::GetPropertyByName(const std::string& name) const
{
    std::map<std::string, boost::any>::const_iterator nameToValueIterator = m_nameToValueMap.find(name);
    
    if (nameToValueIterator == m_nameToValueMap.end())
        return boost::any();
    
    return nameToValueIterator->second;
}

/**
*/
void my::CPropertyObject::RemovePropertyByName(const std::string& name)
{
    m_nameToValueMap.erase(name);
}

/**
 */
std::string my::CPropertyObject::ToString(const std::string& name) const
{
    std::string valueAsString;
    
    std::map<std::string, boost::any>::const_iterator nameToValueIterator = m_nameToValueMap.find(name);
    
    if (nameToValueIterator == m_nameToValueMap.end())
    {
        LOG_ERROR();
        
        return valueAsString;
    }

    try
    {
        if (nameToValueIterator->second.type() == typeid(INT64))
        {
            INT64 value = boost::any_cast<INT64>(nameToValueIterator->second);
            
            valueAsString = boost::lexical_cast<std::string>(value);
        }
        else if (nameToValueIterator->second.type() == typeid(INT32))
        {
            INT32 value = boost::any_cast<INT32>(nameToValueIterator->second);
            
            valueAsString = boost::lexical_cast<std::string>(value);
        }
        else if (nameToValueIterator->second.type() == typeid(double))
        {
            double value = boost::any_cast<double>(nameToValueIterator->second);
            
            valueAsString = boost::lexical_cast<std::string>(value);
        }
        else if (nameToValueIterator->second.type() == typeid(float))
        {
            float value = boost::any_cast<float>(nameToValueIterator->second);
            
            valueAsString = boost::lexical_cast<std::string>(value);
        }
        else if (nameToValueIterator->second.type() == typeid(std::string))
            valueAsString = boost::any_cast<std::string>(nameToValueIterator->second);
        else if (nameToValueIterator->second.type() == typeid(bool))
        {
            if (boost::any_cast<bool>(nameToValueIterator->second))
                valueAsString = "true";
            else
                valueAsString = "false";
        }
        else if (nameToValueIterator->second.type() == typeid(char *))
        {
            char * value = boost::any_cast<char *>(nameToValueIterator->second);
        
            valueAsString = boost::lexical_cast<std::string>(value);
        }
    }
    catch (...)
    {
        LOG_MESSAGE("Failed to convert " + nameToValueIterator->first + " to string.");
    }

    return valueAsString;
}

/**
*/
INT64 my::CPropertyObject::ToINT64(const std::string& name) const
{
    INT64 valueAsINT64 = my::Null<INT64>();

    std::map<std::string, boost::any>::const_iterator nameToValueIterator = m_nameToValueMap.find(name);

    if (nameToValueIterator == m_nameToValueMap.end())
    {
        LOG_ERROR();

        return valueAsINT64;
    }

    try
    {
        if (nameToValueIterator->second.type() == typeid(INT64))
            valueAsINT64 = boost::any_cast<INT64>(nameToValueIterator->second);
        else if (nameToValueIterator->second.type() == typeid(INT32))
            valueAsINT64 = boost::any_cast<INT32>(nameToValueIterator->second);
    }
    catch (...)
    {
        LOG_MESSAGE("Failed to convert " + nameToValueIterator->first + " to INT64.");
    }

    return valueAsINT64;
}

/**
*/
void my::CPropertyObject::CopyProperties(const CPropertyObject& propertyObject)
{
    Copy(propertyObject);
}

// FOR RAPIDJSON, MAKE SURE THE OBJECT IS OPENED BEFORE THE CALL
template <>
void my::CPropertyObject::ToJson(rapidjson::Writer<rapidjson::StringBuffer>& jsonWriter) const
{
    // From property object
    for (std::map<std::string, boost::any>::const_iterator nameToValueIterator = m_nameToValueMap.begin(); nameToValueIterator != m_nameToValueMap.end(); ++nameToValueIterator)
    {
        std::string name = nameToValueIterator->first,
            valueAsString = my::CPropertyObject::ToString(nameToValueIterator->first);

        if (!my::IsNull(name) &&
            !name.empty() &&
            !my::IsNull(valueAsString) &&
            !valueAsString.empty())
        {
            jsonWriter.String(nameToValueIterator->first.c_str());
            jsonWriter.String(valueAsString.c_str());
        }
    }
}

/**
*/
void my::CPropertyObject::Create()
{
    m_nameToValueMap.clear();
}

/**
*/
void my::CPropertyObject::Copy(const my::CPropertyObject& propertyObject)
{
    m_nameToValueMap = propertyObject.m_nameToValueMap;
}

