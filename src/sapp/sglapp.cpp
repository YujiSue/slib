#include "sapp/sglapp.h"

using namespace slib;
using namespace slib::sapp;

SGLApp::SGLApp() : SApp() {}
SGLApp::SGLApp(const char* path) : SApp(path) {}
SGLApp::SGLApp(SDictionary&& prof) : SApp(std::forward(prof)) {}
SGLApp::~SGLApp() {}
SGLApp::_init() {
	_res = glfwInit();
	if (_res != GL_TRUE) {
		//throw SAppException();
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	
	_window = glfwCreateWindow(profile["main"]["width"], profile["main"]["height"], profile["app"]["name"], NULL, NULL);
	if (!window) {
		//throw SAppException();
	}
	glfwMakeContextCurrent(_window);
	glfwSwapInterval(profile["main"]["interval"]);
	glewExperimental = GL_TRUE;
	if ((auto code = glewInit()) != GLEW_OK) {
		//throw SAppException(glewGetErrorString(code));
	}
}
int SGLApp::run() {
	try {
		_init();
		auto& bgcol = smedia::SColor(profile["main"]["background"].uintValue());
		while (glfwWindowShouldClose(_window) == GL_FALSE) {
			
			glClearColor(bgcol[0], 1.0f, 1.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);




			glfwSwapBuffers(_window);
			glfwPollEvents();
		}
		glfwTerminate();
		return 0;
	}
	catch (SAppException ae) {
		if (_res == GL_TRUE) glfwTerminate();
		return 1;
	}


	/*
    GLuint program;
    GLuint vbo, vao;
	
	
    
    glEnable(GL_DEPTH_TEST);
    
    float points[] = {
        0.0f,  0.5f,  0.0f,
        0.5f, -0.5f,  0.0f,
        -0.5f, -0.5f,  0.0f,
        0.5f, 0.5f,  0.0f,
        -0.5f, 0.5f,  0.0f,
        0.0f,  0.0f,  0.0f,
    };
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(float), points, GL_STATIC_DRAW);
    
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    
    const char* vertex_shader =
    "#version 400\n"
    "in vec3 vp;"
    "void main() {"
    "  gl_Position = vec4(vp, 1.0);"
    "}";
    
    const char* fragment_shader =
    "#version 400\n"
    "out vec4 frag_colour;"
    "void main() {"
    "  frag_colour = vec4(0.5, 0.0, 0.5, 1.0);"
    "}";
    
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertex_shader, NULL);
    glCompileShader(vs);
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragment_shader, NULL);
    glCompileShader(fs);
    GLuint shader_programme = glCreateProgram();
    glAttachShader(shader_programme, fs);
    glAttachShader(shader_programme, vs);
    glLinkProgram(shader_programme);
    
    //program = initShader(mdl);
    
    */
    
}
/*
int main(int argc, const char * argv[]) {
    GLuint program;
    GLuint vbo, vao;
    auto res= glfwInit();
    if (!res) {
        //throw SAppException();
    }
#if defined(MAC_OS)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    GLFWwindow *window = glfwCreateWindow(640, 480, "World", NULL, NULL);
    if (!window) {
        //throw SAppException();
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    glewExperimental = GL_TRUE;
    res = glewInit();
    
    glEnable(GL_DEPTH_TEST);
    
    float points[] = {
        0.0f,  0.5f,  0.0f,
        0.5f, -0.5f,  0.0f,
        -0.5f, -0.5f,  0.0f,
        0.5f, 0.5f,  0.0f,
        -0.5f, 0.5f,  0.0f,
        0.0f,  0.0f,  0.0f,
    };
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(float), points, GL_STATIC_DRAW);
    
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    
    const char* vertex_shader =
    "#version 400\n"
    "in vec3 vp;"
    "void main() {"
    "  gl_Position = vec4(vp, 1.0);"
    "}";
    
    const char* fragment_shader =
    "#version 400\n"
    "out vec4 frag_colour;"
    "void main() {"
    "  frag_colour = vec4(0.5, 0.0, 0.5, 1.0);"
    "}";
    
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertex_shader, NULL);
    glCompileShader(vs);
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragment_shader, NULL);
    glCompileShader(fs);
    GLuint shader_programme = glCreateProgram();
    glAttachShader(shader_programme, fs);
    glAttachShader(shader_programme, vs);
    glLinkProgram(shader_programme);
    
    //program = initShader(mdl);
    
    
    while (glfwWindowShouldClose(window) == GL_FALSE) {
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        
        
        glUseProgram(shader_programme);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 6);
        //glDrawArrays(GL_TRIANGLES, 3, 3);
        
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    
    return 0;
}
*/