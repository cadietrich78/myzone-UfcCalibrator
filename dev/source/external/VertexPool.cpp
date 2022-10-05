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

#ifdef _MSC_VER
#define _CRT_SECURE_NO_DEPRECATE
#endif // _MSC_VER

#include <iostream>
#include <fstream>
#include <iomanip>
#include <algorithm>

#include <Common.h>
#include <Logger.h>
#include <MyMath.h>

#include <VertexPool.h>

// An weak attempt to predict the maximum line size of an OBJ file.
#define OBJ_MAX_LINE_SIZE 256

using namespace std;

/**
*/
CVertexPool::CVertexPool()
{
	Create();
}

/**
*/
CVertexPool::~CVertexPool()
{
}

/**
*/
void CVertexPool::SetName(const std::string& name)
{
	m_name = name;
}

/**
*/
const std::string CVertexPool::GetName() const
{
	return m_name;
}

/**
*/
void CVertexPool::SetMode(POLYGON_MODE mode)
{
	m_mode = mode;
}

/**
*/
CVertexPool::POLYGON_MODE CVertexPool::GetMode() const
{
	return CVertexPool::POLYGON_MODE(m_mode);
}

/**
*/
void CVertexPool::AddLabel(int label)
{ 
	m_labelArray.push_back(label);
}

/**
*/
void CVertexPool::SetLabel(int label)
{ 
	int i,
		vertexCount;

	ClearLabels();

	vertexCount = GetVertexCount();

	for (i=0; i<vertexCount; ++i)
		m_labelArray.push_back(label);
}

/**
*/
void CVertexPool::SetLabel(int vertexIndex, int label)
{
	if (vertexIndex < (int)m_labelArray.size())
		m_labelArray[vertexIndex] = label;
	else 
		LOG_ERROR();
}

/**
*/
int CVertexPool::GetLabel(int vertexIndex) const
{
	if (vertexIndex < (int)m_labelArray.size())
		return m_labelArray[vertexIndex];

	LOG_ERROR();

	return 0;
}

/**
*/
const int *CVertexPool::GetLabelArray() const
{
	if (m_labelArray.empty())
		return 0;

	return &m_labelArray[0];
}

/**
*/
int CVertexPool::GetLabelCount() const
{
	return (int)m_labelArray.size();
}

/**
*/
void CVertexPool::ClearLabels()
{
	m_labelArray.clear();
}

/**
*/
int CVertexPool::GetIndexFromLabel(int label) const
{
	std::vector<int>::const_iterator labelIterator = std::find(m_labelArray.begin(), m_labelArray.end(), label);

	if (labelIterator == m_labelArray.end())
		return -1;

	return (int)(labelIterator - m_labelArray.begin());
}

/**
*/
void CVertexPool::AddColor(ValueType r, ValueType g, ValueType b)
{ 
	if (!m_colorSize)
		m_colorSize = 3;

	if (m_colorSize == 3)
	{
		m_colorArray.push_back(r);
		m_colorArray.push_back(g);
		m_colorArray.push_back(b);
	}
	else
		LOG_ERROR();
}

/**
*/
void CVertexPool::AddColor(ValueType r, ValueType g, ValueType b, ValueType a)
{ 
	if (!m_colorSize)
		m_colorSize = 4;

	if (m_colorSize == 4)
	{
		m_colorArray.push_back(r);
		m_colorArray.push_back(g);
		m_colorArray.push_back(b);
		m_colorArray.push_back(a);
	}
	else
		LOG_ERROR();
}

/**
*/
void CVertexPool::AddColor(const ValueType *begin, const ValueType *end)
{
	int size;

	size = (int)(end - begin);

	if (!m_colorSize)
		m_colorSize = size;

	if (m_colorSize == size)
		m_colorArray.insert(m_colorArray.end(), begin, end);
	else
		LOG_ERROR();
}

/**
*/
void CVertexPool::SetColor(ValueType r, ValueType g, ValueType b)
{ 
	int i,
		vertexCount;

	ClearColors();

	m_colorSize = 3;

	vertexCount = GetVertexCount();

	for (i=0; i<vertexCount; ++i) {
		m_colorArray.push_back(r);
		m_colorArray.push_back(g);
		m_colorArray.push_back(b);
	}
}

/**
*/
void CVertexPool::SetColor(int vertexIndex, ValueType r, ValueType g, ValueType b)
{
	ValueType *color;

	vertexIndex *= m_colorSize;

	if ((vertexIndex < (int)m_colorArray.size()) && (m_colorSize == 3))
	{
		color = &m_colorArray[0] + vertexIndex;

		*color++ = r;
		*color++ = g;
		*color = b;
	}
	else 
		LOG_ERROR();
}

/**
*/
void CVertexPool::SetColor(ValueType r, ValueType g, ValueType b, ValueType a)
{ 
	int i,
		vertexCount;

	ClearColors();

	m_colorSize = 4;

	vertexCount = GetVertexCount();

    for (i = 0; i < vertexCount; ++i)
    {
        m_colorArray.push_back(r);
        m_colorArray.push_back(g);
        m_colorArray.push_back(b);
        m_colorArray.push_back(a);
    }
}

/**
*/
void CVertexPool::SetColor(int vertexIndex, ValueType r, ValueType g, ValueType b, ValueType a)
{
	ValueType *color;

	vertexIndex *= m_colorSize;

	if ((vertexIndex < (int)m_colorArray.size()) && (m_colorSize == 4))
	{
		color = &m_colorArray[0] + vertexIndex;

		*color++ = r;
		*color++ = g;
		*color++ = b;
		*color = a;
	}
	else 
		LOG_ERROR();
}

/**
*/
void CVertexPool::SetColor(int vertexIndex, const ValueType *begin, const ValueType *end)
{
	int size;
	ValueType *color;

	size = (int)(end - begin);

	vertexIndex *= m_colorSize;

	if ((vertexIndex < (int)m_colorArray.size()) && (m_colorSize == size))
	{
		color = &m_colorArray[0] + vertexIndex;

		for ( ; size--; )
			*color++ = *begin++;
	}
	else 
		LOG_ERROR();
}

/**
*/
void CVertexPool::SetColorSize(int size)
{
	if (size > 0)
		m_colorSize = size;
	else 
		LOG_ERROR();
}

/**
*/
const CVertexPool::ValueType *CVertexPool::GetColor(int vertexIndex) const
{
	vertexIndex *= m_colorSize;

	if (vertexIndex < (int)m_colorArray.size())
		return (&m_colorArray[0] + vertexIndex);

	LOG_ERROR();

	return 0;
}

/**
*/
const CVertexPool::ValueType *CVertexPool::GetColorArray() const
{
	if (m_colorArray.empty())
		return 0;

	return &m_colorArray[0];
}

/**
*/
int CVertexPool::GetColorSize() const
{
	return m_colorSize;
}

/**
*/
int CVertexPool::GetColorCount() const
{
	if (!m_colorSize)
		return 0;

	return (int)m_colorArray.size()/m_colorSize;
}

/**
*/
void CVertexPool::ClearColors()
{
	m_colorSize = 0;

	m_colorArray.clear();
}

/**
*/
void CVertexPool::SetOpacity(double opacity)
{
	if ((m_colorSize == 4) &&
		!m_colorArray.empty())
	{
		int colorCount = (int)m_colorArray.size() / 4;

		ValueType* color = &m_colorArray[0];

		while (colorCount--)
		{
			color[3] = opacity;

			color += 4;
		}
	}
}

/**
*/
void CVertexPool::AddTexCoord(ValueType s)
{ 
	if (!m_texCoordSize)
		m_texCoordSize = 1;

	if (m_texCoordSize == 1)
		m_texCoordArray.push_back(s);
	else
		LOG_ERROR();
}

