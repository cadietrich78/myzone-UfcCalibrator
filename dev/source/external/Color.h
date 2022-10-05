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

#if !defined(COLOR_INCLUDED)
#define COLOR_INCLUDED

#include <boost/utility.hpp>
#include <boost/shared_ptr.hpp>

#include <Common.h>
#include <Logger.h>

namespace MyColor 
{
    // Gamma-corrected sRGB (device dependent).
    // TRICKY: (28-Jan-2015) CRGB<floating point> RANGES FROM 0 TO 1; CRGB<unsigned char> RANGES FROM 0 TO 255
	template <typename T>
	class CRGB
	{
	public:
		CRGB()
		{
			Create();
		}
	
		CRGB(const CRGB& rgb)
		{
			Copy(rgb);
		}
	
        // TRICKY: (28-Jan-2015) CRGB<floating point> RANGES FROM 0 TO 1; CRGB<unsigned char> RANGES FROM 0 TO 255
        // TODO: (28-Jan-2016) MAKE IT A TEMPLATE!
        CRGB(unsigned char r, unsigned char g, unsigned char b);
        // TRICKY: (28-Jan-2015) CRGB<floating point> RANGES FROM 0 TO 1; CRGB<unsigned char> RANGES FROM 0 TO 255
        // TODO: (28-Jan-2016) MAKE IT A TEMPLATE!
        CRGB(int r, int g, int b);
        // TRICKY: (28-Jan-2015) CRGB<floating point> RANGES FROM 0 TO 1; CRGB<unsigned char> RANGES FROM 0 TO 255
        // TODO: (28-Jan-2016) MAKE IT A TEMPLATE!
        CRGB(double r, double g, double b);

        // 32 BTT COLOR, 8 BPP, ARGB
        CRGB(int color);

	    CRGB(const std::string& colorName)
        {
            if (!LoadPresetColorByName(colorName))
                LOG_ERROR();
        }
        
		void operator=(const CRGB& rgb)
		{
			Copy(rgb);
		}

		T GetR() const
		{
			return m_rgb[0];
		}

		void SetR(T r)
		{
			m_rgb[0] = r;
		}

		T GetG() const
		{
			return m_rgb[1];
		}

		void SetG(T g)
		{
			m_rgb[1] = g;
		}

		T GetB() const
		{
			return m_rgb[2];
		}

		void SetB(T b)
		{
			m_rgb[2] = b;
		}

        // TRICKY: (28-Jan-2015) CRGB<floating point> RANGES FROM 0 TO 1; CRGB<unsigned char> RANGES FROM 0 TO 255
        // TODO: (28-Jan-2016) MAKE IT A TEMPLATE!
        void SetColor(unsigned char r, unsigned char g, unsigned char b);
        // TRICKY: (28-Jan-2015) CRGB<floating point> RANGES FROM 0 TO 1; CRGB<unsigned char> RANGES FROM 0 TO 255
        // TODO: (28-Jan-2016) MAKE IT A TEMPLATE!
        void SetColor(int r, int g, int b);
        // TRICKY: (28-Jan-2015) CRGB<floating point> RANGES FROM 0 TO 1; CRGB<unsigned char> RANGES FROM 0 TO 255
        // TODO: (28-Jan-2016) MAKE IT A TEMPLATE!
        void SetColor(double r, double g, double b);

		void SetColor(int color);

        // (https://en.wikipedia.org/wiki/Relative_luminance) For RGB color spaces that use the ITU-R BT.709 primaries (or sRGB, which defines the same primaries), relative luminance can be calculated from linear RGB components (Y = 0.2126 R + 0.7152 G + 0.0722 B). The formula reflects the luminosity function: green light contributes the most to the intensity perceived by humans, and blue light the least.
        T GetLuminance() const;

