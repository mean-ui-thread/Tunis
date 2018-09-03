#ifndef TUNISCONTEXTDATA_H
#define TUNISCONTEXTDATA_H

#ifdef _WIN32
#define NOMINMAX 1
#endif

#include <TunisPath2D.h>
#include <TunisPaint.h>
#include <TunisTypes.h>
#include <TunisVertex.h>
#include <TunisTexture.h>
#include <TunisGL.h>

#include <easy/profiler.h>

#include <glm/gtc/epsilon.hpp>

#ifndef TUNIS_VERTEX_MAX
#define TUNIS_VERTEX_MAX 16384
#endif

//#ifndef TUNIS_MAX_TEXTURE_SIZE
#define TUNIS_MAX_TEXTURE_SIZE 2048
//#endif

namespace tunis
{

class Context;

namespace detail
{

struct ShaderProgram;

struct GraphicStates
{
    Color backgroundColor = Transparent;
    Viewport viewport = Viewport(0, 0, 100, 100);
    GLuint textureId = 0;
    GLuint program = 0;
    int32_t maxTexSize = 0;
};
extern GraphicStates gfxStates;

struct ShaderProgram
{
    GLuint vert = 0;
    GLuint frag = 0;
    GLuint program = 0;

    // attribute locations
    GLint a_position = 0;
    GLint a_texcoord = 0;
    GLint a_color = 0;

    // uniform locations
    GLint u_viewSize = 0;
    GLint u_texture0 = 0;

    // uniform values
    int32_t viewWidth = 0;
    int32_t viewHeight = 0;
    int32_t texture0 = 0;

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

    virtual ~ShaderProgram() = default;

    void useProgram()
    {
        EASY_FUNCTION(profiler::colors::Red);
        if (gfxStates.program != program)
        {
            glUseProgram(program);
            gfxStates.program = program;
        }
    }

    void setTexture0Uniform(int32_t value)
    {
        EASY_FUNCTION(profiler::colors::Red);
        assert(value >= 0 && value < 32);
        assert(gfxStates.program == program);

        if (texture0 != value)
        {
            glActiveTexture(GL_TEXTURE0 + static_cast<GLenum>(value));
            glUniform1i(u_texture0, value);
            texture0 = value;
        }
    }

    void setViewSizeUniform(int32_t width, int32_t height)
    {
        EASY_FUNCTION(profiler::colors::Red);
        assert(gfxStates.program == program);

        if (viewWidth != width || viewHeight != height)
        {
            glUniform2f(u_viewSize,
                        static_cast<GLfloat>(width),
                        static_cast<GLfloat>(height));

            viewWidth = width;
            viewHeight = height;
        }
    }

    virtual void init()
    {
        EASY_FUNCTION(profiler::colors::Red);

        vert = glCreateShader(GL_VERTEX_SHADER);
        frag = glCreateShader(GL_FRAGMENT_SHADER);
        program = glCreateProgram();

        GLint vert_len = static_cast<GLint>(strlen(vertSrc));
        GLint frag_len = static_cast<GLint>(strlen(fragSrc));

        glShaderSource(vert, 1, &vertSrc, &vert_len);
        glShaderSource(frag, 1, &fragSrc, &frag_len);

        glCompileShader(vert);
        glCompileShader(frag);

        glAttachShader(program, vert);
        glAttachShader(program, frag);

        glLinkProgram(program);

        // attribute locations
        a_position = glGetAttribLocation(program, "a_position");
        a_texcoord = glGetAttribLocation(program, "a_texcoord");
        a_color = glGetAttribLocation(program, "a_color");

        // uniform locations
        u_viewSize = glGetUniformLocation(program, "u_viewSize");
        u_texture0 = glGetUniformLocation(program, "u_texture0");

        // activate shader program.
        useProgram();

        // shader's sampler setup
        setTexture0Uniform(0);
    }

