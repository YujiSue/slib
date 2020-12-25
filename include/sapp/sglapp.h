#ifndef SAPP_SGLAPP_H
#define SAPP_SGLAPP_H

#include "glew/glew.h"
#include "glfw/glfw3.h"
#include "sapp/sappbasic.h"

#include "opencv2/opencv.hpp"
#include "opencv2/core.hpp"

namespace slib {
    namespace sapp {

		struct stexture {
			smedia::simg img;
			v3fvec uv;

			stexture() {}
			~stexture() {}
		};

		struct SMaterial {
			SMaterial() {}
			~SMaterial() {}
		};
		struct SSkin {
			smedia::SImage img;
			v2fvec uv;

			SSkin() {}
			~SSkin() {}

		};

		class SModel {
		protected:
			v3fvec _vertex;
			SMaterial _material;
			SSkin _skin;

		public:
			SModel() {}
			virtual ~SModel() {}

			SSkin& skin() { return _skin; }
			size_t count() { return _vertex.size(); }
			float* vptr() { return reinterpret_cast<float*>(&_vertex[0]); }
		};
		class SPlain : public SModel {

		public:
			SPlain() {}
			SPlain(float x, float y, float w, float h) {
				_vertex.add(v3f(x, y, 0.f));
				_vertex.add(v3f(x + w, y, 0.f));
				_vertex.add(v3f(x + w, y + h, 0.f));
				_vertex.add(v3f(x, y + h, 0.f));
			}
			~SPlain() {}
		};

		//class SWorld {};

#define GLObjArray SVector<GLuint, CMemory<GLuint>> 

