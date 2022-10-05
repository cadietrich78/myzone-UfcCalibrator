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

// Coment this line to link to glew.
//#include <GLee.h>

#if !defined(__GNUC__)
#include <windows.h>
#endif // !defined(__GNUC__)

#if !defined(__glee_h_)
#include <GL/glew.h>

#if defined(_WIN32)
#include <GL/wglew.h>
#else // defined(_WIN32)
#if defined(__APPLE__)
#include <GL/glew.h>
//#else
//#include <GL/glxew.h>
#endif
#endif // defined(_WIN32)
#endif // !defined(__glee_h_)

#if defined(__APPLE__)
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#endif

#include <Logger.h>
#include <GLHelper.h>

#include <Texture.h>

using namespace std;

vector<unsigned int> CTexture::m_textureUnitArray(16, 0);

/**
*/
CTexture::CTexture()
{
	Create();
}

/**
*/
CTexture::~CTexture()
{
	Destroy();
}

/**
*/
bool CTexture::Initialize(unsigned int target, int internalFormat)
{
    if (!GlHelper::IsGlOk())
    {
        LOG_ERROR();

        return false;
    }

	// Check if multitexture extension is supported.
#if !defined(__glee_h_)
	if (!glewIsExtensionSupported("GL_ARB_multitexture"))
#else // !defined(__glee_h_)
	if (!GLEE_ARB_multitexture)
#endif // !defined(__glee_h_)
	{
		LOG_MESSAGE("Unsupported OpenGL extension: GL_ARB_multitexture");

        // BUG: (04-Aug-2017) SHOULD WE LET THE TEXTURE TO BE INITIALIZED EVEN WITHOUT A EXTENDED OPENGL SUPPORT?
		//return false;
	}

	switch (target) {
		case GL_TEXTURE_1D:
			m_binding = GL_TEXTURE_BINDING_1D;
			break;
		case GL_TEXTURE_2D:
		case GL_TEXTURE_DEPTH:
			m_binding = GL_TEXTURE_BINDING_2D;
			break;
		case GL_TEXTURE_RECTANGLE_ARB:
			m_binding = GL_TEXTURE_BINDING_RECTANGLE_ARB;
			break;
		case GL_TEXTURE_3D:
			m_binding = GL_TEXTURE_BINDING_3D;
			break;
		case GL_TEXTURE_CUBE_MAP:
			m_binding = GL_TEXTURE_BINDING_CUBE_MAP;
			break;
		default:
			LOG_ERROR();
			
			return false;
	}
	
	glGenTextures(1, &m_name);

    if (!GlHelper::IsGlOk())
	{
		LOG_ERROR();
		
		return false;
	}

    
	m_target = target;

    // Check if float buffer formats are available.
    if ((internalFormat == RGB16F) ||
        (internalFormat == RGBA16F) ||
        (internalFormat == RGBA32F)) 
    {
#if !defined(__glee_h_)
    	if (!glewIsExtensionSupported("GL_ARB_texture_float"))
#else // !defined(__glee_h_)
    	if (!GLEE_ARB_texture_float)
#endif // !defined(__glee_h_)
        {
    		LOG_MESSAGE("Unsupported OpenGL extension: GL_ARB_texture_float");
        
            return false;
        }
    }

	m_internalFormat = internalFormat;

	InitializeDefaultContext();

	if (!GlHelper::IsGlOk())
	{
		LOG_ERROR();
		
		return false;
	}

	return true;
}

/**
*/
bool CTexture::SetImage(int width, int height, int border, unsigned int format, unsigned int type, const void *pixels)
{
	GuardedBind();

    // BUG: Failed to transfer images with odd sizes.
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    try
    {
        glTexImage2D(m_target, 0, m_internalFormat, width, height, border, format, type, pixels);
    }
    catch (std::exception& e)
    {
        LOG_MESSAGE(e.what());

        return false;
    }

	GuardedUnbind();

	m_width = width;
	m_height = height;
	
	m_depth = 0;

	if (!GlHelper::IsGlOk())
	{
		LOG_ERROR();
		
		return false;
	}

	return true;
}

