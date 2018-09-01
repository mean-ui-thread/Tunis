#ifndef TUNISCONTEXTDATA_H
#define TUNISCONTEXTDATA_H

#include <TunisPath2D.h>
#include <TunisColor.h>
#include <TunisTypes.h>
#include <TunisVertex.h>
#include <TunisTexture.h>
#include <TunisGL.h>

#ifndef TUNIS_VERTEX_MAX
#define TUNIS_VERTEX_MAX 16384
#endif

//#ifndef TUNIS_MAX_TEXTURE_SIZE
#define TUNIS_MAX_TEXTURE_SIZE 2048
//#endif

#ifndef TUNIS_TESS_TOLERANCE
#define TUNIS_TESS_TOLERANCE 0.5
#endif

#ifndef TUNIS_USE_IBO
#define TUNIS_USE_IBO 0
#endif

namespace tunis
{

class Context;

namespace detail
{

using GLUfuncptr = GLvoid (KHRONOS_APIENTRY *) ();

struct GlobalContextData
{
    Color backgroundColor = color::Transparent;
    Viewport viewport = Viewport(0, 0, 100, 100);
    GLuint textureId = 0;
    GLuint programId = 0;
    int32_t maxTexSize = 0;
};

extern GlobalContextData global;

struct Default2DShader
{
    GLuint vert = 0;
    GLuint frag = 0;
    GLuint program = 0;

    /* attribute locations */
    GLint a_position = 0;
    GLint a_texcoord = 0;
    GLint a_color = 0;

    /* uniform locations */
    GLint u_viewSize = 0;
    GLint u_texture0 = 0;

    constexpr static const char * vertSrc =
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
            "    v_texcoord  = a_texcoord;\n"
            "    v_color     = a_color;\n"
            "    gl_Position = vec4(2.0*a_position.x/u_viewSize.x - 1.0, 1.0 - 2.0*a_position.y/u_viewSize.y, 0, 1);\n"
            "}\n";

    constexpr static const char * fragSrc =
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
};

struct BatchArray : public SoA<GLuint, Texture, GLint, GLsizei>
{
    inline GLuint &program(size_t i) { return get<0>(i); }
    inline Texture &texture(size_t i) { return get<1>(i); }
    inline GLint &vertexStartOffset(size_t i) { return get<2>(i); }
    inline GLsizei &vertexCount(size_t i) { return get<3>(i); }
};

struct ContextData
{
    Path2D currentPath;

    std::vector<Texture> textures;

    Default2DShader default2DShader;
    GLuint vao = 0;
    GLuint vbo = 0;
#if TUNIS_USE_IBO
    GLuint ibo = 0;
#endif

    std::vector<Vertex> vertexBuffer; // write-only interleaved VBO data. DO NOT USE SoA on this member!!!

    BatchArray batches;
};

}
}

#endif // TUNISCONTEXTDATA_H