/**
*/
void CVertexPool::AddTexCoord(ValueType s, ValueType t)
{ 
	if (!m_texCoordSize)
		m_texCoordSize = 2;

	if (m_texCoordSize == 2)
	{
		m_texCoordArray.push_back(s);
		m_texCoordArray.push_back(t);
	}
	else
		LOG_ERROR();
}

/**
*/
void CVertexPool::AddTexCoord(ValueType s, ValueType t, ValueType r)
{ 
	if (!m_texCoordSize)
		m_texCoordSize = 3;

	if (m_texCoordSize == 3)
	{
		m_texCoordArray.push_back(s);
		m_texCoordArray.push_back(t);
		m_texCoordArray.push_back(r);
	}
	else
		LOG_ERROR();
}

/**
*/
void CVertexPool::AddTexCoord(ValueType s, ValueType t, ValueType r, ValueType q)
{
	if (!m_texCoordSize)
		m_texCoordSize = 4;

	if (m_texCoordSize == 4)
	{
		m_texCoordArray.push_back(s);
		m_texCoordArray.push_back(t);
		m_texCoordArray.push_back(r);
		m_texCoordArray.push_back(q);
	}
	else
		LOG_ERROR();
}

/**
*/
void CVertexPool::AddTexCoord(const ValueType *begin, const ValueType *end)
{
	int size;

	size = (int)(end - begin);

	if (!m_texCoordSize)
		m_texCoordSize = size;

	if (m_texCoordSize == size)
		m_texCoordArray.insert(m_texCoordArray.end(), begin, end);
	else
		LOG_ERROR();
}

/**
*/
void CVertexPool::SetTexCoord(int vertexIndex, ValueType s)
{
	ValueType *texCoord;

	vertexIndex *= m_texCoordSize;

	if ((vertexIndex < (int)m_texCoordArray.size()) && (m_texCoordSize == 1))
	{
		texCoord = &m_texCoordArray[0] + vertexIndex;

		*texCoord = s;
	}
	else 
		LOG_ERROR();
}

/**
*/
void CVertexPool::SetTexCoord(int vertexIndex, ValueType s, ValueType t)
{
	ValueType *texCoord;

	vertexIndex *= m_texCoordSize;

	if ((vertexIndex < (int)m_texCoordArray.size()) && (m_texCoordSize == 2))
	{
		texCoord = &m_texCoordArray[0] + vertexIndex;

		*texCoord++ = s;
		*texCoord = t;
	}
	else 
		LOG_ERROR();
}

/**
*/
void CVertexPool::SetTexCoord(int vertexIndex, ValueType s, ValueType t, ValueType r)
{
	ValueType *texCoord;

	vertexIndex *= m_texCoordSize;

	if ((vertexIndex < (int)m_texCoordArray.size()) && (m_texCoordSize == 3))
	{
		texCoord = &m_texCoordArray[0] + vertexIndex;

		*texCoord++ = s;
		*texCoord++ = t;
		*texCoord = r;
	}
	else 
		LOG_ERROR();
}

/**
*/
void CVertexPool::SetTexCoord(int vertexIndex, ValueType s, ValueType t, ValueType r, ValueType q)
{
	ValueType *texCoord;

	vertexIndex *= m_texCoordSize;

	if ((vertexIndex < (int)m_texCoordArray.size()) && (m_texCoordSize == 4))
	{
		texCoord = &m_texCoordArray[0] + vertexIndex;

		*texCoord++ = s;
		*texCoord++ = t;
		*texCoord++ = r;
		*texCoord = q;
	}
	else 
		LOG_ERROR();
}

/**
*/
void CVertexPool::SetTexCoord(int vertexIndex, const ValueType *begin, const ValueType *end)
{
	int size;
	ValueType *texCoord;

	size = (int)(end - begin);

	vertexIndex *= m_texCoordSize;

	if ((vertexIndex < (int)m_texCoordArray.size()) && (m_texCoordSize == size))
	{
		texCoord = &m_texCoordArray[0] + vertexIndex;

		for ( ; size--; )
			*texCoord++ = *begin++;
	}
	else 
		LOG_ERROR();
}

/**
*/
void CVertexPool::SetTexCoordSize(int size)
{
	if (size > 0)
		m_texCoordSize = size;
	else 
		LOG_ERROR();
}

/**
*/
const CVertexPool::ValueType *CVertexPool::GetTexCoord(int vertexIndex) const
{
	vertexIndex *= m_texCoordSize;

	if (vertexIndex < (int)m_texCoordArray.size())
		return (&m_texCoordArray[0] + vertexIndex);

	LOG_ERROR();

	return 0;
}

/**
*/
const CVertexPool::ValueType *CVertexPool::GetTexCoordArray() const
{
	if (m_texCoordArray.empty())
		return 0;

	return &m_texCoordArray[0];
}

/**
*/
int CVertexPool::GetTexCoordSize() const
{
	return m_texCoordSize;
}

/**
*/
int CVertexPool::GetTexCoordCount() const
{
	if (!m_texCoordSize)
		return 0;

	return (int)m_texCoordArray.size()/m_texCoordSize;
}

/**
*/
void CVertexPool::ClearTexCoords()
{
	m_texCoordSize = 0;

	m_texCoordArray.clear();
}

/**
*/
void CVertexPool::AddNormal(ValueType x, ValueType y, ValueType z)
{
	m_normalArray.push_back(x);
	m_normalArray.push_back(y);
	m_normalArray.push_back(z);
}

/**
*/
void CVertexPool::AddNormal(const ValueType *begin, const ValueType *end)
{
	int size;

	size = (int)(end - begin);

	if (size == 3)
		m_normalArray.insert(m_normalArray.end(), begin, end);
	else
		LOG_ERROR();
}

/**
*/
void CVertexPool::SetNormal(int vertexIndex, ValueType x, ValueType y, ValueType z)
{
	ValueType *normal;

	vertexIndex *= 3;

	if (vertexIndex < (int)m_normalArray.size())
	{
		normal = &m_normalArray[0] + vertexIndex;

		*normal++ = x;
		*normal++ = y;
		*normal = z;
	}
	else 
		LOG_ERROR();
}

/**
*/
void CVertexPool::SetNormal(int vertexIndex, const ValueType *begin, const ValueType *end)
{
	int size;
	ValueType *normal;

	size = (int)(end - begin);

	vertexIndex *= 3;

	if (vertexIndex < (int)m_normalArray.size())
	{
		normal = &m_normalArray[0] + vertexIndex;

		for ( ; size--; )
			*normal++ = *begin++;
	}
	else 
		LOG_ERROR();
}

/**
*/
const CVertexPool::ValueType *CVertexPool::GetNormal(int vertexIndex) const
{
    vertexIndex *= 3;

    if (vertexIndex < (int)m_normalArray.size())
        return (&m_normalArray[0] + vertexIndex);

    LOG_ERROR();

    return 0;
}

/**
*/
CVertexPool::ValueType *CVertexPool::GetNormal(int vertexIndex)
{
    vertexIndex *= 3;

    if (vertexIndex < (int)m_normalArray.size())
        return (&m_normalArray[0] + vertexIndex);

    LOG_ERROR();

    return 0;
}

/**
*/
const CVertexPool::ValueType *CVertexPool::GetNormalArray() const
{
	if (m_normalArray.empty())
		return 0;

	return &m_normalArray[0];
}

/**
*/
int CVertexPool::GetNormalCount() const
{
	return (int)m_normalArray.size()/3;
}

/**
*/
void CVertexPool::ClearNormals()
{
	m_normalArray.clear();
}

/**
*/
void CVertexPool::AddBinormal(ValueType x, ValueType y, ValueType z)
{
	m_binormalArray.push_back(x);
	m_binormalArray.push_back(y);
	m_binormalArray.push_back(z);
}

/**
*/
void CVertexPool::AddBinormal(const ValueType *begin, const ValueType *end)
{
	int size;

	size = (int)(end - begin);

	if (size == 3)
		m_binormalArray.insert(m_binormalArray.end(), begin, end);
	else
		LOG_ERROR();
}

