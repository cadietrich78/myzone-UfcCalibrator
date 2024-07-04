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
    case CUBE_000_VERTEX_ITEM:
        position.Set(UnitConversion::Meter2Feet(0.0), UnitConversion::Meter2Feet(0.0), UnitConversion::Meter2Feet(0.5));
        break;
    case CUBE_X00_VERTEX_ITEM:
        position.Set(UnitConversion::Meter2Feet(1.0), UnitConversion::Meter2Feet(0.0), UnitConversion::Meter2Feet(0.5));
        break;
    case CUBE_XY0_VERTEX_ITEM:
        position.Set(UnitConversion::Meter2Feet(1.0), UnitConversion::Meter2Feet(1.0), UnitConversion::Meter2Feet(0.5));
        break;
    case CUBE_0Y0_VERTEX_ITEM:
        position.Set(UnitConversion::Meter2Feet(0.0), UnitConversion::Meter2Feet(1.0), UnitConversion::Meter2Feet(0.5));
        break;
    case CUBE_00Z_VERTEX_ITEM:
        position.Set(UnitConversion::Meter2Feet(0.0), UnitConversion::Meter2Feet(0.0), UnitConversion::Meter2Feet(1.0));
        break;
    case CUBE_X0Z_VERTEX_ITEM:
        position.Set(UnitConversion::Meter2Feet(1.0), UnitConversion::Meter2Feet(0.0), UnitConversion::Meter2Feet(1.0));
        break;
    case CUBE_XYZ_VERTEX_ITEM:
        position.Set(UnitConversion::Meter2Feet(1.0), UnitConversion::Meter2Feet(1.0), UnitConversion::Meter2Feet(1.0));
        break;
    case CUBE_0YZ_VERTEX_ITEM:
        position.Set(UnitConversion::Meter2Feet(0.0), UnitConversion::Meter2Feet(1.0), UnitConversion::Meter2Feet(1.0));
        break;
    }

    return position;
}

double my::sport::CEgymSceneryLayout::GetItemLength(my::int64 item, int venueId) const
{
    double length = my::Null<double>();

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

    boundingBox.AddVertex(GetItemPosition(CUBE_000_VERTEX_ITEM));
    boundingBox.AddVertex(GetItemPosition(CUBE_X00_VERTEX_ITEM));
    boundingBox.AddVertex(GetItemPosition(CUBE_XY0_VERTEX_ITEM));
    boundingBox.AddVertex(GetItemPosition(CUBE_0Y0_VERTEX_ITEM));
    boundingBox.AddVertex(GetItemPosition(CUBE_00Z_VERTEX_ITEM));
    boundingBox.AddVertex(GetItemPosition(CUBE_X0Z_VERTEX_ITEM));
    boundingBox.AddVertex(GetItemPosition(CUBE_XYZ_VERTEX_ITEM));
    boundingBox.AddVertex(GetItemPosition(CUBE_0YZ_VERTEX_ITEM));

    return boundingBox;
}

