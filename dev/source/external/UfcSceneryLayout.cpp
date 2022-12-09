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

#include "UfcSceneryLayout.h"

my::sport::CUfcSceneryLayout::CUfcSceneryLayout()
    : CSceneryLayout()
{
    m_mode = 25;
    m_layoutMesh.reset();
}

my::CVector3<double> my::sport::CUfcSceneryLayout::GetItemPosition(my::int64 item, int venueId) const
{
    my::CVector3<double> position;

    switch (item) {
    case CENTRE_ITEM:
        position.Set(0, 0, 0);
        break;
    case _0TH_OCTAGON_VERTEX_ITEM:
        if (m_mode == _25_FEET_OCTAGON)
            return GetItemPosition(_0TH_25_OCTAGON_VERTEX_ITEM);
        else if (m_mode == _30_FEET_OCTAGON)
            return GetItemPosition(_0TH_30_OCTAGON_VERTEX_ITEM);
        break;
    case _1ST_OCTAGON_VERTEX_ITEM:
        if (m_mode == _25_FEET_OCTAGON)
            return GetItemPosition(_1ST_25_OCTAGON_VERTEX_ITEM);
        else if (m_mode == _30_FEET_OCTAGON)
            return GetItemPosition(_1ST_30_OCTAGON_VERTEX_ITEM);
        break;
    case _2ND_OCTAGON_VERTEX_ITEM:
        if (m_mode == _25_FEET_OCTAGON)
            return GetItemPosition(_2ND_25_OCTAGON_VERTEX_ITEM);
        else if (m_mode == _30_FEET_OCTAGON)
            return GetItemPosition(_2ND_30_OCTAGON_VERTEX_ITEM);
        break;
    case _3RD_OCTAGON_VERTEX_ITEM:
        if (m_mode == _25_FEET_OCTAGON)
            return GetItemPosition(_3RD_25_OCTAGON_VERTEX_ITEM);
        else if (m_mode == _30_FEET_OCTAGON)
            return GetItemPosition(_3RD_30_OCTAGON_VERTEX_ITEM);
        break;
    case _4TH_OCTAGON_VERTEX_ITEM:
        if (m_mode == _25_FEET_OCTAGON)
            return GetItemPosition(_4TH_25_OCTAGON_VERTEX_ITEM);
        else if (m_mode == _30_FEET_OCTAGON)
            return GetItemPosition(_4TH_30_OCTAGON_VERTEX_ITEM);
        break;
    case _5TH_OCTAGON_VERTEX_ITEM:
        if (m_mode == _25_FEET_OCTAGON)
            return GetItemPosition(_5TH_25_OCTAGON_VERTEX_ITEM);
        else if (m_mode == _30_FEET_OCTAGON)
            return GetItemPosition(_5TH_30_OCTAGON_VERTEX_ITEM);
        break;
    case _6TH_OCTAGON_VERTEX_ITEM:
        if (m_mode == _25_FEET_OCTAGON)
            return GetItemPosition(_6TH_25_OCTAGON_VERTEX_ITEM);
        else if (m_mode == _30_FEET_OCTAGON)
            return GetItemPosition(_6TH_30_OCTAGON_VERTEX_ITEM);
        break;
    case _7TH_OCTAGON_VERTEX_ITEM:
        if (m_mode == _25_FEET_OCTAGON)
            return GetItemPosition(_7TH_25_OCTAGON_VERTEX_ITEM);
        else if (m_mode == _30_FEET_OCTAGON)
            return GetItemPosition(_7TH_30_OCTAGON_VERTEX_ITEM);
        break;
    case _0TH_INNER_OCTAGON_VERTEX_ITEM:
        if (m_mode == _25_FEET_OCTAGON)
            return GetItemPosition(_0TH_25_INNER_OCTAGON_VERTEX_ITEM);
        else if (m_mode == _30_FEET_OCTAGON)
            return GetItemPosition(_0TH_30_INNER_OCTAGON_VERTEX_ITEM);
        break;
    case _1ST_INNER_OCTAGON_VERTEX_ITEM:
        if (m_mode == _25_FEET_OCTAGON)
            return GetItemPosition(_1ST_25_INNER_OCTAGON_VERTEX_ITEM);
        else if (m_mode == _30_FEET_OCTAGON)
            return GetItemPosition(_1ST_30_INNER_OCTAGON_VERTEX_ITEM);
        break;
    case _2ND_INNER_OCTAGON_VERTEX_ITEM:
        if (m_mode == _25_FEET_OCTAGON)
            return GetItemPosition(_2ND_25_INNER_OCTAGON_VERTEX_ITEM);
        else if (m_mode == _30_FEET_OCTAGON)
            return GetItemPosition(_2ND_30_INNER_OCTAGON_VERTEX_ITEM);
        break;
    case _3RD_INNER_OCTAGON_VERTEX_ITEM:
        if (m_mode == _25_FEET_OCTAGON)
            return GetItemPosition(_3RD_25_INNER_OCTAGON_VERTEX_ITEM);
        else if (m_mode == _30_FEET_OCTAGON)
            return GetItemPosition(_3RD_30_INNER_OCTAGON_VERTEX_ITEM);
        break;
    case _4TH_INNER_OCTAGON_VERTEX_ITEM:
        if (m_mode == _25_FEET_OCTAGON)
            return GetItemPosition(_4TH_25_INNER_OCTAGON_VERTEX_ITEM);
        else if (m_mode == _30_FEET_OCTAGON)
            return GetItemPosition(_4TH_30_INNER_OCTAGON_VERTEX_ITEM);
        break;
    case _5TH_INNER_OCTAGON_VERTEX_ITEM:
        if (m_mode == _25_FEET_OCTAGON)
            return GetItemPosition(_5TH_25_INNER_OCTAGON_VERTEX_ITEM);
        else if (m_mode == _30_FEET_OCTAGON)
            return GetItemPosition(_5TH_30_INNER_OCTAGON_VERTEX_ITEM);
        break;
    case _6TH_INNER_OCTAGON_VERTEX_ITEM:
        if (m_mode == _25_FEET_OCTAGON)
            return GetItemPosition(_6TH_25_INNER_OCTAGON_VERTEX_ITEM);
        else if (m_mode == _30_FEET_OCTAGON)
            return GetItemPosition(_6TH_30_INNER_OCTAGON_VERTEX_ITEM);
        break;
    case _7TH_INNER_OCTAGON_VERTEX_ITEM:
        if (m_mode == _25_FEET_OCTAGON)
            return GetItemPosition(_7TH_25_INNER_OCTAGON_VERTEX_ITEM);
        else if (m_mode == _30_FEET_OCTAGON)
            return GetItemPosition(_7TH_30_INNER_OCTAGON_VERTEX_ITEM);
        break;
    case _0TH_25_OCTAGON_VERTEX_ITEM:
        position.Set(12.5, 5.17766953, 0.0);
        break;
    case _1ST_25_OCTAGON_VERTEX_ITEM:
        position.Set(5.17766953, 12.5, 0.0);
        break;
    case _2ND_25_OCTAGON_VERTEX_ITEM:
        position.Set(-5.17766953, 12.5, 0.0);
        break;
    case _3RD_25_OCTAGON_VERTEX_ITEM:
        position.Set(-12.5, 5.17766953, 0.0);
        break;
    case _4TH_25_OCTAGON_VERTEX_ITEM:
        position.Set(-12.5, -5.17766953, 0.0);
        break;
    case _5TH_25_OCTAGON_VERTEX_ITEM:
        position.Set(-5.17766953, -12.5, 0.0);
        break;
    case _6TH_25_OCTAGON_VERTEX_ITEM:
        position.Set(5.17766953, -12.5, 0.0);
        break;
    case _7TH_25_OCTAGON_VERTEX_ITEM:
        position.Set(12.5, -5.17766953, 0.0);
        break;
    case _0TH_25_INNER_OCTAGON_VERTEX_ITEM:
        position.Set(7.848493545, 3.250952471, 0.0);
        break;
    case _1ST_25_INNER_OCTAGON_VERTEX_ITEM:
        position.Set(3.250952471, 7.848493545, 0.0);
        break;
    case _2ND_25_INNER_OCTAGON_VERTEX_ITEM:
        position.Set(-3.250952471, 7.848493545, 0.0);
        break;
    case _3RD_25_INNER_OCTAGON_VERTEX_ITEM:
        position.Set(-7.848493545, 3.250952471, 0.0);
        break;
    case _4TH_25_INNER_OCTAGON_VERTEX_ITEM:
        position.Set(-7.848493545, -3.250952471, 0.0);
        break;
    case _5TH_25_INNER_OCTAGON_VERTEX_ITEM:
        position.Set(-3.250952471, -7.848493545, 0.0);
        break;
    case _6TH_25_INNER_OCTAGON_VERTEX_ITEM:
        position.Set(3.250952471, -7.848493545, 0.0);
        break;
    case _7TH_25_INNER_OCTAGON_VERTEX_ITEM:
        position.Set(7.848493545, -3.250952471, 0.0);
        break;
    case _0TH_30_OCTAGON_VERTEX_ITEM:
        position.Set(15.0, 6.213203436, 0.0);
        break;
    case _1ST_30_OCTAGON_VERTEX_ITEM:
        position.Set(6.213203436, 15.0, 0.0);
        break;
    case _2ND_30_OCTAGON_VERTEX_ITEM:
        position.Set(-6.213203436, 15.0, 0.0);
        break;
    case _3RD_30_OCTAGON_VERTEX_ITEM:
        position.Set(-15.0, 6.213203436, 0.0);
        break;
    case _4TH_30_OCTAGON_VERTEX_ITEM:
        position.Set(-15.0, -6.213203436, 0.0);
        break;
    case _5TH_30_OCTAGON_VERTEX_ITEM:
        position.Set(-6.213203436, -15.0, 0.0);
        break;
    case _6TH_30_OCTAGON_VERTEX_ITEM:
        position.Set(6.213203436, -15.0, 0.0);
        break;
    case _7TH_30_OCTAGON_VERTEX_ITEM:
        position.Set(15.0, -6.213203436, 0.0);
        break;
    case _0TH_30_INNER_OCTAGON_VERTEX_ITEM:
        position.Set(9.8958335, 4.098988447, 0.0);
        break;
    case _1ST_30_INNER_OCTAGON_VERTEX_ITEM:
        position.Set(4.098988447, 9.8958335, 0.0);
        break;
    case _2ND_30_INNER_OCTAGON_VERTEX_ITEM:
        position.Set(-4.098988447, 9.8958335, 0.0);
        break;
    case _3RD_30_INNER_OCTAGON_VERTEX_ITEM:
        position.Set(-9.8958335, 4.098988447, 0.0);
        break;
    case _4TH_30_INNER_OCTAGON_VERTEX_ITEM:
        position.Set(-9.8958335, -4.098988447, 0.0);
        break;
    case _5TH_30_INNER_OCTAGON_VERTEX_ITEM:
        position.Set(-4.098988447, -9.8958335, 0.0);
        break;
    case _6TH_30_INNER_OCTAGON_VERTEX_ITEM:
        position.Set(4.098988447, -9.8958335, 0.0);
        break;
    case _7TH_30_INNER_OCTAGON_VERTEX_ITEM:
        position.Set(9.8958335, -4.098988447, 0.0);
        break;
    }

    return position;
}