/**
*/
bool CTexture::SetImageByLevel(int level, int width, int height, int border, unsigned int format, unsigned int type, const void* pixels)
{
	GuardedBind();

	// BUG: Failed to transfer images with odd sizes.
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	try
	{
		glTexImage2D(m_target, level, m_internalFormat, width, height, border, format, type, pixels);
	}
	catch (std::exception& e)
	{
		LOG_MESSAGE(e.what());

		return false;
	}

	GuardedUnbind();

	m_width = width;
	m_height = height;

	m_depth = 0;

	if (!GlHelper::IsGlOk())
	{
		LOG_ERROR();

		return false;
	}

	return true;
}

/**
*/
bool CTexture::SetImage(int width, int height, int depth, int border, unsigned int format, unsigned int type, const void* pixels)
{
	GuardedBind();

	glTexImage3D(m_target, 0, m_internalFormat, width, height, depth, border, format, type, pixels);

	GuardedUnbind();

	m_width = width;
	m_height = height;

	m_depth = depth;

	if (!GlHelper::IsGlOk())
	{
		LOG_ERROR();

		return false;
	}

	return true;
}

/**
*/
bool CTexture::SetSubImage(int xOffset, int yOffset, int width, int height, unsigned int format, unsigned int type, const void *pixels)
{
	GuardedBind();

	glTexSubImage2D(m_target, 0, xOffset, yOffset, width, height, format, type, pixels);

	GuardedUnbind();

	if (!GlHelper::IsGlOk())
	{
		LOG_ERROR();
		
		return false;
	}

	return true;
}

/**
*/
bool CTexture::SetSubImage(int xOffset, int yOffset, int zOffset, int width, int height, int depth, unsigned int format, unsigned int type, const void *pixels)
{
	GuardedBind();

	glTexSubImage3D(m_target, 0, xOffset, yOffset, zOffset, width, height, depth, format, type, pixels);

	GuardedUnbind();

	if (!GlHelper::IsGlOk())
	{
		LOG_ERROR();
		
		return false;
	}

	return true;
}

/**
*/
bool CTexture::BuildMipmaps()
{
	vector<unsigned char> textureData;

	if (m_width == 0)
	{
		LOG_ERROR();

		return false;
	}

	if (m_height == 0)
	{
		LOG_ERROR();

		return false;
	}

	if (m_depth != 0)
	{
		LOG_ERROR();

		return false;
	}

	textureData.assign(4*m_width*m_height, 0);

	GuardedBind();

	glGetTexImage(m_target, 0, GL_RGBA, GL_UNSIGNED_BYTE, &textureData[0]);

	if (!GlHelper::IsGlOk())
	{
		LOG_ERROR();

		return false;
	}

	if (gluBuild2DMipmaps(m_target, m_internalFormat, m_width, m_height, GL_RGBA, GL_UNSIGNED_BYTE, &textureData[0]))
	{
		LOG_ERROR();

		return false;
	}

	SetParameter(CTexture::MIN_FILTER, CTexture::LINEAR_MIPMAP_LINEAR);
	SetParameter(CTexture::MAG_FILTER, CTexture::LINEAR);

	GuardedUnbind();

	return true;
}

/**
*/
void CTexture::SetParameter(unsigned int name, int value)
{
	GuardedBind();

	glTexParameteri(m_target, name, value);

	GuardedUnbind();

	if (!GlHelper::IsGlOk())
		LOG_ERROR();
}

/**
*/
void CTexture::SetParameter(unsigned int name, float value)
{
	GuardedBind();

	glTexParameterf(m_target, name, value);

	GuardedUnbind();

	if (!GlHelper::IsGlOk())
		LOG_ERROR();
}

/**
*/
void CTexture::SetParameter(unsigned int name, const int *value)
{
	GuardedBind();

	glTexParameteriv(m_target, name, value);

	GuardedUnbind();

	if (!GlHelper::IsGlOk())
		LOG_ERROR();
}

/**
*/
void CTexture::SetParameter(unsigned int name, const float *value)
{
	GuardedBind();

	glTexParameterfv(m_target, name, value);

	GuardedUnbind();

	if (!GlHelper::IsGlOk())
		LOG_ERROR();
}

/**
*/
void CTexture::SetActiveTextureUnit(unsigned int unit)
{
	glActiveTextureARB(GL_TEXTURE0_ARB + unit);
}