	    bool LoadPresetColorByName(const std::string& colorName)
	    {
		    // http://www.w3.org/TR/SVG/types.html#ColorKeywords
		    if (colorName == "aliceblue")
			    SetColor(240, 248, 255);
		    if (colorName == "antiquewhite")
			    SetColor(250, 235, 215);
		    if (colorName == "aqua")
			    SetColor(0, 255, 255);
		    if (colorName == "aquamarine")
			    SetColor(127, 255, 212);
		    if (colorName == "azure")
			    SetColor(240, 255, 255);
		    if (colorName == "beige")
			    SetColor(245, 245, 220);
		    if (colorName == "bisque")
			    SetColor(255, 228, 196);
		    if (colorName == "black")
			    SetColor(0, 0, 0);
		    if (colorName == "blanchedalmond")
			    SetColor(255, 235, 205);
		    if (colorName == "blue")
			    SetColor(0, 0, 255);
		    if (colorName == "blueviolet")
			    SetColor(138, 43, 226);
		    if (colorName == "brown")
			    SetColor(165, 42, 42);
		    if (colorName == "burlywood")
			    SetColor(222, 184, 135);
		    if (colorName == "cadetblue")
			    SetColor( 95, 158, 160);
		    if (colorName == "chartreuse")
			    SetColor(127, 255, 0);
		    if (colorName == "chocolate")
			    SetColor(210, 105, 30);
		    if (colorName == "coral")
			    SetColor(255, 127, 80);
		    if (colorName == "cornflowerblue")
			    SetColor(100, 149, 237);
		    if (colorName == "cornsilk")
			    SetColor(255, 248, 220);
		    if (colorName == "crimson")
			    SetColor(220, 20, 60);
		    if (colorName == "cyan")
			    SetColor(0, 255, 255);
		    if (colorName == "darkblue")
			    SetColor(0, 0, 139);
		    if (colorName == "darkcyan")
			    SetColor(0, 139, 139);
		    if (colorName == "darkgoldenrod")
			    SetColor(184, 134, 11);
		    if (colorName == "darkgray")
			    SetColor(169, 169, 169);
		    if (colorName == "darkgreen")
			    SetColor(0, 100, 0);
		    if (colorName == "darkgrey")
			    SetColor(169, 169, 169);
		    if (colorName == "darkkhaki")
			    SetColor(189, 183, 107);
		    if (colorName == "darkmagenta")
			    SetColor(139, 0, 139);
		    if (colorName == "darkolivegreen")
			    SetColor( 85, 107, 47);
		    if (colorName == "darkorange")
			    SetColor(255, 140, 0);
		    if (colorName == "darkorchid")
			    SetColor(153, 50, 204);
		    if (colorName == "darkred")
			    SetColor(139, 0, 0);
		    if (colorName == "darksalmon")
			    SetColor(233, 150, 122);
		    if (colorName == "darkseagreen")
			    SetColor(143, 188, 143);
		    if (colorName == "darkslateblue")
			    SetColor( 72, 61, 139);
		    if (colorName == "darkslategray")
			    SetColor( 47, 79, 79);
		    if (colorName == "darkslategrey")
			    SetColor( 47, 79, 79);
		    if (colorName == "darkturquoise")
			    SetColor(0, 206, 209);
		    if (colorName == "darkviolet")
			    SetColor(148, 0, 211);
		    if (colorName == "deeppink")
			    SetColor(255, 20, 147);
		    if (colorName == "deepskyblue")
			    SetColor(0, 191, 255);
		    if (colorName == "dimgray")
			    SetColor(105, 105, 105);
		    if (colorName == "dimgrey")
			    SetColor(105, 105, 105);
		    if (colorName == "dodgerblue")
			    SetColor( 30, 144, 255);
		    if (colorName == "firebrick")
			    SetColor(178, 34, 34);
		    if (colorName == "floralwhite")
			    SetColor(255, 250, 240);
		    if (colorName == "forestgreen")
			    SetColor( 34, 139, 34);
		    if (colorName == "fuchsia")
			    SetColor(255, 0, 255);
		    if (colorName == "gainsboro")
			    SetColor(220, 220, 220);
		    if (colorName == "ghostwhite")
			    SetColor(248, 248, 255);
		    if (colorName == "gold")
			    SetColor(255, 215, 0);
		    if (colorName == "goldenrod")
			    SetColor(218, 165, 32);
		    if (colorName == "gray")
			    SetColor(128, 128, 128);
		    if (colorName == "grey")
			    SetColor(128, 128, 128);
		    if (colorName == "green")
			    SetColor(0, 128, 0);
		    if (colorName == "greenyellow")
			    SetColor(173, 255, 47);
		    if (colorName == "honeydew")
			    SetColor(240, 255, 240);
		    if (colorName == "hotpink")
			    SetColor(255, 105, 180);
		    if (colorName == "indianred")
			    SetColor(205, 92, 92);
		    if (colorName == "indigo")
			    SetColor( 75, 0, 130);
		    if (colorName == "ivory")
			    SetColor(255, 255, 240);
		    if (colorName == "khaki")
			    SetColor(240, 230, 140);
		    if (colorName == "lavender")
			    SetColor(230, 230, 250);
		    if (colorName == "lavenderblush")
			    SetColor(255, 240, 245);
		    if (colorName == "lawngreen")
			    SetColor(124, 252, 0);
		    if (colorName == "lemonchiffon")
			    SetColor(255, 250, 205);
		    if (colorName == "lightblue")
			    SetColor(173, 216, 230);
		    if (colorName == "lightcoral")
			    SetColor(240, 128, 128);
		    if (colorName == "lightcyan")
			    SetColor(224, 255, 255);
		    if (colorName == "lightgoldenrodyellow")
			    SetColor(250, 250, 210);
		    if (colorName == "lightgray")
			    SetColor(211, 211, 211);
		    if (colorName == "lightgreen")
			    SetColor(144, 238, 144);
		    if (colorName == "lightgrey")
			    SetColor(211, 211, 211);
		    if (colorName == "lightpink")
			    SetColor(255, 182, 193);
		    if (colorName == "lightsalmon")
			    SetColor(255, 160, 122);
		    if (colorName == "lightseagreen")
			    SetColor( 32, 178, 170);
		    if (colorName == "lightskyblue")
			    SetColor(135, 206, 250);
		    if (colorName == "lightslategray")
			    SetColor(119, 136, 153);
		    if (colorName == "lightslategrey")
			    SetColor(119, 136, 153);
		    if (colorName == "lightsteelblue")
			    SetColor(176, 196, 222);
		    if (colorName == "lightyellow")
			    SetColor(255, 255, 224);
		    if (colorName == "lime")
			    SetColor(0, 255, 0);
		    if (colorName == "limegreen")
			    SetColor( 50, 205, 50);
		    if (colorName == "linen")
			    SetColor(250, 240, 230);
		    if (colorName == "magenta")
			    SetColor(255, 0, 255);
		    if (colorName == "maroon")
			    SetColor(128, 0, 0);
		    if (colorName == "mediumaquamarine")
			    SetColor(102, 205, 170);
		    if (colorName == "mediumblue")
			    SetColor(0, 0, 205);
		    if (colorName == "mediumorchid")
			    SetColor(186, 85, 211);
		    if (colorName == "mediumpurple")
			    SetColor(147, 112, 219);
		    if (colorName == "mediumseagreen")
			    SetColor( 60, 179, 113);
		    if (colorName == "mediumslateblue")
			    SetColor(123, 104, 238);
		    if (colorName == "mediumspringgreen")
			    SetColor(0, 250, 154);
		    if (colorName == "mediumturquoise")
			    SetColor( 72, 209, 204);
		    if (colorName == "mediumvioletred")
			    SetColor(199, 21, 133);
		    if (colorName == "midnightblue")
			    SetColor( 25, 25, 112);
		    if (colorName == "mintcream")
			    SetColor(245, 255, 250);
		    if (colorName == "mistyrose")
			    SetColor(255, 228, 225);
		    if (colorName == "moccasin")
			    SetColor(255, 228, 181);
		    if (colorName == "navajowhite")
			    SetColor(255, 222, 173);
		    if (colorName == "navy")
			    SetColor(0, 0, 128);
		    if (colorName == "oldlace")
			    SetColor(253, 245, 230);
		    if (colorName == "olive")
			    SetColor(128, 128, 0);
		    if (colorName == "olivedrab")
			    SetColor(107, 142, 35);
		    if (colorName == "orange")
			    SetColor(255, 165, 0);
		    if (colorName == "orangered")
			    SetColor(255, 69, 0);
		    if (colorName == "orchid")
			    SetColor(218, 112, 214);
		    if (colorName == "palegoldenrod")
			    SetColor(238, 232, 170);
		    if (colorName == "palegreen")
			    SetColor(152, 251, 152);
		    if (colorName == "paleturquoise")
			    SetColor(175, 238, 238);
		    if (colorName == "palevioletred")
			    SetColor(219, 112, 147);
		    if (colorName == "papayawhip")
			    SetColor(255, 239, 213);
		    if (colorName == "peachpuff")
			    SetColor(255, 218, 185);
		    if (colorName == "peru")
			    SetColor(205, 133, 63);
		    if (colorName == "pink")
			    SetColor(255, 192, 203);
		    if (colorName == "plum")
			    SetColor(221, 160, 221);
		    if (colorName == "powderblue")
			    SetColor(176, 224, 230);
		    if (colorName == "purple")
			    SetColor(128, 0, 128);
		    if (colorName == "red")
			    SetColor(255, 0, 0);
		    if (colorName == "rosybrown")
			    SetColor(188, 143, 143);
		    if (colorName == "royalblue")
			    SetColor( 65, 105, 225);
		    if (colorName == "saddlebrown")
			    SetColor(139, 69, 19);
		    if (colorName == "salmon")
			    SetColor(250, 128, 114);
		    if (colorName == "sandybrown")
			    SetColor(244, 164, 96);
		    if (colorName == "seagreen")
			    SetColor( 46, 139, 87);
		    if (colorName == "seashell")
			    SetColor(255, 245, 238);
		    if (colorName == "sienna")
			    SetColor(160, 82, 45);
		    if (colorName == "silver")
			    SetColor(192, 192, 192);
		    if (colorName == "skyblue")
			    SetColor(135, 206, 235);
		    if (colorName == "slateblue")
			    SetColor(106, 90, 205);
		    if (colorName == "slategray")
			    SetColor(112, 128, 144);
		    if (colorName == "slategrey")
			    SetColor(112, 128, 144);
		    if (colorName == "snow")
			    SetColor(255, 250, 250);
		    if (colorName == "springgreen")
			    SetColor(0, 255, 127);
		    if (colorName == "steelblue")
			    SetColor( 70, 130, 180);
		    if (colorName == "tan")
			    SetColor(210, 180, 140);
		    if (colorName == "teal")
			    SetColor(0, 128, 128);
		    if (colorName == "thistle")
			    SetColor(216, 191, 216);
		    if (colorName == "tomato")
			    SetColor(255, 99, 71);
		    if (colorName == "turquoise")
			    SetColor( 64, 224, 208);
		    if (colorName == "violet")
			    SetColor(238, 130, 238);
		    if (colorName == "wheat")
			    SetColor(245, 222, 179);
		    if (colorName == "white")
			    SetColor(255, 255, 255);
		    if (colorName == "whitesmoke")
			    SetColor(245, 245, 245);
		    if (colorName == "yellow")
			    SetColor(255, 255, 0);
		    if (colorName == "yellowgreen")
			    SetColor(154, 205, 50);

            m_colorName = colorName;

		    return true;
	    }