/**
*/
void CVertexPool::SetBinormal(int vertexIndex, ValueType x, ValueType y, ValueType z)
{
	ValueType *normal;

	vertexIndex *= 3;

	if (vertexIndex < (int)m_binormalArray.size())
	{
		normal = &m_binormalArray[0] + vertexIndex;

		*normal++ = x;
		*normal++ = y;
		*normal = z;
	}
	else 
		LOG_ERROR();
}

/**
*/
void CVertexPool::SetBinormal(int vertexIndex, const ValueType *begin, const ValueType *end)
{
	int size;
	ValueType *normal;

	size = (int)(end - begin);

	vertexIndex *= 3;

	if (vertexIndex < (int)m_binormalArray.size())
	{
		normal = &m_binormalArray[0] + vertexIndex;

		for ( ; size--; )
			*normal++ = *begin++;
	}
	else 
		LOG_ERROR();
}

/**
*/
const CVertexPool::ValueType *CVertexPool::GetBinormal(int vertexIndex) const
{
	vertexIndex *= 3;

	if (vertexIndex < (int)m_binormalArray.size())
		return (&m_binormalArray[0] + vertexIndex);

	LOG_ERROR();

	return 0;
}

/**
*/
const CVertexPool::ValueType *CVertexPool::GetBinormalArray() const
{
	if (m_binormalArray.empty())
		return 0;

	return &m_binormalArray[0];
}

/**
*/
int CVertexPool::GetBinormalCount() const
{
	return (int)m_binormalArray.size()/3;
}

/**
*/
void CVertexPool::ClearBinormals()
{
	m_binormalArray.clear();
}

/**
*/
void CVertexPool::AddTangent(ValueType x, ValueType y, ValueType z)
{
	m_tangentArray.push_back(x);
	m_tangentArray.push_back(y);
	m_tangentArray.push_back(z);
}

/**
*/
void CVertexPool::AddTangent(const ValueType *begin, const ValueType *end)
{
	int size;

	size = (int)(end - begin);

	if (size == 3)
		m_tangentArray.insert(m_tangentArray.end(), begin, end);
	else
		LOG_ERROR();
}

/**
*/
void CVertexPool::SetTangent(int vertexIndex, ValueType x, ValueType y, ValueType z)
{
	ValueType *normal;

	vertexIndex *= 3;

	if (vertexIndex < (int)m_tangentArray.size())
	{
		normal = &m_tangentArray[0] + vertexIndex;

		*normal++ = x;
		*normal++ = y;
		*normal = z;
	}
	else 
		LOG_ERROR();
}

/**
*/
void CVertexPool::SetTangent(int vertexIndex, const ValueType *begin, const ValueType *end)
{
	int size;
	ValueType *normal;

	size = (int)(end - begin);

	vertexIndex *= 3;

	if (vertexIndex < (int)m_tangentArray.size())
	{
		normal = &m_tangentArray[0] + vertexIndex;

		for ( ; size--; )
			*normal++ = *begin++;
	}
	else 
		LOG_ERROR();
}

/**
*/
const CVertexPool::ValueType *CVertexPool::GetTangent(int vertexIndex) const
{
	vertexIndex *= 3;

	if (vertexIndex < (int)m_tangentArray.size())
		return (&m_tangentArray[0] + vertexIndex);

	LOG_ERROR();

	return 0;
}

/**
*/
const CVertexPool::ValueType *CVertexPool::GetTangentArray() const
{
	if (m_tangentArray.empty())
		return 0;

	return &m_tangentArray[0];
}

/**
*/
int CVertexPool::GetTangentCount() const
{
	return (int)m_tangentArray.size()/3;
}

/**
*/
void CVertexPool::ClearTangents()
{
	m_tangentArray.clear();
}

/**
*/
void CVertexPool::AddVertex(ValueType x)
{ 
	if (!m_vertexSize)
		m_vertexSize = 1;

	if (m_vertexSize == 1)
		m_vertexArray.push_back(x);
	else
		LOG_ERROR();
}

/**
*/
void CVertexPool::AddVertex(ValueType x, ValueType y)
{ 
	if (!m_vertexSize)
		m_vertexSize = 2;

	if (m_vertexSize == 2)
	{
		m_vertexArray.push_back(x);
		m_vertexArray.push_back(y);
	}
	else
		LOG_ERROR();
}

/**
*/
void CVertexPool::AddVertex(ValueType x, ValueType y, ValueType z)
{ 
	if (!m_vertexSize)
		m_vertexSize = 3;

	if (m_vertexSize == 3)
	{
		m_vertexArray.push_back(x);
		m_vertexArray.push_back(y);
		m_vertexArray.push_back(z);
	}
	else
		LOG_ERROR();
}

/**
*/
void CVertexPool::AddVertex(ValueType x, ValueType y, ValueType z, ValueType w)
{ 
	if (!m_vertexSize)
		m_vertexSize = 4;

	if (m_vertexSize == 4)
	{
		m_vertexArray.push_back(x);
		m_vertexArray.push_back(y);
		m_vertexArray.push_back(z);
		m_vertexArray.push_back(w);
	}
	else
		LOG_ERROR();
}

/**
*/
void CVertexPool::AddVertex(const ValueType *begin, const ValueType *end)
{
	int size;

	size = (int)(end - begin);

	if (!m_vertexSize)
		m_vertexSize = size;

	if (m_vertexSize == size)
		m_vertexArray.insert(m_vertexArray.end(), begin, end);
	else
		LOG_ERROR();
}

/**
*/
void CVertexPool::AddVertex(my::CVector3<double> vertex)
{
    AddVertex(vertex.x(), vertex.y(), vertex.z());
}

/**
*/
void CVertexPool::SetVertex(int vertexIndex, ValueType x)
{
	ValueType *vertex;

	vertexIndex *= m_vertexSize;

	if ((vertexIndex < (int)m_vertexArray.size()) && (m_vertexSize == 1))
	{
		vertex = &m_vertexArray[0] + vertexIndex;

		*vertex = x;
	}
	else 
		LOG_ERROR();
}

/**
*/
void CVertexPool::SetVertex(int vertexIndex, ValueType x, ValueType y)
{
	ValueType *vertex;

	vertexIndex *= m_vertexSize;

	if ((vertexIndex < (int)m_vertexArray.size()) && (m_vertexSize == 2))
	{
		vertex = &m_vertexArray[0] + vertexIndex;

		*vertex++ = x;
		*vertex = y;
	}
	else 
		LOG_ERROR();
}

/**
*/
void CVertexPool::SetVertex(int vertexIndex, ValueType x, ValueType y, ValueType z)
{
	ValueType *vertex;

	vertexIndex *= m_vertexSize;

	if ((vertexIndex < (int)m_vertexArray.size()) && (m_vertexSize == 3))
	{
		vertex = &m_vertexArray[0] + vertexIndex;

		*vertex++ = x;
		*vertex++ = y;
		*vertex = z;
	}
	else 
		LOG_ERROR();
}

/**
*/
void CVertexPool::SetVertex(int vertexIndex, ValueType x, ValueType y, ValueType z, ValueType w)
{
	ValueType *vertex;

	vertexIndex *= m_vertexSize;

	if ((vertexIndex < (int)m_vertexArray.size()) && (m_vertexSize == 4))
	{
		vertex = &m_vertexArray[0] + vertexIndex;

		*vertex++ = x;
		*vertex++ = y;
		*vertex++ = z;
		*vertex = w;
	}
	else 
		LOG_ERROR();
}

/**
*/
void CVertexPool::SetVertex(int vertexIndex, const ValueType *begin, const ValueType *end)
{
	int size;
	ValueType *vertex;

	size = (int)(end - begin);

	vertexIndex *= m_vertexSize;

	if ((vertexIndex < (int)m_vertexArray.size()) && (m_vertexSize == size))
	{
		vertex = &m_vertexArray[0] + vertexIndex;

		for ( ; size--; )
			*vertex++ = *begin++;
	}
	else 
		LOG_ERROR();
}

