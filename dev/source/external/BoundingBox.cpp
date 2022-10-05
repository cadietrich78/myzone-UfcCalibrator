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

#include <BoundingBox.h>

/**
*/
CBoundingBox::CBoundingBox()
{
	Create();
}

/**
*/
CBoundingBox::CBoundingBox(const CBoundingBox& boundingBox)
{
    Copy(boundingBox);
}

/**
*/
CBoundingBox::CBoundingBox(double xMin, double yMin, double zMin, double xMax, double yMax, double zMax)
{
    SetMinX(xMin);
    SetMinY(yMin);
    SetMinZ(zMin);
    SetMaxX(xMax);
    SetMaxY(yMax);
    SetMaxZ(zMax);
}

/**
*/
CBoundingBox::~CBoundingBox()
{
}

/**
*/
void CBoundingBox::operator=(const CBoundingBox& boundingBox)
{
    Copy(boundingBox);
}

/**
*/
bool CBoundingBox::SetMesh(const double *vertexArrayPointer, int vertexCount, int vertexSize)
{
    HEALTH_CHECK(!vertexArrayPointer, false);
    HEALTH_CHECK(vertexCount <= 0, false);
    HEALTH_CHECK((vertexSize != 3) && (vertexSize != 2), false);

    const double *vertex = vertexArrayPointer;

    for (my::int32 i = 0; i < vertexCount; i++)
	{
		if (m_xMin > *vertex)
			m_xMin = *vertex;
		if (m_xMax < *vertex)
			m_xMax = *vertex;

		++vertex;

        if (vertexSize >= 2)
        {
		    if (m_yMin > *vertex)
			    m_yMin = *vertex;
		    if (m_yMax < *vertex)
			    m_yMax = *vertex;

		    ++vertex;
        }

        if (vertexSize >= 3)
        {
		    if (m_zMin > *vertex)
			    m_zMin = *vertex;
		    if (m_zMax < *vertex)
			    m_zMax = *vertex;

		    ++vertex;
        }
	}

	m_width = m_xMax - m_xMin;
	m_height = m_yMax - m_yMin;
	m_depth = m_zMax - m_zMin;
	
    m_centerOnX = m_xMin + (double)0.5 * m_width;
    m_centerOnY = m_yMin + (double)0.5 * m_height;
    m_centerOnZ = m_zMin + (double)0.5 * m_depth;

	return true;
}

/**
*/
bool CBoundingBox::SetMesh(const CVertexPool *mesh)
{
	Create();

	return AddMesh(mesh);
}

bool CBoundingBox::SetMesh(boost::shared_ptr<const CVertexPool> mesh)
{
	HEALTH_CHECK(!mesh, false);

	return SetMesh(mesh.get());
}

/**
*/
bool CBoundingBox::AddMesh(const CVertexPool *mesh)
{
	HEALTH_CHECK(mesh == 0, false);

	my::int32 vertexCount = mesh->GetVertexCount(),
        vertexSize = mesh->GetVertexSize();

    const double *vertex = mesh->GetVertexArray();

    HEALTH_CHECK(vertex == 0, false);

    // (BEGIN OF) BUG: (04-Jan-2021)
    if (my::IsNull(m_xMin))
        m_xMin = vertex[0];

    if (my::IsNull(m_xMax))
        m_xMax = vertex[0];

    if (my::IsNull(m_yMin))
        m_yMin = vertex[1];

    if (my::IsNull(m_yMax))
        m_yMax = vertex[1];

    if (my::IsNull(m_zMin))
        m_zMin = vertex[2];

    if (my::IsNull(m_zMax))
        m_zMax = vertex[2];
    // (END OF) BUG: (04-Jan-2021)

    for (my::int32 i = 0; i != vertexCount; i++)
	{
		if (m_xMin > *vertex)
			m_xMin = *vertex;
		if (m_xMax < *vertex)
			m_xMax = *vertex;

		++vertex;

        if (vertexSize >= 2)
        {
            if (m_yMin > *vertex)
                m_yMin = *vertex;
            if (m_yMax < *vertex)
                m_yMax = *vertex;

            ++vertex;
        }

        if (vertexSize >= 3)
        {
            if (m_zMin > *vertex)
                m_zMin = *vertex;
            if (m_zMax < *vertex)
                m_zMax = *vertex;

            ++vertex;
        }
    }

	m_width = m_xMax - m_xMin;
	m_height = m_yMax - m_yMin;
	m_depth = m_zMax - m_zMin;
	
    m_centerOnX = m_xMin + (double)0.5 * m_width;
	m_centerOnY = m_yMin + (double)0.5 * m_height;
	m_centerOnZ = m_zMin + (double)0.5 * m_depth;

	return true;
}

/**
*/
bool CBoundingBox::AddMesh(boost::shared_ptr<CVertexPool>& mesh)
{
	HEALTH_CHECK(!mesh.get(), false);

	return AddMesh(mesh.get());
}

