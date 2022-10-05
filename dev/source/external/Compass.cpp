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

#ifdef _WIN32
#include <windows.h>
#endif // _WIN32

#if defined(__APPLE__)
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#endif

#include <Common.h>
#include <MyMath.h>

#include "Compass.h"

/**
*/
CCompass::CCompass()
{
    Create();
}

/**
*/
void CCompass::Draw(double horizon, float size)
{
	GLboolean isDepthTestEnabled;
	float x,
		z;
	int angle;
	float internalSize,
		externalSize;

	horizon = 2.0f*std::min((float)horizon, 1.0f);

	glMatrixMode(GL_MODELVIEW);

	glPushMatrix();

	glLoadIdentity();

	glMatrixMode(GL_PROJECTION);

	glPushMatrix();

	glLoadIdentity();

	isDepthTestEnabled = glIsEnabled(GL_DEPTH_TEST);

	glDisable(GL_DEPTH_TEST);

	glBegin(GL_QUADS);

	// Sky.
	glColor3fv(m_horizonColor);
	
	glVertex3d(-1.0, horizon, 0.99);
	glVertex3d(1.0, horizon, 0.99);
	
	glColor3fv(m_skyColor);
	
	glVertex3f(1.0f, 1.0f, 0.99f);
	glVertex3f(-1.0f, 1.0f, 0.99f);

	// Ground.
	glColor3fv(m_groundColor);

	glVertex2i(-1, -1);
	glVertex2i(1, -1);

   	glColor3f((m_horizonColor[2] + m_groundColor[0])/2.0f, (m_horizonColor[1] + m_groundColor[1])/2.0f, (m_horizonColor[2] + m_groundColor[2])/2.0f);

	glVertex2d(1.0, horizon);
	glVertex2d(-1.0, horizon);

	glEnd();

	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);

	glPopMatrix();

	if (isDepthTestEnabled)
		glEnable(GL_DEPTH_TEST);

    if (size > FLT_EPSILON)
    {
	    // Compass rose - first ring.
	    internalSize = 0.7f*size;
	    externalSize = 0.725f*size;

	    glLineWidth(1.0f);

	    glBegin(GL_LINES);

	    for (angle=0; angle!=360; ++angle) 
	    {
		    x = cosf(MyMath::DegreesToRadians((float)angle));
		    z = sinf(MyMath::DegreesToRadians((float)angle));

		    glColor3fv(m_groundColor);
		    glVertex3f(internalSize*x, 0, internalSize*z);

		    glColor3f(0.0f, 0.0f, 0.0f);
		    glVertex3f(externalSize*x, 0, externalSize*z);
	    }

	    glEnd();

	    // Second ring.
	    internalSize = 0.65f*size;

	    glLineWidth(2.0f);

	    glBegin(GL_LINES);

	    for (angle=0; angle!=360; angle+=45) 
	    {
		    x = cosf(MyMath::DegreesToRadians((float)angle));
		    z = sinf(MyMath::DegreesToRadians((float)angle));

		    glColor3fv(m_groundColor);
		    glVertex3f(internalSize*x, 0, internalSize*z);

		    glColor3f(0.0f, 0.0f, 0.0f);
		    glVertex3f(externalSize*x, 0, externalSize*z);
	    }

	    glEnd();

	    // Third ring.
	    internalSize = 0.6f*size;

	    glLineWidth(3.0f);

	    glBegin(GL_LINES);

	    for (angle=0; angle!=360; angle+=90) 
	    {
		    x = cosf(MyMath::DegreesToRadians((float)angle));
		    z = sinf(MyMath::DegreesToRadians((float)angle));

		    glColor3fv(m_groundColor);
		    glVertex3f(internalSize*x, 0, internalSize*z);

		    glColor3f(0.0f, 0.0f, 0.0f);
		    glVertex3f(externalSize*x, 0, externalSize*z);
	    }

	    glEnd();

	    // North.
	    internalSize = 0.75f*size;
	    externalSize = 0.85f*size;

	    glLineWidth(1.0f);

	    glColor3f(0.0f, 0.0f, 0.0f);

	    glBegin(GL_LINE_STRIP);

	    angle = -3;

	    x = cosf(MyMath::DegreesToRadians((float)angle));
	    z = sinf(MyMath::DegreesToRadians((float)angle));
	
	    glVertex3f(internalSize*x, 0, internalSize*z);
	    glVertex3f(externalSize*x, 0, externalSize*z);

	    angle = 3;

	    x = cosf(MyMath::DegreesToRadians((float)angle));
	    z = sinf(MyMath::DegreesToRadians((float)angle));
	
	    glVertex3f(internalSize*x, 0, internalSize*z);
	    glVertex3f(externalSize*x, 0, externalSize*z);

	    glEnd();
    }
}

/**
*/
void CCompass::SetGroundColor(float r, float g, float b)
{
	m_groundColor[0] = r;
	m_groundColor[1] = g;
	m_groundColor[2] = b;
}

/**
*/
void CCompass::SetHorizonColor(float r, float g, float b)
{
	m_horizonColor[0] = r;
	m_horizonColor[1] = g;
	m_horizonColor[2] = b;
}

/**
*/
void CCompass::SetSkyColor(float r, float g, float b)
{
	m_skyColor[0] = r;
	m_skyColor[1] = g;
	m_skyColor[2] = b;
}

/**
*/
void CCompass::Create()
{
	m_groundColor[0] = 201.0f/255.0f;
	m_groundColor[1] = 223.0f/255.0f;
	m_groundColor[2] = 175.0f/255.0f;

    m_horizonColor[0] = 0.9375f;
    m_horizonColor[1] = 0.9453f;
    m_horizonColor[2] = 0.9492f;

    m_skyColor[0] = 165.0f/255.0f;
    m_skyColor[1] = 191.0f/255.0f;
    m_skyColor[2] = 221.0f/255.0f;
}