boost::shared_ptr<CVertexPool> my::sport::CEgymSceneryLayout::GetLayoutMesh()
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

        my::CVector3<double> _000 = GetItemPosition(CUBE_000_VERTEX_ITEM),
            _X00 = GetItemPosition(CUBE_X00_VERTEX_ITEM),
            _XY0 = GetItemPosition(CUBE_XY0_VERTEX_ITEM),
            _0Y0 = GetItemPosition(CUBE_0Y0_VERTEX_ITEM),
            _00Z = GetItemPosition(CUBE_00Z_VERTEX_ITEM),
            _X0Z = GetItemPosition(CUBE_X0Z_VERTEX_ITEM),
            _XYZ = GetItemPosition(CUBE_XYZ_VERTEX_ITEM),
            _0YZ = GetItemPosition(CUBE_0YZ_VERTEX_ITEM);

        // SQUARE IN THE MIDDLE OF THE BOX

        m_layoutMesh->AddColor(1.0, 0.0, 0.0, 1.0);
        m_layoutMesh->AddVertex(_000.x(), _000.y(), _000.z());
        m_layoutMesh->AddColor(1.0, 0.0, 0.0, 1.0);
        m_layoutMesh->AddVertex(_X00.x(), _X00.y(), _X00.z());
        ADD_LINE;
        m_layoutMesh->AddColor(1.0, 1.0, 0.0, 1.0);
        m_layoutMesh->AddVertex(_X00.x(), _X00.y(), _X00.z());
        m_layoutMesh->AddColor(1.0, 1.0, 0.0, 1.0);
        m_layoutMesh->AddVertex(_XY0.x(), _XY0.y(), _XY0.z());
        ADD_LINE;
        m_layoutMesh->AddColor(1.0, 1.0, 0.0, 1.0);
        m_layoutMesh->AddVertex(_XY0.x(), _XY0.y(), _XY0.z());
        m_layoutMesh->AddColor(1.0, 1.0, 0.0, 1.0);
        m_layoutMesh->AddVertex(_0Y0.x(), _0Y0.y(), _0Y0.z());
        ADD_LINE;
        m_layoutMesh->AddColor(0.0, 1.0, 0.0, 1.0);
        m_layoutMesh->AddVertex(_0Y0.x(), _0Y0.y(), _0Y0.z());
        m_layoutMesh->AddColor(0.0, 1.0, 0.0, 1.0);
        m_layoutMesh->AddVertex(_000.x(), _000.y(), _000.z());
        ADD_LINE;

        // SQUARE IN THE TOP OF THE BOX

        m_layoutMesh->AddColor(1.0, 0.0, 0.0, 1.0);
        m_layoutMesh->AddVertex(_00Z.x(), _00Z.y(), _00Z.z());
        m_layoutMesh->AddColor(1.0, 0.0, 0.0, 1.0);
        m_layoutMesh->AddVertex(_X0Z.x(), _X0Z.y(), _X0Z.z());
        ADD_LINE;
        m_layoutMesh->AddColor(1.0, 1.0, 0.0, 1.0);
        m_layoutMesh->AddVertex(_X0Z.x(), _X0Z.y(), _X0Z.z());
        m_layoutMesh->AddColor(1.0, 1.0, 0.0, 1.0);
        m_layoutMesh->AddVertex(_XYZ.x(), _XYZ.y(), _XYZ.z());
        ADD_LINE;
        m_layoutMesh->AddColor(1.0, 1.0, 0.0, 1.0);
        m_layoutMesh->AddVertex(_XYZ.x(), _XYZ.y(), _XYZ.z());
        m_layoutMesh->AddColor(1.0, 1.0, 0.0, 1.0);
        m_layoutMesh->AddVertex(_0YZ.x(), _0YZ.y(), _0YZ.z());
        ADD_LINE;
        m_layoutMesh->AddColor(0.0, 1.0, 0.0, 1.0);
        m_layoutMesh->AddVertex(_0YZ.x(), _0YZ.y(), _0YZ.z());
        m_layoutMesh->AddColor(0.0, 1.0, 0.0, 1.0);
        m_layoutMesh->AddVertex(_00Z.x(), _00Z.y(), _00Z.z());
        ADD_LINE;

        // COORDINATE AXES
        m_layoutMesh->AddColor(1.0, 0.0, 0.0, 1.0);
        m_layoutMesh->AddVertex(0.0, 0.0, 0.0);
        m_layoutMesh->AddColor(1.0, 0.0, 0.0, 1.0);
        m_layoutMesh->AddVertex(UnitConversion::Meter2Feet(2.0), 0.0, 0.0);
        ADD_LINE;
        m_layoutMesh->AddColor(0.0, 1.0, 0.0, 1.0);
        m_layoutMesh->AddVertex(0.0, 0.0, 0.0);
        m_layoutMesh->AddColor(0.0, 1.0, 0.0, 1.0);
        m_layoutMesh->AddVertex(0.0, UnitConversion::Meter2Feet(2.0), 0.0);
        ADD_LINE;
        m_layoutMesh->AddColor(0.0, 0.0, 1.0, 1.0);
        m_layoutMesh->AddVertex(0.0, 0.0, 0.0);
        m_layoutMesh->AddColor(0.0, 0.0, 1.0, 1.0);
        m_layoutMesh->AddVertex(0.0, 0.0, UnitConversion::Meter2Feet(2.0));
        ADD_LINE;
    }

    return m_layoutMesh;
}
