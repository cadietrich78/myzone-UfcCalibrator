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

#include <Logger.h>

#include "SelectableObject.h"

#include "SelectionMechanism.h"

SINGLETON_DEFINITION(CSelectionMechanism)

/**
*/
CSelectionMechanism::CSelectionMechanism()
    : m_isEnabled(false),
    m_selectedObjectBitset(CObject::INVALID_TYPE),
    m_highlighedObjectBitset(CObject::INVALID_TYPE)
{
}

/**
*/
void CSelectionMechanism::Enable()
{
	if (m_isEnabled)
		LOG_ERROR();

	m_isEnabled = true;
}

/**
*/
void CSelectionMechanism::Disable()
{
	if (!m_isEnabled)
		LOG_ERROR();

	m_isEnabled = false;
}

/**
*/
bool CSelectionMechanism::IsEnabled() const
{
	return m_isEnabled;
}

/**
*/
int CSelectionMechanism::GetSelectedObjectBitset() const
{
    return m_selectedObjectBitset;
}

/**
*/
void CSelectionMechanism::SetSelectedObjectBitset(int selectedObjectBitset)
{
    m_selectedObjectBitset = selectedObjectBitset;
}

/**
*/
int CSelectionMechanism::GetHighlightedObjectBitset() const
{
    return m_highlighedObjectBitset;
}

/**
*/
void CSelectionMechanism::SetHighlightedObjectBitset(int highlighedObjectBitset)
{
    m_highlighedObjectBitset = highlighedObjectBitset;
}

/**
*/
void CSelectionMechanism::Clear()
{
    m_selectedObjectBitset = CObject::INVALID_TYPE;
}

/**
*/
bool CSelectionMechanism::IsSomethingSelected() const
{
    return m_selectedObjectBitset != CObject::INVALID_TYPE;
}

/**
*/
int CSelectionMechanism::GetSelectedObjectType() const
{
    if (m_selectedObjectBitset != CObject::INVALID_TYPE)
        return CSelectableObject::GetObjectType(m_selectedObjectBitset);

    return CObject::INVALID_TYPE;
}

/**
*/
int CSelectionMechanism::GetSelectedObjectId() const
{
    if (m_selectedObjectBitset != CObject::INVALID_TYPE)
        return CSelectableObject::GetObjectId(m_selectedObjectBitset);

    return CObject::INVALID_ID;
}


/**
*/
int CSelectionMechanism::GetHighlightedObjectType() const
{
    if (m_highlighedObjectBitset != CObject::INVALID_TYPE)
        return CSelectableObject::GetObjectType(m_highlighedObjectBitset);

    return CObject::INVALID_TYPE;
}

/**
*/
int CSelectionMechanism::GetHighlightedObjectId() const
{
    if (m_highlighedObjectBitset != CObject::INVALID_TYPE)
        return CSelectableObject::GetObjectId(m_highlighedObjectBitset);

    return CObject::INVALID_ID;
}