/**
*/
void CTexture::DisableTextureUnit(unsigned int unit)
{
	if (m_textureUnitArray.at(unit))
	{
        if (unit != 0)
            glActiveTextureARB(GL_TEXTURE0_ARB + unit);

		glBindTexture(m_textureUnitArray.at(unit), 0);

		glDisable(m_textureUnitArray.at(unit));

		m_textureUnitArray.at(unit) = 0;

        if (unit != 0)
            glActiveTextureARB(GL_TEXTURE0_ARB);
	}
}

/**
*/
void CTexture::Bind(unsigned int unit)
{
    if (unit != 0)
        glActiveTextureARB(GL_TEXTURE0_ARB + unit);

	m_textureUnitArray.at(unit) = m_target;

	glBindTexture(m_target, m_name);

	glEnable(m_target);
}

/**
*/
int CTexture::GetName() const
{
	return m_name;
}

/**
*/
int CTexture::GetWidth() const
{
	return m_width;
}

/**
*/
int CTexture::GetHeight() const
{
	return m_height;
}

/**
*/
int CTexture::GetDepth() const
{
	return m_depth;
}

/**
*/
unsigned int CTexture::GetTarget() const
{
	return m_target;
}

/**
*/
unsigned int CTexture::GetInternalFormat() const
{
	return m_internalFormat;
}

/**
*/
bool CTexture::IsValid()
{
	return (glIsTexture(m_name) != GL_FALSE);
}

/**
*/
bool CTexture::GetSubImage(int xOffset, int yOffset, int width, int height, unsigned int format, unsigned int type, boost::shared_array<unsigned char>& pixels)
{
	HEALTH_CHECK(xOffset >= m_width, false);
	HEALTH_CHECK(yOffset >= m_height, false);
	HEALTH_CHECK((xOffset + width) >= m_width, false);
	HEALTH_CHECK((yOffset + height) >= m_height, false);

	HEALTH_CHECK(format != RGB, false);
	HEALTH_CHECK(type != UNSIGNED_BYTE, false);

	boost::shared_array<unsigned char> texImage(new unsigned char[3 * m_width * m_height]);
	
	pixels.reset(new unsigned char[3 * width * height]);

	GuardedBind();

	glGetTexImage(m_target, 0, RGB, UNSIGNED_BYTE, texImage.get());

	GuardedUnbind();

	if (!GlHelper::IsGlOk())
	{
		LOG_ERROR();

		return false;
	}

	unsigned char *texPixel = texImage.get(),
		*pixel = pixels.get();

	for (int j = yOffset; j < (yOffset + height); ++j)
	{
		for (int i = xOffset; i < (xOffset + width); ++i)
		{
			for (int c = 0; c < 3; ++c)
			{
				*pixel++ = *(texPixel + 3*(j*m_width + i) + c);
			}
		}
	}

	return true;
}

/**
*/
int CTexture::NextPot(int value)
{
	int highBit;

	if (value & (value - 1))
	{
		highBit = 0;

		while (value >>= 1)
			highBit++;

		value = 1 << (highBit + 1);
	}

	return value;
}

/**
*/
void CTexture::GuardedBind()
{
	int name;

	glGetIntegerv(m_binding, &name);

	if (m_name != name)
	{
		glBindTexture(m_target, m_name);

		m_lastTexture = name;
	}
}

/**
*/
void CTexture::GuardedUnbind()
{
	if (m_name != m_lastTexture)
		glBindTexture(m_target, m_lastTexture);
}

/**
*/
void CTexture::InitializeDefaultContext()
{
	// It seems that the textures are not created with default parameters, and 
	// I have to enforce it.
	GuardedBind();

	glTexParameteri(m_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(m_target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(m_target, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(m_target, GL_TEXTURE_WRAP_T, GL_CLAMP);

	GuardedUnbind();
}

/**
*/
void CTexture::Create()
{
	m_name = 0;
	m_lastTexture = 0;

	m_target = 0;

	m_internalFormat = 0;

	m_width = 0;
	m_height = 0;
	m_depth = 0;

	m_binding = 0;
}

/**
*/
void CTexture::Destroy()
{
	if (m_name)
		if (glIsTexture(m_name) != GL_FALSE)
			glDeleteTextures(1, &m_name);

	m_name = 0;
	m_lastTexture = 0;

	m_target = 0;

	m_internalFormat = 0;

	m_width = 0;
	m_height = 0;
	m_depth = 0;

	m_binding = 0;
}

