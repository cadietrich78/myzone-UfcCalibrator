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

#include "OfficeSceneryLayout.h"

my::sport::COfficeSceneryLayout::COfficeSceneryLayout()
    : CSceneryLayout()
{
    m_mode = 25;
    m_layoutMesh.reset();
}

my::CVector3<double> my::sport::COfficeSceneryLayout::GetItemPosition(my::int64 item, int venueId) const
{
    my::CVector3<double> position;

    switch (item) {
    case LEFT_TOP_VERTEX_ITEM:
        position.Set(0.0, 0.0, 1.453412);
        break;
    case LEFT_CENTER_VERTEX_ITEM:
        position.Set(0.0, 0.0, 0.144357);
        break;
    case LEFT_BOTTOM_VERTEX_ITEM:
        position.Set(0.649606, 0.0, 0.0);
        break;
    case RIGHT_TOP_VERTEX_ITEM:
        position.Set(0.0, 4.937664, 1.87664);
        break;
    case RIGHT_CENTER_VERTEX_ITEM:
        position.Set(0.0, 4.986877, 0.433071);
        break;
    case RIGHT_BOTTOM_VERTEX_ITEM:
        position.Set(0.66273, 4.970472, 0.0);
        break;
    }

    return position;
}

double my::sport::COfficeSceneryLayout::GetItemLength(my::int64 item, int venueId) const
{
    double length = my::Null<double>();

    return length;
}

double my::sport::COfficeSceneryLayout::GetDistanceToItem(double x, double y, double z, my::int64 item) const
{
    LOG_ERROR();

    return my::Null<double>();
}

double my::sport::COfficeSceneryLayout::GetDistanceToItem(my::CVector3<double> position, my::int64 item) const
{
    LOG_ERROR();

    return my::Null<double>();
}

std::string my::sport::COfficeSceneryLayout::GetItemName(my::int64 item) const
{
    LOG_ERROR();

    return my::Null<std::string>();
}

// TRICKY: (??-???-????) BOUNDING BOX OF ALL VENUES
CBoundingBox my::sport::COfficeSceneryLayout::GetBoundingBox() const
{
    CBoundingBox boundingBox;

    boundingBox.AddVertex(GetItemPosition(LEFT_TOP_VERTEX_ITEM));
    boundingBox.AddVertex(GetItemPosition(LEFT_CENTER_VERTEX_ITEM));
    boundingBox.AddVertex(GetItemPosition(LEFT_BOTTOM_VERTEX_ITEM));
    boundingBox.AddVertex(GetItemPosition(RIGHT_TOP_VERTEX_ITEM));
    boundingBox.AddVertex(GetItemPosition(RIGHT_CENTER_VERTEX_ITEM));
    boundingBox.AddVertex(GetItemPosition(RIGHT_BOTTOM_VERTEX_ITEM));

    return boundingBox;
}

boost::shared_ptr<CVertexPool> my::sport::COfficeSceneryLayout::GetLayoutMesh()
{
    // HELPER (IT ONLY MAKES SENSE HERE)
#define ADD_LINE {\
    int vertexIndex = m_layoutMesh->GetVertexCount() - 2;\
    m_layoutMesh->AddLine(vertexIndex, vertexIndex + 1);\
    m_layoutMesh->AddLine(vertexIndex + 1, vertexIndex);\
    }

    if (!m_layoutMesh)
    {
        m_layoutMesh.reset(new CVertexPool);

        HEALTH_CHECK(!m_layoutMesh, m_layoutMesh);

        my::CVector3<double> leftTopVertex = GetItemPosition(LEFT_TOP_VERTEX_ITEM),
            leftCenterVertex = GetItemPosition(LEFT_CENTER_VERTEX_ITEM),
            leftBottomVertex = GetItemPosition(LEFT_BOTTOM_VERTEX_ITEM),
            rightTopVertex = GetItemPosition(RIGHT_TOP_VERTEX_ITEM),
            rightCenterVertex = GetItemPosition(RIGHT_CENTER_VERTEX_ITEM),
            rightBottomVertex = GetItemPosition(RIGHT_BOTTOM_VERTEX_ITEM);

        m_layoutMesh->AddColor(1.0, 0.0, 0.0, 1.0);
        m_layoutMesh->AddVertex(leftTopVertex.x(), leftTopVertex.y(), leftTopVertex.z());
        m_layoutMesh->AddColor(1.0, 0.0, 0.0, 1.0);
        m_layoutMesh->AddVertex(leftCenterVertex.x(), leftCenterVertex.y(), leftCenterVertex.z());
        ADD_LINE;
        m_layoutMesh->AddColor(1.0, 0.0, 0.0, 1.0);
        m_layoutMesh->AddVertex(leftCenterVertex.x(), leftCenterVertex.y(), leftCenterVertex.z());
        m_layoutMesh->AddColor(1.0, 0.0, 0.0, 1.0);
        m_layoutMesh->AddVertex(leftBottomVertex.x(), leftBottomVertex.y(), leftBottomVertex.z());
        ADD_LINE;

        m_layoutMesh->AddColor(0.0, 1.0, 0.0, 1.0);
        m_layoutMesh->AddVertex(rightTopVertex.x(), rightTopVertex.y(), rightTopVertex.z());
        m_layoutMesh->AddColor(0.0, 1.0, 0.0, 1.0);
        m_layoutMesh->AddVertex(rightCenterVertex.x(), rightCenterVertex.y(), rightCenterVertex.z());
        ADD_LINE;
        m_layoutMesh->AddColor(0.0, 1.0, 0.0, 1.0);
        m_layoutMesh->AddVertex(rightCenterVertex.x(), rightCenterVertex.y(), rightCenterVertex.z());
        m_layoutMesh->AddColor(0.0, 1.0, 0.0, 1.0);
        m_layoutMesh->AddVertex(rightBottomVertex.x(), rightBottomVertex.y(), rightBottomVertex.z());
        ADD_LINE;

        m_layoutMesh->AddColor(1.0, 1.0, 0.0, 1.0);
        m_layoutMesh->AddVertex(leftCenterVertex.x(), leftCenterVertex.y(), leftCenterVertex.z());
        m_layoutMesh->AddColor(1.0, 1.0, 0.0, 1.0);
        m_layoutMesh->AddVertex(rightCenterVertex.x(), rightCenterVertex.y(), rightCenterVertex.z());
        ADD_LINE;
    }

    return m_layoutMesh;
}
