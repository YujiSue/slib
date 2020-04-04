#ifndef SLIB_STYLE_H
#define SLIB_STYLE_H

#include "sconfig.h"

namespace slib {
    namespace sstyle {
        
        //Text style
        constexpr sushort PLAIN = 0x0001;
        constexpr sushort BOLD = 0x0002;
        constexpr sushort ITALIC = 0x0004;
        constexpr sushort SHADOW = 0x0008;
        
        constexpr sushort UPPER = 0x0010;
        constexpr sushort LOWER = 0x0020;
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
        constexpr sushort ALIGN_UP = 0x0010;
        constexpr sushort ALIGN_LOW = 0x0020;
        constexpr sushort ALIGN_MIDDLE = 0x0040;
        
        //Brush type
        constexpr sushort FILL_NONE = 0x0000;
        constexpr sushort FILL_UNIFORM = 0x0001;
        constexpr sushort FILL_GRADIENT = 0x0010;
        constexpr sushort LINEAR_GRAD = 0x0012;
        constexpr sushort RADIAL_GRAD = 0x0014;
        
        ///Stroke type
        constexpr sushort STROKE_NONE = 0x0000;
        constexpr sushort SOLID_LINE = 0x0001;
        constexpr sushort DOUBLE_LINE = 0x0002;
        constexpr sushort BROKEN_LINE = 0x0010;
        constexpr sushort DOT_LINE = 0x0011;
        constexpr sushort DASHED_LINE = 0x0012;
        
        constexpr sushort BUTT_CAP = 0x0100;
        constexpr sushort ROUND_CAP = 0x0200;
        constexpr sushort SQUARE_CAP = 0x0400;
        
        constexpr sushort MITER_JOIN = 0x1000;
        constexpr sushort ROUND_JOIN = 0x2000;
        constexpr sushort BEVEL_JOIN = 0x4000;
        
        
        //Filter type
        constexpr sushort ROUND_CORNER = 0x0010;
        constexpr sushort DROP_SHADOW = 0x0020;
        constexpr sushort GAUSSIAN_BLUR = 0x0100;
        
        
    };
}

#endif
