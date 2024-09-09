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

#include "../model/Footage.h"
#include "../StringResource.h"

#include "LacrosseMarkerArray.h"

CLacrosseMarkerArray::CLacrosseMarkerArray()
    : CMarkerGroup(),
    m_pointMarkerArray()
{
    if (!Initialize())
    {
        LOG_ERROR();

        return /*false*/;
    }
}

double CLacrosseMarkerArray::GetFitness(boost::shared_ptr<CPinholeCamera2>& pinholeCamera)
{
	double fitness = 0;

	fitness += GetPointFitness(pinholeCamera) + GetLineFitness(pinholeCamera);

	return fitness;
}

double CLacrosseMarkerArray::GetPointFitness(boost::shared_ptr<CPinholeCamera2>& pinholeCamera)
{
	const int *viewport = pinholeCamera->GetViewport();

	HEALTH_CHECK(!viewport, 0);

	double fitness = 0;

	for (auto& marker : m_pointMarkerArray)
	{
		if (marker->IsEnabled())
		{
            const double *_3dWorldCoord = marker->GetWorldCoord();

            HEALTH_CHECK(!_3dWorldCoord, my::Null<double>());

            // TESTING: (21-Dec-2015) WORLD-SPACE ERROR COMPUTATION

            double _2dMarker[3] = { 0 };

            MyMath::Assign2(marker->GetScreenCoord(), _2dMarker);

            // TRICKY: (21-Dec-2015) INTERSECTION BETWEEN THE RAY SPANNED BY THE MARKER AND THE ICE RINK

            double s0[3] = { 0 },
                s1[3] = { 0 },
                normal[3] = { 0.0, 0.0, 1.0 },
                point[3] = { 0.0, 0.0, _3dWorldCoord[2] },
                intersection[3] = { 0 };

            _2dMarker[2] = 0.0;

            pinholeCamera->UnProjectVertex(_2dMarker, s0);

            _2dMarker[2] = 1.0;

            pinholeCamera->UnProjectVertex(_2dMarker, s1);

            double error = MyMath::Maximum<double>();

            if (MyMath::SegmentPlaneIntersection3(s0, s1, normal, point, intersection) == 1)
            {
                double worldCoordError[3] = { 0 };

                // BUG: (25-Dec-2015) 2D!
                MyMath::Subtract2(_3dWorldCoord, intersection, worldCoordError);

                error = MyMath::Magnitude3(worldCoordError);

                marker->SetWorldCoordError(worldCoordError);

                // (BEGIN OF) TESTING: (25-Dec-2015) PIXEL-BASED ERROR
                my::CVector3<double> w0,
                    w1,
                    windowCoordError;

                w0 = pinholeCamera->ProjectVertex(my::CVector3<double>(_3dWorldCoord[0], _3dWorldCoord[1], _3dWorldCoord[2]));
                w1 = pinholeCamera->ProjectVertex(my::CVector3<double>(intersection[0], intersection[1], intersection[2]));

                windowCoordError = w0 - w1;

                marker->SetWindowCoordError(worldCoordError[0], worldCoordError[1]);
                // (END OF) TESTING: (25-Dec-2015) PIXEL-BASED ERROR
            }

            fitness += error;
		}
	}

	return fitness;
}

