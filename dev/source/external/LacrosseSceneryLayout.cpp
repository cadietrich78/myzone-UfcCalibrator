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

#include "LacrosseSceneryLayout.h"

my::sport::CLacrosseSceneryLayout::CLacrosseSceneryLayout()
    : CSceneryLayout()
{
    m_layoutMesh.reset();
}

my::CVector3<double> my::sport::CLacrosseSceneryLayout::GetItemPosition(my::int64 item, int venueId) const
{
    my::CVector3<double> position;

    double fieldHalfLength = UnitConversion::Yard2Feet(100.0) / 2.0,
        fieldHalfWidth = UnitConversion::Yard2Feet(60.0) / 2.0,
        areaHalfWidth = UnitConversion::Yard2Feet(26.0/*30.0*/) / 2.0;

    switch (item) {
        case CENTER_BOTTOM_VERTEX_ITEM:
            position.Set(0.0, -fieldHalfWidth, 0.0);
            break;
        case CENTER_TOP_VERTEX_ITEM:
            position.Set(0.0, fieldHalfWidth, 0.0);
            break;
        case CENTER_WING_BOTTOM_VERTEX_ITEM:
            position.Set(0.0, UnitConversion::Yard2Feet(-19.0), 0.0);
            break;
        case CENTER_WING_TOP_VERTEX_ITEM:
            position.Set(0.0, UnitConversion::Yard2Feet(19.0), 0.0);
            break;
        case LEFT_BOTTOM_VERTEX_ITEM:
            position.Set(-fieldHalfLength, -fieldHalfWidth, 0.0);
            break;
        case LEFT_CREASE_CENTER_VERTEX_ITEM:
            position.Set(-fieldHalfLength + UnitConversion::Yard2Feet(15.0), 0.0, 0.0);
            break;
        case LEFT_END_AREA_BOTTOM_VERTEX_ITEM:
            position.Set(-fieldHalfLength, -areaHalfWidth, 0.0);
            break;
        case LEFT_END_AREA_TOP_VERTEX_ITEM:
            position.Set(-fieldHalfLength, areaHalfWidth, 0.0);
            break;
        case LEFT_TOP_VERTEX_ITEM:
            position.Set(-fieldHalfLength, fieldHalfWidth, 0.0);
            break;
        case LEFT_WING_BOTTOM_VERTEX_ITEM:
            position.Set(UnitConversion::Yard2Feet(-10.0), UnitConversion::Yard2Feet(-19.0), 0.0);
            break;
        case LEFT_WING_TOP_VERTEX_ITEM:
            position.Set(UnitConversion::Yard2Feet(-10.0), UnitConversion::Yard2Feet(19.0), 0.0);
            break;
        case RIGHT_BOTTOM_VERTEX_ITEM:
            position.Set(fieldHalfLength, -fieldHalfWidth, 0.0);
            break;
        case RIGHT_CREASE_CENTER_VERTEX_ITEM:
            position.Set(fieldHalfLength - UnitConversion::Yard2Feet(15.0), 0.0, 0.0);
            break;
        case RIGHT_END_AREA_BOTTOM_VERTEX_ITEM:
            position.Set(fieldHalfLength, -areaHalfWidth, 0.0);
            break;
        case RIGHT_END_AREA_TOP_VERTEX_ITEM:
            position.Set(fieldHalfLength, areaHalfWidth, 0.0);
            break;
        case RIGHT_TOP_VERTEX_ITEM:
            position.Set(fieldHalfLength, fieldHalfWidth, 0.0);
            break;
        case RIGHT_WING_BOTTOM_VERTEX_ITEM:
            position.Set(UnitConversion::Yard2Feet(10.0), UnitConversion::Yard2Feet(-19.0), 0.0);
            break;
        case RIGHT_WING_TOP_VERTEX_ITEM:
            position.Set(UnitConversion::Yard2Feet(10.0), UnitConversion::Yard2Feet(19.0), 0.0);
            break;
    }

    return position;
}

double my::sport::CLacrosseSceneryLayout::GetItemLength(my::int64 item, int venueId) const
{
    double length = my::Null<double>();

    return length;
}

double my::sport::CLacrosseSceneryLayout::GetDistanceToItem(double x, double y, double z, my::int64 item) const
{
    LOG_ERROR();

    return my::Null<double>();
}