    virtual void shutdown()
    {
        EASY_FUNCTION(profiler::colors::Red);

        if (gfxStates.program == program)
        {
            glUseProgram(0);
            gfxStates.program = 0;
        }

        glDeleteProgram(program);
        glDeleteShader(vert);
        glDeleteShader(frag);

        vert = 0;
        frag = 0;
        program = 0;
        a_position = 0;
        a_texcoord = 0;
        a_color = 0;
        u_viewSize = 0;
        u_texture0 = 0;
        viewWidth = 0;
        viewHeight = 0;
        texture0 = 0;
    }

};

struct BatchArray : public SoA<ShaderProgram*, Texture, size_t, size_t>
{
    inline ShaderProgram* &program(size_t i) { return get<0>(i); }
    inline Texture &texture(size_t i) { return get<1>(i); }
    inline size_t &offset(size_t i) { return get<2>(i); }
    inline size_t &count(size_t i) { return get<3>(i); }
};

class ContextState
{
public:

    /*!
     * \brief currentTransform the current transformation matrix.
     */
    SVGMatrix currentTransform = SVGMatrix(1.0f);

    /*!
     * \brief strokeStyle specifies the color or style to use for the lines
     * around shapes. The default is the color black.
     */
    Paint strokeStyle = Black;

    /*!
     * \brief fillStyle specifies the color or style to use inside shapes. The
     * default is the color black.
     */
    Paint fillStyle = Black;

    /*!
     * \brief globalAlpha specifies the alpha value that is applied to shapes
     * and images before they are drawn onto the window. The value is in the
     * range from 0.0 (fully transparent) to 1.0 (fully opaque). The default
     * value is 1.0.
     */
    float globalAlpha = 1.0f;

    /*!
     * \brief lineWidth sets the thickness of lines in space units. When
     * getting, it returns the current value (1.0 by default). When setting,
     * zero, negative, Infinity and NaN values are ignored; otherwise the
     * current value is set to the new value.
     */
    float lineWidth = 1.0f;

    /*!
     * \brief lineCap determines how the end points of every line are drawn.
     * There are three possible values for this property and those are: butt,
     * round and square. By default this property is set to butt.
     */
    LineCap lineCap = LineCapButt;

    /*!
     * \brief lineJoin determines how two connecting segments (of lines, arcs or
     * curves) with non-zero lengths in a shape are joined together (degenerate
     * segments with zero lengths, whose specified endpoints and control points
     * are exactly at the same position, are skipped). There are three possible
     * values for this property: round, bevel and miter. By default this
     * property is set to miter.
     *
     * \note the lineJoin setting has no effect if the two connected segments
     * have the same direction, because no joining area will be added in this
     * case.
     */
    LineJoin lineJoin = LineJoinMiter;

    /*!
     * \brief miterLimit sets the miter limit ratio in space units. When
     * getting, it returns the current value (10.0 by default). When setting,
     * zero, negative, Infinity and NaN values are ignored; otherwise the
     * current value is set to the new value.
     */
    float miterLimit = 10.0f;

    /*!
     * \brief lineDashOffset  sets the line dash pattern offset or "phase" to
     * achieve a "marching ants" effect, for example.
     */
    float lineDashOffset = 0.0f;

    /*!
     * \brief shadowOffsetX specifies the distance that the shadow will be
     * offset in horizontal distance.
     */
    float shadowOffsetX = 0.0f;

    /*!
     * \brief shadowOffsetY specifies the distance
     * that the shadow will be offset in vertical distance.
     */
    float shadowOffsetY = 0.0f;

    /*!
     * \brief shadowBlur specifies the level of the blurring effect; this value
     * doesn't correspond to a number of pixels and is not affected by the
     * current transformation matrix. The default value is 0.
     */
    float shadowBlur = 0.0f;

    /*!
     * \brief shadowColor specifies the color of the shadow. The default value
     * is fully-transparent black.
     */
    Color shadowColor = Transparent;

    /*!
     * \brief globalCompositeOperation sets the type of compositing operation to
     * apply when drawing new shapes, where type is a string identifying which
     * of the compositing or blending mode operations to use. The default is
     * CompositeOpSourceOver
     */
    CompositeOp globalCompositeOperation = CompositeOpSourceOver;

    /*!
     * \brief font specifies the current text style being used when drawing
     * text. This string uses the same syntax as the CSS font specifier. The
     * default font is '10px sans-serif'.
     */
    std::string font = "10px sans-serif";

