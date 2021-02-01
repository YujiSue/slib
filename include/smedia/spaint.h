#ifndef SLIB_SPAINT_H
#define SLIB_SPAINT_H

#include "sobj/sdict.h"
#include "smedia/smedia.h"
#include "smedia/sgraphic.h"

namespace slib {
    namespace smedia {
        
		class SLIB_DLL SDot {
		public:
			sushort shape;
			float size;


		};

        class SLIB_DLL SStroke {
        public:
            sushort type;
            float width;
            scolor color;
            intarray interval;
            
        public:
            static sushort toLineStyle(const char *style);
            
        public:
            SStroke();
            SStroke(sushort t, float w = 1.0, const SColor &col = color::BLACK);
            SStroke(sobj obj);
            SStroke(const SStroke &stroke);
            ~SStroke();
            
            SStroke &operator=(const SStroke &stroke);
        };
        
        class SLIB_DLL SBrush {
        public:
			sushort type;
			scolor color;
            
        public:
            static sushort toBrushStyle(const String &style);
            
        public:
            SBrush();
            SBrush(sushort t, const SColor &col = color::WHITE);
            SBrush(const SColor& col);
            SBrush(const SBrush &brush);
			SBrush(const sobj& obj);
			~SBrush();
            
            SBrush &operator=(const SBrush &brush);
            
            bool isGradient() const;
			SGradient& gradient();
			const SGradient& gradient() const;
        };
        
        class SLIB_DLL SFilter {
        protected:
            sdict _filter;
            
        public:
            SFilter();
            SFilter(sobj obj);
            SFilter(const SFilter &filter);
            ~SFilter();
        };

		class SLIB_DLL SPaint {
        public:
			SDot dot;
            SStroke stroke;
            SBrush brush;
            SFilter filter;
            
        public:
            SPaint();
            SPaint(const SStroke &s);
            SPaint(const SBrush &b);
            SPaint(const SDictionary &dic);
            SPaint(const SPaint &p);
            ~SPaint();
        };
    }
}

#endif
