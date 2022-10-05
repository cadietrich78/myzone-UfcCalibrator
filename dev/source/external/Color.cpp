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

#include <cmath>

#include <Color.h>

namespace MyColor 
{
    // TRICKY: (28-Jan-2015) CRGB<floating point> RANGES FROM 0 TO 1; CRGB<unsigned char> RANGES FROM 0 TO 255
    // BUG: (27-Feb-2016) It's here because Clang and gcc don't know how to deal with partial template specializations.
    template <>
    void CRGB<unsigned char>::SetColor(unsigned char r, unsigned char g, unsigned char b)
    {
        m_rgb[0] = r;
        m_rgb[1] = g;
        m_rgb[2] = b;
    }
    
    // TRICKY: (28-Jan-2015) CRGB<floating point> RANGES FROM 0 TO 1; CRGB<unsigned char> RANGES FROM 0 TO 255
    // BUG: (27-Feb-2016) It's here because Clang and gcc don't know how to deal with partial template specializations.
    template <>
    void CRGB<double>::SetColor(double r, double g, double b)
    {
        HEALTH_CHECK(r < 0.0, /*false*/);
        HEALTH_CHECK(r > 1.0, /*false*/);
        HEALTH_CHECK(g < 0.0, /*false*/);
        HEALTH_CHECK(g > 1.0, /*false*/);
        HEALTH_CHECK(b < 0.0, /*false*/);
        HEALTH_CHECK(b > 1.0, /*false*/);
        
        m_rgb[0] = r;
        m_rgb[1] = g;
        m_rgb[2] = b;
    }
    
    // TRICKY: (28-Jan-2015) CRGB<floating point> RANGES FROM 0 TO 1; CRGB<unsigned char> RANGES FROM 0 TO 255
    // BUG: (27-Feb-2016) It's here because Clang and gcc don't know how to deal with partial template specializations.
    template <>
    void CRGB<double>::SetColor(unsigned char r, unsigned char g, unsigned char b)
    {
        SetColor(0.0039215686274509803921568627451 * r, 0.0039215686274509803921568627451 * g, 0.0039215686274509803921568627451 * b);
    }

    template <>
    std::string CRGB<unsigned char>::ToSvg() const
    {
        return "rgb("
            + my::NumberToString((my::int32)m_rgb[0])
            + ","
            + my::NumberToString((my::int32)m_rgb[1])
            + ","
            + my::NumberToString((my::int32)m_rgb[2])
            + ")";
    }

    template <>
    std::string CRGB<double>::ToSvg() const
    {
        return "rgb("
            + my::NumberToString((my::int32)(m_rgb[0] * 255.0))
            + ","
            + my::NumberToString((my::int32)(m_rgb[1] * 255.0))
            + ","
            + my::NumberToString((my::int32)(m_rgb[2] * 255.0))
            + ")";
    }
        
    // http://www.ryanjuckett.com/programming/rgb-color-space-conversion/
    template <typename T>
    T GammaExpand_sRGB(T nonlinear);

    // http://www.ryanjuckett.com/programming/rgb-color-space-conversion/
    template <>
    inline float GammaExpand_sRGB(float nonlinear)
    {
        // http://en.wikipedia.org/wiki/SRGB#Theory_of_the_transformation
        return   (nonlinear <= 0.04045f)
            ? (nonlinear / 12.92f)
            : (powf((nonlinear + 0.055f) / 1.055f, 2.4f));
    }

    // http://www.ryanjuckett.com/programming/rgb-color-space-conversion/
    template <>
    inline double GammaExpand_sRGB(double nonlinear)
    {
        // http://en.wikipedia.org/wiki/SRGB#Theory_of_the_transformation
        return   (nonlinear <= 0.04045)
            ? (nonlinear / 12.92)
            : (pow((nonlinear + 0.055) / 1.055, 2.4));
    }

    // http://www.ryanjuckett.com/programming/rgb-color-space-conversion/
    template<typename T>
    void GammaExpand_sRGB(T& r, T& g, T&b)
    {
        r = GammaExpand_sRGB(r);
        g = GammaExpand_sRGB(g);
        b = GammaExpand_sRGB(b);
    }

    template <>
    CRGB<unsigned char>::CRGB(unsigned char r, unsigned char g, unsigned char b)
    {
        SetColor(r, g, b);
    }

    template <>
    CRGB<unsigned char>::CRGB(int r, int g, int b)
    {
        SetColor((unsigned char)r, (unsigned char)g, (unsigned char)b);
    }

    template <>
    CRGB<double>::CRGB(unsigned char r, unsigned char g, unsigned char b)
    {
        SetColor(r, g, b);
    }

    template <>
    CRGB<double>::CRGB(int r, int g, int b)
    {
        SetColor((unsigned char)r, (unsigned char)g, (unsigned char)b);
    }

