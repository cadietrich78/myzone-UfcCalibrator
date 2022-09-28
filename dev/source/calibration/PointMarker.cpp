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

#include "PointMarker.h"

/**
*/
my::video::CPointMarker::CPointMarker()
    : CMarker()
{
    Create();
}

/**
*/
my::video::CPointMarker::CPointMarker(const CPointMarker& pointMarker)
    : CMarker(pointMarker)
{
    Copy(pointMarker);
}

/**
*/
my::video::CPointMarker::~CPointMarker()
{
}

/**
*/
void my::video::CPointMarker::operator=(const CPointMarker& pointMarker)
{
    my::video::CMarker::operator=(pointMarker);

    Copy(pointMarker);
}

// TRICKY: (21-Sep-2016) ERROR ON SCREEN COORDINATES
double my::video::CPointMarker::GetFitness(boost::shared_ptr<CPinholeCamera2>& pinholeCamera)
{
    HEALTH_CHECK(!pinholeCamera, my::Null<double>());

    if (!IsEnabled())
        return my::Null<double>();

    const int *viewport = pinholeCamera->GetViewport();

    HEALTH_CHECK(!viewport, my::Null<double>());

    double fitness = 0;

    // (WORLD) POSITION WHERE THE MARKER IS
    const double *_3dMarkerCoord = GetWorldCoord();

    HEALTH_CHECK(!_3dMarkerCoord, my::Null<double>());

    // (SCREEN) POSITION WHERE THE MARKER SHOULD BE PROJECTED (2D) [-1, 1]. (1) THE MARKER HAS A WORLD POSITION; (2) IT IS PROJECTED SOMEWHERE UNDER THE CURRENT CAMERA; (3) THIS ATTRIBUTE HOLDS THE COORDINATES WHERE THE MARKER SHOULD BE PROJECTED (THE ACTUAL FEATURE ON THE IMAGE).
    my::CVector3<double> _2dMarkerCoord = pinholeCamera->ProjectVertex(my::CVector3<double>(_3dMarkerCoord[0], _3dMarkerCoord[1], _3dMarkerCoord[2]));

    _2dMarkerCoord.x() = 2.0 * (_2dMarkerCoord.x() / viewport[2]) - 1.0;
    _2dMarkerCoord.y() = 2.0 * (_2dMarkerCoord.y() / viewport[3]) - 1.0;

    // (SCREEN) POSITION WHERE THE MARKER SHOULD BE
    double _2dReferenceCoord[3] = { 0 };

    MyMath::Assign2(GetScreenCoord(), _2dReferenceCoord);

    fitness += my::CVector2<double>(_2dReferenceCoord[0], _2dReferenceCoord[1]).Distance(_2dMarkerCoord.xy());

    SetScreenCoordError(_2dReferenceCoord[0] - _2dMarkerCoord[0], _2dReferenceCoord[1] - _2dMarkerCoord[1]);

    // WORLD COORD ERROR

    double _3dReferencePointA[3] = { 0 },
        _3dReferencePointB[3] = { 0 };

    _2dReferenceCoord[2] = 0;
    pinholeCamera->UnProjectVertex(_2dReferenceCoord, _3dReferencePointA);

    _2dReferenceCoord[2] = 1;
    pinholeCamera->UnProjectVertex(_2dReferenceCoord, _3dReferencePointB);

    double u = _3dReferencePointA[2] / (_3dReferencePointA[2] - _3dReferencePointB[2]);

    double _3dReferenceCoord[3] = {
        _3dReferencePointA[0] + u * (_3dReferencePointB[0] - _3dReferencePointA[0]),
        _3dReferencePointA[1] + u * (_3dReferencePointB[1] - _3dReferencePointA[1]),
        _3dReferencePointA[2] + u * (_3dReferencePointB[2] - _3dReferencePointA[2])
    };

    SetWorldCoordError(_3dReferenceCoord[0] - _3dMarkerCoord[0], _3dReferenceCoord[1] - _3dMarkerCoord[1], 0);

    return fitness;
}

/**
*/
void my::video::CPointMarker::Create()
{
    //
}

/**
*/
void my::video::CPointMarker::Copy(const CPointMarker& pointMarker)
{
    //
}