        std::string GetColorName() const
        {
            return m_colorName;
        }

        int GetPresetColorCount() const
        {
            return 147;
        }

	    bool LoadPresetColorByIndex(int colorIndex)
	    {
		    switch (colorIndex) {
		    case 0: //aliceblue
			    SetColor(240, 248, 255);
			    break;
		    case 1: //antiquewhite
			    SetColor(250, 235, 215);
			    break;
		    case 2: //aqua
			    SetColor(0, 255, 255);
			    break;
		    case 3: //aquamarine
			    SetColor(127, 255, 212);
			    break;
		    case 4: //azure
			    SetColor(240, 255, 255);
			    break;
		    case 5: //beige
			    SetColor(245, 245, 220);
			    break;
		    case 6: //bisque
			    SetColor(255, 228, 196);
			    break;
		    case 7: //black
			    SetColor(0, 0, 0);
			    break;
		    case 8: //blanchedalmond
			    SetColor(255, 235, 205);
			    break;
		    case 9: //blue
			    SetColor(0, 0, 255);
			    break;
		    case 10: //blueviolet
			    SetColor(138, 43, 226);
			    break;
		    case 11: //brown
			    SetColor(165, 42, 42);
			    break;
		    case 12: //burlywood
			    SetColor(222, 184, 135);
			    break;
		    case 13: //cadetblue
			    SetColor( 95, 158, 160);
			    break;
		    case 14: //chartreuse
			    SetColor(127, 255, 0);
			    break;
		    case 15: //chocolate
			    SetColor(210, 105, 30);
			    break;
		    case 16: //coral
			    SetColor(255, 127, 80);
			    break;
		    case 17: //cornflowerblue
			    SetColor(100, 149, 237);
			    break;
		    case 18: //cornsilk
			    SetColor(255, 248, 220);
			    break;
		    case 19: //crimson
			    SetColor(220, 20, 60);
			    break;
		    case 20: //cyan
			    SetColor(0, 255, 255);
			    break;
		    case 21: //darkblue
			    SetColor(0, 0, 139);
			    break;
		    case 22: //darkcyan
			    SetColor(0, 139, 139);
			    break;
		    case 23: //darkgoldenrod
			    SetColor(184, 134, 11);
			    break;
		    case 24: //darkgray
			    SetColor(169, 169, 169);
			    break;
		    case 25: //darkgreen
			    SetColor(0, 100, 0);
			    break;
		    case 26: //darkgrey
			    SetColor(169, 169, 169);
			    break;
		    case 27: //darkkhaki
			    SetColor(189, 183, 107);
			    break;
		    case 28: //darkmagenta
			    SetColor(139, 0, 139);
			    break;
		    case 29: //darkolivegreen
			    SetColor( 85, 107, 47);
			    break;
		    case 30: //darkorange
			    SetColor(255, 140, 0);
			    break;
		    case 31: //darkorchid
			    SetColor(153, 50, 204);
			    break;
		    case 32: //darkred
			    SetColor(139, 0, 0);
			    break;
		    case 33: //darksalmon
			    SetColor(233, 150, 122);
			    break;
		    case 34: //darkseagreen
			    SetColor(143, 188, 143);
			    break;
		    case 35: //darkslateblue
			    SetColor( 72, 61, 139);
			    break;
		    case 36: //darkslategray
			    SetColor( 47, 79, 79);
			    break;
		    case 37: //darkslategrey
			    SetColor( 47, 79, 79);
			    break;
		    case 38: //darkturquoise
			    SetColor(0, 206, 209);
			    break;
		    case 39: //darkviolet
			    SetColor(148, 0, 211);
			    break;
		    case 40: //deeppink
			    SetColor(255, 20, 147);
			    break;
		    case 41: //deepskyblue
			    SetColor(0, 191, 255);
			    break;
		    case 42: //dimgray
			    SetColor(105, 105, 105);
			    break;
		    case 43: //dimgrey
			    SetColor(105, 105, 105);
			    break;
		    case 44: //dodgerblue
			    SetColor( 30, 144, 255);
			    break;
		    case 45: //firebrick
			    SetColor(178, 34, 34);
			    break;
		    case 46: //floralwhite
			    SetColor(255, 250, 240);
			    break;
		    case 47: //forestgreen
			    SetColor( 34, 139, 34);
			    break;
		    case 48: //fuchsia
			    SetColor(255, 0, 255);
			    break;
		    case 49: //gainsboro
			    SetColor(220, 220, 220);
			    break;
		    case 50: //ghostwhite
			    SetColor(248, 248, 255);
			    break;
		    case 51: //gold
			    SetColor(255, 215, 0);
			    break;
		    case 52: //goldenrod
			    SetColor(218, 165, 32);
			    break;
		    case 53: //gray
			    SetColor(128, 128, 128);
			    break;
		    case 54: //grey
			    SetColor(128, 128, 128);
			    break;
		    case 55: //green
			    SetColor(0, 128, 0);
			    break;
		    case 56: //greenyellow
			    SetColor(173, 255, 47);
			    break;
		    case 57: //honeydew
			    SetColor(240, 255, 240);
			    break;
		    case 58: //hotpink
			    SetColor(255, 105, 180);
			    break;
		    case 59: //indianred
			    SetColor(205, 92, 92);
			    break;
		    case 60: //indigo
			    SetColor( 75, 0, 130);
			    break;
		    case 61: //ivory
			    SetColor(255, 255, 240);
			    break;
		    case 62: //khaki
			    SetColor(240, 230, 140);
			    break;
		    case 63: //lavender
			    SetColor(230, 230, 250);
			    break;
		    case 64: //lavenderblush
			    SetColor(255, 240, 245);
			    break;
		    case 65: //lawngreen
			    SetColor(124, 252, 0);
			    break;
		    case 66: //lemonchiffon
			    SetColor(255, 250, 205);
			    break;
		    case 67: //lightblue
			    SetColor(173, 216, 230);
			    break;
		    case 68: //lightcoral
			    SetColor(240, 128, 128);
			    break;
		    case 69: //lightcyan
			    SetColor(224, 255, 255);
			    break;
		    case 70: //lightgoldenrodyellow
			    SetColor(250, 250, 210);
			    break;
		    case 71: //lightgray
			    SetColor(211, 211, 211);
			    break;
		    case 72: //lightgreen
			    SetColor(144, 238, 144);
			    break;
		    case 73: //lightgrey
			    SetColor(211, 211, 211);
			    break;
		    case 74: //lightpink
			    SetColor(255, 182, 193);
			    break;
		    case 75: //lightsalmon
			    SetColor(255, 160, 122);
			    break;
		    case 76: //lightseagreen
			    SetColor( 32, 178, 170);
			    break;
		    case 77: //lightskyblue
			    SetColor(135, 206, 250);
			    break;
		    case 78: //lightslategray
			    SetColor(119, 136, 153);
			    break;
		    case 79: //lightslategrey
			    SetColor(119, 136, 153);
			    break;
		    case 80: //lightsteelblue
			    SetColor(176, 196, 222);
			    break;
		    case 81: //lightyellow
			    SetColor(255, 255, 224);
			    break;
		    case 82: //lime
			    SetColor(0, 255, 0);
			    break;
		    case 83: //limegreen
			    SetColor( 50, 205, 50);
			    break;
		    case 84: //linen
			    SetColor(250, 240, 230);
			    break;
		    case 85: //magenta
			    SetColor(255, 0, 255);
			    break;
		    case 86: //maroon
			    SetColor(128, 0, 0);
			    break;
		    case 87: //mediumaquamarine
			    SetColor(102, 205, 170);
			    break;
		    case 88: //mediumblue
			    SetColor(0, 0, 205);
			    break;
		    case 89: //mediumorchid
			    SetColor(186, 85, 211);
			    break;
		    case 90: //mediumpurple
			    SetColor(147, 112, 219);
			    break;
		    case 91: //mediumseagreen
			    SetColor( 60, 179, 113);
			    break;
		    case 92: //mediumslateblue
			    SetColor(123, 104, 238);
			    break;
		    case 93: //mediumspringgreen
			    SetColor(0, 250, 154);
			    break;
		    case 94: //mediumturquoise
			    SetColor( 72, 209, 204);
			    break;
		    case 95: //mediumvioletred
			    SetColor(199, 21, 133);
			    break;
		    case 96: //midnightblue
			    SetColor( 25, 25, 112);
			    break;
		    case 97: //mintcream
			    SetColor(245, 255, 250);
			    break;
		    case 98: //mistyrose
			    SetColor(255, 228, 225);
			    break;
		    case 99: //moccasin
			    SetColor(255, 228, 181);
			    break;
		    case 100: //navajowhite
			    SetColor(255, 222, 173);
			    break;
		    case 101: //navy
			    SetColor(0, 0, 128);
			    break;
		    case 102: //oldlace
			    SetColor(253, 245, 230);
			    break;
		    case 103: //olive
			    SetColor(128, 128, 0);
			    break;
		    case 104: //olivedrab
			    SetColor(107, 142, 35);
			    break;
		    case 105: //orange
			    SetColor(255, 165, 0);
			    break;
		    case 106: //orangered
			    SetColor(255, 69, 0);
			    break;
		    case 107: //orchid
			    SetColor(218, 112, 214);
			    break;
		    case 108: //palegoldenrod
			    SetColor(238, 232, 170);
			    break;
		    case 109: //palegreen
			    SetColor(152, 251, 152);
			    break;
		    case 110: //paleturquoise
			    SetColor(175, 238, 238);
			    break;
		    case 111: //palevioletred
			    SetColor(219, 112, 147);
			    break;
		    case 112: //papayawhip
			    SetColor(255, 239, 213);
			    break;
		    case 113: //peachpuff
			    SetColor(255, 218, 185);
			    break;
		    case 114: //peru
			    SetColor(205, 133, 63);
			    break;
		    case 115: //pink
			    SetColor(255, 192, 203);
			    break;
		    case 116: //plum
			    SetColor(221, 160, 221);
			    break;
		    case 117: //powderblue
			    SetColor(176, 224, 230);
			    break;
		    case 118: //purple
			    SetColor(128, 0, 128);
			    break;
		    case 119: //red
			    SetColor(255, 0, 0);
			    break;
		    case 120: //rosybrown
			    SetColor(188, 143, 143);
			    break;
		    case 121: //royalblue
			    SetColor( 65, 105, 225);
			    break;
		    case 122: //saddlebrown
			    SetColor(139, 69, 19);
			    break;
		    case 123: //salmon
			    SetColor(250, 128, 114);
			    break;
		    case 124: //sandybrown
			    SetColor(244, 164, 96);
			    break;
		    case 125: //seagreen
			    SetColor( 46, 139, 87);
			    break;
		    case 126: //seashell
			    SetColor(255, 245, 238);
			    break;
		    case 127: //sienna
			    SetColor(160, 82, 45);
			    break;
		    case 128: //silver
			    SetColor(192, 192, 192);
			    break;
		    case 129: //skyblue
			    SetColor(135, 206, 235);
			    break;
		    case 130: //slateblue
			    SetColor(106, 90, 205);
			    break;
		    case 131: //slategray
			    SetColor(112, 128, 144);
			    break;
		    case 132: //slategrey
			    SetColor(112, 128, 144);
			    break;
		    case 133: //snow
			    SetColor(255, 250, 250);
			    break;
		    case 134: //springgreen
			    SetColor(0, 255, 127);
			    break;
		    case 135: //steelblue
			    SetColor( 70, 130, 180);
			    break;
		    case 136: //tan
			    SetColor(210, 180, 140);
			    break;
		    case 137: //teal
			    SetColor(0, 128, 128);
			    break;
		    case 138: //thistle
			    SetColor(216, 191, 216);
			    break;
		    case 139: //tomato
			    SetColor(255, 99, 71);
			    break;
		    case 140: //turquoise
			    SetColor( 64, 224, 208);
			    break;
		    case 141: //violet
			    SetColor(238, 130, 238);
			    break;
		    case 142: //wheat
			    SetColor(245, 222, 179);
			    break;
		    case 143: //white
			    SetColor(255, 255, 255);
			    break;
		    case 144: //whitesmoke
			    SetColor(245, 245, 245);
			    break;
		    case 145: //yellow
			    SetColor(255, 255, 0);
			    break;
		    case 146: //yellowgreen
			    SetColor(154, 205, 50);
			    break;
		    default:
			    return false;
		    }

		    return true;
	    }