    template <>
    CRGB<double>::CRGB(double r, double g, double b)
    {
        SetColor(r, g, b);
    }

    // 32 BTT COLOR, 8 BPP, ARGB
    template <>
    CRGB<unsigned char>::CRGB(int color)
    {
        m_rgb[0] = (unsigned char)((color >> 16) & 0xFF);
        m_rgb[1] = (unsigned char)((color >> 8) & 0xFF);
        m_rgb[2] = (unsigned char)(color & 0xFF);
    }

    // 32 BTT COLOR, 8 BPP, ARGB
    template <>
    CRGB<double>::CRGB(int color)
    {
        m_rgb[0] = 0.0039215686274509803921568627451 * ((color >> 16) & 0xFF);
        m_rgb[1] = 0.0039215686274509803921568627451 * ((color >> 8) & 0xFF);
        m_rgb[2] = 0.0039215686274509803921568627451 * (color & 0xFF);
    }

    // 32 BTT COLOR, 8 BPP, ARGB
    template <>
    void CRGB<unsigned char>::SetColor(int color)
    {
        m_rgb[0] = (unsigned char)((color >> 16) & 0xFF);
        m_rgb[1] = (unsigned char)((color >> 8) & 0xFF);
        m_rgb[2] = (unsigned char)(color & 0xFF);
    }

    // 32 BTT COLOR, 8 BPP, ARGB
    template <>
    void CRGB<double>::SetColor(int color)
    {
        m_rgb[0] = 0.0039215686274509803921568627451 * ((color >> 16) & 0xFF);
        m_rgb[1] = 0.0039215686274509803921568627451 * ((color >> 8) & 0xFF);
        m_rgb[2] = 0.0039215686274509803921568627451 * (color & 0xFF);
    }

    // TRICKY: (28-Jan-2015) CRGB<floating point> RANGES FROM 0 TO 1; CRGB<unsigned char> RANGES FROM 0 TO 255
    template <>
    void CRGB<unsigned char>::SetColor(int r, int g, int b)
    {
        SetColor((unsigned char)r, (unsigned char)g, (unsigned char)b);
    }

    // TRICKY: (28-Jan-2015) CRGB<floating point> RANGES FROM 0 TO 1; CRGB<unsigned char> RANGES FROM 0 TO 255
    template <>
    void CRGB<double>::SetColor(int r, int g, int b)
    {
        SetColor(0.0039215686274509803921568627451 * r, 0.0039215686274509803921568627451 * g, 0.0039215686274509803921568627451 * b);
    }

    // (https://en.wikipedia.org/wiki/Relative_luminance) For RGB color spaces that use the ITU-R BT.709 primaries (or sRGB, which defines the same primaries), relative luminance can be calculated from linear RGB components (Y = 0.2126 R + 0.7152 G + 0.0722 B)
    template <>
	unsigned char CRGB<unsigned char>::GetLuminance() const
    {
        double r = 0.0039215686274509803921568627451 * m_rgb[0],
            g = 0.0039215686274509803921568627451 * m_rgb[1],
            b = 0.0039215686274509803921568627451 * m_rgb[2];

        // Convert from gamma-corrected sRGB to linear sRGB
        GammaExpand_sRGB(r, g, b);

        double luminance = 0.2126 * r + 0.7152 * g + 0.0722 * b;

        return (unsigned char)((int)(luminance * 255.0));
    }

    // (https://en.wikipedia.org/wiki/Relative_luminance) For RGB color spaces that use the ITU-R BT.709 primaries (or sRGB, which defines the same primaries), relative luminance can be calculated from linear RGB components (Y = 0.2126 R + 0.7152 G + 0.0722 B)
    template <>
	double CRGB<double>::GetLuminance() const
    {
        double r = m_rgb[0],
            g = m_rgb[1],
            b = m_rgb[2];

        // Convert from gamma-corrected sRGB to linear sRGB
        GammaExpand_sRGB(r, g, b);

        double luminance = 0.2126 * r + 0.7152 * g + 0.0722 * b;

        return luminance;
    }