    /*!
     * \brief textAlign specifies the current text alignment being used when
     * drawing text. Beware that the alignment is based on the x value of the
     * fillText() method. So if textAlign is "center", then the text would be
     * drawn at x - (width / 2). The default value is TextAlignStart.
     */
    TextAlign textAlign = TextAlignStart;

    /*!
     * \brief textBaseline  specifies the current text baseline being used when
     * drawing text. The default value is TextBaselineAlphabetic.
     */
    TextBaseline textBaseline = TextBaselineAlphabetic;

    /*!
     * \brief direction I specifies the current text direction used when drawing
     * text.
     */
    Direction direction = DirectionInherit;

    /*!
     * \brief imageSmoothingEnabled can be set to change if images are smoothed
     * (true, default) or not (false). On getting the imageSmoothingEnabled
     * property, the last value it was set to, is returned.
     *
     * This property is useful for pixel-art themed games, when scaling the
     * window for example. The default resizing algorithm can create blurry
     * effects and ruins the beautiful pixels. Set this property to false in
     * that case.
     */
    bool imageSmoothingEnabled = true;

protected:


    Path2D clipRegion;
    std::vector<float> lineDashes;

};

struct ContextData
{
    std::vector<ContextState> states;

    Path2D currentPath;

    std::vector<Texture> textures;

    ShaderProgram default2DProgram;
    GLuint vao = 0;

    enum {
        VBO = 0,
        IBO = 1
    };
    GLuint buffers[2] = {0, 0};

    int32_t viewWidth = 0;
    int32_t viewHeight = 0;

    std::vector<Vertex> vertexBuffer; // write-only interleaved VBO data. DO NOT USE SoA on this member!!!
    std::vector<uint16_t> indexBuffer; // write-only

    BatchArray batches;

    float tessTol = 0.25f;
    float distTol = 0.01f;

    void addBatch(ShaderProgram *program, Texture texture, size_t offset, size_t count)
    {
        EASY_FUNCTION(profiler::colors::DarkRed);

        if (batches.size() > 0)
        {
            size_t id = batches.size() - 1;

            if (batches.program(id) == program && batches.texture(id) == texture)
            {
                // the batch may continue
                batches.count(id) += count;
                return;
            }
        }

        // start a new batch. RenderDefault2D can use any textures for now, as long
        // as they have that little white square in them.
        batches.push(std::move(program),
                     std::move(texture),
                     std::move(offset),
                     std::move(count));
    }

    void pushColorRect(float x, float y, float w, float h, const Color &color)
    {
        EASY_FUNCTION(profiler::colors::DarkRed);

        /*
            (tl)<---------(tr)
             | \           ^
             |   \  tri 2  |
             |     \       |
             |       \     |
             | tri 1   \   |
             V           \ |
            (bl)-------->(br)
        */

        // TODO prevent 16bit index overflow here.
        uint16_t tl = static_cast<uint16_t>(vertexBuffer.size());
        uint16_t bl = tl+1;
        uint16_t br = tl+2;
        uint16_t tr = tl+3;

        addBatch(&default2DProgram, textures.back(), indexBuffer.size(), 6); // two triangles, 6 points

        vertexBuffer.insert(vertexBuffer.end(), {
                                {{x,   y  }, {0, 0}, color}, // top left
                                {{x,   y+h}, {0, 1}, color}, // bottom left
                                {{x+w, y+h}, {1, 1}, color}, // bottom right
                                {{x+w, y  }, {1, 0}, color}, // top right
                            });

        indexBuffer.insert(indexBuffer.end(), {
                               tl, bl, br,   // first triangle
                               br, tr, tl    // second triangle
                           });

        /*
         * Note:
         *
         * Vertex is Position(2floats=8bytes)+TCoord(2xshort=4bytes)+Color(4bytes) = 16 bytes
         *
         * A rectangle is 4 vertices = 64 bytes + 6 indicies (12 bytes) = 76 bytes per rectangle.
         *
         * Without an IBO, it would have been 6 vertices = 96 bytes, so using an ibo
         * saves us 20 bytes per rectangles.
         */

    }

