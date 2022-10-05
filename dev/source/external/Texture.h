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

#if !defined(TEXTURE_INCLUDED)
#define TEXTURE_INCLUDED

#include <vector>

#include <boost/utility.hpp>
#include <boost/shared_array.hpp>

class CTexture
	: boost::noncopyable
{
public:
	enum TARGET {
		TEXTURE_2D = 0x0DE1,
		TEXTURE_RECTANGLE = 0X84F5,
		TEXTURE_3D = 0x806f,
	};

	enum INTERNAL_FORMAT {
		RED = 0x1903,
		ALPHA = 0x1906,
		DEPTH_COMPONENT = 0x1902,
		DEPTH_COMPONENT24 = 0x81A6,
		LUMINANCE = 0x1909,
		LUMINANCE_ALPHA = 0x190A,
		RGB = 0x1907,
		RGBA = 0x1908,
		RGB16F = 0x881B,
		RGBA16F = 0x881A,
		RGBA32F = 0x8814,
        BGR = 0x80E0
	};

	enum TYPE {
		FLOAT = 0x1406,
		INT = 0x1404,
		SHORT = 0x1402,
		UNSIGNED_BYTE = 0x1401,
		UNSIGNED_INT = 0x1405,
		UNSIGNED_SHORT = 0x1403
	};

	enum PARAMETER {
		CLAMP = 0x2900,
		CLAMP_TO_EDGE = 0x812f,
		LINEAR = 0x2601,
		LINEAR_MIPMAP_LINEAR = 0x2703,
		MAG_FILTER = 0x2800,
		MIN_FILTER = 0x2801,
		NEAREST = 0x2600,
		REPEAT = 0x2901,
		MIRRORED_REPEAT = 0x8370,
		WRAP_S = 0x2802,
		WRAP_T = 0x2803,
		TEXTURE_MAX_LEVEL = 0x813D
	};

	CTexture();
	virtual ~CTexture();

	bool Initialize(unsigned int target, int internalFormat);

	bool SetImage(int width, int height, int border, unsigned int format, unsigned int type, const void* pixels);
	bool SetImageByLevel(int level, int width, int height, int border, unsigned int format, unsigned int type, const void* pixels);
	bool SetImage(int width, int height, int depth, int border, unsigned int format, unsigned int type, const void* pixels);

	bool SetSubImage(int xOffset, int yOffset, int width, int height, unsigned int format, unsigned int type, const void *pixels);
	bool SetSubImage(int xOffset, int yOffset, int zOffset, int width, int height, int depth, unsigned int format, unsigned int type, const void *pixels);
	
	bool BuildMipmaps();
	
	void SetParameter(unsigned int name, int value);
	void SetParameter(unsigned int name, float value);
	void SetParameter(unsigned int name, const int *value);
	void SetParameter(unsigned int name, const float *value);
	
	static void SetActiveTextureUnit(unsigned int unit);
	static void DisableTextureUnit(unsigned int unit);
	
	void Bind(unsigned int unit = 0);
	
	int GetName() const;
	int GetWidth() const;
	int GetHeight() const;
	int GetDepth() const;
	unsigned int GetTarget() const;
	unsigned int GetInternalFormat() const;
	
	bool IsValid();

	bool GetSubImage(int xOffset, int yOffset, int width, int height, unsigned int format, unsigned int type, boost::shared_array<unsigned char>& pixels);

	static int NextPot(int value);

private:
	void GuardedBind();
	void GuardedUnbind();

	void InitializeDefaultContext();

	void Create();
	void Destroy();

protected:
	unsigned int m_name;
	unsigned int m_lastTexture;
	unsigned int m_target;
	unsigned int m_internalFormat;
	unsigned int m_binding;

	int m_width;
	int m_height;
	int m_depth;

	static std::vector<unsigned int> m_textureUnitArray;
};

#endif //#if !defined(TEXTURE_INCLUDED)