/**
*/
void CVertexPool::SetVertexSize(int size)
{
	if (size > 0)
		m_vertexSize = size;
	else 
		LOG_ERROR();
}

/**
*/
const CVertexPool::ValueType *CVertexPool::GetVertex(int vertexIndex) const
{
	vertexIndex *= m_vertexSize;

#if !defined(NDEBUG)
	if (vertexIndex >= (int)m_vertexArray.size())
	{
		LOG_ERROR();

		return 0;
	}
#endif // #if !defined(NDEBUG)
	
	return (&m_vertexArray[0] + vertexIndex);
}

/**
*/
CVertexPool::ValueType *CVertexPool::GetVertex(int vertexIndex)
{
	vertexIndex *= m_vertexSize;

#if !defined(NDEBUG)
	if (vertexIndex >= (int)m_vertexArray.size())
	{
		LOG_ERROR();

		return 0;
	}
#endif // #if !defined(NDEBUG)
	
	return (&m_vertexArray[0] + vertexIndex);
}

/**
*/
const CVertexPool::ValueType *CVertexPool::GetVertexByLabel(int label)
{
	int labelCount,
		labelIndex;
	const int *labelIterator;

	labelCount = (int)m_labelArray.size();

#if !defined(NDEBUG)
	if (!labelCount)
	{
		LOG_ERROR();

		return 0;
	}
#endif // #if !defined(NDEBUG)

	labelIterator = &m_labelArray[0];

	for (labelIndex=0; labelIndex!=labelCount; ++labelIndex)
		if (*(labelIterator++) == label)
			return GetVertex(labelIndex);

	return 0;
}

/**
*/
const CVertexPool::ValueType *CVertexPool::GetVertexArray() const
{
	if (m_vertexArray.empty())
		return 0;

	return &m_vertexArray[0];
}

/**
*/
int CVertexPool::GetVertexSize() const
{
	return m_vertexSize;
}

/**
*/
int CVertexPool::GetVertexCount() const
{
	if (!m_vertexSize)
		return 0;

	return (int)m_vertexArray.size()/m_vertexSize;
}

/**
*/
void CVertexPool::ClearVertices()
{
	m_vertexSize = 0;

	m_vertexArray.clear();
}

/**
*/
void CVertexPool::AddElement(unsigned int vertexIndex)
{
	m_elementArray.push_back(vertexIndex);
}

/**
*/
void CVertexPool::AddElementArray(const unsigned int *begin, const unsigned int *end)
{
	m_elementArray.insert(m_elementArray.end(), begin, end);
}

/**
*/
void CVertexPool::AddPoint(unsigned int vertexIndex)
{
	if (m_mode == INVALID_MODE)
		m_mode = POINTS;

	if (m_mode == POINTS)
	{
		m_elementArray.push_back(vertexIndex);
	}
	else
		LOG_ERROR();
}

/**
*/
void CVertexPool::AddLine(unsigned int firstIndex, unsigned int secondIndex)
{
	if (m_mode == INVALID_MODE)
		m_mode = LINES;

	if (m_mode == LINES)
	{
		m_elementArray.push_back(firstIndex);
		m_elementArray.push_back(secondIndex);
	}
	else
		LOG_ERROR();
}

/**
*/
void CVertexPool::AddTriangle(unsigned int firstIndex, unsigned int secondIndex, unsigned int thirdIndex)
{
	if (m_mode == INVALID_MODE)
		m_mode = TRIANGLES;

	if (m_mode == TRIANGLES)
	{
		m_elementArray.push_back(firstIndex);
		m_elementArray.push_back(secondIndex);
		m_elementArray.push_back(thirdIndex);
	}
	else
		LOG_ERROR();
}

void CVertexPool::SetTriangle(int triangleId, unsigned int firstIndex, unsigned int secondIndex, unsigned int thirdIndex)
{
	unsigned int *triangle;

#if !defined(NDEBUG)
	if (m_elementArray.empty())
		LOG_ERROR();

	if (m_mode != TRIANGLES)
		LOG_ERROR();

	if (triangleId >= GetPrimitiveCount())
		LOG_ERROR();
#endif // #if !defined(NDEBUG)

	triangle = &m_elementArray[0] + triangleId*3;

	*triangle++ = firstIndex;
	*triangle++ = secondIndex;
	*triangle = thirdIndex;
}

/**
*/
void CVertexPool::AddQuad(unsigned int firstIndex, unsigned int secondIndex, unsigned int thirdIndex, unsigned int fourthIndex)
{
	if (m_mode == INVALID_MODE)
		m_mode = QUADS;

	if (m_mode == QUADS)
	{
		m_elementArray.push_back(firstIndex);
		m_elementArray.push_back(secondIndex);
		m_elementArray.push_back(thirdIndex);
		m_elementArray.push_back(fourthIndex);
	}
	else
		LOG_ERROR();
}

/**
*/
unsigned int CVertexPool::GetElement(int elementIndex) const
{
#if !defined(NDEBUG)
	if (elementIndex < (int)m_elementArray.size())
#endif // #if !defined(NDEBUG)
		return m_elementArray[elementIndex];

	LOG_ERROR();

	return 0;
}

/**
*/
const unsigned int *CVertexPool::GetElementArray() const
{
	if (m_elementArray.empty())
		return 0;

	return &m_elementArray[0];
}

/**
*/
unsigned int *CVertexPool::GetElementArray()
{
	if (m_elementArray.empty())
		return 0;

	return &m_elementArray[0];
}

/**
*/
int CVertexPool::GetElementCount() const
{
	return (int)m_elementArray.size();
}

/**
*/
void CVertexPool::ClearElements()
{
	m_elementArray.clear();
}

/**
*/
unsigned int CVertexPool::GetPrimitiveType() const
{
	return m_mode;
}

/**
*/
const unsigned *CVertexPool::GetPrimitive(int primitiveId) const
{
	int size;

#if !defined(NDEBUG)
	if (m_elementArray.empty())
	{
		LOG_ERROR();

		return 0;
	}

	if ((m_mode != POINTS) && 
		(m_mode != LINES) &&
		(m_mode != TRIANGLES) &&
		(m_mode != QUADS))
	{
		LOG_ERROR();

		return 0;
	}
#endif // #if !defined(NDEBUG)

	if (m_mode == TRIANGLES)
		size = 3;
	else if (m_mode == POINTS)
		size = 1;
	else if (m_mode == LINES)
		size = 2;
	else //if (m_mode == QUADS)
		size = 4;

	return &m_elementArray[0] + primitiveId*size;
}

/**
*/
unsigned *CVertexPool::GetPrimitive(int primitiveId)
{
	int size;

#if !defined(NDEBUG)
	if (m_elementArray.empty())
	{
		LOG_ERROR();

		return 0;
	}

	if ((m_mode != POINTS) && 
		(m_mode != LINES) &&
		(m_mode != TRIANGLES) &&
		(m_mode != QUADS))
	{
		LOG_ERROR();

		return 0;
	}
#endif // #if !defined(NDEBUG)

	if (m_mode == TRIANGLES)
		size = 3;
	else if (m_mode == POINTS)
		size = 1;
	else if (m_mode == LINES)
		size = 2;
	else //if (m_mode == QUADS)
		size = 4;

	return &m_elementArray[0] + primitiveId*size;
}

/**
*/
int CVertexPool::GetPrimitiveSize() const
{
	int size;

	switch (m_mode)
	{
	case POINTS:
		size = 1;
		break;
	case LINES:
		size = 2;
		break;
	case TRIANGLES:
		size = 3;
		break;
	case QUADS:
		size = 4;
		break;
	default:
		return -1;
	}

	return size;
}

