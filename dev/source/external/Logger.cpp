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

// (BEGIN OF) DEPRECATED: (03-Sep-2019)
//#include <iostream>
//#include <fstream>
//#include <stdexcept> 
//
//#include <Logger.h>
//
//#define MAXIMUM_NUMBER_OF_MESSAGES 500
//
//SINGLETON_DEFINITION(CLogger)
//
///**
//*/
//CLogger::CLogger()
//: m_messageArray(500)
//{
//}
//
///**
//*/
//void CLogger::PushError(char *file, int line)
//{
//    std::string message;
//
//    message = FormatMessage(file, line, "An error has occurred.");
//
//    AddMessage(message);
//}
//
///**
//*/
//void CLogger::PushMessage(char *file, int line, const char *message)
//{
//    std::string entireMessage(message);
//    std::vector<std::string> partialMessageArray;
//    std::string::size_type last = 0,
//		current = 0;
//
//    HEALTH_CHECK(!message, /*false*/);
//
//	// The last element of the string.
//	last = entireMessage.size();
//		
//	// The beginning of the current message.
//	current = entireMessage.rfind('\n');
//		
//	// Walk backward through the string.
//	while (current != std::string::npos)
//	{
//		// Push each message into the vector. Current is incremented 
//		// before copying to avoid copying the delimiter.
//        ++current;
//		partialMessageArray.push_back(entireMessage.substr(current, last - current));
//			
//		// Back over the delimiter we just found, and set last to the end 
//		// of the next message.
//		current -= 2;
//		last = current;
//			
//		// Find the next delimiter
//		current = entireMessage.rfind('\n', current);
//	}
//		
//	// Pick up the first message - it's not preceded by a delimiter.
//	partialMessageArray.push_back(entireMessage.substr(0, last - current));
//
//	while (!partialMessageArray.empty())
//	{
//		if (!partialMessageArray.back().empty())
//            AddMessage(FormatMessage(file, line, partialMessageArray.back().c_str()));
//
//		partialMessageArray.pop_back();
//	}
//}
//
///**
//*/
//void CLogger::PushMessage(char *file, int line, const std::string& message)
//{
//	if (!message.empty())
//        PushMessage(file, line, message.c_str());
//}
//
///**
//*/
//void CLogger::PushMessage(const char *message)
//{
//	if (message)
//	{
//        std::string entireMessage(message);
//        std::vector<std::string> partialMessageArray;
//        std::string::size_type last,
//			current;
//
//		// The last element of the string.
//		last = entireMessage.size();
//		
//		// The beginning of the current message.
//		current = entireMessage.rfind('\n');
//		
//		// Walk backward through the string.
//        while (current != std::string::npos)
//		{
//			// Push each message into the vector. Current is incremented 
//			// before copying to avoid copying the delimiter.
//            ++current;
//			partialMessageArray.push_back(entireMessage.substr(current, last - current));
//			
//			// Back over the delimiter we just found, and set last to the end 
//			// of the next message.
//			current -= 2;
//			last = current;
//			
//			// Find the next delimiter
//			current = entireMessage.rfind('\n', current);
//		}
//		
//		// Pick up the first message - it's not preceded by a delimiter.
//		partialMessageArray.push_back(entireMessage.substr(0, last - current));
//
//		while (!partialMessageArray.empty())
//		{
//			if (!partialMessageArray.back().empty())
//                AddMessage(partialMessageArray.back());
//
//			partialMessageArray.pop_back();
//		}
//	}
//}
//
///**
//*/
//void CLogger::PrintErrors() const
//{
//	int errorCount;
//
//    errorCount = 0;
//
//    for (boost::circular_buffer<std::string>::const_iterator messageIterator = m_messageArray.begin(); messageIterator != m_messageArray.end(); ++messageIterator)
//    {
//        if ((messageIterator->find("error") != std::string::npos) ||
//            (messageIterator->find("Error") != std::string::npos) ||
//            (messageIterator->find("warning") != std::string::npos) ||
//            (messageIterator->find("Warning") != std::string::npos))
//        {
//            std::cout << (*messageIterator) << std::endl;
//
//		    ++errorCount;
//	    }
//    }
//
//	std::cout << errorCount << " errors(s)." << std::endl;
//}
//
///**
//*/
//void CLogger::PrintMessages() const
//{
//    for (boost::circular_buffer<std::string>::const_iterator messageIterator = m_messageArray.begin(); messageIterator != m_messageArray.end(); ++messageIterator)
//    {
//        std::cout << (*messageIterator) << std::endl;
//    }
//
//	std::cout << m_messageArray.size() << " errors(s)." << std::endl;
//}
//
///**
//*/
//void CLogger::PrintMessages(std::vector<std::string>& errorMessageArray) const
//{
//    errorMessageArray.clear();
//
//    for (boost::circular_buffer<std::string>::const_iterator messageIterator = m_messageArray.begin(); messageIterator != m_messageArray.end(); ++messageIterator)
//    {
//        errorMessageArray.push_back(*messageIterator);
//    }
//}
//
///**
//*/
//void CLogger::PrintMessages(const std::string& fileName) const
//{
//	std::ofstream fileStream;
//
//	fileStream.open(fileName.c_str());
//
//    if (fileStream.is_open())
//    {
//        for (boost::circular_buffer<std::string>::const_iterator messageIterator = m_messageArray.begin(); messageIterator != m_messageArray.end(); ++messageIterator)
//        {
//            fileStream << (*messageIterator) << std::endl;
//        }
//
//	    fileStream << m_messageArray.size() << " messages(s)." << std::endl;
//    }
//}
//
///**
//*/
//void CLogger::Clear()
//{
//    m_messageArray.clear();
//}
//
///**
//*/
//bool CLogger::IsOk() const
//{
//	return m_messageArray.empty();
//}
//
///**
//*/
//void CLogger::AddMessage(const std::string& message)
//{
//    m_messageArray.push_back(message);
//}
//
///**
//*/
//std::string CLogger::FormatMessage(char *file, int line, const char *message) const
//{
//    char formattedMessageBuffer[64] = { 0 };
//    std::string formattedMessage;
//
//#ifdef _MSC_VER
//	sprintf_s(formattedMessageBuffer, 64, "%i", line);
//#else // !_MSC_VER
//	sprintf(formattedMessageBuffer, "%i", line);
//#endif // _MSC_VER
//
//	formattedMessage = file;
//	formattedMessage += "(";
//	formattedMessage += formattedMessageBuffer;
//	formattedMessage += "): ";
//
//	if (message)
//		formattedMessage += message;
//	else 
//		formattedMessage += "(empty message)";
//
//	return formattedMessage;
//}
// (END OF) DEPRECATED: (03-Sep-2019)