double my::sport::CUfcSceneryLayout::GetItemLength(my::int64 item, int venueId) const
{
    double length = my::Null<double>();

    return length;
}

double my::sport::CUfcSceneryLayout::GetDistanceToItem(double x, double y, double z, my::int64 item) const
{
    LOG_ERROR();

    return my::Null<double>();
}

double my::sport::CUfcSceneryLayout::GetDistanceToItem(my::CVector3<double> position, my::int64 item) const
{
    LOG_ERROR();

    return my::Null<double>();
}

std::string my::sport::CUfcSceneryLayout::GetItemName(my::int64 item) const
{
    LOG_ERROR();

    return my::Null<std::string>();
}

// TRICKY: (??-???-????) BOUNDING BOX OF ALL VENUES
CBoundingBox my::sport::CUfcSceneryLayout::GetBoundingBox() const
{
    CBoundingBox boundingBox;

    boundingBox.AddVertex(15.0, 15.0, 0.0);
    boundingBox.AddVertex(-15.0, -15.0, 0.0);

    return boundingBox;
}

boost::shared_ptr<CVertexPool> my::sport::CUfcSceneryLayout::GetLayoutMesh()
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

        my::CVector3<double> _0thOctagonVertex = GetItemPosition(_0TH_OCTAGON_VERTEX_ITEM),
            _1stOctagonVertex = GetItemPosition(_1ST_OCTAGON_VERTEX_ITEM),
            _2ndOctagonVertex = GetItemPosition(_2ND_OCTAGON_VERTEX_ITEM),
            _3rdOctagonVertex = GetItemPosition(_3RD_OCTAGON_VERTEX_ITEM),
            _4thOctagonVertex = GetItemPosition(_4TH_OCTAGON_VERTEX_ITEM),
            _5thOctagonVertex = GetItemPosition(_5TH_OCTAGON_VERTEX_ITEM),
            _6thOctagonVertex = GetItemPosition(_6TH_OCTAGON_VERTEX_ITEM),
            _7thOctagonVertex = GetItemPosition(_7TH_OCTAGON_VERTEX_ITEM),
            _0thInnerOctagonVertex = GetItemPosition(_0TH_INNER_OCTAGON_VERTEX_ITEM),
            _1stInnerOctagonVertex = GetItemPosition(_1ST_INNER_OCTAGON_VERTEX_ITEM),
            _2ndInnerOctagonVertex = GetItemPosition(_2ND_INNER_OCTAGON_VERTEX_ITEM),
            _3rdInnerOctagonVertex = GetItemPosition(_3RD_INNER_OCTAGON_VERTEX_ITEM),
            _4thInnerOctagonVertex = GetItemPosition(_4TH_INNER_OCTAGON_VERTEX_ITEM),
            _5thInnerOctagonVertex = GetItemPosition(_5TH_INNER_OCTAGON_VERTEX_ITEM),
            _6thInnerOctagonVertex = GetItemPosition(_6TH_INNER_OCTAGON_VERTEX_ITEM),
            _7thInnerOctagonVertex = GetItemPosition(_7TH_INNER_OCTAGON_VERTEX_ITEM);

        // OCTAGON

        m_layoutMesh->AddVertex(_0thOctagonVertex.x(), _0thOctagonVertex.y(), _0thOctagonVertex.z());
        m_layoutMesh->AddVertex(_1stOctagonVertex.x(), _1stOctagonVertex.y(), _1stOctagonVertex.z());
        ADD_LINE;
        m_layoutMesh->AddVertex(_1stOctagonVertex.x(), _1stOctagonVertex.y(), _1stOctagonVertex.z());
        m_layoutMesh->AddVertex(_2ndOctagonVertex.x(), _2ndOctagonVertex.y(), _2ndOctagonVertex.z());
        ADD_LINE;
        m_layoutMesh->AddVertex(_2ndOctagonVertex.x(), _2ndOctagonVertex.y(), _2ndOctagonVertex.z());
        m_layoutMesh->AddVertex(_3rdOctagonVertex.x(), _3rdOctagonVertex.y(), _3rdOctagonVertex.z());
        ADD_LINE;
        m_layoutMesh->AddVertex(_3rdOctagonVertex.x(), _3rdOctagonVertex.y(), _3rdOctagonVertex.z());
        m_layoutMesh->AddVertex(_4thOctagonVertex.x(), _4thOctagonVertex.y(), _4thOctagonVertex.z());
        ADD_LINE;
        m_layoutMesh->AddVertex(_4thOctagonVertex.x(), _4thOctagonVertex.y(), _4thOctagonVertex.z());
        m_layoutMesh->AddVertex(_5thOctagonVertex.x(), _5thOctagonVertex.y(), _5thOctagonVertex.z());
        ADD_LINE;
        m_layoutMesh->AddVertex(_5thOctagonVertex.x(), _5thOctagonVertex.y(), _5thOctagonVertex.z());
        m_layoutMesh->AddVertex(_6thOctagonVertex.x(), _6thOctagonVertex.y(), _6thOctagonVertex.z());
        ADD_LINE;
        m_layoutMesh->AddVertex(_6thOctagonVertex.x(), _6thOctagonVertex.y(), _6thOctagonVertex.z());
        m_layoutMesh->AddVertex(_7thOctagonVertex.x(), _7thOctagonVertex.y(), _7thOctagonVertex.z());
        ADD_LINE;
        m_layoutMesh->AddVertex(_7thOctagonVertex.x(), _7thOctagonVertex.y(), _7thOctagonVertex.z());
        m_layoutMesh->AddVertex(_0thOctagonVertex.x(), _0thOctagonVertex.y(), _0thOctagonVertex.z());
        ADD_LINE;

        // INNER OCTAGON

        m_layoutMesh->AddVertex(_0thInnerOctagonVertex.x(), _0thInnerOctagonVertex.y(), _0thInnerOctagonVertex.z());
        m_layoutMesh->AddVertex(_1stInnerOctagonVertex.x(), _1stInnerOctagonVertex.y(), _1stInnerOctagonVertex.z());
        ADD_LINE;
        m_layoutMesh->AddVertex(_1stInnerOctagonVertex.x(), _1stInnerOctagonVertex.y(), _1stInnerOctagonVertex.z());
        m_layoutMesh->AddVertex(_2ndInnerOctagonVertex.x(), _2ndInnerOctagonVertex.y(), _2ndInnerOctagonVertex.z());
        ADD_LINE;
        m_layoutMesh->AddVertex(_2ndInnerOctagonVertex.x(), _2ndInnerOctagonVertex.y(), _2ndInnerOctagonVertex.z());
        m_layoutMesh->AddVertex(_3rdInnerOctagonVertex.x(), _3rdInnerOctagonVertex.y(), _3rdInnerOctagonVertex.z());
        ADD_LINE;
        m_layoutMesh->AddVertex(_3rdInnerOctagonVertex.x(), _3rdInnerOctagonVertex.y(), _3rdInnerOctagonVertex.z());
        m_layoutMesh->AddVertex(_4thInnerOctagonVertex.x(), _4thInnerOctagonVertex.y(), _4thInnerOctagonVertex.z());
        ADD_LINE;
        m_layoutMesh->AddVertex(_4thInnerOctagonVertex.x(), _4thInnerOctagonVertex.y(), _4thInnerOctagonVertex.z());
        m_layoutMesh->AddVertex(_5thInnerOctagonVertex.x(), _5thInnerOctagonVertex.y(), _5thInnerOctagonVertex.z());
        ADD_LINE;
        m_layoutMesh->AddVertex(_5thInnerOctagonVertex.x(), _5thInnerOctagonVertex.y(), _5thInnerOctagonVertex.z());
        m_layoutMesh->AddVertex(_6thInnerOctagonVertex.x(), _6thInnerOctagonVertex.y(), _6thInnerOctagonVertex.z());
        ADD_LINE;
        m_layoutMesh->AddVertex(_6thInnerOctagonVertex.x(), _6thInnerOctagonVertex.y(), _6thInnerOctagonVertex.z());
        m_layoutMesh->AddVertex(_7thInnerOctagonVertex.x(), _7thInnerOctagonVertex.y(), _7thInnerOctagonVertex.z());
        ADD_LINE;
        m_layoutMesh->AddVertex(_7thInnerOctagonVertex.x(), _7thInnerOctagonVertex.y(), _7thInnerOctagonVertex.z());
        m_layoutMesh->AddVertex(_0thInnerOctagonVertex.x(), _0thInnerOctagonVertex.y(), _0thInnerOctagonVertex.z());
        ADD_LINE;

        m_layoutMesh->SetColor(1.0, 1.0, 0.0, 1.0);

        // 'M'

        std::vector<my::CVector3<double> > monsterVertexArray = {
            { -2.89664, 2.91314, 0.0 }, 
            { -1.89748, 2.81117, 0.0 }, 
            { -0.99749, -3.18952, 0.0 }, 
            { -0.86254, 3.06487, 0.0 }, 
            { -0.289347, 2.99027, 0.0 }, 
            { 0.183381, -3.76642, 0.0 }, 
            { 0.713262, 2.71753, 0.0 }, 
            { 1.62514, 2.72939, 0.0 }, 
            { 1.81536, -2.78327, 0.0 }, 
            { 2.51788, 2.99285, 0.0 }, 
            { 1.85186, 3.58328, 0.0 }, 
            { 1.086361, 3.23021, 0.0 }, 
            { 0.3750558, 3.75161, 0.0 }, 
            { -0.77323, 3.35305, 0.0 }, 
            { -1.63073, 3.59695, 0.0 }
        };

        if (m_mode == _30_FEET_OCTAGON)
        {
            std::transform(monsterVertexArray.cbegin(), monsterVertexArray.cend(), monsterVertexArray.begin(), [](const my::CVector3<double>& monsterVertex) { return monsterVertex * 1.3; });
        }

        std::vector<my::CVector3<double> >::const_iterator monsterVertexIterator = monsterVertexArray.begin();

        while (monsterVertexIterator != monsterVertexArray.end())
        {
            std::vector<my::CVector3<double> >::const_iterator nextMonsterVertexIterator = monsterVertexIterator;

            ++nextMonsterVertexIterator;

            if (nextMonsterVertexIterator == monsterVertexArray.end())
                nextMonsterVertexIterator = monsterVertexArray.begin();

            m_layoutMesh->AddColor(0.0, 1.0, 0.0, 1.0);
            m_layoutMesh->AddVertex(monsterVertexIterator->x(), monsterVertexIterator->y(), monsterVertexIterator->z());
            m_layoutMesh->AddColor(0.0, 1.0, 0.0, 1.0);
            m_layoutMesh->AddVertex(nextMonsterVertexIterator->x(), nextMonsterVertexIterator->y(), nextMonsterVertexIterator->z());
            ADD_LINE;

            ++monsterVertexIterator;
        }
    }

    return m_layoutMesh;
}

bool my::sport::CUfcSceneryLayout::SetMode(int mode)
{
    HEALTH_CHECK((mode != 25) && (mode != 30), false);

    m_mode = mode;

    m_layoutMesh.reset();

    return true;
}