double CLacrosseMarkerArray::GetLineFitness(boost::shared_ptr<CPinholeCamera2>& pinholeCamera)
{
    const int* viewport = pinholeCamera->GetViewport();

    HEALTH_CHECK(!viewport, 0);

    double fitness = 0;

    my::sport::CLacrosseSceneryLayout lacrosseSceneryLayout;

    for (auto& marker : m_lineMarkerArray)
    {
        if (marker->IsEnabled())
        {
            std::vector<std::pair<my::CVector3<double>, my::CVector3<double> > > lineSegmentArray;

            if ((marker->GetName() == "SIDELINE-TOP (1)") ||
                (marker->GetName() == "SIDELINE-TOP (2)"))
            {
                my::CVector3<double> leftTop = lacrosseSceneryLayout.GetItemPosition(my::sport::CLacrosseSceneryLayout::LEFT_TOP_VERTEX_ITEM),
                    rightTop = lacrosseSceneryLayout.GetItemPosition(my::sport::CLacrosseSceneryLayout::RIGHT_TOP_VERTEX_ITEM);

                lineSegmentArray.push_back({ leftTop, rightTop });
            }
            if ((marker->GetName() == "ENDLINE-LEFT (1)") ||
                (marker->GetName() == "ENDLINE-LEFT (2)"))
            {
                my::CVector3<double> leftTop = lacrosseSceneryLayout.GetItemPosition(my::sport::CLacrosseSceneryLayout::LEFT_TOP_VERTEX_ITEM),
                    leftBottom = lacrosseSceneryLayout.GetItemPosition(my::sport::CLacrosseSceneryLayout::LEFT_BOTTOM_VERTEX_ITEM);

                lineSegmentArray.push_back({ leftTop, leftBottom });
            }
            else if ((marker->GetName() == "SIDELINE-BOTTOM (1)") ||
                (marker->GetName() == "SIDELINE-BOTTOM (2)"))
            {
                my::CVector3<double> leftBottom = lacrosseSceneryLayout.GetItemPosition(my::sport::CLacrosseSceneryLayout::LEFT_BOTTOM_VERTEX_ITEM),
                    rightBottom = lacrosseSceneryLayout.GetItemPosition(my::sport::CLacrosseSceneryLayout::RIGHT_BOTTOM_VERTEX_ITEM);

                lineSegmentArray.push_back({ leftBottom, rightBottom });
            }
            else if (marker->GetName() == "LEFT-AREA-LINE")
            {
                my::CVector3<double> leftEndAreaTop = lacrosseSceneryLayout.GetItemPosition(my::sport::CLacrosseSceneryLayout::LEFT_END_AREA_TOP_VERTEX_ITEM),
                    leftEndAreaBottom = lacrosseSceneryLayout.GetItemPosition(my::sport::CLacrosseSceneryLayout::LEFT_END_AREA_BOTTOM_VERTEX_ITEM),
                    leftCreaseCenter = lacrosseSceneryLayout.GetItemPosition(my::sport::CLacrosseSceneryLayout::LEFT_CREASE_CENTER_VERTEX_ITEM);

                lineSegmentArray.push_back({ leftEndAreaTop, { leftCreaseCenter.x(), leftEndAreaTop.y(), leftEndAreaTop.z() } });
                lineSegmentArray.push_back({ leftEndAreaBottom, { leftCreaseCenter.x(), leftEndAreaBottom.y(), leftEndAreaBottom.z() } });
            }
            else if (marker->GetName() == "RIGHT-AREA-LINE")
            {
                my::CVector3<double> rightEndAreaTop = lacrosseSceneryLayout.GetItemPosition(my::sport::CLacrosseSceneryLayout::RIGHT_END_AREA_TOP_VERTEX_ITEM),
                    rightEndAreaBottom = lacrosseSceneryLayout.GetItemPosition(my::sport::CLacrosseSceneryLayout::RIGHT_END_AREA_BOTTOM_VERTEX_ITEM),
                    rightCreaseCenter = lacrosseSceneryLayout.GetItemPosition(my::sport::CLacrosseSceneryLayout::RIGHT_CREASE_CENTER_VERTEX_ITEM);

                lineSegmentArray.push_back({ rightEndAreaTop, { rightCreaseCenter.x(), rightEndAreaTop.y(), rightEndAreaTop.z() } });
                lineSegmentArray.push_back({ rightEndAreaBottom, { rightCreaseCenter.x(), rightEndAreaBottom.y(), rightEndAreaBottom.z() } });
            }

            if (lineSegmentArray.empty())
            {
                LOG_ERROR();

                return 0.0;
            }

            double lineSegmentFitness = DBL_MAX;

            for (const auto& lineSegment : lineSegmentArray)
            {
                double p1[3] = { lineSegment.first.x(), lineSegment.first.y(), lineSegment.first.z() },
                    p2[3] = { lineSegment.second.x(), lineSegment.second.y(), lineSegment.second.z() };

                double _2dMarker[3] = { 0 };

                MyMath::Assign2(marker->GetScreenCoord(), _2dMarker);

                double p3[3] = { 0 },
                    p4[3] = { 0 };

                _2dMarker[2] = 0;
                pinholeCamera->UnProjectVertex(_2dMarker, p3);

                _2dMarker[2] = 1;
                pinholeCamera->UnProjectVertex(_2dMarker, p4);

                double pa[3] = { 0 },
                    pb[3] = { 0 },
                    mua = 0,
                    mub = 0;

                if (MyMath::SegmentSegmentIntersection3(p1, p2, p3, p4, pa, pb, &mua, &mub))
                {
                    double worldCoordError[3] = { 0 };

                    MyMath::Subtract2(pa, pb, worldCoordError);

                    marker->SetWorldCoordError(worldCoordError);

                    lineSegmentFitness = std::min(lineSegmentFitness, MyMath::EuclideanDistance3(pa, pb));
                }
            }

            fitness += lineSegmentFitness;
        }
    }

    return fitness;
}