    // TRICKY: (28-Jan-2016) FROM GAMMA-CORRECTED SRGB (DEVICE DEPENDENT)
    template <>
	void CCieXYZ<double>::FromRGB(const CRGB<double>& rgb)
	{
        double r = rgb.GetR(),
            g = rgb.GetG(),
            b = rgb.GetB();
        
        //// (BEGIN OF) TESTING: (??-???-????) http://en.wikipedia.org/wiki/CIE_1931_color_space
        //m_xyz[0] = 0.17697 * (0.49 * r + 0.31 * g + 0.2 * b);
        //m_xyz[1] = 0.17697 * (0.17697 * r + 0.81240 * g + 0.01063 * b);
        //m_xyz[2] = 0.17697 * (0.0 * r + 0.01 * g + 0.99 * b);
        //// (END OF) TESTING: (??-???-????) http://en.wikipedia.org/wiki/CIE_1931_color_space

        //// (BEGIN OF) TESTING: (??-???-????) http://www.martinreddy.net/gfx/faqs/colorconv.faq
        //m_xyz[0] = 0.431 * r + 0.342 * g + 0.178 * b;
        //m_xyz[1] = 0.222 * r + 0.707 * g + 0.071 * b;
        //m_xyz[2] = 0.020 * r + 0.130 * g + 0.939 * b;
        //// (END OF) TESTING: (??-???-????) http://www.martinreddy.net/gfx/faqs/colorconv.faq

        //// (BEGIN OF) TESTING: (??-???-????) http://cs.haifa.ac.il/hagit/courses/ist/Lectures/Demos/ColorApplet2/t_convert.html#RGB to XYZ & XYZ to RGB
        //m_xyz[0] = 0.412453 * r + 0.357580 * g + 0.180423 * b;
        //m_xyz[1] = 0.212671 * r + 0.715160 * g + 0.072169 * b;
        //m_xyz[2] = 0.019334 * r + 0.119193 * g + 0.950227 * b;
        //// (END OF) TESTING: (??-???-????) http://cs.haifa.ac.il/hagit/courses/ist/Lectures/Demos/ColorApplet2/t_convert.html#RGB to XYZ & XYZ to RGB

        // (BEGIN OF) TESTING: (28-Jan-2016) http://www.ryanjuckett.com/programming/rgb-color-space-conversion/
        // Convert from gamma-corrected sRGB to linear sRGB
        GammaExpand_sRGB(r, g, b);

        m_xyz[0] = 0.412390888 * r + 0.357584417 * g + 0.180480808 * b;
        m_xyz[1] = 0.212639064 * r + 0.715168834 * g + 0.0721923187 * b;
        m_xyz[2] = 0.0193308052 * r + 0.119194761 * g + 0.950532198 * b;
        // (END OF) TESTING: (28-Jan-2016) http://www.ryanjuckett.com/programming/rgb-color-space-conversion/
    }

    //// TODO: (??-???-????) ?
	//template <>
	//void CCieXYZ<double>::FromCieLAB(const CCieLab<double>& lab)
	//{
	//	double delta,
	//		fy,
	//		fx,
	//		fz;

	//	delta = 6.0/29.0;  

	//	fy = (lab.GetL() + 16.0)/116.0;  
	//	fx = fy + lab.GetA()/500.0;  
	//	fz = fy - lab.GetB()/200.0;  

	//	m_xyz[0] = (fx > delta) ? Xn*pow(fx, 3.0) : (fx - 16.0/116.0)*3.0*pow(delta, 2.0);  
	//	m_xyz[1] = (fy > delta) ? Yn*pow(fy, 3.0) : (fy - 16.0/116.0)*3.0*pow(delta, 2.0);  
	//	m_xyz[2] = (fz > delta) ? Zn*pow(fz, 3.0) : (fz - 16.0/116.0)*3.0*pow(delta, 2.0);
	//}
    
    // BUG: (27-Feb-2016) It's here because Clang and gcc don't know how to deal with partial template specializations.
    template <>
    double CCieLab<double>::f(double t) const
    {
        double ft = my::Null<double>();
        
        //// (BEGIN OF) TESTING: (??-???-????) http://en.wikipedia.org/wiki/Lab_color_space
        //if (t > (pow(6.0 / 29.0, 3.0)))
        //    ft = pow(t, 1.0 / 3.0);
        //else
        //    ft = (1.0 / 3.0) * pow(29.0 / 6.0, 2.0) * t + (4.0 / 29.0);
        //// (END OF) TESTING: (??-???-????)
        
        // (BEGIN OF) TESTING: (??-???-????) http://www.martinreddy.net/gfx/faqs/colorconv.faq
        if (t > 0.008856)
            ft = pow(t, 1.0 / 3.0);
        else
            ft = 7.787 * t + 16.0 / 116.0;
        // (END OF) TESTING: (??-???-????)
        
        HEALTH_CHECK(my::IsNull(ft), 0);
        
        return ft;
    }
    
