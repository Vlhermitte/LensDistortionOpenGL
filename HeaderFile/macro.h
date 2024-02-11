//
// Created by Valentin Lhermitte on 11/02/2024.
//

#ifndef OPENGLPROJECT_MACRO_H
#define OPENGLPROJECT_MACRO_H

#define CHECK_GL_ERROR() {\
    GLenum err = glGetError();\
    while (err != GL_NO_ERROR) {\
        std::string error;\
        switch (err) {\
            case GL_INVALID_OPERATION:\
                error = "INVALID_OPERATION";\
                break;\
            case GL_INVALID_ENUM:\
                error = "INVALID_ENUM";\
                break;\
            case GL_INVALID_VALUE:\
                error = "INVALID_VALUE";\
                break;\
            case GL_OUT_OF_MEMORY:\
                error = "OUT_OF_MEMORY";\
                break;\
            case GL_INVALID_FRAMEBUFFER_OPERATION:\
                error = "INVALID_FRAMEBUFFER_OPERATION";\
                break;\
            default:\
                error = "UNKNOWN";\
                break;\
        }\
        std::cerr << __FILE__ << ":" << __LINE__<< " OpenGL Error: " << error  << std::endl;\
        err = glGetError();\
    }\
}

#endif //OPENGLPROJECT_MACRO_H
