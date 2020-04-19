#ifndef SLIB_SPAINT_H
#define SLIB_SPAINT_H

#include "sobj/sdict.h"
#include "smedia/smedia.h"
#include "smedia/sgraphic.h"

namespace slib {
    namespace smedia {
        
        class SOBJ_DLL SStroke {
        public:
            sushort type;
            float width;
            scolor color;
            intarray interval;
            
        public:
            static sushort toLineStyle(const char *style);
            
        public:
            SStroke();
            SStroke(sushort t, float w = 1.0, const SColor &col = SColor::BLACK);
            SStroke(sobj obj);
            SStroke(const SStroke &stroke);
            ~SStroke();
            
            SStroke &operator=(const SStroke &stroke);
        };
        
        class SOBJ_DLL SBrush {
        public:
			sushort type;
            scolor color;
            
        public:
            static sushort toBrushStyle(const String &style);
            
        public:
            SBrush();
            SBrush(sushort t, const SColor &col = SColor::WHITE);
            SBrush(sobj obj);
            SBrush(const SBrush &brush);
            ~SBrush();
            
            SBrush &operator=(const SColor &col);
            SBrush &operator=(const SBrush &brush);
            
            bool isGradient() const;
			const SGradient &gradient() const;
        };
        
        class SOBJ_DLL SFilter {
        protected:
            sdict _filter;
            
        public:
            SFilter();
            SFilter(sobj obj);
            SFilter(const SFilter &filter);
            ~SFilter();
        };
        
        #define spaint sptr<SPaint>
        class SOBJ_DLL SPaint {
        public:
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
