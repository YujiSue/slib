#ifndef SAPP_SGLAPP_H
#define SAPP_SGLAPP_H

#include "glew/glew.h"
#include "glfw/glfw3.h"
#include "sapp/sapp.h"

namespace slib {
    namespace sapp {
        
        class SGLApp : public SApp {
		protected:
			GLFWwindow* _window;
			sint _res;

		private:
			_init();

        public:
            SGLApp();
            SGLApp(const char *path);
            SGLApp(SDictionary &&prof);
            ~SGLApp();
            
            int run();
			GLFWwindow* mainWindow();
        };
    }
}



#endif