void CLacrosseMarkerArray::SetError(boost::shared_ptr<CPinholeCamera2>& pinholeCamera)
{
    GetFitness(pinholeCamera);
}

boost::shared_ptr<my::video::CMarker> CLacrosseMarkerArray::GetMarker(const std::string& markerName) const
{
	for (const auto& marker : m_markerArray)
	{
		if (marker->GetName() == markerName)
			return marker;
	}

	return boost::shared_ptr<my::video::CMarker>();
}

bool CLacrosseMarkerArray::Initialize()
{
#define ADD_MARKER_TO_QUEUE(MARKER_NAME, MARKER_ICON, WORLD_COORD, SCREEN_COORD, LOCAL_ARRAY, IS_ENABLED) { \
	boost::shared_ptr<my::video::CMarker> marker(new my::video::CMarker); \
	HEALTH_CHECK(!marker, false); \
	marker->SetName(MARKER_NAME); \
	marker->SetIcon(MARKER_ICON); \
	marker->SetWorldCoord(WORLD_COORD[0], WORLD_COORD[1], WORLD_COORD[2]); \
	marker->SetScreenCoord(SCREEN_COORD); \
    marker->SetEnabled(IS_ENABLED); \
	m_markerArray.push_back(marker); \
	LOCAL_ARRAY.push_back(marker); \
}

    my::sport::CLacrosseSceneryLayout lacrosseSceneryLayout;

    my::CVector3<double> leftTop = lacrosseSceneryLayout.GetItemPosition(my::sport::CLacrosseSceneryLayout::LEFT_TOP_VERTEX_ITEM),
        centerTop = lacrosseSceneryLayout.GetItemPosition(my::sport::CLacrosseSceneryLayout::CENTER_TOP_VERTEX_ITEM),
        rightTop = lacrosseSceneryLayout.GetItemPosition(my::sport::CLacrosseSceneryLayout::RIGHT_TOP_VERTEX_ITEM),
        leftEndAreaTop = lacrosseSceneryLayout.GetItemPosition(my::sport::CLacrosseSceneryLayout::LEFT_END_AREA_TOP_VERTEX_ITEM),
        leftWingTop = lacrosseSceneryLayout.GetItemPosition(my::sport::CLacrosseSceneryLayout::LEFT_WING_TOP_VERTEX_ITEM),
        rightWingTop = lacrosseSceneryLayout.GetItemPosition(my::sport::CLacrosseSceneryLayout::RIGHT_WING_TOP_VERTEX_ITEM),
        rightEndAreaTop = lacrosseSceneryLayout.GetItemPosition(my::sport::CLacrosseSceneryLayout::RIGHT_END_AREA_TOP_VERTEX_ITEM),
        leftEndAreaBottom = lacrosseSceneryLayout.GetItemPosition(my::sport::CLacrosseSceneryLayout::LEFT_END_AREA_BOTTOM_VERTEX_ITEM),
        leftWingBottom = lacrosseSceneryLayout.GetItemPosition(my::sport::CLacrosseSceneryLayout::LEFT_WING_BOTTOM_VERTEX_ITEM),
        rightWingBottom = lacrosseSceneryLayout.GetItemPosition(my::sport::CLacrosseSceneryLayout::RIGHT_WING_BOTTOM_VERTEX_ITEM),
        rightEndAreaBottom = lacrosseSceneryLayout.GetItemPosition(my::sport::CLacrosseSceneryLayout::RIGHT_END_AREA_BOTTOM_VERTEX_ITEM),
        leftBottom = lacrosseSceneryLayout.GetItemPosition(my::sport::CLacrosseSceneryLayout::LEFT_BOTTOM_VERTEX_ITEM),
        centerBottom = lacrosseSceneryLayout.GetItemPosition(my::sport::CLacrosseSceneryLayout::CENTER_BOTTOM_VERTEX_ITEM),
        rightBottom = lacrosseSceneryLayout.GetItemPosition(my::sport::CLacrosseSceneryLayout::RIGHT_BOTTOM_VERTEX_ITEM),
        centerWingBottom = lacrosseSceneryLayout.GetItemPosition(my::sport::CLacrosseSceneryLayout::CENTER_WING_BOTTOM_VERTEX_ITEM),
        centerWingTop = lacrosseSceneryLayout.GetItemPosition(my::sport::CLacrosseSceneryLayout::CENTER_WING_TOP_VERTEX_ITEM),
        leftCreaseCenter = lacrosseSceneryLayout.GetItemPosition(my::sport::CLacrosseSceneryLayout::LEFT_CREASE_CENTER_VERTEX_ITEM),
        rightCreaseCenter = lacrosseSceneryLayout.GetItemPosition(my::sport::CLacrosseSceneryLayout::RIGHT_CREASE_CENTER_VERTEX_ITEM);

    double screenCoord[2] = { 0 },
        emptyWorldCoord[3] = { 0 };

    double yScreenCoordMax = 0.75,
        yScreenCoordStep = 0.3,
        xScreenCoordMax = -0.75,
        xScreenCoordStep = 0.5;

    int markerIndex = 0;