/**
*/
bool CBoundingBox::AddVertex(double x, double y, double z)
{
    if (my::IsNull(m_xMin) ||
        (m_xMin > x))
    {
        m_xMin = x;
    }

    if (my::IsNull(m_xMax) ||
        (m_xMax < x))
    {
        m_xMax = x;
    }

    if (my::IsNull(m_yMin) ||
        (m_yMin > y))
    {
        m_yMin = y;
    }

    if (my::IsNull(m_yMax) ||
        (m_yMax < y))
    {
        m_yMax = y;
    }

    if (my::IsNull(m_zMin) ||
        (m_zMin > z))
    {
        m_zMin = z;
    }

    if (my::IsNull(m_zMax) ||
        (m_zMax < z))
    {
        m_zMax = z;
    }

	m_width = m_xMax - m_xMin;
	m_height = m_yMax - m_yMin;
	m_depth = m_zMax - m_zMin;
	
	m_centerOnX = m_xMin + (double)0.5 * m_width;
	m_centerOnY = m_yMin + (double)0.5 * m_height;
	m_centerOnZ = m_zMin + (double)0.5 * m_depth;

	return true;
}

/**
*/
bool CBoundingBox::AddVertex(my::CVector3<double> vertex)
{
    HEALTH_CHECK(!vertex.IsValid(), false);

    if (!AddVertex(vertex.x(), vertex.y(), vertex.z()))
    {
        LOG_ERROR();

        return false;
    }

    return true;
}


/**
*/
bool CBoundingBox::AddBoundingBox(const CBoundingBox& boundingBox)
{
	if (m_xMin > boundingBox.m_xMin)
		m_xMin = boundingBox.m_xMin;
	if (m_xMax < boundingBox.m_xMax)
		m_xMax = boundingBox.m_xMax;

	if (m_yMin > boundingBox.m_yMin)
		m_yMin = boundingBox.m_yMin;
	if (m_yMax < boundingBox.m_yMax)
		m_yMax = boundingBox.m_yMax;

	if (m_zMin > boundingBox.m_zMin)
		m_zMin = boundingBox.m_zMin;
	if (m_zMax < boundingBox.m_zMax)
		m_zMax = boundingBox.m_zMax;

	m_width = m_xMax - m_xMin;
	m_height = m_yMax - m_yMin;
	m_depth = m_zMax - m_zMin;
	
	m_centerOnX = m_xMin + (double)0.5 * m_width;
	m_centerOnY = m_yMin + (double)0.5 * m_height;
	m_centerOnZ = m_zMin + (double)0.5 * m_depth;

	return true;
}

/**
*/
bool CBoundingBox::Contains(double x, double y, double z) const
{
    if ((x < m_xMin) ||
        (x > m_xMax))
    {
        return false;
    }

    if ((y < m_yMin) ||
        (y > m_yMax))
    {
        return false;
    }

    if ((z < m_zMin) ||
        (z > m_zMax))
    {
        return false;
    }

	return true;
}

/**
*/
bool CBoundingBox::Intersects(const CBoundingBox& boundingBox) const
{
	double xMin,
        xMax,
        yMin,
        yMax,
        zMin,
        zMax;

    // Max of min's.
    xMin = (m_xMin > boundingBox.m_xMin) ? m_xMin : boundingBox.m_xMin;
    yMin = (m_yMin > boundingBox.m_yMin) ? m_yMin : boundingBox.m_yMin;
    zMin = (m_zMin > boundingBox.m_zMin) ? m_zMin : boundingBox.m_zMin;

    // Min of max's.
    xMax = (m_xMax < boundingBox.m_xMax) ? m_xMax : boundingBox.m_xMax;
    yMax = (m_yMax < boundingBox.m_yMax) ? m_yMax : boundingBox.m_yMax;
    zMax = (m_zMax < boundingBox.m_zMax) ? m_zMax : boundingBox.m_zMax;

    return (xMin <= xMax) &&
        (yMin <= yMax) &&
        (zMin <= zMax);
}

bool CBoundingBox::IsEmpty() const
{
    return (m_width == 0) ||
        (m_height == 0) ||
        (m_depth == 0);
}

bool CBoundingBox::IsValid() const
{
    if (my::IsNull(m_xMin))
        return false;

    if (my::IsNull(m_xMax))
        return false;

    if (my::IsNull(m_yMin))
        return false;

    if (my::IsNull(m_yMax))
        return false;

    if (my::IsNull(m_zMin))
        return false;

    if (my::IsNull(m_zMax))
        return false;

    return true;
}

double CBoundingBox::GetMinX() const
{
	return m_xMin;
}

void CBoundingBox::SetMinX(double xMin)
{
    m_xMin = xMin;

    if (!my::IsNull(m_xMin) &&
        !my::IsNull(m_xMax))
    {
        m_width = m_xMax - m_xMin;

        m_centerOnX = m_xMin + (double)0.5 * m_width;
    }
}

double CBoundingBox::GetMinY() const
{
	return m_yMin;
}

void CBoundingBox::SetMinY(double yMin)
{
    m_yMin = yMin;

    if (!my::IsNull(m_yMin) &&
        !my::IsNull(m_yMax))
    {
        m_height = m_yMax - m_yMin;

        m_centerOnY = m_yMin + (double)0.5 * m_height;
    }
}