double my::sport::CLacrosseSceneryLayout::GetDistanceToItem(my::CVector3<double> position, my::int64 item) const
{
    LOG_ERROR();

    return my::Null<double>();
}

std::string my::sport::CLacrosseSceneryLayout::GetItemName(my::int64 item) const
{
    LOG_ERROR();

    return my::Null<std::string>();
}

// TRICKY: (??-???-????) BOUNDING BOX OF ALL VENUES
CBoundingBox my::sport::CLacrosseSceneryLayout::GetBoundingBox() const
{
    CBoundingBox boundingBox;

    boundingBox.AddVertex(GetItemPosition(LEFT_BOTTOM_VERTEX_ITEM));
    boundingBox.AddVertex(GetItemPosition(LEFT_TOP_VERTEX_ITEM));
    boundingBox.AddVertex(GetItemPosition(RIGHT_BOTTOM_VERTEX_ITEM));
    boundingBox.AddVertex(GetItemPosition(RIGHT_TOP_VERTEX_ITEM));

    return boundingBox;
}

boost::shared_ptr<CVertexPool> my::sport::CLacrosseSceneryLayout::GetLayoutMesh()
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

        my::CVector3<double> centerBottom = GetItemPosition(CENTER_BOTTOM_VERTEX_ITEM),
            centerTop = GetItemPosition(CENTER_TOP_VERTEX_ITEM),
            leftBottom = GetItemPosition(LEFT_BOTTOM_VERTEX_ITEM),
            leftCrease = GetItemPosition(LEFT_CREASE_CENTER_VERTEX_ITEM),
            leftEndAreaBottom = GetItemPosition(LEFT_END_AREA_BOTTOM_VERTEX_ITEM),
            leftEndAreaTop = GetItemPosition(LEFT_END_AREA_TOP_VERTEX_ITEM),
            leftTop = GetItemPosition(LEFT_TOP_VERTEX_ITEM),
            leftWingBottom = GetItemPosition(LEFT_WING_BOTTOM_VERTEX_ITEM),
            leftWingTop = GetItemPosition(LEFT_WING_TOP_VERTEX_ITEM),
            rightBottom = GetItemPosition(RIGHT_BOTTOM_VERTEX_ITEM),
            rightCrease = GetItemPosition(RIGHT_CREASE_CENTER_VERTEX_ITEM),
            rightEndAreaBottom = GetItemPosition(RIGHT_END_AREA_BOTTOM_VERTEX_ITEM),
            rightEndAreaTop = GetItemPosition(RIGHT_END_AREA_TOP_VERTEX_ITEM),
            rightTop = GetItemPosition(RIGHT_TOP_VERTEX_ITEM),
            rightWingBottom = GetItemPosition(RIGHT_WING_BOTTOM_VERTEX_ITEM),
            rightWingTop = GetItemPosition(RIGHT_WING_TOP_VERTEX_ITEM);

        // RIGHT AREA

        m_layoutMesh->AddColor(1.0, 1.0, 0.0, 1.0);
        m_layoutMesh->AddVertex(rightEndAreaTop.x(), rightEndAreaTop.y(), rightEndAreaTop.z());
        m_layoutMesh->AddColor(1.0, 1.0, 0.0, 1.0);
        m_layoutMesh->AddVertex(rightCrease.x(), rightEndAreaTop.y(), rightEndAreaTop.z());
        ADD_LINE;
        m_layoutMesh->AddColor(1.0, 1.0, 0.0, 1.0);
        m_layoutMesh->AddVertex(rightEndAreaBottom.x(), rightEndAreaBottom.y(), rightEndAreaBottom.z());
        m_layoutMesh->AddColor(1.0, 1.0, 0.0, 1.0);
        m_layoutMesh->AddVertex(rightCrease.x(), rightEndAreaBottom.y(), rightEndAreaBottom.z());
        ADD_LINE;
        m_layoutMesh->AddColor(1.0, 1.0, 0.0, 1.0);
        m_layoutMesh->AddVertex(rightCrease.x(), rightEndAreaTop.y(), rightEndAreaTop.z());
        m_layoutMesh->AddColor(1.0, 1.0, 0.0, 1.0);
        m_layoutMesh->AddVertex(rightCrease.x(), rightEndAreaBottom.y(), rightEndAreaBottom.z());
        ADD_LINE;

        // LEFT AREA

        m_layoutMesh->AddColor(1.0, 1.0, 0.0, 1.0);
        m_layoutMesh->AddVertex(leftEndAreaTop.x(), leftEndAreaTop.y(), leftEndAreaTop.z());
        m_layoutMesh->AddColor(1.0, 1.0, 0.0, 1.0);
        m_layoutMesh->AddVertex(leftCrease.x(), leftEndAreaTop.y(), leftEndAreaTop.z());
        ADD_LINE;
        m_layoutMesh->AddColor(1.0, 1.0, 0.0, 1.0);
        m_layoutMesh->AddVertex(leftEndAreaBottom.x(), leftEndAreaBottom.y(), leftEndAreaBottom.z());
        m_layoutMesh->AddColor(1.0, 1.0, 0.0, 1.0);
        m_layoutMesh->AddVertex(leftCrease.x(), leftEndAreaBottom.y(), leftEndAreaBottom.z());
        ADD_LINE;
        m_layoutMesh->AddColor(1.0, 1.0, 0.0, 1.0);
        m_layoutMesh->AddVertex(leftCrease.x(), leftEndAreaTop.y(), leftEndAreaTop.z());
        m_layoutMesh->AddColor(1.0, 1.0, 0.0, 1.0);
        m_layoutMesh->AddVertex(leftCrease.x(), leftEndAreaBottom.y(), leftEndAreaBottom.z());
        ADD_LINE;

        // WING AREA

        m_layoutMesh->AddColor(1.0, 1.0, 0.0, 1.0);
        m_layoutMesh->AddVertex(leftWingBottom.x(), leftWingBottom.y(), leftWingBottom.z());
        m_layoutMesh->AddColor(1.0, 1.0, 0.0, 1.0);
        m_layoutMesh->AddVertex(rightWingBottom.x(), rightWingBottom.y(), rightWingBottom.z());
        ADD_LINE;
        m_layoutMesh->AddColor(1.0, 1.0, 0.0, 1.0);
        m_layoutMesh->AddVertex(leftWingTop.x(), leftWingTop.y(), leftWingTop.z());
        m_layoutMesh->AddColor(1.0, 1.0, 0.0, 1.0);
        m_layoutMesh->AddVertex(rightWingTop.x(), rightWingTop.y(), rightWingTop.z());
        ADD_LINE;

        // FIELD

        m_layoutMesh->AddColor(1.0, 1.0, 0.0, 1.0);
        m_layoutMesh->AddVertex(leftBottom.x(), leftBottom.y(), leftBottom.z());
        m_layoutMesh->AddColor(1.0, 1.0, 0.0, 1.0);
        m_layoutMesh->AddVertex(rightBottom.x(), rightBottom.y(), rightBottom.z());
        ADD_LINE;
        m_layoutMesh->AddColor(1.0, 1.0, 0.0, 1.0);
        m_layoutMesh->AddVertex(rightBottom.x(), rightBottom.y(), rightBottom.z());
        m_layoutMesh->AddColor(1.0, 1.0, 0.0, 1.0);
        m_layoutMesh->AddVertex(rightTop.x(), rightTop.y(), rightTop.z());
        ADD_LINE;
        m_layoutMesh->AddColor(1.0, 1.0, 0.0, 1.0);
        m_layoutMesh->AddVertex(rightTop.x(), rightTop.y(), rightTop.z());
        m_layoutMesh->AddColor(1.0, 1.0, 0.0, 1.0);
        m_layoutMesh->AddVertex(leftTop.x(), leftTop.y(), leftTop.z());
        ADD_LINE;
        m_layoutMesh->AddColor(1.0, 1.0, 0.0, 1.0);
        m_layoutMesh->AddVertex(leftTop.x(), leftTop.y(), leftTop.z());
        m_layoutMesh->AddColor(1.0, 1.0, 0.0, 1.0);
        m_layoutMesh->AddVertex(leftBottom.x(), leftBottom.y(), leftBottom.z());
        ADD_LINE;
        m_layoutMesh->AddColor(1.0, 1.0, 0.0, 1.0);
        m_layoutMesh->AddVertex(centerTop.x(), centerTop.y(), centerTop.z());
        m_layoutMesh->AddColor(1.0, 1.0, 0.0, 1.0);
        m_layoutMesh->AddVertex(centerBottom.x(), centerBottom.y(), centerBottom.z());
        ADD_LINE;

    }

    return m_layoutMesh;
}