        std::string ToSvg() const;

        // TODO: (28-Jan-2016)
        //void FromCieXYZ(const CCieXYZ<T>& xyz);

        // TODO: (28-Jan-2016)
        //void FromCieLAB(const CCieLab<T>& lab);

	private:
		void Create()
		{
			m_rgb[0] = 0;
			m_rgb[1] = 0;
			m_rgb[2] = 0;
            m_colorName = my::Null<std::string>();
		}

		void Copy(const CRGB& rgb)
		{
			m_rgb[0] = rgb.m_rgb[0];
			m_rgb[1] = rgb.m_rgb[1];
			m_rgb[2] = rgb.m_rgb[2];
            m_colorName = rgb.m_colorName;
		}

	protected:
		T m_rgb[3];

        std::string m_colorName;
	};

    // Gamma-corrected sRGB (device dependent).
    template <typename T>
	class CRGBA
        : public CRGB<T>
	{
	public:
		CRGBA()
            : CRGB<T>()
		{
			Create();
		}
	
		CRGBA(const CRGBA& rgba)
            : CRGB<T>(rgba)
        {
			Copy(rgba);
		}
	
		CRGBA(T r, T g, T b, T a) 
            : CRGB<T>(r, g, b)
        {
			m_alpha = a; 
		}

		void operator=(const CRGBA& rgba)
		{
            CRGB<T>::operator=(rgba);

			Copy(rgba);
		}

		T GetA() const
		{
			return m_alpha;
		}

		void SetA(T a)
		{
            m_alpha = a;
		}

	private:
		void Create()
		{
			m_alpha = 0;
		}

		void Copy(const CRGBA& rgba)
		{
			m_alpha = rgba.m_alpha;
		}

	protected:
        T m_alpha;
	};