/**
*/
int CVertexPool::GetPrimitiveCount() const
{
	int size;

	switch (m_mode)
	{
	case POINTS:
		size = 1;
		break;
	case LINES:
		size = 2;
		break;
	case TRIANGLES:
		size = 3;
		break;
	case QUADS:
		size = 4;
		break;
	default:
		return -1;
	}

	return (int)m_elementArray.size()/size;
}

/**
*/
bool CVertexPool::HasProperty(const std::string& name) const
{
    return m_nameToValueMap.find(name) != m_nameToValueMap.end();
}

/**
*/
void CVertexPool::AddPropertyByName(const std::string& name, boost::any value)
{
    m_nameToValueMap[name] = value;
}

/**
*/
boost::any CVertexPool::GetPropertyByName(const std::string& name)
{
    return m_nameToValueMap[name];
}

/**
*/
void CVertexPool::Clear()
{
	m_labelArray.clear();

	m_vertexArray.clear();
	m_colorArray.clear();
	m_normalArray.clear();
	m_binormalArray.clear();
	m_tangentArray.clear();
	m_texCoordArray.clear();

	m_elementArray.clear();

    m_nameToValueMap.clear();
}

/**
*/
bool CVertexPool::Copy(const boost::shared_ptr<const CVertexPool>& vertexPool)
{
    HEALTH_CHECK(!vertexPool.get(), false);

	m_name = vertexPool->m_name;

	m_mode = vertexPool->m_mode;
	
	m_labelArray = vertexPool->m_labelArray;
	
	m_colorSize = vertexPool->m_colorSize;
	m_colorArray = vertexPool->m_colorArray;
	
	m_texCoordSize = vertexPool->m_texCoordSize;
	m_texCoordArray = vertexPool->m_texCoordArray;
	
	m_normalArray = vertexPool->m_normalArray;
	m_binormalArray = vertexPool->m_binormalArray;
	m_tangentArray = vertexPool->m_tangentArray;
	
	m_vertexSize = vertexPool->m_vertexSize;
	m_vertexArray = vertexPool->m_vertexArray;
	
	m_elementArray = vertexPool->m_elementArray;

    m_nameToValueMap = vertexPool->m_nameToValueMap;

    return true;
}

/**
*/
bool CVertexPool::Append(const CVertexPool *vertexPool)
{
	int vertexCount;
	vector<unsigned int>::const_iterator element;

	if (m_mode != INVALID_MODE)
	{
		if (m_mode != vertexPool->m_mode)
		{
			LOG_ERROR();

			return false;
		}
	}
	else
		m_mode = vertexPool->m_mode;

	if (!vertexPool->m_labelArray.empty())
		m_labelArray.insert(m_labelArray.end(), vertexPool->m_labelArray.begin(), vertexPool->m_labelArray.end());

	if (m_colorSize != 0)
		if (m_colorSize != vertexPool->m_colorSize)
		{
			LOG_ERROR();

			return false;
		}

	m_colorSize = vertexPool->m_colorSize;

	if (!vertexPool->m_colorArray.empty())
		m_colorArray.insert(m_colorArray.end(), vertexPool->m_colorArray.begin(), vertexPool->m_colorArray.end());

	if (m_texCoordSize != 0)
		if (m_texCoordSize != vertexPool->m_texCoordSize)
		{
			LOG_ERROR();

			return false;
		}

	if (!vertexPool->m_texCoordArray.empty())
		m_texCoordArray.insert(m_texCoordArray.end(), vertexPool->m_texCoordArray.begin(), vertexPool->m_texCoordArray.end());

	m_texCoordSize = vertexPool->m_texCoordSize;

	if (!vertexPool->m_normalArray.empty())
		m_normalArray.insert(m_normalArray.end(), vertexPool->m_normalArray.begin(), vertexPool->m_normalArray.end());

	if (!vertexPool->m_binormalArray.empty())
		m_binormalArray.insert(m_binormalArray.end(), vertexPool->m_binormalArray.begin(), vertexPool->m_binormalArray.end());

	if (!vertexPool->m_tangentArray.empty())
		m_tangentArray.insert(m_tangentArray.end(), vertexPool->m_tangentArray.begin(), vertexPool->m_tangentArray.end());

	if (m_vertexSize != 0)
		if (m_vertexSize != vertexPool->m_vertexSize)
		{
			LOG_ERROR();

			return false;
		}

	m_vertexSize = vertexPool->m_vertexSize;

	vertexCount = GetVertexCount();

	if (!vertexPool->m_vertexArray.empty())
		m_vertexArray.insert(m_vertexArray.end(), vertexPool->m_vertexArray.begin(), vertexPool->m_vertexArray.end());

	for (element=vertexPool->m_elementArray.begin(); element!=vertexPool->m_elementArray.end(); element++)
		m_elementArray.push_back(vertexCount + *element);

    m_nameToValueMap.insert(vertexPool->m_nameToValueMap.begin(), vertexPool->m_nameToValueMap.end());

	if (!IsValid())
	{
		LOG_ERROR();

		return false;
	}

	return true;
}

/**
*/
bool CVertexPool::Open(const string& fileName, STREAM_FORMAT type)
{
	switch (type) 
	{
	case OFF:
		if (!OpenOFF(fileName))
		{
			LOG_ERROR();

			return false;
		}
		break;

	case PLY:
	case ASCII_STL:
	case SURFEL:
		LOG_ERROR();

		return false;

	case CRYSTAL_STRUCTURE_DISLOCATIONS:
		if (!OpenCrystalStructureDislocation(fileName))
		{
			LOG_ERROR();

			return false;
		}
		break;

	case OBJ:
		if (!OpenOBJ(fileName))
		{
			LOG_ERROR();

			return false;
		}
		break;

	case POINT_CLOUD:
		if (!OpenPointCloud(fileName))
		{
			LOG_ERROR();

			return false;
		}
		break;
	}

	SetName(fileName);

	return true;
}

/**
*/
bool CVertexPool::Save(const string& fileName, STREAM_FORMAT type) const
{
	ofstream modelFile;
	const ValueType *vertex,
		*color;
	const unsigned int *element;
	int vertexCount,
		elementCount,
		primitiveSize,
		primitiveCount,
		i;

	if (m_vertexArray.empty())
	{
		LOG_ERROR();

		return false;
	}

	vertex = &m_vertexArray[0];
	vertexCount = (int)m_vertexArray.size()/m_vertexSize;

	if (m_elementArray.empty())
	{
		LOG_ERROR();

		return false;
	}

	element = &m_elementArray[0];
	elementCount = (int)m_elementArray.size();

	if (!m_colorArray.empty())
		color = &m_colorArray[0];
	else
		color = 0;

	primitiveSize = 1;

	switch (m_mode) {
	case POINTS:
		primitiveSize = 1;
		break;
	case LINES:
		primitiveSize = 2;
		break;
	case TRIANGLES:
		primitiveSize = 3;
		break;
	case QUADS:
		primitiveSize = 4;
		break;
	case LINE_LOOP:
	case LINE_STRIP:
	case TRIANGLE_STRIP:
	case TRIANGLE_FAN:
	case QUAD_STRIP:
	case POLYGON:
		LOG_ERROR();

		return false;
	};

	primitiveCount = elementCount/primitiveSize;

	switch (type) {
	case OFF:
		modelFile.open(fileName.c_str());

		modelFile << "OFF" << endl;
		modelFile << vertexCount << " " << primitiveCount << " " << primitiveCount*primitiveSize << endl;

		for ( ; vertexCount--; )
		{
			for (i=0; i!=m_vertexSize; i++)
				modelFile << *vertex++ << " ";

			modelFile << endl;
		}

		for ( ; primitiveCount--; )
		{
			modelFile << primitiveSize << " ";

			for (i=primitiveSize; i--; )
				modelFile << *element++ << " ";

			modelFile << endl;
		}
		break;
	case PLY:
		modelFile.open(fileName.c_str());

		modelFile << "ply" << endl;
		modelFile << "format ascii 1.0" << endl;

		modelFile << "element vertex " << vertexCount << endl;
		if (m_vertexSize >= 1)
			modelFile << "property float32 x" << endl;
		if (m_vertexSize >= 2)
			modelFile << "property float32 y" << endl;
		if (m_vertexSize >= 3)
			modelFile << "property float32 z" << endl;
		if (m_vertexSize >= 4)
			modelFile << "property float32 w" << endl;

		modelFile << "element face " << primitiveCount << endl;
		modelFile << "property list uint8 int32 vertex_indices" << endl;

		modelFile << "end_header" << endl;

		for ( ; vertexCount--; )
		{
			for (i=0; i!=m_vertexSize; i++)
				modelFile << *vertex++ << " ";

			modelFile << endl;
		}

		for ( ; primitiveCount--; )
		{
			modelFile << primitiveSize << " ";

			for (i=primitiveSize; i--; )
				modelFile << *element++ << " ";

			modelFile << endl;
		}
		break;

	case ASCII_STL:
		if (!SaveASCIISTL(fileName))
		{
			LOG_ERROR();

			return false;
		}
		break;

	case SURFEL:
		if (!SaveSURFEL(fileName))
		{
			LOG_ERROR();

			return false;
		}
		break;

	case OBJ:
		if (!SaveOBJ(fileName))
		{
			LOG_ERROR();

			return false;
		}
		break;
            
        case CRYSTAL_STRUCTURE_DISLOCATIONS:
        case POINT_CLOUD:
            LOG_ERROR();
            
            return false;
            break;
	};

	return true;
}