#define ADD_MARKER(MARKER_NAME, MARKER_ICON, WORLD_COORD, LOCAL_ARRAY) { \
    screenCoord[0] = xScreenCoordMax + xScreenCoordStep * (double)(markerIndex % 4);\
    screenCoord[1] = yScreenCoordMax - yScreenCoordStep * (double)((int)(markerIndex / 4));\
    ++markerIndex;\
    ADD_MARKER_TO_QUEUE(MARKER_NAME, MARKER_ICON, WORLD_COORD, screenCoord, LOCAL_ARRAY, true);\
}

    ADD_MARKER("LEFT-TOP", "leftTop", leftTop, m_pointMarkerArray);
    ADD_MARKER("SIDELINE-TOP (1)", "sidelineTop", emptyWorldCoord, m_lineMarkerArray);
    ADD_MARKER("CENTER-TOP", "centerTop", centerTop, m_pointMarkerArray);
    ADD_MARKER("RIGHT-TOP", "rightTop", rightTop, m_pointMarkerArray);
    ADD_MARKER("LEFT-END-AREA-TOP", "leftEndAreaTop", leftEndAreaTop, m_pointMarkerArray);
    ADD_MARKER("LEFT-WING-TOP", "leftWingTop", leftWingTop, m_pointMarkerArray);
    ADD_MARKER("RIGHT-WING-TOP", "rightWingTop", rightWingTop, m_pointMarkerArray);
    ADD_MARKER("RIGHT-END-AREA-TOP", "rightEndAreaTop", rightEndAreaTop, m_pointMarkerArray);
    ADD_MARKER("LEFT-END-AREA-BOTTOM", "leftEndAreaBottom", leftEndAreaBottom, m_pointMarkerArray);
    ADD_MARKER("LEFT-WING-BOTTOM", "leftWingBottom", leftWingBottom, m_pointMarkerArray);
    ADD_MARKER("RIGHT-WING-BOTTOM", "rightWingBottom", rightWingBottom, m_pointMarkerArray);
    ADD_MARKER("RIGHT-END-AREA-BOTTOM", "rightEndAreaBottom", rightEndAreaBottom, m_pointMarkerArray);
    ADD_MARKER("LEFT-BOTTOM", "leftBottom", leftBottom, m_pointMarkerArray);
    ADD_MARKER("SIDELINE-BOTTOM (1)", "sidelineBottom", emptyWorldCoord, m_lineMarkerArray);
    ADD_MARKER("CENTER-BOTTOM", "centerBottom", centerBottom, m_pointMarkerArray);
    ADD_MARKER("RIGHT-BOTTOM", "rightBottom", rightBottom, m_pointMarkerArray);
    ADD_MARKER("LEFT-AREA-LINE", "leftAreaLine", emptyWorldCoord, m_lineMarkerArray);
    ADD_MARKER("CENTER-WING-BOTTOM", "centerWingBottom", centerWingBottom, m_pointMarkerArray);
    ADD_MARKER("CENTER-WING-TOP", "centerWingTop", centerWingTop, m_pointMarkerArray);
    ADD_MARKER("RIGHT-AREA-LINE", "rightAreaLine", emptyWorldCoord, m_lineMarkerArray);
    ADD_MARKER("LEFT-CREASE-CENTER", "leftCreaseCenter", leftCreaseCenter, m_pointMarkerArray);
    ADD_MARKER("RIGHT-CREASE-CENTER", "rightCreaseCenter", rightCreaseCenter, m_pointMarkerArray);
    ADD_MARKER("SIDELINE-TOP (2)", "sidelineTop", emptyWorldCoord, m_lineMarkerArray);
    ADD_MARKER("SIDELINE-BOTTOM (2)", "sidelineBottom", emptyWorldCoord, m_lineMarkerArray);
    ADD_MARKER("ENDLINE-LEFT (1)", "endlineLeft", emptyWorldCoord, m_lineMarkerArray);
    ADD_MARKER("ENDLINE-LEFT (2)", "endlineLeft", emptyWorldCoord, m_lineMarkerArray);

    return true;
}