    // BUG: (27-Feb-2016) It's here because Clang and gcc don't know how to deal with partial template specializations.
    template <>
    void CCieLab<double>::FromCieXYZ(const CCieXYZ<double>& xyz)
    {
        //// (BEGIN OF) TESTING: (??-???-????) http://en.wikipedia.org/wiki/Lab_color_space
        //double fx,
        //    fy,
        //    fz;
        
        //fx = f(xyz.GetX() / Xn);
        //fy = f(xyz.GetY() / Yn);
        //fz = f(xyz.GetZ() / Zn);
        
        //m_lab[0] = 116.0 * fy - 16.0;
        //m_lab[1] = 500.0 * (fx - fy);
        //m_lab[2] = 200.0 * (fy - fz);
        //// (END OF) TESTING: (??-???-????) http://en.wikipedia.org/wiki/Lab_color_space
        
        // (BEGIN OF) TESTING: (??-???-????) http://www.martinreddy.net/gfx/faqs/colorconv.faq
        double delta = xyz.GetY() / Yn;
        
        if (delta > 0.008856)
            m_lab[0] = 116.0 * pow(delta, 1.0 / 3.0);
        else
            m_lab[0] = 903.3 * delta;
        
        double fx = f(xyz.GetX() / Xn),
        fy = f(xyz.GetY() / Yn),
        fz = f(xyz.GetZ() / Zn);
        
        m_lab[1] = 500.0 * (fx - fy);
        m_lab[2] = 200.0 * (fy - fz);
        // (END OF) TESTING: (??-???-????) http://www.martinreddy.net/gfx/faqs/colorconv.faq
        
        //// (BEGIN OF) TESTING: (??-???-????) http://komozo.blogspot.com/2011/04/rgbcielab.html
        //double delta = 6.0 / 29.0;
        
        //double fx = (xyz.GetX() / Xn > pow(delta, 3.0)) ? pow(xyz.GetX() / Xn, 1.0 / 3.0) : (1.0 / 3.0) * pow((1.0 / delta), 2.0) * xyz.GetX() / Xn + (4.0 / 29.0);
        
        //double fy = (xyz.GetY() / Yn > pow(delta, 3.0)) ? powf(xyz.GetY() / Yn, 1.0 / 3.0) : (1.0 / 3.0) * pow((1.0 / delta), 2.0) * xyz.GetY() / Yn + (4.0 / 29.0);
        
        //double fz = (xyz.GetZ() / Zn > pow(delta, 3.0)) ? pow(xyz.GetZ() / Zn, 1.0 / 3.0) : (1.0 / 3.0) * pow((1.0 / delta), 2.0) * xyz.GetZ() / Zn + (4.0 / 29.0);
        
        //m_lab[0] = 116.0 * fy - 16.0;
        //m_lab[1] = 500.0 * (fx - fy);
        //m_lab[2] = 200.0 * (fy - fz);
        //// (END OF) TESTING: (??-???-????) http://komozo.blogspot.com/2011/04/rgbcielab.html
    }

    template <>
	void CCieLab<double>::FromRGB(const CRGB<double>& rgb)
	{
		CCieXYZ<double> xyz(rgb);

		FromCieXYZ(xyz);
	}

    template <>
    bool CHSV<double>::FromRGB(const CRGB<double>& rgb)
    {
        // https://stackoverflow.com/questions/3018313/algorithm-to-convert-rgb-to-hsv-and-hsv-to-rgb-in-range-0-255-for-both

        double min, 
            max, 
            delta;

        min = rgb.GetR() < rgb.GetG() ? rgb.GetR() : rgb.GetG();
        min = min  < rgb.GetB() ? min : rgb.GetB();

        max = rgb.GetR() > rgb.GetG() ? rgb.GetR() : rgb.GetG();
        max = max  > rgb.GetB() ? max : rgb.GetB();

        m_hsv[2] = max;

        delta = max - min;

        if (delta < 1.0e-7)
        {
            m_hsv[1] = 0.0;
            // BUG: (19-Mar-2019) ORIGINALLY "undefined, maybe nan?", BUT AFTER CHECKING THE RESULT OF THE CONVERSION ON GIMP, HUE IS NOW SET TO 0.
            m_hsv[0] = 0.0;

            return true;
        }

        if (max > 0.0)
            m_hsv[1] = (delta / max);
        else 
        {
            // "if max is 0, then r = g = b = 0, s = 0, h is undefined"
            m_hsv[1] = 0.0;
            m_hsv[0] = my::Null<double>();
            
            return false;
        }

        if (rgb.GetR() >= max)
            // "between yellow & magenta"
            m_hsv[0] = (rgb.GetG() - rgb.GetB()) / delta;
        else
        {
            if (rgb.GetG() >= max)
                // "between cyan & yellow"
                m_hsv[0] = 2.0 + (rgb.GetB() - rgb.GetR()) / delta;
            else
                // "between magenta & cyan"
                m_hsv[0] = 4.0 + (rgb.GetR() - rgb.GetG()) / delta;
        }

        m_hsv[0] *= 60.0;

        if (m_hsv[0] < 0.0)
            m_hsv[0] += 360.0;

        return true;
    }
}

