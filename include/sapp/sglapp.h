#ifndef SAPP_SGLAPP_H
#define SAPP_SGLAPP_H

#include "glew/glew.h"
#include "glfw/glfw3.h"
#include "sapp/sapp.h"

namespace slib {
    namespace sapp {
        
        class SGLApp : public SApp {
            
        public:
            SGLApp();
            SGLApp(const char *path);
            SGLApp(SDictionary &&prof);
            ~SGLApp();
            
            int run(int argc, const char **argv);
            int init(int argc, const char **argv);
            int exec();
        };
    }
}



#endif
