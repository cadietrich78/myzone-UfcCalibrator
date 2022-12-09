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

#ifndef UFC_SCENERY_LAYOUT_INCLUDED
#define UFC_SCENERY_LAYOUT_INCLUDED

#include <boost/utility.hpp>

#include <Vector3.h>

#include <SceneryLayout.h>

namespace my {
    namespace sport {
        class CUfcSceneryLayout
            : public my::CSceneryLayout,
            public boost::noncopyable
        {
        public:
            enum OCTAGON_TYPE {
                _25_FEET_OCTAGON = 25,
                _30_FEET_OCTAGON = 30
            };

            enum UFC_ITEM {
                CENTRE_ITEM,
                _0TH_OCTAGON_VERTEX_ITEM,
                _1ST_OCTAGON_VERTEX_ITEM,
                _2ND_OCTAGON_VERTEX_ITEM,
                _3RD_OCTAGON_VERTEX_ITEM,
                _4TH_OCTAGON_VERTEX_ITEM,
                _5TH_OCTAGON_VERTEX_ITEM,
                _6TH_OCTAGON_VERTEX_ITEM,
                _7TH_OCTAGON_VERTEX_ITEM,
                _0TH_INNER_OCTAGON_VERTEX_ITEM,
                _1ST_INNER_OCTAGON_VERTEX_ITEM,
                _2ND_INNER_OCTAGON_VERTEX_ITEM,
                _3RD_INNER_OCTAGON_VERTEX_ITEM,
                _4TH_INNER_OCTAGON_VERTEX_ITEM,
                _5TH_INNER_OCTAGON_VERTEX_ITEM,
                _6TH_INNER_OCTAGON_VERTEX_ITEM,
                _7TH_INNER_OCTAGON_VERTEX_ITEM,
                _0TH_25_OCTAGON_VERTEX_ITEM,
                _1ST_25_OCTAGON_VERTEX_ITEM,
                _2ND_25_OCTAGON_VERTEX_ITEM,
                _3RD_25_OCTAGON_VERTEX_ITEM,
                _4TH_25_OCTAGON_VERTEX_ITEM,
                _5TH_25_OCTAGON_VERTEX_ITEM,
                _6TH_25_OCTAGON_VERTEX_ITEM,
                _7TH_25_OCTAGON_VERTEX_ITEM,
                _0TH_25_INNER_OCTAGON_VERTEX_ITEM,
                _1ST_25_INNER_OCTAGON_VERTEX_ITEM,
                _2ND_25_INNER_OCTAGON_VERTEX_ITEM,
                _3RD_25_INNER_OCTAGON_VERTEX_ITEM,
                _4TH_25_INNER_OCTAGON_VERTEX_ITEM,
                _5TH_25_INNER_OCTAGON_VERTEX_ITEM,
                _6TH_25_INNER_OCTAGON_VERTEX_ITEM,
                _7TH_25_INNER_OCTAGON_VERTEX_ITEM,
                _0TH_30_OCTAGON_VERTEX_ITEM,
                _1ST_30_OCTAGON_VERTEX_ITEM,
                _2ND_30_OCTAGON_VERTEX_ITEM,
                _3RD_30_OCTAGON_VERTEX_ITEM,
                _4TH_30_OCTAGON_VERTEX_ITEM,
                _5TH_30_OCTAGON_VERTEX_ITEM,
                _6TH_30_OCTAGON_VERTEX_ITEM,
                _7TH_30_OCTAGON_VERTEX_ITEM,
                _0TH_30_INNER_OCTAGON_VERTEX_ITEM,
                _1ST_30_INNER_OCTAGON_VERTEX_ITEM,
                _2ND_30_INNER_OCTAGON_VERTEX_ITEM,
                _3RD_30_INNER_OCTAGON_VERTEX_ITEM,
                _4TH_30_INNER_OCTAGON_VERTEX_ITEM,
                _5TH_30_INNER_OCTAGON_VERTEX_ITEM,
                _6TH_30_INNER_OCTAGON_VERTEX_ITEM,
                _7TH_30_INNER_OCTAGON_VERTEX_ITEM,
                LAST_ITEM
            };

            CUfcSceneryLayout();

            my::CVector3<double> GetItemPosition(my::int64 item, int venueId = my::Null<int>()) const;

            double GetItemLength(my::int64 item, int venueId = my::Null<int>()) const;

            double GetDistanceToItem(double x, double y, double z, my::int64 item) const;
            double GetDistanceToItem(my::CVector3<double> position, my::int64 item) const;

            std::string GetItemName(my::int64 item) const;

            // TRICKY: (??-???-????) BOUNDING BOX OF ALL VENUES
            CBoundingBox GetBoundingBox() const;

            boost::shared_ptr<CVertexPool> GetLayoutMesh();

            /// <summary>
            /// 25 or 30
            /// </summary>
            /// <param name="mode"></param>
            /// <returns></returns>
            bool SetMode(int mode);

        protected:
            int m_mode;

            boost::shared_ptr<CVertexPool> m_layoutMesh;
        };
    }; // sport

    template <>
    inline sport::CUfcSceneryLayout::UFC_ITEM Null()
    {
        return sport::CUfcSceneryLayout::UFC_ITEM::LAST_ITEM;
    }
}; // my

#endif // UFC_SCENERY_LAYOUT_INCLUDED