/**
*/
bool CVertexPool::IsValid() const
{
	int colorCount,
		texCoordCount,
		normalCount,	
		vertexCount,
		elementCount;
	const unsigned int *element;

	colorCount = (int)m_colorArray.size();

	if (colorCount)
	{
		if (!m_colorSize)
        {
            LOG_ERROR();

			return false;
        }

		colorCount /= m_colorSize;
	}

	texCoordCount = (int)m_texCoordArray.size();

	if (texCoordCount)
	{
		if (!m_texCoordSize)
        {
            LOG_ERROR();

			return false;
        }

		texCoordCount /= m_texCoordSize;
	}

	normalCount = (int)m_normalArray.size()/3;

	vertexCount = (int)m_vertexArray.size();

	if (vertexCount)
	{
		if (!m_vertexSize)
        {
            LOG_ERROR();

			return false;
        }

		vertexCount /= m_vertexSize;

		if (colorCount &&
            (colorCount != vertexCount))
        {
            LOG_ERROR();

			return false;
        }

		if (normalCount &&
            (normalCount != vertexCount))
        {
            LOG_ERROR();
            
            return false;
        }

		if (texCoordCount &&
            (texCoordCount != vertexCount))
        {
            LOG_ERROR();
            
            return false;
        }
	}

	elementCount = (int)m_elementArray.size();

	if (elementCount)
	{
		element = &m_elementArray[0];

		for ( ; elementCount--; element++)
        {
			if (*element > (unsigned int)vertexCount)
            {
                LOG_ERROR();

				return false;
            }
        }
	}

	return true;
}

/**
*/
bool CVertexPool::SaveASCIISTL(const std::string& fileName) const
{
	ofstream modelFileStream;
	// Pointers to triangle vertices.
	const ValueType *firstVertex,
		*secondVertex,
		*thirdVertex;
	const unsigned int *element;
	int elementCount,
		primitiveCount,
		i,
		j;
	const ValueType *vertexNormal;
	ValueType facetNormal[3];

	if (!m_vertexArray.empty() ||
		!m_normalArray.empty() ||
		!m_elementArray.empty())
	{
		LOG_ERROR();

		return false;
	}

	if (m_vertexSize != 3)
	{
		LOG_ERROR();

		return false;
	}

	element = &m_elementArray[0];
	elementCount = (int)m_elementArray.size();

	if (m_mode != TRIANGLES)
	{
		LOG_ERROR();

		return false;
	}

	primitiveCount = elementCount/3;

	modelFileStream.open(fileName.c_str());

	if (!modelFileStream.is_open())
	{
		LOG_ERROR();

		return false;
	}

	modelFileStream << "solid " << fileName.substr(0, fileName.size() - 4) << endl;

	for ( ; primitiveCount--; )
	{
		firstVertex = GetVertex(element[0]);
		secondVertex = GetVertex(element[1]);
		thirdVertex = GetVertex(element[2]);

		for (i=0; i!=3; ++i)
			facetNormal[i] = 0.0f;

		for (i=0; i!=3; ++i)
		{
			vertexNormal = GetNormal(element[i]);

			for (j=0; j!=3; ++j)
				facetNormal[j] += vertexNormal[j];
		}

		MyMath::Normalize(facetNormal, facetNormal + 1, facetNormal + 2);

		modelFileStream << "facet normal " << facetNormal[0] << " " << facetNormal[1] << " " << facetNormal[2] << endl;

		modelFileStream << "  outer loop" << endl;

		modelFileStream << "    vertex   " << firstVertex[0] << " " << firstVertex[1] << " " << firstVertex[2] << endl;
		modelFileStream << "    vertex   " << secondVertex[0] << " " << secondVertex[1] << " " << secondVertex[2] << endl;
		modelFileStream << "    vertex   " << thirdVertex[0] << " " << thirdVertex[1] << " " << thirdVertex[2] << endl;

		modelFileStream << "  endloop" << endl;

		modelFileStream << "endfacet" << endl;

		element += 3;
	}

	modelFileStream << "endsolid" << endl;

	return true;
}

/**
*/
bool CVertexPool::SaveSURFEL(const std::string& fileName) const
{
	ofstream modelFileStream;
	const ValueType *vertexCoord,
		*normalCoord,
		*colorCoord;
	int primitiveCount,
		i;

	if (m_vertexArray.empty())
	{
		LOG_ERROR();

		return false;
	}

	if (m_vertexSize != 3)
	{
		LOG_ERROR();

		return false;
	}

	vertexCoord = &m_vertexArray[0];

	if (m_normalArray.empty())
	{
		LOG_ERROR();

		return false;
	}

	normalCoord = &m_normalArray[0];

	if (m_colorArray.empty())
		colorCoord = 0;
	else
		colorCoord = &m_colorArray[0];

	primitiveCount = GetVertexCount();

	modelFileStream.open(fileName.c_str());

	if (!modelFileStream.is_open())
	{
		LOG_ERROR();

		return false;
	}

	modelFileStream << primitiveCount << endl;

	for (i=0; i<primitiveCount; i++)
	{
		modelFileStream << vertexCoord[0] << " " << vertexCoord[1] << " " << vertexCoord[2];

		modelFileStream << " ";

		if (colorCoord != 0)
			modelFileStream << colorCoord[0] << " " << colorCoord[1] << " " << colorCoord[2];
		else
			modelFileStream << "1.0 1.0 1.0";

		modelFileStream << " ";

		modelFileStream << normalCoord[0] << " " << normalCoord[1] << " " << normalCoord[2];

		modelFileStream << " ";

		modelFileStream << "1.0" << endl;

		vertexCoord += m_vertexSize;

		if (colorCoord != 0)
			colorCoord += m_colorSize;

		normalCoord += 3;
	}

	return true;
}

