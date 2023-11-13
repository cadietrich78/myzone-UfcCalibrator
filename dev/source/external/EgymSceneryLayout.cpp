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
#include <MyMath.h>
#include <UnitConversion.h>

#include "EgymSceneryLayout.h"

my::sport::CEgymSceneryLayout::CEgymSceneryLayout()
    : CSceneryLayout()
{
    m_layoutMesh.reset();
}

my::CVector3<double> my::sport::CEgymSceneryLayout::GetItemPosition(my::int64 item, int venueId) const
{
    my::CVector3<double> position;

    switch (item) {
    case CUSTOM_0_ITEM:
        position.Set(0.0, 0.0, 0.0);
        break;
    case CUSTOM_1_ITEM:
        position.Set(-2.55906, -7.48031, 0.0);
        break;
    case CUSTOM_2_ITEM:
        position.Set(14.96063, 0.0, 0.0);
        break;
    case CUSTOM_3_ITEM:
        position.Set(7.48031, 4.98688, 0.0);
        break;
    case CUSTOM_4_ITEM:
        position.Set(11.02362, 4.98688, 0.0);
        break;
    }

    return position;
}

double my::sport::CEgymSceneryLayout::GetItemLength(my::int64 item, int venueId) const
{
    double length = my::Null<double>();

    // NONE

    return length;
}

double my::sport::CEgymSceneryLayout::GetDistanceToItem(double x, double y, double z, my::int64 item) const
{
    LOG_ERROR();

    return my::Null<double>();
}

double my::sport::CEgymSceneryLayout::GetDistanceToItem(my::CVector3<double> position, my::int64 item) const
{
    LOG_ERROR();

    return my::Null<double>();
}

std::string my::sport::CEgymSceneryLayout::GetItemName(my::int64 item) const
{
    LOG_ERROR();

    return my::Null<std::string>();
}

// TRICKY: (??-???-????) BOUNDING BOX OF ALL VENUES
CBoundingBox my::sport::CEgymSceneryLayout::GetBoundingBox() const
{
    CBoundingBox boundingBox;

    boundingBox.AddVertex(GetItemPosition(CUSTOM_0_ITEM));
    boundingBox.AddVertex(GetItemPosition(CUSTOM_1_ITEM));
    boundingBox.AddVertex(GetItemPosition(CUSTOM_2_ITEM));
    boundingBox.AddVertex(GetItemPosition(CUSTOM_3_ITEM));
    boundingBox.AddVertex(GetItemPosition(CUSTOM_4_ITEM));

    return boundingBox;
}

boost::shared_ptr<CVertexPool> my::sport::CEgymSceneryLayout::GetLayoutMesh()
{
    // HELPER (IT ONLY MAKES SENSE HERE)
#define ADD_LINE {\
    int vertexIndex = m_layoutMesh->GetVertexCount() - 2;\
    m_layoutMesh->AddLine(vertexIndex, vertexIndex + 1);\
    }

    if (!m_layoutMesh)
    {
        m_layoutMesh.reset(new CVertexPool);

        HEALTH_CHECK(!m_layoutMesh, m_layoutMesh);

        my::CVector3<double> custom0 = GetItemPosition(CUSTOM_0_ITEM),
            custom1 = GetItemPosition(CUSTOM_1_ITEM),
            custom2 = GetItemPosition(CUSTOM_2_ITEM),
            custom3 = GetItemPosition(CUSTOM_3_ITEM),
            custom4 = GetItemPosition(CUSTOM_4_ITEM);

        m_layoutMesh->AddColor(1.0, 0.0, 0.0, 1.0);
        m_layoutMesh->AddVertex(custom0.x(), custom0.y(), custom0.z());
        m_layoutMesh->AddColor(1.0, 0.0, 0.0, 1.0);
        m_layoutMesh->AddVertex(custom1.x(), custom1.y(), custom1.z());
        ADD_LINE;
        m_layoutMesh->AddColor(1.0, 0.0, 0.0, 1.0);
        m_layoutMesh->AddVertex(custom0.x(), custom0.y(), custom0.z());
        m_layoutMesh->AddColor(1.0, 0.0, 0.0, 1.0);
        m_layoutMesh->AddVertex(custom2.x(), custom2.y(), custom2.z());
        ADD_LINE;

        m_layoutMesh->AddColor(1.0, 0.0, 0.0, 1.0);
        m_layoutMesh->AddVertex(custom3.x(), custom3.y(), custom3.z());
        m_layoutMesh->AddColor(1.0, 0.0, 0.0, 1.0);
        m_layoutMesh->AddVertex(custom4.x(), custom4.y(), custom4.z());
        ADD_LINE;
    }

    return m_layoutMesh;
}