double CBoundingBox::GetMinZ() const
{
	return m_zMin;
}

void CBoundingBox::SetMinZ(double zMin)
{
    m_zMin = zMin;

    if (!my::IsNull(m_zMin) &&
        !my::IsNull(m_zMax))
    {
        m_depth = m_zMax - m_zMin;

        m_centerOnZ = m_zMin + (double)0.5 * m_depth;
    }
}

double CBoundingBox::GetMaxX() const
{
	return m_xMax;
}

void CBoundingBox::SetMaxX(double xMax)
{
    m_xMax = xMax;

    if (!my::IsNull(m_xMin) &&
        !my::IsNull(m_xMax))
    {
        m_width = m_xMax - m_xMin;

        m_centerOnX = m_xMin + (double)0.5 * m_width;
    }
}

double CBoundingBox::GetMaxY() const
{
	return m_yMax;
}

void CBoundingBox::SetMaxY(double yMax)
{
	m_yMax = yMax;

    if (!my::IsNull(m_yMin) &&
        !my::IsNull(m_yMax))
    {
        m_height = m_yMax - m_yMin;

        m_centerOnY = m_yMin + (double)0.5 * m_height;
    }
}

double CBoundingBox::GetMaxZ() const
{
	return m_zMax;
}

void CBoundingBox::SetMaxZ(double zMax)
{
	m_zMax = zMax;

    if (!my::IsNull(m_zMin) &&
        !my::IsNull(m_zMax))
    {
        m_depth = m_zMax - m_zMin;

        m_centerOnZ = m_zMin + (double)0.5 * m_depth;
    }
}

void CBoundingBox::Set(double xMin, double yMin, double zMin, double xMax, double yMax, double zMax)
{
    SetMinX(xMin);
    SetMinY(yMin);
    SetMinZ(zMin);
    SetMaxX(xMax);
    SetMaxY(yMax);
    SetMaxZ(zMax);
}

double CBoundingBox::GetWidth() const
{
	return m_width;
}

double CBoundingBox::GetHeight() const
{
	return m_height;
}

double CBoundingBox::GetDepth() const
{
	return m_depth;
}

double CBoundingBox::GetCenterOnX() const
{
	return m_centerOnX;
}

double CBoundingBox::GetCenterOnY() const
{
	return m_centerOnY;
}

double CBoundingBox::GetCenterOnZ() const
{
	return m_centerOnZ;
}

/**
*/
double CBoundingBox::GetDiagonal() const
{
    HEALTH_CHECK(!IsValid(), false);

    return MyMath::Magnitude(m_width, m_height, m_depth);
}

// [WORLD COORDINATES] -> [[0,1], [0,1], [0,1]]
my::CVector3<double> CBoundingBox::Project(my::CVector3<double> point) const
{
    HEALTH_CHECK(!point.IsValid(), my::CVector3<double>());

    my::CVector3<double> projectedPoint(0.0, 0.0, 0.0);

    if (!MyMath::IsZero(m_width))
        projectedPoint.x() = (point.x() - m_xMin) / m_width;

    if (!MyMath::IsZero(m_height))
        projectedPoint.y() = (point.y() - m_yMin) / m_height;

    if (!MyMath::IsZero(m_depth))
        projectedPoint.z() = (point.z() - m_zMin) / m_depth;

    return projectedPoint;
}

// [[0,1], [0,1], [0,1]] -> [WORLD COORDINATES]
my::CVector3<double> CBoundingBox::UnProject(my::CVector3<double> point) const
{
    HEALTH_CHECK(!point.IsValid(), my::CVector3<double>());

    my::CVector3<double> unProjectedPoint(0.0, 0.0, 0.0);

    unProjectedPoint.Set(m_xMin + point.x() * m_width, m_yMin + point.y() * m_height, m_zMin + point.z() * m_depth);

    return unProjectedPoint;
}

/**
*/
void CBoundingBox::Clear()
{
	Create();
}

/**
*/
void CBoundingBox::Copy(const CBoundingBox& boundingBox)
{
    m_xMin = boundingBox.m_xMin;
    m_xMax = boundingBox.m_xMax;
    m_yMin = boundingBox.m_yMin;
    m_yMax = boundingBox.m_yMax;
    m_zMin = boundingBox.m_zMin;
    m_zMax = boundingBox.m_zMax;
    m_width = boundingBox.m_width;
    m_height = boundingBox.m_height;
    m_depth = boundingBox.m_depth;
    m_centerOnX = boundingBox.m_centerOnX;
    m_centerOnY = boundingBox.m_centerOnY;
    m_centerOnZ = boundingBox.m_centerOnZ;
}

/**
*/
void CBoundingBox::Create()
{
	m_xMin = my::Null<double>();
    m_xMax = my::Null<double>();
    m_yMin = my::Null<double>();
    m_yMax = my::Null<double>();
    m_zMin = my::Null<double>();
    m_zMax = my::Null<double>();
	m_width = 0;
	m_height = 0;
	m_depth = 0;
	m_centerOnX = 0;
	m_centerOnY = 0;
	m_centerOnZ = 0;
}