/**
*/
bool CVertexPool::SaveOBJ(const std::string& fileName) const
{
	unsigned int vertexCount,
		vertexIndex;
	const ValueType *vertexCoord,
		*vertexNormal;
	int primitiveCount,
		elementCount,
		primitiveIndex;
	const unsigned int *primitive;

	if (!IsValid()) 
	{
		LOG_ERROR();

		return false;
	}

	std::ofstream fileStream(fileName.c_str());

	if (!fileStream.is_open()) 
	{
		LOG_ERROR();

		return false;
	}

	if (!GetName().empty())
		fileStream << "o " << GetName() << std::endl;

	vertexCount = GetVertexCount();

	for (vertexIndex=0; vertexIndex!=vertexCount; ++vertexIndex)
	{
		vertexCoord = GetVertex(vertexIndex);
		
		fileStream << "v " << std::setprecision(8) << std::fixed << vertexCoord[0] << " " << vertexCoord[1] << " " << vertexCoord[2] << std::endl;
		
		if (!m_normalArray.empty())
		{
			vertexNormal = GetNormal(vertexIndex);

			fileStream << "vn " << std::setprecision(8) << std::fixed << vertexNormal[0] << " " << vertexNormal[1] << " " << vertexNormal[2] << std::endl;
		}
	}

	primitiveCount = GetPrimitiveCount();
	elementCount = GetPrimitiveSize();

    char elementGroup;

    switch (elementCount) {
    case 1:
        elementGroup = 'p';
        break;
    case 2:
        elementGroup = 'l';
        break;
    case 3:
    case 4:
        elementGroup = 'f';
        break;
    default:
        LOG_ERROR();

        return false;
    }
	
	for (primitiveIndex=0; primitiveIndex!=primitiveCount; ++primitiveIndex)
	{
		primitive = GetPrimitive(primitiveIndex);

        fileStream << elementGroup;

        for (int elementIndex = 0; elementIndex < elementCount; ++elementIndex)
            fileStream << " " << (primitive[elementIndex] + 1);

		fileStream << std::endl;
	}

	return true;
}

/**
*/
bool CVertexPool::OpenOFF(const string& fileName)
{
	FILE *fileStream;
	char token[128];
	int fileStatus,
		vertexCount,
		cellCount,
		dummyInt[5],
		i,
		j;
	ValueType dummyFloat[3];

	if ((fileStream = fopen(fileName.c_str(), "r")) == NULL)
	{
		LOG_ERROR();

		return false;
	}

	fileStatus = fscanf(fileStream, "%s", token);

	if ((fileStatus == EOF) || (fileStatus == 0))
	{
		LOG_ERROR();

		return false;
	}

	if (strcmp(token, "OFF") != 0)
	{
		LOG_ERROR();

		return false;
	}

	fileStatus = fscanf(fileStream, "%i", &vertexCount);

	if ((fileStatus == EOF) || (fileStatus == 0))
	{
		LOG_ERROR();

		return false;
	}

	fileStatus = fscanf(fileStream, "%i", &cellCount);

	if ((fileStatus == EOF) || (fileStatus == 0))
	{
		LOG_ERROR();

		return false;
	}

	fileStatus = fscanf(fileStream, "%i", dummyInt);

	if ((fileStatus == EOF) || (fileStatus == 0))
	{
		LOG_ERROR();

		return false;
	}

	Clear();

	for (i=0; i!=vertexCount; i++)
	{
		for (j=0; j!=3; j++)
		{
			fileStatus = fscanf(fileStream, "%lff", dummyFloat + j);

			if ((fileStatus == EOF) || (fileStatus == 0))
			{
				LOG_ERROR();

				return false;
			}
		}

		AddVertex(dummyFloat, dummyFloat + 3);
	}

	for (i=0; i!=cellCount; i++)
	{
		fileStatus = fscanf(fileStream, "%i", dummyInt);

		if ((fileStatus == EOF) || (fileStatus == 0))
		{
			LOG_ERROR();

			return false;
		}

		for (j=1; j<=dummyInt[0]; j++)
		{
			fileStatus = fscanf(fileStream, "%i", dummyInt + j);

			if ((fileStatus == EOF) || (fileStatus == 0))
			{
				LOG_ERROR();

				return false;
			}
		}

		if (dummyInt[0] == 3)
			AddTriangle(dummyInt[1], dummyInt[2], dummyInt[3]);
		else if (dummyInt[0] == 1)
			AddPoint(dummyInt[1]);
		else
		{
			LOG_ERROR();

			return false;
		}
	}

	fclose(fileStream);

	return true;
}

/**
*/
bool CVertexPool::OpenCrystalStructureDislocation(const string& fileName)
{
	FILE *fileStream;
	char token[128];
	int fileStatus,
		lineSizeCount,
		dummyInt,
		j;
	ValueType dummyFloat[3],
		texCoord[3],
		vertexCoord[3];

	if ((fileStream = fopen(fileName.c_str(), "r")) == NULL)
	{
		LOG_ERROR();

		return false;
	}

	// Ignores the header of the file.
	do 
	{
		fileStatus = fscanf(fileStream, "%s", token);

		if ((fileStatus == EOF) || (fileStatus == 0))
		{
			LOG_ERROR();

			return false;
		}

		// Searches for the end of the line.
		if (token[0] == '#')
		{
			lineSizeCount = 0;

			while (fgetc(fileStream) != '\n')
				if (++lineSizeCount > 2048)
				{
					LOG_ERROR();

					return false;
				}
		}
		else
			break;
	} while ((fileStatus != EOF) && (fileStatus != 0));

	dummyFloat[0] = ValueType(atof(token));

	Clear();

	SetMode(POINTS);

	while ((fileStatus != EOF) && (fileStatus != 0))
	{
		// number (readed from file in the previous iteration), type and mass.
		for (j=1; j!=3; ++j)
		{
			fileStatus = fscanf(fileStream, "%lf", texCoord + j);

			if ((fileStatus == EOF) || (fileStatus == 0))
			{
				LOG_ERROR();

				return false;
			}
		}

		// x, y and z.
		for (j=0; j!=3; ++j)
		{
			fileStatus = fscanf(fileStream, "%lf", vertexCoord + j);

			if ((fileStatus == EOF) || (fileStatus == 0))
			{
				LOG_ERROR();

				return false;
			}
		}

		// vx, vy and vz.
		for (j=0; j!=3; ++j)
		{
			fileStatus = fscanf(fileStream, "%lf", dummyFloat + j);

			if ((fileStatus == EOF) || (fileStatus == 0))
			{
				LOG_ERROR();

				return false;
			}
		}

		// Crist.
		fileStatus = fscanf(fileStream, "%i", &dummyInt);

		if ((fileStatus == EOF) || (fileStatus == 0))
		{
			LOG_ERROR();

			return false;
		}

		// Discards red points (points along planes).
		if (dummyInt != 1)
		{
			AddLabel(dummyInt);

			AddTexCoord(texCoord[0], texCoord[1], texCoord[2]);

			AddVertex(vertexCoord, vertexCoord + 3);

			// Color computed from crist.
			AddColor(ValueType(dummyInt&1), ValueType((dummyInt&2)>>1), ValueType((dummyInt&4)>>2));

			AddElement(GetElementCount());
		}

		fileStatus = fscanf(fileStream, "%lf", dummyFloat);
	}

	fclose(fileStream);

	return true;
}

