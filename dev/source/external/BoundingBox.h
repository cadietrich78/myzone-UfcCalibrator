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

#if !defined(BOUNDING_BOX_INCLUDED)
#define BOUNDING_BOX_INCLUDED

#include <boost/shared_ptr.hpp>

#include <VertexPool.h>
#include <Vector3.h>

class CBoundingBox 
{
public:
	CBoundingBox();
	CBoundingBox(const CBoundingBox& boundingBox);
    CBoundingBox(double xMin, double yMin, double zMin, double xMax, double yMax, double zMax);

    virtual ~CBoundingBox();

    void operator=(const CBoundingBox& boundingBox);

    bool SetMesh(const double *vertexArrayPointer, int vertexCount, int vertexSize);
	bool SetMesh(const CVertexPool *mesh);
	bool SetMesh(boost::shared_ptr<const CVertexPool> mesh);

	bool AddMesh(const CVertexPool *mesh);
	bool AddMesh(boost::shared_ptr<CVertexPool>& mesh);

    bool AddVertex(double x, double y, double z);
    bool AddVertex(my::CVector3<double> vertex);

    bool AddBoundingBox(const CBoundingBox& boundingBox);

    bool Contains(double x, double y, double z) const;
    bool Intersects(const CBoundingBox& boundingBox) const;

    bool IsEmpty() const;
    bool IsValid() const;

    double GetMinX() const;
    void SetMinX(double xMin);
    double GetMinY() const;
    void SetMinY(double yMin);
    double GetMinZ() const;
    void SetMinZ(double zMin);
    double GetMaxX() const;
    void SetMaxX(double xMax);
    double GetMaxY() const;
    void SetMaxY(double yMax);
    double GetMaxZ() const;
    void SetMaxZ(double zMax);

    void Set(double xMin, double yMin, double zMin, double xMax, double yMax, double zMax);

    double GetWidth() const;
    double GetHeight() const;
    double GetDepth() const;

    double GetCenterOnX() const;
    double GetCenterOnY() const;
    double GetCenterOnZ() const;

    double GetDiagonal() const;

    // [WORLD COORDINATES] -> [[0,1], [0,1], [0,1]]
    my::CVector3<double> Project(my::CVector3<double> point) const;
    // [[0,1], [0,1], [0,1]] -> [WORLD COORDINATES]
    my::CVector3<double> UnProject(my::CVector3<double> point) const;

    void Clear();

private:
	void Create();
    void Copy(const CBoundingBox& boundingBox);

public:
    double m_xMin;
    double m_xMax;
    double m_yMin;
    double m_yMax;
    double m_zMin;
    double m_zMax;

	double m_width;
    double m_height;
    double m_depth;
	
    double m_centerOnX;
    double m_centerOnY;
    double m_centerOnZ;
};

#endif // #if !defined(BOUNDING_BOX_INCLUDED)