    // (http://www.ryanjuckett.com/programming/rgb-color-space-conversion/) The XYZ space was designed around being able to describe all colors visible to humans.
    // Commission internationale de l'éclairage (French for International Commission on Illumination and often abbreviated as CIE).
    template <typename T>
	class CCieXYZ
	{
	public:
		CCieXYZ()
		{
			Create();
		}
	
		CCieXYZ(const CCieXYZ& xyz)
		{
			Copy(xyz);
		}
	
		CCieXYZ(T x, T y, T z) 
		{
			m_xyz[0] = x;
			m_xyz[1] = y;
			m_xyz[2] = z;
		}

		CCieXYZ(const CRGB<T>& rgb) 
		{
            FromRGB(rgb);
		}

		void operator=(const CCieXYZ& xyz)
		{
			Copy(xyz);
		}

		T GetX() const
		{
			return m_xyz[0];
		}

		void SetX(T x)
		{
			m_xyz[0] = x;
		}

        // (http://www.ryanjuckett.com/programming/rgb-color-space-conversion/) The Y value of an XYZ color represents the relative luminance of the color as percieved by the human eye (because all eyes are a bit different, this is really an approximation based on experimental data). Colors with higher Y values are perceived brighter and colors with equal Y values are perceived to have the same brightness.
		T GetY() const
		{
			return m_xyz[1];
		}

