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

// Coment this line to link to glew.
//#include <GLee.h>

#if !defined(__glee_h_)
#include <GL/glew.h>

#if defined(_WIN32)
#include <GL/wglew.h>
//#else // defined(_WIN32)
//#include <GL/glxew.h>
#endif // defined(_WIN32)
#endif // !defined(__glee_h_)

#include <Logger.h>
#include <Object.h>

#include "SelectableObject.h"

/**
*/
CSelectableObject::CSelectableObject(int objectType, int objectId) 
{
	SetObjectBitset(objectType, objectId);
}

/**
*/
CSelectableObject::~CSelectableObject() 
{
	//GLint currentColor[4];

	//glGetIntegerv(GL_CURRENT_COLOR, currentColor);

	//if (ColorToObjectBitset((unsigned char)currentColor[0], (unsigned char)currentColor[1], (unsigned char)currentColor[2], (unsigned char)currentColor[3]) != m_objectBitset)
	//	LOG_ERROR();
}

/**
*/
int CSelectableObject::GetObjectBitset() const
{
	return m_objectBitset;
}

/**
*/
void CSelectableObject::SetObjectBitset(int objectType, int objectId) 
{
	m_objectBitset = (objectType << 27) | objectId;
}

/**
*/
int CSelectableObject::GetObjectType() const
{
	int objectType;

	objectType = (m_objectBitset & 4160749568) >> 27;

	if (objectType < CObject::INVALID_TYPE)
	{
		LOG_ERROR();

		return CObject::INVALID_TYPE;
	}

	// 5 bits for type index.
	return objectType;
}

/**
*/
int CSelectableObject::GetObjectId() const
{
	// 27 bits for object index.
	return m_objectBitset & 134217727;
}

/**
*/
void CSelectableObject::Load() const
{
	unsigned char red,
		green,
		blue,
		alpha;

	ObjectBitsetToColor(m_objectBitset, &red, &green, &blue, &alpha);

	glColor4ub(red, green, blue, alpha);
}

/**
*/
int CSelectableObject::GetObjectType(int objectBitset)
{
	int objectType;

	objectType = (objectBitset & 4160749568) >> 27;

	if (objectType < CObject::INVALID_TYPE)
	{
		LOG_ERROR();

		return CObject::INVALID_TYPE;
	}

	// 5 bits for type index.
	return objectType;
}

/**
*/
int CSelectableObject::GetObjectId(int objectBitset)
{
	// 27 bits for object index.
	return objectBitset & 134217727;
}

/**
*/
int CSelectableObject::ColorToObjectBitset(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha)
{
	int objectBitset;

    objectBitset = 0;

	objectBitset |= (int)red << 24;
	objectBitset |= (int)green << 16;
	objectBitset |= (int)blue << 8;
	objectBitset |= (int)alpha;

	return objectBitset;
}

void CSelectableObject::ObjectBitsetToColor(int objectBitset, unsigned char *red, unsigned char *green, unsigned char *blue, unsigned char *alpha)
{
	*red = (unsigned char)((objectBitset >> 24) & 255);
	*green = (unsigned char)((objectBitset >> 16) & 255);
	*blue = (unsigned char)((objectBitset >> 8) & 255);
	*alpha = (unsigned char)(objectBitset & 255);
}