/**
*/
bool CVertexPool::OpenOBJ(const string& fileName)
{
	std::ifstream fileStream(fileName.c_str());
	std::string token;
	char line[OBJ_MAX_LINE_SIZE];
	ValueType x, 
		y, 
		z;
	std::vector<unsigned int> face;

	Clear();

	if (!fileStream.is_open())
    {
		LOG_MESSAGE("Failed to open file " + fileName);

		return false;
	}

	fileStream >> token;

	while (!fileStream.eof()) {
        switch (token[0]) {
			// Comment.
		case '#':
            // Eat up rest of line. It includes the newline and eof characters.
			fileStream.getline(line, OBJ_MAX_LINE_SIZE);
            break;

			// Object name.
		case 'o':
			fileStream.getline(line, OBJ_MAX_LINE_SIZE);

			line[strlen(line) - 1] = '\0';

			SetName(line);
			break;

			// Vertex coordinates, normal or texture coordinates.
        case 'v':
			// Vertex coordinates.
			if (token.size() == 1) {
				fileStream >> x;
				fileStream >> y;
				fileStream >> z;
                
				AddVertex(x, y, z);
			}
			else {
				switch (token[1]) {
					// Vertex normal.
				case 'n':
					fileStream >> x;
					fileStream >> y;
					fileStream >> z;
                
					AddNormal(x, y, z);
					break;

					// Point in the parameter space of a curve or surface.
				case 'p':
					// Do nothing by now.
					fileStream.getline(line, OBJ_MAX_LINE_SIZE);
					break;

					// Texture coordinates.
				case 't':
					// Do nothing by now.
					fileStream.getline(line, OBJ_MAX_LINE_SIZE);
					break;
				}
			}
            break;

			// Material (from usemtl tag).
		case 'u':
            // Do nothing by now.
			fileStream.getline(line, OBJ_MAX_LINE_SIZE);
            break;

			// Group of elements.
		case 'g':
            // Do nothing by now.
			fileStream.getline(line, OBJ_MAX_LINE_SIZE);
            break;

			// Referencing Groups of Vertices 
			// (See more at http://local.wasp.uwa.edu.au/~pbourke/dataformats/obj/).
			// Some elements, such as faces and surfaces, may have a triplet of
			// numbers that reference vertex data.These numbers are the reference
			// numbers for a geometric vertex, a texture vertex, and a vertex normal.
			// Each triplet of numbers specifies a geometric vertex, texture vertex,
			// and vertex normal. The reference numbers must be in order and must
			// separated by slashes (/).
			//
			// - The first reference number is the geometric vertex.
			// - The second reference number is the texture vertex. It follows
			//   the first slash.
			// - The third reference number is the vertex normal. It follows the
			//   second slash.
			//
			// There is no space between numbers and the slashes. There may be more
			// than one series of geometric vertex/texture vertex/vertex normal
			// numbers on a line.
			//
			// The following is a portion of a sample file for a four-sided face
			// element:
			//
			//  f 1/1/1 2/2/2 3/3/3 4/4/4
			//
			// Using v, vt, and vn to represent geometric vertices, texture vertices,
			// and vertex normals, the statement would read:
			//
			//  f v/vt/vn v/vt/vn v/vt/vn v/vt/vn
			//
			// If there are only vertices and vertex normals for a face element (no
			// texture vertices), you would enter two slashes (//). For example, to
			// specify only the vertex and vertex normal reference numbers, you would
			// enter:
			//
			//  f 1//1 2//2 3//3 4//4
			//
			// When you are using a series of triplets, you must be consistent in the
			// way you reference the vertex data. For example, it is illegal to give
			// vertex normals for some vertices, but not all.
			//
			// The following is an example of an illegal statement.
			//
			//  f 1/1/1 2/2/2 3//3 4//4

			// Point.
		case 'p':
            // Do nothing by now.
			fileStream.getline(line, OBJ_MAX_LINE_SIZE);
            break;

			// Line.
		case 'l':
			{
				// The index format can be one of v//n, v/t/n, v/t or v. We're 
				// only interested on v (vertex index).
				face.clear();

				// A local parser for face information.
				fileStream.getline(line, OBJ_MAX_LINE_SIZE);

				// Skips the whitespace between 'f' and the begin of face 
				// information.
				std::string faceInfo(line + 1),
					// One of the index groups (v/n/t) of the face.
					faceIndex;
				// The list of index groups of the face.
				std::vector<std::string> faceIndexList;

				// Parse all index groups.
				while (!faceInfo.empty()) 
                {
					faceIndex = faceInfo.substr(0, faceInfo.find(" "));

					faceIndexList.push_back(faceIndex);

					// Explicitly clears the string when there are no more 
					// index groups.
					if (faceInfo.find(" ") == std::string::npos)
						faceInfo.clear();
					else
						// Go to the next index group.
						faceInfo = faceInfo.substr(faceInfo.find(" ") + 1);
				}

				// Extracts the vertex index (and only the vertex index) from 
				// the index group list.
				for (std::vector<std::string>::const_iterator index=faceIndexList.begin(); index!=faceIndexList.end(); ++index) 
                {
					if (index->find("/") == std::string::npos)
						face.push_back(atoi(index->c_str()) - 1);
					else
						// Gets only what's before the first slash.
						face.push_back((atoi((index->substr(0, index->find("/")).c_str())) - 1));
				}

				if (face.size() != 2) 
                {
					LOG_ERROR();

					return false;
				}

				// Only triangles by now.
				AddLine(face[0], face[1]);
			}
            break;

			// Face. 
		case 'f':
			{
				// The index format can be one of v//n, v/t/n, v/t or v. We're 
				// only interested on v (vertex index).
				face.clear();

				// A local parser for face information.
				fileStream.getline(line, OBJ_MAX_LINE_SIZE);

				// Skips the whitespace between 'f' and the begin of face 
				// information.
				std::string faceInfo(line + 1),
					// One of the index groups (v/n/t) of the face.
					faceIndex;
				// The list of index groups of the face.
				std::vector<std::string> faceIndexList;

				// Parse all index groups.
				while (!faceInfo.empty())
                {
					faceIndex = faceInfo.substr(0, faceInfo.find(" "));

					faceIndexList.push_back(faceIndex);

					// Explicitly clears the string when there are no more 
					// index groups.
					if (faceInfo.find(" ") == std::string::npos)
						faceInfo.clear();
					else
						// Go to the next index group.
						faceInfo = faceInfo.substr(faceInfo.find(" ") + 1);
				}

				// Extracts the vertex index (and only the vertex index) from 
				// the index group list.
				for (std::vector<std::string>::const_iterator index=faceIndexList.begin(); index!=faceIndexList.end(); ++index)
                {
					if (index->find("/") == std::string::npos)
						face.push_back(atoi(index->c_str()) - 1);
					else
						// Gets only what's before the first slash.
						face.push_back((atoi((index->substr(0, index->find("/")).c_str())) - 1));
				}

				if (face.size() != 3) 
                {
					LOG_ERROR();

					return false;
				}

				// Only triangles by now.
				AddTriangle(face[0], face[1], face[2]);
			}
			break;

		default:
            // Eat up rest of line. 
			fileStream.getline(line, OBJ_MAX_LINE_SIZE);
            break;
		}

		fileStream >> token;
	}

	return true;
}

/**
*/
bool CVertexPool::OpenPointCloud(const std::string& fileName)
{
	FILE *fileStream;
	int fileStatus;
	ValueType dummyFloat[3];
	int i;

	Clear();

	if ((fileStream = fopen(fileName.c_str(), "r")) == NULL)
	{
		LOG_ERROR();

		return false;
	}

	fileStatus = 1;

	SetMode(POINTS);

	while ((fileStatus != EOF) && 
		(fileStatus != 0))
	{
		for (i=0; i!=3; i++)
		{
			fileStatus = fscanf(fileStream, "%lf", dummyFloat + i);

			if ((fileStatus == EOF) ||
				(fileStatus == 0))
			{
				if (i != 0)
				{
					LOG_ERROR();

					return false;
				}
				else
					break;
			}
		}

		if ((fileStatus != EOF) && 
			(fileStatus != 0))
		{
			AddElement(GetVertexCount());
			AddVertex(dummyFloat, dummyFloat + 3);
		}
	}

	fclose(fileStream);

	return true;
}

/**
*/
void CVertexPool::Create()
{
	m_name.clear();

	m_labelArray.clear();

	m_vertexSize = 0;
	m_vertexArray.clear();

	m_colorSize = 0;
	m_colorArray.clear();

	m_normalArray.clear();

	m_texCoordSize = 0;
	m_texCoordArray.clear();

	m_mode = INVALID_MODE;

	m_elementArray.clear();

    m_nameToValueMap.clear();
}

