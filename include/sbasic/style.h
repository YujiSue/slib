#ifndef SLIB_STYLE_H
#define SLIB_STYLE_H
#include "sconfig.h"
namespace slib {
    /**
    * @namespace sstyle
    * ~english @brief Style definition constants.
    */
    namespace sstyle {
		//Date style
        const char ISO8601[21] = "YYYY-MM-DDTHH:mm:ssL";
        const char YMD[13] = "YYYY/MM/DD";
		const char YMD_J[13] = u8"号年月日";
		const char HMS[10] = "HH:mm:ss";
		const char HMS_J[10] = u8"時分秒";
		const char YMDHMS[20] = "YYYY/MM/DD HH:mm:ss";
		const char YMDHMS_J[25] = u8"号年月日　時分秒";
		const char EMAIL[32] = "WWW, DD MMM YYYY HH:mm:ss L (Z)";

        constexpr sushort NONE = 0x0000;

        //Text style
        constexpr sushort PLAIN = 0x0001;
        constexpr sushort BOLD = 0x0002;
        constexpr sushort ITALIC = 0x0004;
        constexpr sushort SHADOW = 0x0008;
        
        constexpr sushort SUPERSCRIPT = 0x0010;
        constexpr sushort SUBSCRIPT = 0x0020;
        constexpr sushort RUBY = 0x0040;
        constexpr sushort COMMENT = 0x0080;
        
        constexpr sushort UNDERLINE = 0x0100;
        constexpr sushort DEPRECATE = 0x0200;
        constexpr sushort OVERLINE = 0x0400;
        constexpr sushort LINK = 0x0800;
        
        constexpr sushort MATH = 0x1000;
        constexpr sushort REFERENCE = 0x2000;
        //constexpr sushort ANIME = 0x4000;
        
        constexpr sushort ALIGN_LEFT = 0x0001;
        constexpr sushort ALIGN_CENTER = 0x0002;
        constexpr sushort ALIGN_RIGHT = 0x0004;
        constexpr sushort ALIGN_TOP = 0x0010;
        constexpr sushort ALIGN_BOTTOM = 0x0020;
        constexpr sushort ALIGN_MIDDLE = 0x0040;
        
        //Brush type
        constexpr sushort FILL_NONE = 0x0000;
        constexpr sushort FILL_UNIFORM = 0x0001;
        constexpr sushort FILL_GRADIENT = 0x0010;
        constexpr sushort LINEAR_GRAD = 0x0010;
        constexpr sushort RADIAL_GRAD = 0x0020;

        ///Stroke type
        //// Line style
        constexpr sushort NO_STROKE = 0x0000;
        constexpr sushort SOLID_LINE = 0x0001;
        constexpr sushort DOUBLE_LINE = 0x0002;
        constexpr sushort BROKEN_LINE = 0x0004;
        
        //// Cap style
        constexpr sushort BUTT_CAP = 0x0100;
        constexpr sushort ROUND_CAP = 0x0200;
        constexpr sushort SQUARE_CAP = 0x0400;
        
        //// Line join
        constexpr sushort MITER_JOIN = 0x1000;
        constexpr sushort ROUND_JOIN = 0x2000;
        constexpr sushort BEVEL_JOIN = 0x4000;
        
        
        //Filter type
        constexpr sushort ROUND_CORNER = 0x0010;
        constexpr sushort DROP_SHADOW = 0x0020;
        constexpr sushort GAUSSIAN_BLUR = 0x0100;
        
    };
    /**
    * @namespace sstyle
    * ~english @brief Constants to edit text style on the terminal display.
    */
    namespace stext {
        constexpr char PLAIN_TAG[5] = { (char)0x1b, '[','0','m' };
        constexpr char BOLD_TAG[5] = { (char)0x1b, '[','1','m' };
        constexpr char ITALIC_TAG[5] = { (char)0x1b, '[','3','m' };
        constexpr char UNDERLINE_TAG[5] = { (char)0x1b, '[','4','m' };
        
        constexpr char BLACK_TAG[6] = { (char)0x1b, '[','3', '0', 'm'};
        constexpr char RED_TAG[6] = { (char)0x1b, '[','3', '1', 'm' };
        constexpr char GREEN_TAG[6] = { (char)0x1b, '[','3', '2', 'm' };
        constexpr char YELLOW_TAG[6] = { (char)0x1b, '[','3', '3', 'm' };
        constexpr char BLUE_TAG[6] = { (char)0x1b, '[','3', '4', 'm' };
        constexpr char MAGENTA_TAG[6] = { (char)0x1b, '[','3', '5', 'm' };
        constexpr char CYAN_TAG[6] = { (char)0x1b, '[','3', '6', 'm' };
        constexpr char WHITE_TAG[6] = { (char)0x1b, '[','3', '7', 'm' };
        constexpr char DEFAULT_COLOR[6] = { (char)0x1b, '[','3', '9', 'm' };

        constexpr char BLACK_BG_TAG[6] = { (char)0x1b, '[','4', '0', 'm' };
        constexpr char RED_BG_TAG[6] = { (char)0x1b, '[','4', '1', 'm' };
        constexpr char GREEN_BG_TAG[6] = { (char)0x1b, '[','4', '2', 'm' };
        constexpr char YELLOW_BG_TAG[6] = { (char)0x1b, '[','4', '3', 'm' };
        constexpr char BLUE_BG_TAG[6] = { (char)0x1b, '[','4', '4', 'm' };
        constexpr char MAGENTA_BG_TAG[6] = { (char)0x1b, '[','4', '5', 'm' };
        constexpr char CYAN_BG_TAG[6] = { (char)0x1b, '[','4', '6', 'm' };
        constexpr char WHITE_BG_TAG[6] = { (char)0x1b, '[','4', '7', 'm' };
        constexpr char DEFAULT_BG_TAG[6] = { (char)0x1b, '[','4', '9', 'm' };
    }
}

#endif
