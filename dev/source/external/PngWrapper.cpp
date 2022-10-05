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

#include <iostream>

#include <png.h>

#include "Common.h"
#include "Logger.h"

#include <PngWrapper.h>

#ifdef PNG_NO_STDIO
static void _ReadDataPNG (png_structp png, png_bytep data, png_size_t length)
{
  png_size_t check;
  check = (png_size_t)fread(data, (png_size_t)1, length, (FILE*)png->io_ptr);
  if (check != length) png_error(png, "fileStream read error");
}
#endif

static void _ErrorHandlerPNG (png_structp png, png_const_charp message)
{
#if PNG_LIBPNG_VER > 10413
    png_longjmp(png, 1); 
#else //#if PNG_LIBPNG_VER > 10413
	longjmp(png->jmpbuf, 1); 
#endif // #if PNG_LIBPNG_VER > 10413
}

/**
*/
CPngWrapper::CPngWrapper()
    : m_width(0),
    m_height(0),
    m_channelCount(0)
{
}

/**
*/
bool CPngWrapper::Open(const std::string& fileName)
{
    FILE *fileStream = 0;
    png_structp png = 0;
    png_infop info = 0;
    png_bytepp row = 0;
    png_byte pngSignature[8] = {0};
    png_uint_32 rowsize = 0;
    int depth = 0;
    int type = 0;
    double gamma = 0;
    unsigned int i = 0;

    HEALTH_CHECK(fileName.empty(), false);

    PORTABLE_FOPEN(fileStream, fileName.c_str(), "rb", false);

    HEALTH_CHECK(!fileStream, false);

    // first check the eight byte PNG signature
    fread(pngSignature, 1, 8, fileStream);
    
    if (!png_check_sig(pngSignature, 8))
    {
        LOG_ERROR();

        return false;
    }

    // Create png struct with the error handlers above
    png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, (png_error_ptr)_ErrorHandlerPNG, (png_error_ptr)NULL);

    HEALTH_CHECK(!png, false);

    // Create image info struct
    info = png_create_info_struct(png);

    if (!info)
    {
        LOG_ERROR();

        png_destroy_read_struct(&png, NULL, NULL);
        
        return false;
    }

    // Set up png style error handling
    if (setjmp(png_jmpbuf(png)))
    {
        LOG_ERROR();

        // The error is reported inside the handler, but we still 
        // need to clean up and return
        png_destroy_read_struct(&png, &info, NULL);

        fclose(fileStream);
        
        return false;
    }

    png_init_io(png, fileStream);

    // If we are here, we have already read 8 bytes from the fileStream
    png_set_sig_bytes(png, 8);

    // png_read_info() returns all information from the fileStream 
    // before the first data chunk
    png_read_info(png, info);

    // Query required image info
    png_get_IHDR(png, info, (png_uint_32p)&m_width, (png_uint_32p)&m_height, &depth, &type, NULL, NULL, NULL);
            
    // Expand images of all color-type and bit-depth to 3x8 bit RGB images
    // let the library process things like alpha, transparency, background
    if (depth == 16) 
        png_set_strip_16(png);

    // Expand paletted colors into rgb triplets
    if (type == PNG_COLOR_TYPE_PALETTE)
        png_set_palette_to_rgb(png);

    // Expand grayscale images to 8 bits from 1, 2, or 4 bits
    if (type == PNG_COLOR_TYPE_GRAY && depth < 8)
        //png_set_gray_1_2_4_to_8(png);
        png_set_expand_gray_1_2_4_to_8(png);

    // Expand paletted or rgb images with transparency to full alpha
    // channels so the data will be available as rgba quartets
    if (png_get_valid(png, info, PNG_INFO_tRNS))
        png_set_tRNS_to_alpha(png);

    // Convert grayscale images to rgb
    if (type == PNG_COLOR_TYPE_GRAY ||
        type == PNG_COLOR_TYPE_GRAY_ALPHA) 
        png_set_gray_to_rgb(png);

    // Set gamma conversion
    if (png_get_gAMA(png, info, &gamma))
        png_set_gamma(png, (double)2.2, gamma);

    // After the transformations have been registered update info
    png_read_update_info(png, info);

    // Get width, height again and the new bit-depth and color-type
    png_get_IHDR(png, info, &m_width, &m_height, &depth, &type, NULL, NULL, NULL);
    
    m_channelCount = (INT32)png_get_channels(png, info);

    // row_bytes is the width x number of channels
    rowsize = (png_uint_32)png_get_rowbytes(png, info);

    HEALTH_CHECK(rowsize != (m_width * m_channelCount), false);

    m_buffer.reset(new unsigned char[rowsize*(m_height + 1)]);

    HEALTH_CHECK(!m_buffer.get(), false);

    // Set up row pointers
    row = (png_bytepp)malloc(m_height*sizeof(png_bytep));

    HEALTH_CHECK(row == NULL, false);

    for (i=0; i<m_height; i++)
        row[i] = &((png_bytep)m_buffer.get())[i*rowsize];

    // Read the whole image
    png_read_image(png, row);

    // Finish reading
    png_read_end(png, NULL);

    // Clean up
    png_destroy_read_struct(&png, &info, NULL);

    free(row); 
    row = NULL;
    
    fclose(fileStream);
    
    return true;
}