    void renderViewport(int w, int h, float devicePixelRatio)
    {
        EASY_FUNCTION(profiler::colors::DarkRed);

        viewWidth = std::move(w);
        viewHeight = std::move(h);
        tessTol = 0.25f / devicePixelRatio;
        distTol = 0.01f / devicePixelRatio;

    }

    void renderFlush()
    {
        EASY_FUNCTION(profiler::colors::DarkRed);

        // flush the vertex buffer.
        if (vertexBuffer.size() > 0) {
            glBufferData(GL_ARRAY_BUFFER,
                         static_cast<GLsizeiptr>(vertexBuffer.size() * sizeof(Vertex)),
                         &vertexBuffer.front(),
                         GL_STREAM_DRAW);
            vertexBuffer.resize(0);
        }


        // flush the index buffer.
        if (indexBuffer.size() > 0) {
            glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                         static_cast<GLsizeiptr>(indexBuffer.size() * sizeof(uint16_t)),
                         &indexBuffer.front(),
                         GL_STREAM_DRAW);
            indexBuffer.resize(0);
        }


        // flush the batches
        for (size_t i = 0; i < batches.size(); ++i)
        {
            batches.program(i)->useProgram();
            batches.program(i)->setViewSizeUniform(viewWidth, viewHeight);
            batches.texture(i).bind();

            glDrawElements(GL_TRIANGLES,
                           static_cast<GLsizei>(batches.count(i)),
                           GL_UNSIGNED_SHORT,
                           reinterpret_cast<void*>(batches.offset(i) * sizeof(GLushort)));
        }

        batches.resize(0);
    }


    size_t addSubPath(Path2D &path, float startX, float startY)
    {
        detail::PointArray points;
        addPoint(points, std::move(startX), std::move(startY), PointCorner);
        path.subpaths().push(std::move(points), false, 0);
        return path.subpaths().size();
    }

    void addPoint(detail::PointArray &points, float x, float y, PointMask flags)
    {
        size_t ptID = points.size();
        if (ptID-- > 0)
        {
            if (glm::epsilonEqual(points.x(ptID), x, distTol) &&
                glm::epsilonEqual(points.y(ptID), y, distTol))
            {
                points.flags(ptID) |= flags;
                return;
            }
        }

        points.push(std::move(x), std::move(y), 0, 0, 0, 0, 0, std::move(flags));
    }

    void tesselateBezier(detail::PointArray &points,
                         float x1, float y1, float x2, float y2,
                         float x3, float y3, float x4, float y4,
                         int level, PointMask type)
    {
        float x12,y12,x23,y23,x34,y34,x123,y123,x234,y234,x1234,y1234;
        float dx,dy,d2,d3;

        if (level > 10) return;

        x12 = (x1+x2)*0.5f;
        y12 = (y1+y2)*0.5f;
        x23 = (x2+x3)*0.5f;
        y23 = (y2+y3)*0.5f;
        x34 = (x3+x4)*0.5f;
        y34 = (y3+y4)*0.5f;
        x123 = (x12+x23)*0.5f;
        y123 = (y12+y23)*0.5f;

        dx = x4 - x1;
        dy = y4 - y1;
        d2 = glm::abs(((x2 - x4) * dy - (y2 - y4) * dx));
        d3 = glm::abs(((x3 - x4) * dy - (y3 - y4) * dx));

        if ((d2 + d3)*(d2 + d3) < tessTol * (dx*dx + dy*dy))
        {
            addPoint(points, std::move(x4), std::move(y4), std::move(type));
            return;
        }

        x234 = (x23+x34)*0.5f;
        y234 = (y23+y34)*0.5f;
        x1234 = (x123+x234)*0.5f;
        y1234 = (y123+y234)*0.5f;

        tesselateBezier(points,
                        std::move(x1), std::move(y1),
                        std::move(x12), std::move(y12),
                        std::move(x123), std::move(y123),
                        x1234, y1234,
                        level+1, 0);

        tesselateBezier(points, std::move(x1234), std::move(y1234),
                        x234, y234,
                        x34, y34,
                        x4, y4,
                        level+1, type);

    }

    float triarea2(float ax, float ay, float bx, float by, float cx, float cy)
    {
        float abx = bx - ax;
        float aby = by - ay;
        float acx = cx - ax;
        float acy = cy - ay;
        return acx*aby - abx*acy;
    }

