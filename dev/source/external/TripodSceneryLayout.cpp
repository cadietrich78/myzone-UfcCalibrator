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

#include "TripodSceneryLayout.h"

my::sport::CTripodSceneryLayout::CTripodSceneryLayout()
    : CSceneryLayout()
{
    m_layoutMesh.reset();
}

my::CVector3<double> my::sport::CTripodSceneryLayout::GetItemPosition(my::int64 item, int venueId) const
{
    my::CVector3<double> position;

    switch (item) {
    case RED_LEG_BOTTOM_ITEM:
        position.Set(UnitConversion::Centimeters2Feet(44.45597), 0.0, UnitConversion::Centimeters2Feet(1.5));
        break;
    case RED_LEG_MIDDLE_ITEM:
        position.Set(UnitConversion::Centimeters2Feet(25.40341), 0.0, UnitConversion::Centimeters2Feet(48.0));
        break;
    case GREEN_LEG_BOTTOM_ITEM:
        position.Set(UnitConversion::Centimeters2Feet(-22.22799), UnitConversion::Centimeters2Feet(-38.5), UnitConversion::Centimeters2Feet(1.5));
        break;
    case GREEN_LEG_MIDDLE_ITEM:
        position.Set(UnitConversion::Centimeters2Feet(-12.70171), UnitConversion::Centimeters2Feet(-22.0), UnitConversion::Centimeters2Feet(48.0));
        break;
    case BLUE_LEG_BOTTOM_ITEM:
        position.Set(UnitConversion::Centimeters2Feet(-22.22799), UnitConversion::Centimeters2Feet(38.5), UnitConversion::Centimeters2Feet(1.5));
        break;
    case BLUE_LEG_MIDDLE_ITEM:
        position.Set(UnitConversion::Centimeters2Feet(-12.70171), UnitConversion::Centimeters2Feet(22.0), UnitConversion::Centimeters2Feet(48.0));
        break;
    case BALL_ON_TOP_ITEM:
        position.Set(0.0, 0.0, UnitConversion::Centimeters2Feet(107.5));
        break;
    }

    return position;
}

double my::sport::CTripodSceneryLayout::GetItemLength(my::int64 item, int venueId) const
{
    double length = my::Null<double>();

    // NONE

    return length;
}

double my::sport::CTripodSceneryLayout::GetDistanceToItem(double x, double y, double z, my::int64 item) const
{
    LOG_ERROR();

    return my::Null<double>();
}

double my::sport::CTripodSceneryLayout::GetDistanceToItem(my::CVector3<double> position, my::int64 item) const
{
    LOG_ERROR();

    return my::Null<double>();
}

std::string my::sport::CTripodSceneryLayout::GetItemName(my::int64 item) const
{
    LOG_ERROR();

    return my::Null<std::string>();
}

// TRICKY: (??-???-????) BOUNDING BOX OF ALL VENUES
CBoundingBox my::sport::CTripodSceneryLayout::GetBoundingBox() const
{
    CBoundingBox boundingBox;

    boundingBox.AddVertex(GetItemPosition(RED_LEG_BOTTOM_ITEM));
    boundingBox.AddVertex(GetItemPosition(GREEN_LEG_BOTTOM_ITEM));
    boundingBox.AddVertex(GetItemPosition(BLUE_LEG_BOTTOM_ITEM));

    return boundingBox;
}

boost::shared_ptr<CVertexPool> my::sport::CTripodSceneryLayout::GetLayoutMesh()
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

        my::CVector3<double> redLegBottom = GetItemPosition(RED_LEG_BOTTOM_ITEM),
            redLegMiddle = GetItemPosition(RED_LEG_MIDDLE_ITEM),
            greenLegBottom = GetItemPosition(GREEN_LEG_BOTTOM_ITEM),
            greenLegMiddle = GetItemPosition(GREEN_LEG_MIDDLE_ITEM),
            blueLegBottom = GetItemPosition(BLUE_LEG_BOTTOM_ITEM),
            blueLegMiddle = GetItemPosition(BLUE_LEG_MIDDLE_ITEM),
            ballOnTop = GetItemPosition(BALL_ON_TOP_ITEM);

        m_layoutMesh->AddColor(1.0, 0.0, 0.0, 1.0);
        m_layoutMesh->AddVertex(redLegBottom.x(), redLegBottom.y(), redLegBottom.z());
        m_layoutMesh->AddColor(1.0, 0.0, 0.0, 1.0);
        m_layoutMesh->AddVertex(redLegMiddle.x(), redLegMiddle.y(), redLegMiddle.z());
        ADD_LINE;
        m_layoutMesh->AddColor(1.0, 0.0, 0.0, 1.0);
        m_layoutMesh->AddVertex(redLegMiddle.x(), redLegMiddle.y(), redLegMiddle.z());
        m_layoutMesh->AddColor(1.0, 0.0, 0.0, 1.0);
        m_layoutMesh->AddVertex(ballOnTop.x(), ballOnTop.y(), ballOnTop.z());
        ADD_LINE;

        m_layoutMesh->AddColor(0.0, 1.0, 0.0, 1.0);
        m_layoutMesh->AddVertex(greenLegBottom.x(), greenLegBottom.y(), greenLegBottom.z());
        m_layoutMesh->AddColor(0.0, 1.0, 0.0, 1.0);
        m_layoutMesh->AddVertex(greenLegMiddle.x(), greenLegMiddle.y(), greenLegMiddle.z());
        ADD_LINE;
        m_layoutMesh->AddColor(0.0, 1.0, 0.0, 1.0);
        m_layoutMesh->AddVertex(greenLegMiddle.x(), greenLegMiddle.y(), greenLegMiddle.z());
        m_layoutMesh->AddColor(0.0, 1.0, 0.0, 1.0);
        m_layoutMesh->AddVertex(ballOnTop.x(), ballOnTop.y(), ballOnTop.z());
        ADD_LINE;

        m_layoutMesh->AddColor(0.0, 0.0, 1.0, 1.0);
        m_layoutMesh->AddVertex(blueLegBottom.x(), blueLegBottom.y(), blueLegBottom.z());
        m_layoutMesh->AddColor(0.0, 0.0, 1.0, 1.0);
        m_layoutMesh->AddVertex(blueLegMiddle.x(), blueLegMiddle.y(), blueLegMiddle.z());
        ADD_LINE;
        m_layoutMesh->AddColor(0.0, 0.0, 1.0, 1.0);
        m_layoutMesh->AddVertex(blueLegMiddle.x(), blueLegMiddle.y(), blueLegMiddle.z());
        m_layoutMesh->AddColor(0.0, 0.0, 1.0, 1.0);
        m_layoutMesh->AddVertex(ballOnTop.x(), ballOnTop.y(), ballOnTop.z());
        ADD_LINE;
    }

    return m_layoutMesh;
}
