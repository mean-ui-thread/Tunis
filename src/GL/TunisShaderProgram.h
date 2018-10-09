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
#ifndef TUNISSHADERPROGRAM_H
#define TUNISSHADERPROGRAM_H

#include <TunisGL.h>

#include <array>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>


namespace tunis
{
    namespace detail
    {

        class Shader
        {
        public:

            Shader(const char * name);
            virtual ~Shader();
            operator GLuint() const;
            bool status() const;
            const char *name() const;

        protected:

            void compile(GLenum type, const char *source, int len);

            const char *shaderName;
            GLuint shaderId = 0;
            GLint compileStatus = GL_FALSE;
        };

        class ShaderVertTexture : public Shader
        {
        public: ShaderVertTexture();
        };


        class ShaderFragTexture : public Shader
        {
        public: ShaderFragTexture();
        };

        class ShaderVertGradientLinear : public Shader
        {
        public: ShaderVertGradientLinear();
        };

        class ShaderFragGradientLinear : public Shader
        {
        public: ShaderFragGradientLinear();
        };

        class ShaderVertGradientRadial : public Shader
        {
        public: ShaderVertGradientRadial();
        };

        class ShaderFragGradientRadial : public Shader
        {
        public: ShaderFragGradientRadial();
        };

        class ShaderProgram
        {
        public:

            ShaderProgram(const Shader &vert, const Shader &frag, const char *name);
            virtual ~ShaderProgram();
            operator GLuint() const;
            bool status() const;
            const char *name() const;
            void useProgram();

            void setViewSizeUniform(int32_t width, int32_t height);

            virtual void enableVertexAttribArray() = 0;
            virtual void disableVertexAttribArray() = 0;

        protected:

            const char* programName;
            GLuint programId = 0;
            GLint linkStatus = GL_FALSE;

        private:

            // uniform locations
            GLint u_viewSize = 0;

            // uniform values
            int32_t viewWidth = 0;
            int32_t viewHeight = 0;

        };

        class ShaderProgramTexture : public ShaderProgram
        {
        public:
            ShaderProgramTexture();

            virtual void enableVertexAttribArray() override;
            virtual void disableVertexAttribArray() override;

        private:

            // attribute locations
            GLint a_position = 0;
            GLint a_texcoord = 0;
            GLint a_texoffset = 0;
            GLint a_texsize = 0;
            GLint a_color = 0;

        };

        union UniformBlock
        {
            struct RadialGradient
            {
                glm::vec2 u_dt;
                glm::vec2 u_focal;
                float u_r0;
                float u_dr;
                float u_a;
                float u_colorStopCount;
                glm::vec4 u_offset;
                glm::vec4 u_color[4];
            } radialGradient;

            struct LinearGradient
            {
                glm::vec2 u_start;
                glm::vec2 u_dt;
                float u_lenSq;
                float UNUSED1;
                float UNUSED2;
                float u_colorStopCount;
                glm::vec4 u_offset;
                glm::vec4 u_color[4];
            } linearGradient;
        };

        class ShaderProgramGradient : public ShaderProgram
        {
        protected:
            ShaderProgramGradient(const Shader &vert, const Shader &frag, const char *name);

        public:


            virtual void enableVertexAttribArray() override;
            virtual void disableVertexAttribArray() override;

            void setUniforms(const UniformBlock &uniforms);

        private:

            // attribute locations
            GLint a_position = 0;

            // uniform lacations
            GLint u_uniforms = 0;
        };

        class ShaderProgramGradientLinear : public ShaderProgramGradient
        {
        public:
            ShaderProgramGradientLinear();
        };

        class ShaderProgramGradientRadial : public ShaderProgramGradient
        {
        public:
            ShaderProgramGradientRadial();
        };
    }

}

#include "TunisShaderProgram.inl"

#endif // TUNISSHADERPROGRAM_H