    float polyArea(detail::PointArray &pts)
    {
        float area = 0;
        for (size_t i = 2; i < pts.size(); ++i)
        {
            area += triarea2(pts.x(0), pts.y(0),
                             pts.x(i-1), pts.y(i-1),
                             pts.x(i), pts.x(i));
        }
        return area * 0.5f;
    }

    void polyReverse(detail::PointArray &pts)
    {
        size_t i = 0, j = pts.size()-1;
        while (i < j)
        {
            pts.swap(i, j);
            i++;
            j--;
        }
    }

    float normalize(float &x, float &y)
    {
        float d = glm::sqrt( x * x + y * y);
        if (glm::epsilonNotEqual(d, 0.0f, glm::epsilon<float>()))
        {
            float id = 1.0f / d;
            x *= id;
            y *= id;
        }
        return d;
    }

    void flattenPaths(Path2D &path, Fill fillRule)
    {
        uint8_t &dirty = path.dirty();
        Fill &rFillRule = path.fillRule();

        if (dirty || rFillRule != fillRule)
        {
            size_t ptID;
            size_t npaths = 0;

            auto &subpaths = path.subpaths();
            auto &commands = path.commands();

            subpaths.resize(0);

            for(size_t i = 0; i < commands.size(); ++i)
            {
                switch(commands.type(i))
                {
                case detail::CLOSE:
                    if (npaths > 0)
                    {
                        subpaths.closed(npaths-1) = true;
                    }
                    break;
                case detail::MOVE_TO:
                {
                    npaths = addSubPath(path, commands.param0(i), commands.param1(i));
                    break;
                }
                case detail::LINE_TO:
                {
                    if (npaths == 0) { npaths = addSubPath(path, 0, 0); }
                    addPoint(subpaths.points(npaths-1), commands.param0(i), commands.param1(i), PointCorner);
                    break;
                }
                case detail::BEZIER_TO:
                    if (npaths == 0) { npaths = addSubPath(path, 0, 0); }
                    ptID = subpaths.points(npaths-1).size() - 1;
                    tesselateBezier(subpaths.points(npaths-1),
                                    subpaths.points(npaths-1).x(ptID),
                                    subpaths.points(npaths-1).y(ptID),
                                    commands.param0(i), commands.param1(i),
                                    commands.param2(i), commands.param3(i),
                                    commands.param4(i), commands.param5(i),
                                    0, PointCorner);

                    break;
                case detail::QUAD_TO:
                {
                    if (npaths == 0) { npaths = addSubPath(path, 0, 0); }
                    ptID = subpaths.points(npaths-1).size() - 1;
                    float x0 = subpaths.points(npaths-1).x(ptID);
                    float y0 = subpaths.points(npaths-1).y(ptID);
                    float cx = commands.param0(i);
                    float cy = commands.param1(i);
                    float x = commands.param2(i);
                    float y = commands.param3(i);
                    float c1x = x0 + 2.0f/3.0f*(cx - x0);
                    float c1y = y0 + 2.0f/3.0f*(cy - y0);
                    float c2x = x + 2.0f/3.0f*(cx - x);
                    float c2y = y + 2.0f/3.0f*(cy - y);
                    tesselateBezier(subpaths.points(npaths-1),
                                    std::move(x0), std::move(y0),
                                    std::move(c1x), std::move(c1y),
                                    std::move(c2x), std::move(c2y),
                                    std::move(x), std::move(y),
                                    0, PointCorner);
                    break;
                }
                case detail::ARC:
                    // TODO
                    break;
                case detail::ARC_TO:
                    // TODO
                    break;
                case detail::ELLIPSE:
                    // TODO
                    break;
                case detail::RECT:
                    // TODO
                    break;
                }
            }

            auto &boundTop = path.boundTop();
            auto &boundRight = path.boundRight();
            auto &boundBottom = path.boundBottom();
            auto &boundLeft = path.boundLeft();

            boundTop = FLT_MAX;
            boundRight = -FLT_MAX;
            boundBottom = -FLT_MAX;
            boundLeft = FLT_MAX;

            // Calculate the direction and length of line segments.
            for (size_t j = 0; j < npaths; ++j)
            {
                auto &pts = subpaths.points(j);
                size_t ptCount = pts.size();
                size_t p0 = ptCount - 1;
                size_t p1 = 0;

                // If the first and last points are the same, remove the last, mark as closed path.
                if (glm::epsilonEqual(pts.x(p0), pts.x(p1), distTol) &&
                        glm::epsilonEqual(pts.y(p0), pts.y(p1), distTol))
                {
                    pts.pop_back();
                    subpaths.closed(j) = true;
                }

//                // Enforce winding.
//                if (ptCount > 2)
//                {
//                    float area = polyArea(pts);

//                    if (fillRule == FillNonZero && area > 0.0f)
//                    {
//                        polyReverse(pts);
//                    }
//                    else if(fillRule == FillEvenOdd && area < 0.0f)
//                    {
//                        polyReverse(pts);
//                    }
//                }

                for(size_t i = 0; i < ptCount; ++i)
                {
                    // Calculate segment direction and length
                    pts.dx(p0) = pts.x(p1) - pts.x(p0);
                    pts.dy(p0) = pts.y(p1) - pts.y(p0);
                    pts.len(p0) = normalize(pts.dx(p0), pts.dy(p0));

                    // Update bounds
                    boundLeft = glm::min(boundLeft, pts.x(p0));
                    boundTop = glm::min(boundTop, pts.y(p0));
                    boundRight = glm::max(boundRight, pts.x(p0));
                    boundBottom = glm::max(boundBottom, pts.y(p0));

                    // Advance
                    p0 = p1++;
                }
            }

            path.dirty() = false;
            path.fillRule() = std::move(fillRule);
        }
    }

