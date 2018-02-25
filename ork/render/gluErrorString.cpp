#include <string>
#include <GL/glu.h>

const GLubyte* gluErrorString( GLenum error )
{
    switch (error)
    {
        case GL_NO_ERROR:
            return (const GLubyte*) std::string("GL_NO_ERROR").c_str();
        case GL_INVALID_ENUM:
            return (const GLubyte*) std::string("GL_INVALID_ENUM").c_str();
        case GL_INVALID_VALUE:
            return (const GLubyte*) std::string("GL_INVALID_VALUE").c_str();
        case GL_INVALID_OPERATION:
            return (const GLubyte*) std::string("GL_INVALID_OPERATION").c_str();
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            return (const GLubyte*) std::string("GL_INVALID_FRAMEBUFFER_OPERATION").c_str();
        case GL_OUT_OF_MEMORY:
            return (const GLubyte*) std::string("GL_OUT_OF_MEMORY").c_str();
        case GLU_INVALID_ENUM:
            return (const GLubyte*) std::string("GLU_INVALID_ENUM").c_str();
        case GLU_INVALID_VALUE:
            return (const GLubyte*) std::string("GLU_INVALID_VALUE").c_str();
        case GLU_OUT_OF_MEMORY:
            return (const GLubyte*) std::string("GLU_OUT_OF_MEMORY").c_str();
    }
    return (const GLubyte*) std::string("").c_str();
}