		void SetY(T y)
		{
			m_xyz[1] = y;
		}

		T GetZ() const
		{
			return m_xyz[2];
		}

		void SetZ(T z)
		{
			m_xyz[2] = z;
		}

        // TRICKY: (28-Jan-2016) FROM GAMMA-CORRECTED SRGB (DEVICE DEPENDENT)
        void FromRGB(const CRGB<T>& rgb);

        // TODO: (28-Jan-2016)
        //void FromCieLAB(const CCieLab<T>& lab);

	private:
		void Create()
		{
			m_xyz[0] = 0;
			m_xyz[1] = 0;
			m_xyz[2] = 0;
		}

		void Copy(const CCieXYZ& xyz)
		{
			m_xyz[0] = xyz.m_xyz[0];
			m_xyz[1] = xyz.m_xyz[1];
			m_xyz[2] = xyz.m_xyz[2];
		}

	protected:
		T m_xyz[3];
	};

    // Commission internationale de l'éclairage (French for International Commission on Illumination and often abbreviated as CIE).
    // This is based directly on CIE XYZ (1931) and is another attempt to linearise the perceptibility of unit vector colour differences. Again, it is non-linear, and the conversions are still reversible. Colouring information is referred to the colour of the white point of the system, subscript n. The non-linear relationships for L* a* and b* are the same as for CIELUV and are intended to mimic the logarithmic  response of the eye. The red/green opponent colors are represented along the a* axis, with green at negative a* values and red at positive a* values. The yellow/blue opponent colors are represented along the b* axis, with blue at negative b* values and yellow at positive b* values.
	template <typename T>
	class CCieLab
	{
	public:
		CCieLab()
		{
			Create();
		}
	