    void calculateJoins(Path2D &path, LineJoin lineJoin, float miterLimit)
    {
        // Calculate which joins needs extra vertices to append, and gather vertex count.
        for (size_t i = 0; i < path.subpaths().size(); ++i)
        {
            auto &pts = path.subpaths().points(i);

            size_t p0 = pts.size() - 1;
            size_t p1 = 0;
            int nleft = 0;

            path.subpaths().nbevel(i) = 0;

            for (size_t j = 0; j < pts.size(); j++)
            {
                float dlx0, dly0, dlx1, dly1, dmr2, cross, limit;
                dlx0 = pts.dy(p0);
                dly0 = -pts.dx(p0);
                dlx1 = pts.dy(p1);
                dly1 = -pts.dx(p1);

                // Calculate extrusions
                pts.dmx(p1) = (dlx0 + dlx1) * 0.5f;
                pts.dmy(p1) = (dly0 + dly1) * 0.5f;
                dmr2 = pts.dmx(p1) * pts.dmx(p1) + pts.dmy(p1) * pts.dmy(p1);
                if (dmr2 > 0.000001f)
                {
                    float scale = 1.0f / dmr2;
                    if (scale > 600.0f)
                    {
                        scale = 600.0f;
                    }
                    pts.dmx(p1) *= scale;
                    pts.dmy(p1) *= scale;
                }

                // Clear flags, but keep the corner.
                pts.flags(p1) = (pts.flags(p1) & PointCorner) ? PointCorner : 0;

                // Keep track of left turns.
                cross = pts.dx(p1) * pts.dy(p0) - pts.dx(p0) * pts.dy(p1);
                if (cross > 0.0f)
                {
                    nleft++;
                    pts.flags(p1) |= PointLeft;
                }

                // Calculate if we should use bevel or miter for inner join.
                limit = 1.01f;
                if ((dmr2 * limit*limit) < 1.0f)
                    pts.flags(p1) |= PointInnerBevel;

                // Check to see if the corner needs to be beveled.
                if (pts.flags(p1) & PointCorner)
                {
                    if ((dmr2 * miterLimit*miterLimit) < 1.0f || lineJoin == LineJoinBevel || lineJoin == LineJoinRound)
                    {
                        pts.flags(p1) |= PointBevel;
                    }
                }

                if ((pts.flags(p1) & (PointBevel | PointInnerBevel)) != 0)
                    path.subpaths().nbevel(i)++;

                p0 = p1++;
            }
        }
    }


};

}
}

#endif // TUNISCONTEXTDATA_H
