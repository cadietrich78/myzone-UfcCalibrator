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

#if !defined(VERTEX_POOL_INCLUDED)
#define VERTEX_POOL_INCLUDED

#include <vector>
#include <string>
#include <map>

#include <boost/utility.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/any.hpp>

#include <Vector3.h>

class CVertexPool 
	: boost::noncopyable
{
public:
    // BUG: (1-Dec-2015) Changed from float to double
	typedef double ValueType;

	enum STREAM_FORMAT {
		OFF = 0,
		PLY = 1,
		ASCII_STL = 2,
		SURFEL = 3,
		CRYSTAL_STRUCTURE_DISLOCATIONS = 4,
		OBJ = 5,
		POINT_CLOUD = 6
	};

	enum POLYGON_MODE {
		INVALID_MODE = -1,
		POINTS = 0x0000,
		LINES = 0x0001,
		LINE_LOOP = 0x0002,
		LINE_STRIP = 0x0003,
		TRIANGLES = 0x0004,
		TRIANGLE_STRIP = 0x0005,
		TRIANGLE_FAN = 0x0006,
		QUADS = 0x0007,
		QUAD_STRIP = 0x0008,
		POLYGON = 0x0009
	};

	CVertexPool();
	virtual ~CVertexPool();
	
	void SetName(const std::string& name);
	const std::string GetName() const;

	void SetMode(POLYGON_MODE mode);
	POLYGON_MODE GetMode() const;
	
	void AddLabel(int label);
	void SetLabel(int label);
	void SetLabel(int vertexIndex, int label);
	int GetLabel(int vertexIndex) const;
	const int *GetLabelArray() const;
	int GetLabelCount() const;
	void ClearLabels();

	int GetIndexFromLabel(int label) const;
	
	void AddColor(ValueType r, ValueType g, ValueType b);
	void AddColor(ValueType r, ValueType g, ValueType b, ValueType a);
	void AddColor(const ValueType *begin, const ValueType *end);
	void SetColor(ValueType r, ValueType g, ValueType b);
	void SetColor(int vertexIndex, ValueType r, ValueType g, ValueType b);
	void SetColor(ValueType r, ValueType g, ValueType b, ValueType a);
	void SetColor(int vertexIndex, ValueType r, ValueType g, ValueType b, ValueType a);
	void SetColor(int vertexIndex, const ValueType *begin, const ValueType *end);
	void SetColorSize(int size);
	const ValueType *GetColor(int vertexIndex) const;
	const ValueType *GetColorArray() const;
	int GetColorSize() const;
	int GetColorCount() const;
	void ClearColors();
	void SetOpacity(double opacity);
	
	void AddTexCoord(ValueType s);
	void AddTexCoord(ValueType s, ValueType t);
	void AddTexCoord(ValueType s, ValueType t, ValueType r);
	void AddTexCoord(ValueType s, ValueType t, ValueType r, ValueType q);
	void AddTexCoord(const ValueType *begin, const ValueType *end);
	void SetTexCoord(int vertexIndex, ValueType s);
	void SetTexCoord(int vertexIndex, ValueType s, ValueType t);
	void SetTexCoord(int vertexIndex, ValueType s, ValueType t, ValueType r);
	void SetTexCoord(int vertexIndex, ValueType s, ValueType t, ValueType r, ValueType q);
	void SetTexCoord(int vertexIndex, const ValueType *begin, const ValueType *end);
	void SetTexCoordSize(int size);
	const ValueType *GetTexCoord(int vertexIndex) const;
	const ValueType *GetTexCoordArray() const;
	int GetTexCoordSize() const;
	int GetTexCoordCount() const;
	void ClearTexCoords();
	
	void AddNormal(ValueType x, ValueType y, ValueType z);
	void AddNormal(const ValueType *begin, const ValueType *end);
	void SetNormal(int vertexIndex, ValueType x, ValueType y, ValueType z);
	void SetNormal(int vertexIndex, const ValueType *begin, const ValueType *end);
    const ValueType *GetNormal(int vertexIndex) const;
    ValueType *GetNormal(int vertexIndex);
    const ValueType *GetNormalArray() const;
	int GetNormalCount() const;
	void ClearNormals();
	
	void AddBinormal(ValueType x, ValueType y, ValueType z);
	void AddBinormal(const ValueType *begin, const ValueType *end);
	void SetBinormal(int vertexIndex, ValueType x, ValueType y, ValueType z);
	void SetBinormal(int vertexIndex, const ValueType *begin, const ValueType *end);
	const ValueType *GetBinormal(int vertexIndex) const;
	const ValueType *GetBinormalArray() const;
	int GetBinormalCount() const;
	void ClearBinormals();
	
	void AddTangent(ValueType x, ValueType y, ValueType z);
	void AddTangent(const ValueType *begin, const ValueType *end);
	void SetTangent(int vertexIndex, ValueType x, ValueType y, ValueType z);
	void SetTangent(int vertexIndex, const ValueType *begin, const ValueType *end);
	const ValueType *GetTangent(int vertexIndex) const;
	const ValueType *GetTangentArray() const;
	int GetTangentCount() const;
	void ClearTangents();
	
	void AddVertex(ValueType x);
	void AddVertex(ValueType x, ValueType y);
	void AddVertex(ValueType x, ValueType y, ValueType z);
	void AddVertex(ValueType x, ValueType y, ValueType z, ValueType w);
    void AddVertex(const ValueType *begin, const ValueType *end);
    void AddVertex(my::CVector3<double> vertex);

	void SetVertex(int vertexIndex, ValueType x);
	void SetVertex(int vertexIndex, ValueType x, ValueType y);
	void SetVertex(int vertexIndex, ValueType x, ValueType y, ValueType z);
	void SetVertex(int vertexIndex, ValueType x, ValueType y, ValueType z, ValueType w);
	void SetVertex(int vertexIndex, const ValueType *begin, const ValueType *end);
	void SetVertexSize(int size);
	
	const ValueType *GetVertex(int vertexIndex) const;
	ValueType *GetVertex(int vertexIndex);
	const ValueType *GetVertexByLabel(int label);
	const ValueType *GetVertexArray() const;
	int GetVertexSize() const;
	int GetVertexCount() const;
	
	void ClearVertices();

	void AddElement(unsigned int vertexIndex);
	void AddElementArray(const unsigned int *begin, const unsigned int *end);

	void AddPoint(unsigned int vertexIndex);
	void AddLine(unsigned int firstIndex, unsigned int secondIndex);
	void AddTriangle(unsigned int firstIndex, unsigned int secondIndex, unsigned int thirdIndex);
	void SetTriangle(int triangleId, unsigned int firstIndex, unsigned int secondIndex, unsigned int thirdIndex);
	void AddQuad(unsigned int firstIndex, unsigned int secondIndex, unsigned int thirdIndex, unsigned int fourthIndex);

	unsigned int GetElement(int elementIndex) const;
	const unsigned *GetElementArray() const;
	unsigned *GetElementArray();
	int GetElementCount() const;
	void ClearElements();
	
	unsigned int GetPrimitiveType() const;
	const unsigned *GetPrimitive(int primitiveId) const;
	unsigned *GetPrimitive(int primitiveId);
	int GetPrimitiveSize() const;
	int GetPrimitiveCount() const;

    bool HasProperty(const std::string& name) const;
    void AddPropertyByName(const std::string& name, boost::any value);
    boost::any GetPropertyByName(const std::string& name);
	
	virtual void Clear();
	
	bool Copy(const boost::shared_ptr<const CVertexPool>& vertexPool);
	bool Append(const CVertexPool *vertexPool);
	
	bool Open(const std::string& fileName, STREAM_FORMAT type);
	bool Save(const std::string& fileName, STREAM_FORMAT type) const;
	
	bool IsValid() const;

protected:
	bool SaveASCIISTL(const std::string& fileName) const;
	bool SaveSURFEL(const std::string& fileName) const;
	bool SaveOBJ(const std::string& fileName) const;
	
	bool OpenOFF(const std::string& fileName);
	bool OpenCrystalStructureDislocation(const std::string& fileName);
	bool OpenOBJ(const std::string& fileName);
	bool OpenPointCloud(const std::string& fileName);
	
	virtual void Create();
public:
	std::string m_name;

	unsigned int m_mode;
	
	std::vector<int> m_labelArray;
	
	int m_colorSize;
	std::vector<ValueType> m_colorArray;
	
	int m_texCoordSize;
	std::vector<ValueType> m_texCoordArray;
	
	std::vector<ValueType> m_normalArray;
	std::vector<ValueType> m_binormalArray;
	std::vector<ValueType> m_tangentArray;
	
	int m_vertexSize;
	std::vector<ValueType> m_vertexArray;
	
	std::vector<unsigned int> m_elementArray;

    std::map<std::string, boost::any> m_nameToValueMap;
};

#endif // #if !defined(VERTEX_POOL_INCLUDED)