		CCieLab(const CCieLab& lab)
		{
            Create();

			Copy(lab);
		}
	
		CCieLab(T l, T a, T b) 
		{
            Create();

			m_lab[0] = l;
			m_lab[1] = a;
			m_lab[2] = b;
		}

		CCieLab(const CRGB<T>& rgb) 
		{
            Create();

            FromRGB(rgb);
		}

		void operator=(const CCieLab& lab)
		{
			Copy(lab);
		}

		T GetL() const
		{
			return m_lab[0];
		}

		void SetL(T l)
		{
			m_lab[0] = l;
		}

		T GetA() const
		{
			return m_lab[1];
		}

		void SetA(T a)
		{
			m_lab[1] = a;
		}

		T GetB() const
		{
			return m_lab[2];
		}

		void SetB(T b)
		{
			m_lab[2] = b;
		}

		T GetXn() const
		{
			return Xn;
		}

		T GetYn() const
		{
			return Yn;
		}

		T GetZn() const
		{
			return Zn;
		}

		T* GetPointer()
		{
			return m_lab;
		}

        void FromRGB(const CRGB<T>& rgb);

        void FromCieXYZ(const CCieXYZ<T>& xyz);

	private:
		void Create()
		{
            Xn = (T)0.950456;
            Yn = (T)1.0;
            Zn = (T)1.088754;

  			m_lab[0] = 0;
			m_lab[1] = 0;
			m_lab[2] = 0;
		}

