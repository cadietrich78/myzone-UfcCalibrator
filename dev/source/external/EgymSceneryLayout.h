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

#ifndef OFFICE_SCENERY_LAYOUT_INCLUDED
#define OFFICE_SCENERY_LAYOUT_INCLUDED

#include <boost/utility.hpp>

#include <Vector3.h>

#include <SceneryLayout.h>

namespace my {
    namespace sport {
        class CEgymSceneryLayout
            : public my::CSceneryLayout,
            public boost::noncopyable
        {
        public:
            enum OFFICE_ITEM {
                CUBE_000_VERTEX_ITEM,
                CUBE_X00_VERTEX_ITEM,
                CUBE_XY0_VERTEX_ITEM,
                CUBE_0Y0_VERTEX_ITEM,
                CUBE_00Z_VERTEX_ITEM,
                CUBE_X0Z_VERTEX_ITEM,
                CUBE_XYZ_VERTEX_ITEM,
                CUBE_0YZ_VERTEX_ITEM,
                LAST_ITEM
            };

            CEgymSceneryLayout();

            my::CVector3<double> GetItemPosition(my::int64 item, int venueId = my::Null<int>()) const;

            double GetItemLength(my::int64 item, int venueId = my::Null<int>()) const;

            double GetDistanceToItem(double x, double y, double z, my::int64 item) const;
            double GetDistanceToItem(my::CVector3<double> position, my::int64 item) const;

            std::string GetItemName(my::int64 item) const;

            // TRICKY: (??-???-????) BOUNDING BOX OF ALL VENUES
            CBoundingBox GetBoundingBox() const;

            boost::shared_ptr<CVertexPool> GetLayoutMesh();

        protected:
            boost::shared_ptr<CVertexPool> m_layoutMesh;
        };
    }; // sport

    template <>
    inline sport::CEgymSceneryLayout::OFFICE_ITEM Null()
    {
        return sport::CEgymSceneryLayout::OFFICE_ITEM::LAST_ITEM;
    }
}; // my

#endif // OFFICE_SCENERY_LAYOUT_INCLUDED

