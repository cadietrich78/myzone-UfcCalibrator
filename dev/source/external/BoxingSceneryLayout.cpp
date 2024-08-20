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

#include "BoxingSceneryLayout.h"

my::sport::CBoxingSceneryLayout::CBoxingSceneryLayout()
    : CSceneryLayout()
{
    m_layoutMesh.reset();
}

my::CVector3<double> my::sport::CBoxingSceneryLayout::GetItemPosition(my::int64 item, int venueId) const
{
    my::CVector3<double> position;

    double halfOfAverageDistanceAmongTopOfPoles = UnitConversion::Inch2Feet(238.875) / 2.0,
        averageHeightOfTopOfPoles = UnitConversion::Inch2Feet(56.875),
        averageHeightOfSupportingPoles = UnitConversion::Inch2Feet(55.375),
        averageHeightOfBottomOfPoles = UnitConversion::Inch2Feet(9.625),
        bottomOfThePolesOffset = UnitConversion::Inch2Feet(4.242640687),
        supportingPoleOffset = UnitConversion::Inch2Feet(23.51130047);

    switch (item) {
        case SUPPORTING_POLE_RED_VERTEX_ITEM:
            position.Set(-(halfOfAverageDistanceAmongTopOfPoles + supportingPoleOffset), -(halfOfAverageDistanceAmongTopOfPoles + supportingPoleOffset), averageHeightOfSupportingPoles);
            break;
        case TOP_POLE_RED_VERTEX_ITEM:
            position.Set(-halfOfAverageDistanceAmongTopOfPoles, -halfOfAverageDistanceAmongTopOfPoles, averageHeightOfTopOfPoles);
            break;
        case BOTTOM_POLE_RED_VERTEX_ITEM:
            position.Set(-(halfOfAverageDistanceAmongTopOfPoles - bottomOfThePolesOffset), -(halfOfAverageDistanceAmongTopOfPoles - bottomOfThePolesOffset), averageHeightOfBottomOfPoles);
            break;

        case SUPPORTING_POLE_10_VERTEX_ITEM:
            position.Set(halfOfAverageDistanceAmongTopOfPoles + supportingPoleOffset, -(halfOfAverageDistanceAmongTopOfPoles + supportingPoleOffset), averageHeightOfSupportingPoles);
            break;
        case TOP_POLE_10_VERTEX_ITEM:
            position.Set(halfOfAverageDistanceAmongTopOfPoles, -halfOfAverageDistanceAmongTopOfPoles, averageHeightOfTopOfPoles);
            break;
        case BOTTOM_POLE_10_VERTEX_ITEM:
            position.Set(halfOfAverageDistanceAmongTopOfPoles - bottomOfThePolesOffset, -(halfOfAverageDistanceAmongTopOfPoles - bottomOfThePolesOffset), averageHeightOfBottomOfPoles);
            break;

        case SUPPORTING_POLE_BLUE_VERTEX_ITEM:
            position.Set(halfOfAverageDistanceAmongTopOfPoles + supportingPoleOffset, halfOfAverageDistanceAmongTopOfPoles + supportingPoleOffset, averageHeightOfSupportingPoles);
            break;
        case TOP_POLE_BLUE_VERTEX_ITEM:
            position.Set(halfOfAverageDistanceAmongTopOfPoles, halfOfAverageDistanceAmongTopOfPoles, averageHeightOfTopOfPoles);
            break;
        case BOTTOM_POLE_BLUE_VERTEX_ITEM:
            position.Set(halfOfAverageDistanceAmongTopOfPoles - bottomOfThePolesOffset, halfOfAverageDistanceAmongTopOfPoles - bottomOfThePolesOffset, averageHeightOfBottomOfPoles);
            break;

        case SUPPORTING_POLE_01_VERTEX_ITEM:
            position.Set(-(halfOfAverageDistanceAmongTopOfPoles + supportingPoleOffset), halfOfAverageDistanceAmongTopOfPoles + supportingPoleOffset, averageHeightOfSupportingPoles);
            break;
        case TOP_POLE_01_VERTEX_ITEM:
            position.Set(-halfOfAverageDistanceAmongTopOfPoles, halfOfAverageDistanceAmongTopOfPoles, averageHeightOfTopOfPoles);
            break;
        case BOTTOM_POLE_01_VERTEX_ITEM:
            position.Set(-(halfOfAverageDistanceAmongTopOfPoles - bottomOfThePolesOffset), halfOfAverageDistanceAmongTopOfPoles - bottomOfThePolesOffset, averageHeightOfBottomOfPoles);
            break;
    }

    return position;
}

double my::sport::CBoxingSceneryLayout::GetItemLength(my::int64 item, int venueId) const
{
    double length = my::Null<double>();

    return length;
}

double my::sport::CBoxingSceneryLayout::GetDistanceToItem(double x, double y, double z, my::int64 item) const
{
    LOG_ERROR();

    return my::Null<double>();
}