		extern inline void setGLOpt(SDictionary&& dic) {
			if (dic.hasKey("depth")) {
				glEnable(GL_DEPTH_TEST);
				glDepthFunc(dic["depth"].intValue());
			}
			if (dic.hasKey("blend")) {
				glEnable(GL_BLEND);
				suint tmp = dic["depth"].uinteger();
				glBlendFunc((tmp>>32)&0xFFFF, tmp&0xFFFF);
			}
		}
		extern inline void initGLFW() {
			auto res = glfwInit();
			if (res != GL_TRUE) throw SAppException(ERR_INFO);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
			//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
			//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		}
		extern inline GLFWwindow* initWindow(sint w, sint h, const char *t = nullptr, sint i = 10) {
			GLFWwindow* window;
			window = glfwCreateWindow(w, h, (t == nullptr ? "Window" : t), NULL, NULL);
			if (!window) throw SAppException(ERR_INFO);
			glfwMakeContextCurrent(window);
			glfwSwapInterval(i);
			return window;
		}
		extern inline void initGLEW(bool exp = true) {
			glewExperimental = exp?GL_TRUE:GL_FALSE;
			auto res = glewInit();
			if (res != GLEW_OK) throw SAppException(ERR_INFO, 0, nullptr, (const char*)glewGetErrorString(res));
		}
		extern inline String vertexShader(sint ver, bool texture = false) {
			String str = "#version ";
			str << ver << LF;
			str << "layout(location=0) in vec3 position;";
			if (texture) str << "layout(location=1) in vec2 uvcoords;";
			stringarray keys;
			if (texture) str << "out vec2 fraguv;";
			str << "void main(void) {";
			if (texture) str << "fraguv = uvcoords;";
			str << "gl_Position =vec4(position, 1.0);";
			str << "}";
			return str;
		}
		extern inline String fragmentShader(sint ver, bool texture = false, v4f col = { 1.0, 1.0, 1.0, 1.0 }) {
			String str = "#version ";
			str << ver << LF;
			if (texture) {
				str << "in vec2 fraguv;";
				str << "uniform sampler2D texture;";
			}
			str << "out vec4 frag_color;";
			str << "void main(void) {";
			if (texture) str << "frag_color = texture2D(texture, fraguv);";
			else str << "frag_color = vec4(" << col.x << "," << col.y << "," << col.z << "," << col.w << ");";
			str << "}";
			return str;
		}
		extern inline GLuint shaderProgram(const char *vshader, const char *fshader) {
			GLuint program;
			GLuint vs = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(vs, 1, &vshader, NULL);
			glCompileShader(vs);
			GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(fs, 1, &fshader, NULL);
			glCompileShader(fs);
			program = glCreateProgram();
			glAttachShader(program, fs);
			glAttachShader(program, vs);
			glLinkProgram(program);
			return program;
		}
		extern inline void makeObjects(GLuint&vao, GLuint& vbo, GLuint& uvo, SModel &model) {
			glGenVertexArrays(1, &vao);
			glBindVertexArray(vao);
			
			glGenBuffers(1, &vbo);
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferData(GL_ARRAY_BUFFER, model.count() * 3 * sizeof(float), model.vptr(), GL_STATIC_DRAW);

			glGenBuffers(1, &uvo);
			glBindBuffer(GL_ARRAY_BUFFER, uvo);
			glBufferData(GL_ARRAY_BUFFER, model.skin().uv.size() * 2 * sizeof(float), model.skin().uv.ptr(), GL_STATIC_DRAW);
		}
		extern inline void makeTextures(GLuint& tex, GLuint& loc, GLint& program) {
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
			glGenTextures(1, &tex);
			loc = glGetUniformLocation(program, "texture");
		}
		extern inline void terminateGL(GLuint &tex) {
			glDeleteTextures(1, &tex);
			glfwTerminate();
		}
		extern inline void displayGL(smedia::SImage& image) {
			initGLFW();
			GLFWwindow* window = initWindow(1280, 720, "Image");
			initGLEW();
			setGLOpt({
					kv("depth", GL_LESS),
					kv("blend", (GL_SRC_ALPHA << 32) | GL_ONE_MINUS_SRC_ALPHA)
				});
			SPlain frame(-1.0f, -1.0f, 2.0f, 2.0f);
			frame.skin().img = image;
			frame.skin().uv = { v2f(0.0f, 1.0f), v2f(1.0f, 1.0f), v2f(1.0f, 0.0f), v2f(0.0f, 0.0f) };
			auto vs = vertexShader(400, true), fs = fragmentShader(400, true);
			GLint program = shaderProgram(vs, fs);
			GLuint vao, location;
			GLuint vbo, uvo, tex;
			makeObjects(vao, vbo, uvo, frame);
			makeTextures(tex, location, program);
			
			while (!glfwWindowShouldClose(window)) {
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				
				glBindTexture(GL_TEXTURE_2D, tex);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, frame.skin().img.width(), frame.skin().img.height(), 0, GL_RGB, GL_UNSIGNED_BYTE, frame.skin().img.ptr());
				
				glUseProgram(program);
				glEnableVertexAttribArray(0);
				glBindBuffer(GL_ARRAY_BUFFER, vbo);
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
				glEnableVertexAttribArray(1);
				glBindBuffer(GL_ARRAY_BUFFER, uvo);
				glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
				glUniform1i(location, 0);
				glBindTexture(GL_TEXTURE_2D, tex);
				glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
				glDisableVertexAttribArray(0);
				glDisableVertexAttribArray(1);

				glfwPollEvents();
				glfwSwapBuffers(window);
			}
			terminateGL(tex);
		}
		extern inline void displayGL(smedia::SCanvas& cnvs) {
			initGLFW();
			GLFWwindow* window = initWindow(cnvs.width(), cnvs.height(), "Image");
			initGLEW();
			setGLOpt({
					kv("depth", GL_LESS),
					kv("blend", (GL_SRC_ALPHA << 32) | GL_ONE_MINUS_SRC_ALPHA)
				});



		}
		//extern void displayGL(SWorld& world) {}
        
        class SAPP_DLL SGLApp : public SApp {
		protected:
			GLFWwindow* _window;
			sint _res;

		private:
			void _init();

        public:
			GLuint vbo, vao;
			GLuint program;


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
