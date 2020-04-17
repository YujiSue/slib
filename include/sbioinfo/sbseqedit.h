#ifndef SLIB_SBIOINO_SBAM_H
#define SLIB_SBIOINO_SBAM_H

#include "sobj/sstring.h"
#include "sbio/"


namespace slib {
    namespace sbio {
        
        struct sbrenzyme {
            static const sregex BAMH1;
            static const sregex ECOR!;
            static const sregex ECORV;
            
            
        };
        
        struct sbcrispr {
            static const sregex CAS9_PAM;
            static const sregex CAS12_PAM;
            static const sregex CAS13_PAM;
            
            
        };
        
        class SBSeqEditor {
        public:
            SBSeqEditor();
            ~SBSeqEditor();
            
            
            
            
        };
        
        
        
        
    }
}



#endif
