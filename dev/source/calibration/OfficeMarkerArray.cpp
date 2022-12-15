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

#include "OfficeMarkerArray.h"

COfficeMarkerArray::COfficeMarkerArray()
    : CMarkerGroup(),
    m_pointMarkerArray()
{
    if (!Initialize())
    {
        LOG_ERROR();

        return /*false*/;
    }
}

double COfficeMarkerArray::GetFitness(boost::shared_ptr<CPinholeCamera2>& pinholeCamera)
{
	double fitness = 0;

	fitness += GetPointFitness(pinholeCamera);

	return fitness;
}

double COfficeMarkerArray::GetPointFitness(boost::shared_ptr<CPinholeCamera2>& pinholeCamera)
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

void COfficeMarkerArray::SetError(boost::shared_ptr<CPinholeCamera2>& pinholeCamera)
{
    GetFitness(pinholeCamera);
}

boost::shared_ptr<my::video::CMarker> COfficeMarkerArray::GetMarker(const std::string& markerName) const
{
	for (const auto& marker : m_markerArray)
	{
		if (marker->GetName() == markerName)
			return marker;
	}

	return boost::shared_ptr<my::video::CMarker>();
}

bool COfficeMarkerArray::Initialize()
{
    // Helper
#define ADD_MARKER(MARKER_NAME, MARKER_ICON, WORLD_COORD, SCREEN_COORD, LOCAL_ARRAY, IS_ENABLED) { \
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

    my::sport::COfficeSceneryLayout officeSceneryLayout;

    double screenCoord[2] = { 0 };

    // OCTAGON

    my::CVector3<double> leftTopVertex = officeSceneryLayout.GetItemPosition(my::sport::COfficeSceneryLayout::LEFT_TOP_VERTEX_ITEM);

    if (leftTopVertex.IsValid())
    {
        screenCoord[0] = -0.75;
        screenCoord[1] = 0.5;

        ADD_MARKER("Left top", "officeLeftTopVertex", leftTopVertex, screenCoord, m_pointMarkerArray, true);
    }

    my::CVector3<double> leftCenterVertex = officeSceneryLayout.GetItemPosition(my::sport::COfficeSceneryLayout::LEFT_CENTER_VERTEX_ITEM);

    if (leftCenterVertex.IsValid())
    {
        screenCoord[0] = -0.75;
        screenCoord[1] = 0.0;

        ADD_MARKER("Left center", "officeLeftCenterVertex", leftCenterVertex, screenCoord, m_pointMarkerArray, true);
    }

    my::CVector3<double> leftBottomVertex = officeSceneryLayout.GetItemPosition(my::sport::COfficeSceneryLayout::LEFT_BOTTOM_VERTEX_ITEM);

    if (leftBottomVertex.IsValid())
    {
        screenCoord[0] = -0.75;
        screenCoord[1] = -0.5;

        ADD_MARKER("Left bottom", "officeLeftBottomVertex", leftBottomVertex, screenCoord, m_pointMarkerArray, true);
    }

    my::CVector3<double> rightTopVertex = officeSceneryLayout.GetItemPosition(my::sport::COfficeSceneryLayout::RIGHT_TOP_VERTEX_ITEM);

    if (rightTopVertex.IsValid())
    {
        screenCoord[0] = 0.75;
        screenCoord[1] = 0.5;

        ADD_MARKER("Right top", "officeRightTopVertex", rightTopVertex, screenCoord, m_pointMarkerArray, true);
    }

    my::CVector3<double> rightCenterVertex = officeSceneryLayout.GetItemPosition(my::sport::COfficeSceneryLayout::RIGHT_CENTER_VERTEX_ITEM);

    if (rightCenterVertex.IsValid())
    {
        screenCoord[0] = 0.75;
        screenCoord[1] = 0.0;

        ADD_MARKER("Right center", "officeRightCenterVertex", rightCenterVertex, screenCoord, m_pointMarkerArray, true);
    }

    my::CVector3<double> rightBottomVertex = officeSceneryLayout.GetItemPosition(my::sport::COfficeSceneryLayout::RIGHT_BOTTOM_VERTEX_ITEM);

    if (rightBottomVertex.IsValid())
    {
        screenCoord[0] = 0.75;
        screenCoord[1] = -0.5;

        ADD_MARKER("Right bottom", "officeRightBottomVertex", rightBottomVertex, screenCoord, m_pointMarkerArray, true);
    }

    return true;
}