		void Copy(const CCieLab& lab)
		{
			m_lab[0] = lab.m_lab[0];
			m_lab[1] = lab.m_lab[1];
			m_lab[2] = lab.m_lab[2];
		}

        T f(T t) const;

	protected:
        T Xn;
        T Yn;
        T Zn;
  
        T m_lab[3];
	};

    template <typename T>
    class CHSV
    {
    public:
        CHSV()
        {
            Create();
        }

        CHSV(const CHSV& hsv)
        {
            Create();

            Copy(hsv);
        }

        CHSV(T h, T s, T v)
        {
            Create();

            m_hsv[0] = h;
            m_hsv[1] = s;
            m_hsv[2] = v;
        }

        CHSV(const CRGB<T>& rgb)
        {
            Create();

            FromRGB(rgb);
        }

        void operator=(const CHSV& hsv)
        {
            Copy(hsv);
        }

        T GetH() const
        {
            return m_hsv[0];
        }

        void SetH(T h)
        {
            m_hsv[0] = h;
        }

        T GetS() const
        {
            return m_hsv[1];
        }

        void SetS(T s)
        {
            m_hsv[1] = s;
        }

        T GetV() const
        {
            return m_hsv[2];
        }

        void SetV(T v)
        {
            m_hsv[2] = v;
        }

        T* GetPointer()
        {
            return m_hsv;
        }

        bool FromRGB(const CRGB<T>& rgb);

    private:
        void Create()
        {
            m_hsv[0] = 0;
            m_hsv[1] = 0;
            m_hsv[2] = 0;
        }

        void Copy(const CHSV& hsv)
        {
            m_hsv[0] = hsv.m_hsv[0];
            m_hsv[1] = hsv.m_hsv[1];
            m_hsv[2] = hsv.m_hsv[2];
        }

    protected:
        T m_hsv[3];
    };
}

#endif // #if !defined(COLOR_INCLUDED)

