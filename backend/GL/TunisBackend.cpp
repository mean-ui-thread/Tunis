#include <TunisBackend.h>

#include "TunisGL.h"
#include <cstring>

#include <cassert>

#ifndef TUNIS_VERTEX_MAX
#define TUNIS_VERTEX_MAX 16384
#endif


using namespace tunis;

static const char * shader_default2D_vert =
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

static const char * shader_default2D_frag =
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

};


struct tunis::BackendData
{
    Default2DShader default2DShader;
    GLuint vao = 0;
    GLuint vbo = 0;
    GLuint ibo = 0;
};


Backend::Backend() :
    m_clearColor(glm::vec4(0, 0, 0, 0)),
    m_viewport(glm::ivec4(0, 0, 0, 0)),
    m_texture(),
    m_data(new BackendData())
{
    vertexBuffer.reserve(TUNIS_VERTEX_MAX);

    auto tunisGL_initialized = tunisGLInit();
    assert(KHRONOS_TRUE == tunisGL_initialized);

    m_data->default2DShader.vert = glCreateShader(GL_VERTEX_SHADER);
    m_data->default2DShader.frag = glCreateShader(GL_FRAGMENT_SHADER);
    m_data->default2DShader.program = glCreateProgram();

    GLint vert_len = (GLint)strlen(shader_default2D_vert);
    GLint frag_len = (GLint)strlen(shader_default2D_frag);

    glShaderSource(m_data->default2DShader.vert, 1, &shader_default2D_vert, &vert_len);
    glShaderSource(m_data->default2DShader.frag, 1, &shader_default2D_frag, &frag_len);

    glCompileShader(m_data->default2DShader.vert);
    glCompileShader(m_data->default2DShader.frag);

    glAttachShader(m_data->default2DShader.program, m_data->default2DShader.vert);
    glAttachShader(m_data->default2DShader.program, m_data->default2DShader.frag);

    glLinkProgram(m_data->default2DShader.program);

    // attribute locations
    m_data->default2DShader.a_position = glGetAttribLocation(m_data->default2DShader.program, "a_position");
    m_data->default2DShader.a_texcoord = glGetAttribLocation(m_data->default2DShader.program, "a_texcoord");
    m_data->default2DShader.a_color = glGetAttribLocation(m_data->default2DShader.program, "a_color");

    // uniform locations
    m_data->default2DShader.u_viewSize = glGetUniformLocation(m_data->default2DShader.program, "u_viewSize");
    m_data->default2DShader.u_texture0 = glGetUniformLocation(m_data->default2DShader.program, "u_texture0");

    // activate shader program.
    glUseProgram(m_data->default2DShader.program);
    glUniform1i(m_data->default2DShader.u_texture0, 0);
    glActiveTexture(GL_TEXTURE0);

    // Create a vertex array object
    if (tunisGLSupport(GL_VERSION_3_0) || tunisGLSupport(GL_ES_VERSION_3_0))
    {
        glGenVertexArrays(1, &m_data->vao);
        glBindVertexArray(m_data->vao);
    }

    // Create static index buffer object.
    uint16_t indices[TUNIS_VERTEX_MAX / 4 * 6];
    for (uint16_t i = 0; i < TUNIS_VERTEX_MAX / 4; ++i)
    {
        indices[i * 6 + 0] = i * 4 + 0;
        indices[i * 6 + 1] = i * 4 + 1;
        indices[i * 6 + 2] = i * 4 + 2;
        indices[i * 6 + 3] = i * 4 + 2;
        indices[i * 6 + 4] = i * 4 + 3;
        indices[i * 6 + 5] = i * 4 + 0;
    }
    glGenBuffers(1, &m_data->ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_data->ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices, GL_STATIC_DRAW);

    // Create dynamic vertex buffer object
    glGenBuffers(1, &m_data->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_data->vbo);
    glBufferData(GL_ARRAY_BUFFER, TUNIS_VERTEX_MAX * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(static_cast<GLuint>(m_data->default2DShader.a_position), sizeof(Vertex::pos)    / sizeof(float), GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void *)0);
    glVertexAttribPointer(static_cast<GLuint>(m_data->default2DShader.a_texcoord), sizeof(Vertex::tcoord) / sizeof(float), GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void *)(sizeof(Vertex::pos)));
    glVertexAttribPointer(static_cast<GLuint>(m_data->default2DShader.a_color),    sizeof(Vertex::color)  / sizeof(float), GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void *)(sizeof(Vertex::pos) + sizeof(Vertex::tcoord)));
    glEnableVertexAttribArray(static_cast<GLuint>(m_data->default2DShader.a_position));
    glEnableVertexAttribArray(static_cast<GLuint>(m_data->default2DShader.a_texcoord));
    glEnableVertexAttribArray(static_cast<GLuint>(m_data->default2DShader.a_color));


    /* set default state */
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

Backend::~Backend()
{
    tunisGLShutdown();
}

void Backend::setClearColor(const glm::vec4 &color)
{
    if (m_clearColor != color)
    {
        glClearColor(color.r, color.g, color.b, color.a);
        m_clearColor = color;
    }
}

void Backend::setViewport(const glm::ivec4 &rect)
{
    if (m_viewport != rect)
    {
        glViewport(rect.x, rect.y, rect.z, rect.w);
        glUniform2f(m_data->default2DShader.u_viewSize, (GLfloat)rect.z, (GLfloat)rect.w);
        m_viewport = rect;
    }
}

void Backend::clearFrame()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void Backend::bindTexture(const Texture &tex)
{
    if (m_texture != tex)
    {
        m_texture = tex;
        m_texture.bind();
    }
}

void Backend::reset()
{
    m_clearColor = glm::vec4(0, 0, 0, 0);
    m_viewport = glm::ivec4(0, 0, 0, 0);
    m_texture = Texture();
}

void Backend::flushVertexBuffer()
{
    glBufferSubData(GL_ARRAY_BUFFER, 0,
                    static_cast<GLsizeiptr>(vertexBuffer.size() * sizeof(Vertex)),
                    &vertexBuffer.front());
}

void Backend::render(size_t vertexStartIndex, size_t vertexCount)
{
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(vertexCount/4*6), GL_UNSIGNED_SHORT, (void*)(vertexStartIndex * sizeof(GLushort)));
}


float Backend::getTextelSize() const
{
    static float s_textelWidth = 1.0f / getMaxTextureSize();
    return s_textelWidth;
}

int32_t Backend::getMaxTextureSize() const
{
    static int32_t s_maxTextureSize = 0;
    if (s_maxTextureSize == 0)
    {
        glGetIntegerv(GL_MAX_TEXTURE_SIZE, &s_maxTextureSize);
    }
    return s_maxTextureSize;
}

int32_t Backend::getTexturePadding() const
{
    static int32_t s_texturePadding = 16; // should be enough I hope...
    return s_texturePadding;
}