/**
*/
bool CPngWrapper::Save(std::string fileName, unsigned int width, unsigned int height, unsigned char *rgbArray) const
{
    FILE *fp = NULL;
    png_structp png_ptr = NULL;
    png_infop info_ptr = NULL;
    png_bytep row = NULL;

    HEALTH_CHECK(fileName.empty(), false);

    if ((fileName.size() >= 4) &&
        (fileName.substr(fileName.size() - 4) != ".png"))
    {
        fileName += ".png";
    }

    // Open file for writing (binary mode)
    PORTABLE_FOPEN(fp, fileName.c_str(), "wb", false);
    
    if (fp == NULL)
    {
        LOG_MESSAGE("Could not open file " + fileName + " for writing");

        return false;
    }

    // Initialize write structure
    png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    
    if (png_ptr == NULL)
    {
        LOG_MESSAGE("Could not allocate write struct");
        
        if (fp != NULL)
            fclose(fp);

        return false;
    }

    // Initialize info structure
    info_ptr = png_create_info_struct(png_ptr);
    
    if (info_ptr == NULL)
    {
        LOG_MESSAGE("Could not allocate info struct");
        
        if (fp != NULL)
            fclose(fp);
        
        if (png_ptr != NULL)
            png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
        
        return false;
    }

    // Setup Exception handling
    if (setjmp(png_jmpbuf(png_ptr)))
    {
        LOG_MESSAGE("Error during png creation");
        
        if (fp != NULL)
            fclose(fp);
        
        if (info_ptr != NULL)
            png_free_data(png_ptr, info_ptr, PNG_FREE_ALL, -1);
        
        if (png_ptr != NULL)
            png_destroy_write_struct(&png_ptr, (png_infopp)NULL);

        return false;
    }

    png_init_io(png_ptr, fp);

    // Write header (8 bit colour depth)
    png_set_IHDR(png_ptr, info_ptr, width, height, 8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

    //// Set title
    //if (title != NULL) {
    //    png_text title_text;
    //    title_text.compression = PNG_TEXT_COMPRESSION_NONE;
    //    title_text.key = "Title";
    //    title_text.text = title;
    //    png_set_text(png_ptr, info_ptr, &title_text, 1);
    //}

    png_write_info(png_ptr, info_ptr);

    // Allocate memory for one row (3 bytes per pixel - RGB)
    row = (png_bytep)malloc(3 * width * sizeof(png_byte));

    // Write image data
    unsigned int x,
    y;
    
    for (y = 0; y<height; y++)
    {
        for (x = 0; x<width; x++)
        {
            row[x * 3 + 0] = rgbArray[3 * (y*width + x) + 0];
            row[x * 3 + 1] = rgbArray[3 * (y*width + x) + 1];
            row[x * 3 + 2] = rgbArray[3 * (y*width + x) + 2];
            //setRGB(&(row[x * 3]), rgbArray[y*width + x]);
        }
        
        png_write_row(png_ptr, row);
    }

    // End write
    png_write_end(png_ptr, NULL);

    if (fp != NULL)
        fclose(fp);
    
    if (info_ptr != NULL)
        png_free_data(png_ptr, info_ptr, PNG_FREE_ALL, -1);
    
    if (png_ptr != NULL)
        png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
    
    if (row != NULL)
        free(row);

    return true;
}

/**
*/
bool CPngWrapper::Save(std::string fileName, boost::shared_ptr<my::image::CImage> image) const
{
    HEALTH_CHECK(!image->IsValid(), false);

    if (!Save(fileName, image->GetWidth(), image->GetHeight(), image->GetBuffer().get()))
    {
        LOG_ERROR();

        return false;
    }

    return true;
}

// HELPER FOR CPngWrapper::Encode
void EncodeWriteCallback(png_structp  png_ptr, png_bytep data, png_size_t length)
{
    std::vector<unsigned char> *p = (std::vector<unsigned char>*)png_get_io_ptr(png_ptr);

    p->insert(p->end(), data, data + length);
}

/**
*/
bool CPngWrapper::Encode(my::int32 width, my::int32 height, int channelCount, const unsigned char *rgbArray, std::vector<unsigned char> *byteArray) const
{
    byteArray->clear();

    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

    HEALTH_CHECK(!png_ptr, false);

    png_infop info_ptr = png_create_info_struct(png_ptr);

    HEALTH_CHECK(!info_ptr, false);
    HEALTH_CHECK(setjmp(png_jmpbuf(png_ptr)), false);

    my::int32 colorType = my::Null<my::int32>();

    switch (channelCount) {
    case 3:
        colorType = PNG_COLOR_TYPE_RGB;
        break;

    case 4:
        colorType = PNG_COLOR_TYPE_RGBA;
        break;
    }

    HEALTH_CHECK(my::IsNull(colorType), false);

    png_set_IHDR(png_ptr, info_ptr, width, height, 8, colorType, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

    //png_set_compression_level(png_ptr, 1);

    std::vector<unsigned char*> rows(height);

    for (my::int32 y = 0; y < height; ++y)
        rows[y] = (unsigned char*)rgbArray + y * width * channelCount;

    png_set_rows(png_ptr, info_ptr, &rows[0]);
    png_set_write_fn(png_ptr, byteArray, EncodeWriteCallback, NULL);
    png_write_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);

    if (info_ptr != NULL)
        png_free_data(png_ptr, info_ptr, PNG_FREE_ALL, -1);

    if (png_ptr != NULL)
        png_destroy_write_struct(&png_ptr, (png_infopp)NULL);

    return true;
}

/**
*/
boost::shared_array<unsigned char> CPngWrapper::GetBuffer() const
{
    return m_buffer;
}

/**
*/
void CPngWrapper::SetBuffer(boost::shared_array<unsigned char> buffer)
{
    m_buffer = buffer;
}
    
/**
*/
unsigned int CPngWrapper::GetWidth() const
{
    return m_width;
}

/**
*/
void CPngWrapper::SetWidth(unsigned int width)
{
    m_width = width;
}

/**
*/
unsigned int CPngWrapper::GetHeight() const
{
    return m_height;
}

/**
*/
void CPngWrapper::SetHeight(unsigned int height)
{
    m_height = height;
}
    
/**
*/
INT32 CPngWrapper::GetChannelCount() const
{
    return m_channelCount;
}

/**
*/
void CPngWrapper::SetChannelCount(INT32 channelCount)
{
    m_channelCount = channelCount;
}