double my::sport::CBoxingSceneryLayout::GetDistanceToItem(my::CVector3<double> position, my::int64 item) const
{
    LOG_ERROR();

    return my::Null<double>();
}

std::string my::sport::CBoxingSceneryLayout::GetItemName(my::int64 item) const
{
    LOG_ERROR();

    return my::Null<std::string>();
}

// TRICKY: (??-???-????) BOUNDING BOX OF ALL VENUES
CBoundingBox my::sport::CBoxingSceneryLayout::GetBoundingBox() const
{
    CBoundingBox boundingBox;

    boundingBox.AddVertex(GetItemPosition(SUPPORTING_POLE_RED_VERTEX_ITEM));
    boundingBox.AddVertex(GetItemPosition(SUPPORTING_POLE_10_VERTEX_ITEM));
    boundingBox.AddVertex(GetItemPosition(SUPPORTING_POLE_BLUE_VERTEX_ITEM));
    boundingBox.AddVertex(GetItemPosition(SUPPORTING_POLE_01_VERTEX_ITEM));
    boundingBox.AddVertex(0.0, 0.0, 0.0);

    return boundingBox;
}

boost::shared_ptr<CVertexPool> my::sport::CBoxingSceneryLayout::GetLayoutMesh()
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

        my::CVector3<double> supportingPoleRedVertexItem = GetItemPosition(SUPPORTING_POLE_RED_VERTEX_ITEM),
            topPoleRedVertexItem = GetItemPosition(TOP_POLE_RED_VERTEX_ITEM),
            bottomPoleRedVertexItem = GetItemPosition(BOTTOM_POLE_RED_VERTEX_ITEM),
            supportingPole10VertexItem = GetItemPosition(SUPPORTING_POLE_10_VERTEX_ITEM),
            topPole10VertexItem = GetItemPosition(TOP_POLE_10_VERTEX_ITEM),
            bottomPole10VertexItem = GetItemPosition(BOTTOM_POLE_10_VERTEX_ITEM),
            supportingPoleBlueVertexItem = GetItemPosition(SUPPORTING_POLE_BLUE_VERTEX_ITEM),
            topPoleBlueVertexItem = GetItemPosition(TOP_POLE_BLUE_VERTEX_ITEM),
            bottomPoleBlueVertexItem = GetItemPosition(BOTTOM_POLE_BLUE_VERTEX_ITEM),
            supportingPole01VertexItem = GetItemPosition(SUPPORTING_POLE_01_VERTEX_ITEM),
            topPole01VertexItem = GetItemPosition(TOP_POLE_01_VERTEX_ITEM),
            bottomPole01VertexItem = GetItemPosition(BOTTOM_POLE_01_VERTEX_ITEM);

        m_layoutMesh->AddColor(1.0, 1.0, 0.0, 1.0);
        m_layoutMesh->AddVertex(bottomPoleRedVertexItem.x(), bottomPoleRedVertexItem.y(), bottomPoleRedVertexItem.z());
        m_layoutMesh->AddColor(1.0, 1.0, 0.0, 1.0);
        m_layoutMesh->AddVertex(bottomPole10VertexItem.x(), bottomPole10VertexItem.y(), bottomPole10VertexItem.z());
        ADD_LINE;
        m_layoutMesh->AddColor(1.0, 1.0, 0.0, 1.0);
        m_layoutMesh->AddVertex(bottomPole10VertexItem.x(), bottomPole10VertexItem.y(), bottomPole10VertexItem.z());
        m_layoutMesh->AddColor(1.0, 1.0, 0.0, 1.0);
        m_layoutMesh->AddVertex(bottomPoleBlueVertexItem.x(), bottomPoleBlueVertexItem.y(), bottomPoleBlueVertexItem.z());
        ADD_LINE;
        m_layoutMesh->AddColor(1.0, 1.0, 0.0, 1.0);
        m_layoutMesh->AddVertex(bottomPoleBlueVertexItem.x(), bottomPoleBlueVertexItem.y(), bottomPoleBlueVertexItem.z());
        m_layoutMesh->AddColor(1.0, 1.0, 0.0, 1.0);
        m_layoutMesh->AddVertex(bottomPole01VertexItem.x(), bottomPole01VertexItem.y(), bottomPole01VertexItem.z());
        ADD_LINE;
        m_layoutMesh->AddColor(1.0, 1.0, 0.0, 1.0);
        m_layoutMesh->AddVertex(bottomPole01VertexItem.x(), bottomPole01VertexItem.y(), bottomPole01VertexItem.z());
        m_layoutMesh->AddColor(1.0, 1.0, 0.0, 1.0);
        m_layoutMesh->AddVertex(bottomPoleRedVertexItem.x(), bottomPoleRedVertexItem.y(), bottomPoleRedVertexItem.z());
        ADD_LINE;

        m_layoutMesh->AddColor(1.0, 1.0, 0.0, 1.0);
        m_layoutMesh->AddVertex(supportingPoleRedVertexItem.x(), supportingPoleRedVertexItem.y(), supportingPoleRedVertexItem.z());
        m_layoutMesh->AddColor(1.0, 1.0, 0.0, 1.0);
        m_layoutMesh->AddVertex(topPoleRedVertexItem.x(), topPoleRedVertexItem.y(), topPoleRedVertexItem.z());
        ADD_LINE;
        m_layoutMesh->AddColor(1.0, 1.0, 0.0, 1.0);
        m_layoutMesh->AddVertex(supportingPole10VertexItem.x(), supportingPole10VertexItem.y(), supportingPole10VertexItem.z());
        m_layoutMesh->AddColor(1.0, 1.0, 0.0, 1.0);
        m_layoutMesh->AddVertex(topPole10VertexItem.x(), topPole10VertexItem.y(), topPole10VertexItem.z());
        ADD_LINE;
        m_layoutMesh->AddColor(1.0, 1.0, 0.0, 1.0);
        m_layoutMesh->AddVertex(supportingPoleBlueVertexItem.x(), supportingPoleBlueVertexItem.y(), supportingPoleBlueVertexItem.z());
        m_layoutMesh->AddColor(1.0, 1.0, 0.0, 1.0);
        m_layoutMesh->AddVertex(topPoleBlueVertexItem.x(), topPoleBlueVertexItem.y(), topPoleBlueVertexItem.z());
        ADD_LINE;
        m_layoutMesh->AddColor(1.0, 1.0, 0.0, 1.0);
        m_layoutMesh->AddVertex(supportingPole01VertexItem.x(), supportingPole01VertexItem.y(), supportingPole01VertexItem.z());
        m_layoutMesh->AddColor(1.0, 1.0, 0.0, 1.0);
        m_layoutMesh->AddVertex(topPole01VertexItem.x(), topPole01VertexItem.y(), topPole01VertexItem.z());
        ADD_LINE;

        m_layoutMesh->AddColor(1.0, 1.0, 0.0, 1.0);
        m_layoutMesh->AddVertex(topPoleRedVertexItem.x(), topPoleRedVertexItem.y(), topPoleRedVertexItem.z());
        m_layoutMesh->AddColor(1.0, 1.0, 0.0, 1.0);
        m_layoutMesh->AddVertex(topPole10VertexItem.x(), topPole10VertexItem.y(), topPole10VertexItem.z());
        ADD_LINE;
        m_layoutMesh->AddColor(1.0, 1.0, 0.0, 1.0);
        m_layoutMesh->AddVertex(topPole10VertexItem.x(), topPole10VertexItem.y(), topPole10VertexItem.z());
        m_layoutMesh->AddColor(1.0, 1.0, 0.0, 1.0);
        m_layoutMesh->AddVertex(topPoleBlueVertexItem.x(), topPoleBlueVertexItem.y(), topPoleBlueVertexItem.z());
        ADD_LINE;
        m_layoutMesh->AddColor(1.0, 1.0, 0.0, 1.0);
        m_layoutMesh->AddVertex(topPoleBlueVertexItem.x(), topPoleBlueVertexItem.y(), topPoleBlueVertexItem.z());
        m_layoutMesh->AddColor(1.0, 1.0, 0.0, 1.0);
        m_layoutMesh->AddVertex(topPole01VertexItem.x(), topPole01VertexItem.y(), topPole01VertexItem.z());
        ADD_LINE;
        m_layoutMesh->AddColor(1.0, 1.0, 0.0, 1.0);
        m_layoutMesh->AddVertex(topPole01VertexItem.x(), topPole01VertexItem.y(), topPole01VertexItem.z());
        m_layoutMesh->AddColor(1.0, 1.0, 0.0, 1.0);
        m_layoutMesh->AddVertex(topPoleRedVertexItem.x(), topPoleRedVertexItem.y(), topPoleRedVertexItem.z());
        ADD_LINE;

        // COORDINATE AXES
        m_layoutMesh->AddColor(1.0, 0.0, 0.0, 1.0);
        m_layoutMesh->AddVertex(0.0, 0.0, 0.0);
        m_layoutMesh->AddColor(1.0, 0.0, 0.0, 1.0);
        m_layoutMesh->AddVertex(UnitConversion::Meter2Feet(1.0), 0.0, 0.0);
        ADD_LINE;
        m_layoutMesh->AddColor(0.0, 1.0, 0.0, 1.0);
        m_layoutMesh->AddVertex(0.0, 0.0, 0.0);
        m_layoutMesh->AddColor(0.0, 1.0, 0.0, 1.0);
        m_layoutMesh->AddVertex(0.0, UnitConversion::Meter2Feet(1.0), 0.0);
        ADD_LINE;
        m_layoutMesh->AddColor(0.0, 0.0, 1.0, 1.0);
        m_layoutMesh->AddVertex(0.0, 0.0, 0.0);
        m_layoutMesh->AddColor(0.0, 0.0, 1.0, 1.0);
        m_layoutMesh->AddVertex(0.0, 0.0, UnitConversion::Meter2Feet(1.0));
        ADD_LINE;
    }

    return m_layoutMesh;
}
