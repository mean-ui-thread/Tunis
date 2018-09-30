#ifndef TUNISSHADERPROGRAM_H
#define TUNISSHADERPROGRAM_H

#include <TunisGL.h>
#include <TunisGraphicStates.h>

#include <string>
#include <iostream>

namespace tunis
{
    namespace detail
    {
        extern GraphicStates gfxStates;

        class Shader
        {
        public:

            Shader(const char * name) : shaderName(name)
            {
            }

            virtual ~Shader()
            {
                if (shaderId)
                {
                    glDeleteShader(shaderId);
                    shaderId = 0;
                }
            }

            operator GLuint() const
            {
                return shaderId;
            }

            bool status() const
            {
                return compileStatus == GL_TRUE;
            }

            const char *name() const
            {
                return shaderName;
            }

        protected:

            void compile(GLenum type, const char *source, int len)
            {
                shaderId = glCreateShader(type);
                glShaderSource(shaderId, 1, &source, &len);
                glCompileShader(shaderId);
                glGetShaderiv(shaderId, GL_COMPILE_STATUS, &compileStatus);

                if(compileStatus == GL_FALSE)
                {
                    GLint infoLogLength = 0;
                    glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &infoLogLength);
                    if(infoLogLength > 1)
                    {
                        std::string infoLog;
                        infoLog.resize(infoLogLength);
                        glGetShaderInfoLog(shaderId, infoLogLength, nullptr, const_cast<char*>(infoLog.data()));
                        std::cerr << "Error compiling " << shaderName << ":\n" << infoLog << "\n\n";
                    }
                    glDeleteShader(shaderId);
                    shaderId = 0;
                }
            }

            const char *shaderName;
            GLuint shaderId = 0;
            GLint compileStatus = GL_FALSE;
        };

        class ShaderVertDefault : public Shader
        {
        public:
            ShaderVertDefault() : Shader("ShaderVertDefault")
            {
                const char * source =
                        "#if defined(GL_ES)\n"
                        "precision highp float;\n"
                        "#endif\n"
                        "\n"
                        "uniform vec2 u_viewSize;\n"
                        "\n"
                        "attribute vec2 a_position;\n"
                        "attribute vec2 a_texcoord;\n"
                        "attribute vec4 a_color;\n"
                        "\n"
                        "varying vec2 v_texcoord;\n"
                        "varying vec4 v_color;\n"
                        "\n"
                        "void main()\n"
                        "{\n"
                        "    v_texcoord   = a_texcoord;\n"
                        "    v_color      = a_color;\n"
                        "    gl_Position  = vec4(2.0*a_position.x/u_viewSize.x - 1.0, 1.0 - 2.0*a_position.y/u_viewSize.y, 0, 1);\n"
                        "}\n";

                compile(GL_VERTEX_SHADER, source, strlen(source));
            }
        };

        class ShaderFragDefault : public Shader
        {
        public:
            ShaderFragDefault() : Shader("ShaderFragDefault")
            {
                const char * source =
                        "#if defined(GL_ES)\n"
                        "precision highp float;\n"
                        "#endif\n"
                        "\n"
                        "varying vec2 v_texcoord;\n"
                        "varying vec4 v_color;\n"
                        "\n"
                        "uniform sampler2D u_texture0;\n"
                        "\n"
                        "void main()\n"
                        "{\n"
                        "    gl_FragColor = texture2D(u_texture0, v_texcoord) * v_color;\n"
                        "}\n";

                compile(GL_FRAGMENT_SHADER, source, strlen(source));
            }
        };


        class ShaderProgram
        {
        public:

            ShaderProgram(const Shader &vert, const Shader &frag, const char *name) : programName(name)
            {
                if (!vert.status())
                {
                    std::cerr << "Cannot attach broken " << vert.name() << " to " << programName << "\n";
                    return;
                }

                if (!frag.status())
                {
                    std::cerr << "Cannot attach broken " << frag.name() << " to " << programName << "\n";
                    return;
                }

                programId = glCreateProgram();
                glAttachShader(programId, vert);
                glAttachShader(programId, frag);
                glLinkProgram(programId);
                glGetProgramiv(programId, GL_LINK_STATUS, &linkStatus);

                if(linkStatus == GL_FALSE)
                {
                    GLint infoLogLength = 0;
                    glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &infoLogLength);

                    if(infoLogLength > 1)
                    {
                        std::string infoLog;
                        infoLog.resize(infoLogLength);
                        glGetShaderInfoLog(programId, infoLogLength, nullptr, const_cast<char*>(infoLog.data()));
                        std::cerr << "Error linking " << name << ":\n" << infoLog << "\n\n";
                    }

                    glDeleteProgram(programId);
                    programId = 0;
                    return;
                }

                // attribute locations
                a_position = glGetAttribLocation(programId, "a_position");
                a_texcoord = glGetAttribLocation(programId, "a_texcoord");
                a_color = glGetAttribLocation(programId, "a_color");

                // uniform locations
                u_viewSize = glGetUniformLocation(programId, "u_viewSize");
                u_texture0 = glGetUniformLocation(programId, "u_texture0");

                // activate shader program.
                useProgram();

                // shader's sampler setup
                setTexture0Uniform(0);
            }

            virtual ~ShaderProgram()
            {
                if (gfxStates.programId == programId)
                {
                    glUseProgram(0);
                    gfxStates.programId = 0;
                }

                glDeleteProgram(programId);

                programId = 0;
                a_position = 0;
                a_texcoord = 0;
                a_color = 0;
                u_viewSize = 0;
                u_texture0 = 0;
                viewWidth = 0;
                viewHeight = 0;
                texture0 = 0;
            }

            operator GLuint() const
            {
                return programId;
            }

            bool status() const
            {
                return linkStatus == GL_TRUE;
            }

            const char *name() const
            {
                return programName;
            }

            void useProgram()
            {
                if (gfxStates.programId != programId)
                {
                    glUseProgram(programId);
                    gfxStates.programId = programId;
                }
            }

            void setTexture0Uniform(int32_t value)
            {
                assert(value >= 0 && value < 32);
                assert(gfxStates.programId == programId);

                if (texture0 != value)
                {
                    glActiveTexture(GL_TEXTURE0 + static_cast<GLenum>(value));
                    glUniform1i(u_texture0, value);
                    texture0 = value;
                }
            }

            void setViewSizeUniform(int32_t width, int32_t height)
            {
                assert(gfxStates.programId == programId);

                if (viewWidth != width || viewHeight != height)
                {
                    glUniform2f(u_viewSize,
                                static_cast<GLfloat>(width),
                                static_cast<GLfloat>(height));

                    viewWidth = width;
                    viewHeight = height;
                }
            }

            // mandatory attribute locations
            GLint a_position = 0;
            GLint a_texcoord = 0;
            GLint a_color = 0;

            // mandatory uniform locations
            GLint u_viewSize = 0;
            GLint u_texture0 = 0;

        protected:

            const char* programName;
            GLuint programId = 0;
            GLint linkStatus = GL_FALSE;

            // uniform values
            int32_t viewWidth = 0;
            int32_t viewHeight = 0;
            int32_t texture0 = 0;

        };
    }

}


#endif // TUNISSHADERPROGRAM_H
