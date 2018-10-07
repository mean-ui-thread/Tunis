/**
 * MIT License
 *
 * Copyright (c) 2018 Matt Chiasson
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 **/
#include "TunisShaderProgram.h"

#include <TunisGraphicStates.h>
#include <TunisVertex.h>

#include <string>
#include <iostream>

namespace tunis
{
    namespace detail
    {
        inline Shader::Shader(const char * name) : shaderName(name)
        {
        }

        inline Shader::~Shader()
        {
            if (shaderId)
            {
                glDeleteShader(shaderId);
                shaderId = 0;
            }
        }

        inline Shader::operator GLuint() const
        {
            return shaderId;
        }

        inline bool Shader::status() const
        {
            return compileStatus == GL_TRUE;
        }

        inline const char *Shader::name() const
        {
            return shaderName;
        }

        inline void Shader::compile(GLenum type, const char *source, int len)
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
                    infoLog.resize(static_cast<size_t>(infoLogLength));
                    glGetShaderInfoLog(shaderId, infoLogLength, nullptr, const_cast<char*>(infoLog.data()));
                    std::cerr << "Error compiling " << shaderName << ":\n" << infoLog << "\n\n";
                }
                glDeleteShader(shaderId);
                shaderId = 0;
            }
        }

        inline ShaderVertTexture::ShaderVertTexture() : Shader("ShaderVertTexture")
        {
            const char * source =
                #include "GL/texture.vert"
                    ;

            compile(GL_VERTEX_SHADER, source, static_cast<int>(strlen(source)));
        }


        inline ShaderFragTexture::ShaderFragTexture() : Shader("ShaderFragTexture")
        {
            const char * source =
                #include "GL/texture.frag"
                    ;

            compile(GL_FRAGMENT_SHADER, source, static_cast<int>(strlen(source)));
        }

        inline ShaderVertGradientLinear::ShaderVertGradientLinear() : Shader("ShaderVertGradientLinear")
        {
            const char * source =
                #include "GL/gradientLinear.vert"
                    ;

            compile(GL_VERTEX_SHADER, source, static_cast<int>(strlen(source)));
        }

        inline ShaderFragGradientLinear::ShaderFragGradientLinear() : Shader("ShaderFragGradientLinear")
        {
            const char * source =
                #include "GL/gradientLinear.frag"
                    ;

            compile(GL_FRAGMENT_SHADER, source, static_cast<int>(strlen(source)));
        }


        inline ShaderVertGradientRadial::ShaderVertGradientRadial() : Shader("ShaderVertGradientRadial")
        {
            const char * source =
                #include "GL/gradientRadial.vert"
                    ;

            compile(GL_VERTEX_SHADER, source, static_cast<int>(strlen(source)));
        }

        inline ShaderFragGradientRadial::ShaderFragGradientRadial() : Shader("ShaderFragGradientRadial")
        {
            const char * source =
                #include "GL/gradientRadial.frag"
                    ;

            compile(GL_FRAGMENT_SHADER, source, static_cast<int>(strlen(source)));
        }


        /**
         * ShaderProgram (Base)
         */

        inline ShaderProgram::ShaderProgram(const Shader &vert, const Shader &frag, const char *name) : programName(name)
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
            }

            // uniform locations
            u_viewSize = glGetUniformLocation(programId, "u_viewSize");
            assert(u_viewSize != -1);

        }

        inline ShaderProgram::~ShaderProgram()
        {
            if (gfxStates.programId == programId)
            {
                glUseProgram(0);
                gfxStates.programId = 0;
            }

            glDeleteProgram(programId);

            programId = 0;
        }

        inline ShaderProgram::operator GLuint() const
        {
            return programId;
        }

        inline bool ShaderProgram::status() const
        {
            return linkStatus == GL_TRUE;
        }

        inline const char *ShaderProgram::name() const
        {
            return programName;
        }

        inline void ShaderProgram::useProgram()
        {
            if (gfxStates.programId != programId)
            {
                glUseProgram(programId);
                gfxStates.programId = programId;
                enableVertexAttribArray();
            }
        }

        inline void ShaderProgram::setViewSizeUniform(int32_t width, int32_t height)
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


        /**
         * ShaderProgramTexture
         */


        inline ShaderProgramTexture::ShaderProgramTexture() :
            ShaderProgram(ShaderVertTexture(), ShaderFragTexture(), "ShaderProgramTexture")
        {
            // attribute locations
            a_position = glGetAttribLocation(programId, "a_position");
            a_texcoord = glGetAttribLocation(programId, "a_texcoord");
            a_color = glGetAttribLocation(programId, "a_color");

            assert(a_position != -1);
            assert(a_texcoord != -1);
            assert(a_color != -1);
        }

        inline void ShaderProgramTexture::enableVertexAttribArray()
        {
            glVertexAttribPointer(static_cast<GLuint>(a_position), decltype(VertexTexture::pos)::length(),    GL_FLOAT,          GL_FALSE, sizeof(VertexTexture), reinterpret_cast<const void *>(0));
            glVertexAttribPointer(static_cast<GLuint>(a_texcoord), decltype(VertexTexture::tcoord)::length(), GL_UNSIGNED_SHORT, GL_TRUE,  sizeof(VertexTexture), reinterpret_cast<const void *>(sizeof(VertexTexture::pos)));
            glVertexAttribPointer(static_cast<GLuint>(a_color),    decltype(VertexTexture::color)::length(),  GL_UNSIGNED_BYTE,  GL_TRUE,  sizeof(VertexTexture), reinterpret_cast<const void *>(sizeof(VertexTexture::pos) + sizeof(VertexTexture::tcoord)));
            glEnableVertexAttribArray(static_cast<GLuint>(a_position));
            glEnableVertexAttribArray(static_cast<GLuint>(a_texcoord));
            glEnableVertexAttribArray(static_cast<GLuint>(a_color));
        }

        inline void ShaderProgramTexture::disableVertexAttribArray()
        {
            glDisableVertexAttribArray(static_cast<GLuint>(a_position));
            glDisableVertexAttribArray(static_cast<GLuint>(a_texcoord));
            glDisableVertexAttribArray(static_cast<GLuint>(a_color));
        }


        /**
         * ShaderProgramGradient
         */

        inline ShaderProgramGradient::ShaderProgramGradient(const Shader &vert,
                                                            const Shader &frag,
                                                            const char *name) :
            ShaderProgram(vert, frag, name)
        {

            // attribute locations
            a_position = glGetAttribLocation(programId, "a_position");
            assert(a_position != -1);


            // uniform locations
            u_uniforms = glGetUniformLocation(programId, "u_uniforms");
            assert(u_uniforms != -1);
        }


        inline void ShaderProgramGradient::enableVertexAttribArray()
        {
            glVertexAttribPointer(static_cast<GLuint>(a_position), decltype(VertexGradient::pos)::length(), GL_FLOAT, GL_FALSE, sizeof(VertexGradient), reinterpret_cast<const void *>(0));
            glEnableVertexAttribArray(static_cast<GLuint>(a_position));
        }

        inline void ShaderProgramGradient::disableVertexAttribArray()
        {
            glDisableVertexAttribArray(static_cast<GLuint>(a_position));
        }

        inline void ShaderProgramGradient::setUniforms(const UniformBlock &f)
        {
            assert(gfxStates.programId == programId);
            glUniform4fv(u_uniforms, static_cast<GLsizei>(sizeof(UniformBlock)/sizeof(glm::vec4)), reinterpret_cast<const GLfloat*>(&f));
        }

        inline  ShaderProgramGradientLinear::ShaderProgramGradientLinear() :
            ShaderProgramGradient(ShaderVertGradientLinear(),
                                  ShaderFragGradientLinear(),
                                  "ShaderProgramGradientLinear")
        {
        }

        inline  ShaderProgramGradientRadial::ShaderProgramGradientRadial() :
            ShaderProgramGradient(ShaderVertGradientRadial(),
                                  ShaderFragGradientRadial(),
                                  "ShaderProgramGradientRadial")
        {
        }
    }

}
