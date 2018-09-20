#define MPE_POLY2TRI_IMPLEMENTATION
#define TUNIS_GL_IMPLEMENTATION

#ifdef _WIN32
#define NOMINMAX
#endif

#ifndef TUNIS_CURVE_RECURSION_LIMIT
#define TUNIS_CURVE_RECURSION_LIMIT 32
#endif

#ifndef TUNIS_MAX_TEXTURE_SIZE
#define TUNIS_MAX_TEXTURE_SIZE 2048
#endif

#ifndef TUNIS_VERTEX_MAX
#define TUNIS_VERTEX_MAX 16384
#endif

#include <Tunis.h>

#include <TunisGL.h>
#include <TunisGraphicStates.h>
#include <TunisPaint.h>
#include <TunisPath2D.h>
#include <TunisSOA.h>
#include <TunisTexture.h>
#include <TunisVertex.h>


#include <easy/profiler.h>
#include <glm/gtc/epsilon.hpp>

#include <thread>

namespace tunis
{
    namespace detail
    {
        GraphicStates gfxStates;

        enum DrawOp
        {
            DRAW_FILL,
            DRAW_STROKE
        };

        struct ShaderProgram
        {

            GLuint programId = 0;

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

            ShaderProgram() = default;
            virtual ~ShaderProgram() = default;

            virtual void init()
            {
                GLuint vert = glCreateShader(GL_VERTEX_SHADER);
                GLuint frag = glCreateShader(GL_FRAGMENT_SHADER);
                programId = glCreateProgram();

                const char * vertSrc =
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

                const char * fragSrc =
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

                GLint vert_len = static_cast<GLint>(strlen(vertSrc));
                GLint frag_len = static_cast<GLint>(strlen(fragSrc));

                glShaderSource(vert, 1, &vertSrc, &vert_len);
                glShaderSource(frag, 1, &fragSrc, &frag_len);

                glCompileShader(vert);
                glCompileShader(frag);

                glAttachShader(programId, vert);
                glAttachShader(programId, frag);

                glLinkProgram(programId);

                glDeleteShader(vert);
                glDeleteShader(frag);

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

            virtual void shutdown()
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
        };

        struct BatchArray : public SoA<ShaderProgram*, Texture, size_t, size_t>
        {
            inline ShaderProgram* &program(size_t i) { return get<0>(i); }
            inline Texture &texture(size_t i) { return get<1>(i); }
            inline size_t &offset(size_t i) { return get<2>(i); }
            inline size_t &count(size_t i) { return get<3>(i); }
        };

        struct DrawOpArray : public SoA<DrawOp, Path2D, ContextState>
        {
            inline DrawOp &op(size_t i) { return get<0>(i); }
            inline Path2D &path(size_t i) { return get<1>(i); }
            inline ContextState &state(size_t i) { return get<2>(i); }
        };

        struct ContextPriv
        {
            std::vector<ContextState> states;

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

            DrawOpArray renderQueue;
            BatchArray batches;

            float tessTol = 0.25f;
            float distTol = 0.01f;

            ContextPriv()
            {
                auto tunisGL_initialized = tunisGLInit();
                if (!tunisGL_initialized)
                {
                    abort();
                }

                // Create a default texture atlas.
#ifdef TUNIS_MAX_TEXTURE_SIZE
                gfxStates.maxTexSize = TUNIS_MAX_TEXTURE_SIZE;
#else
                glGetIntegerv(GL_MAX_TEXTURE_SIZE, &global.maxTexSize);
#endif
                Texture tex = Texture(gfxStates.maxTexSize, gfxStates.maxTexSize);
                textures.push_back(tex); // retain

                Paint::reserve(64);
                Path2D::reserve(64);
                renderQueue.reserve(1024);
                batches.reserve(1024);

                vertexBuffer.reserve(TUNIS_VERTEX_MAX);
                indexBuffer.reserve((TUNIS_VERTEX_MAX-2)*3);

                // Initialize our default shader.
                default2DProgram.init();

                if (tunisGLSupport(GL_VERSION_3_0))
                {
                    // Create a dummy vertex array object (mandatory since GL Core profile)
                    glGenVertexArrays(1, &vao);
                    glBindVertexArray(vao);
                }

                // Create vertex and index buffer objects for the batches
                glGenBuffers(2, buffers);
                glBindBuffer(GL_ARRAY_BUFFER, buffers[ContextPriv::VBO]);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[ContextPriv::IBO]);

                // Configure our vertex attributes
                glVertexAttribPointer(static_cast<GLuint>(default2DProgram.a_position), decltype(Vertex::pos)::length(),    GL_FLOAT,          GL_FALSE, sizeof(Vertex), reinterpret_cast<const void *>(0));
                glVertexAttribPointer(static_cast<GLuint>(default2DProgram.a_texcoord), decltype(Vertex::tcoord)::length(), GL_UNSIGNED_SHORT, GL_TRUE,  sizeof(Vertex), reinterpret_cast<const void *>(sizeof(Vertex::pos)));
                glVertexAttribPointer(static_cast<GLuint>(default2DProgram.a_color),    decltype(Vertex::color)::length(),  GL_UNSIGNED_BYTE,  GL_TRUE,  sizeof(Vertex), reinterpret_cast<const void *>(sizeof(Vertex::pos) + sizeof(Vertex::tcoord)));
                glEnableVertexAttribArray(static_cast<GLuint>(default2DProgram.a_position));
                glEnableVertexAttribArray(static_cast<GLuint>(default2DProgram.a_texcoord));
                glEnableVertexAttribArray(static_cast<GLuint>(default2DProgram.a_color));

                /* set default state */
                glEnable(GL_CULL_FACE);
                glCullFace(GL_BACK);
                glFrontFace(GL_CCW);
                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                glDisable(GL_DEPTH_TEST);
                glDisable(GL_SCISSOR_TEST);
                glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
            }

            ~ContextPriv()
            {
                // unload texture data by deleting every potential texture holders.
                textures.resize(0);
                batches.resize(0);

                // unload shader program
                default2DProgram.shutdown();

                // unload vertex and index buffers
                glDeleteBuffers(2, buffers);
                buffers[ContextPriv::VBO] = 0;
                buffers[ContextPriv::IBO] = 0;

                // reset global states.
                gfxStates = GraphicStates();

                // reset global GL states
                glBindTexture(GL_TEXTURE_2D , 0);
                if (tunisGLSupport(GL_VERSION_3_0))
                {
                    glBindVertexArray(0);
                }
                glBindBuffer(GL_ARRAY_BUFFER, 0);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

                // shut down GL wrangler/
                tunisGLShutdown();
            }

            void clearFrame(int32_t fbLeft, int32_t fbTop, int32_t fbWidth, int32_t fbHeight, Color backgroundColor)
            {
                // update the clear color if necessary
                if (gfxStates.backgroundColor != backgroundColor)
                {
                    glClearColor(backgroundColor.r/255.0f,
                                 backgroundColor.g/255.0f,
                                 backgroundColor.b/255.0f,
                                 backgroundColor.a/255.0f);

                    gfxStates.backgroundColor = backgroundColor;
                }

                // update the viewport if necessary
                Viewport viewport(fbLeft, fbTop, fbWidth, fbHeight);
                if (gfxStates.viewport != viewport)
                {
                    glViewport(fbLeft, fbTop, fbWidth, fbHeight);
                    gfxStates.viewport = viewport;
                }

                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
            }


            uint16_t addBatch(ShaderProgram *program, Texture texture, uint32_t vertexCount, uint32_t indexCount, Vertex **vout, Index **iout)
            {
                assert(vertexCount >= 3);

                size_t istart = indexBuffer.size();
                size_t iend = istart + indexCount;
                indexBuffer.resize(iend);
                if (iout) *iout = &indexBuffer[istart];

                size_t vstart = vertexBuffer.size();
                size_t vend = vstart + vertexCount;
                vertexBuffer.resize(vend);
                if (vout) *vout = &vertexBuffer[vstart];

                if (batches.size() > 0)
                {
                    size_t id = batches.size() - 1; // last batch.

                    if (batches.program(id) == program &&
                        batches.texture(id) == texture)
                    {
                        // the batch may continue
                        batches.count(id) += indexCount;
                        return static_cast<uint16_t>(vstart);
                    }
                }

                // start a new batch. RenderDefault2D can use any textures for now, as long
                // as they have that little white square in them.
                batches.push(std::move(program),
                             std::move(texture),
                             std::move(istart),
                             std::move(indexCount));

                return static_cast<uint16_t>(vstart);
            }

            void beginFrame(int w, int h, float devicePixelRatio)
            {
                viewWidth = std::move(w);
                viewHeight = std::move(h);
                tessTol = 0.25f / devicePixelRatio;
                distTol = 0.01f / devicePixelRatio;
            }

            void endFrame()
            {
                // flush the render Queue.
                if (renderQueue.size() > 0)
                {
                    // Generate Geometry (Multi-threaded)
                    #pragma omp parallel for num_threads(std::thread::hardware_concurrency())
                    for (int i = 0; i < renderQueue.size(); ++i)
                    {
                        EASY_THREAD_SCOPE("OpenMP");
                        EASY_BLOCK("Poly2Tri", profiler::colors::DarkBlue)
                        auto &path = renderQueue.path(i);
                        if (path.dirty())
                        {
                            switch(renderQueue.op(i))
                            {
                                case DRAW_FILL:
                                    generateFillContour(path);
                                    break;
                                case DRAW_STROKE:
                                    generateStrokeContour(path, renderQueue.state(i).lineWidth);
                                    break;
                            }

                            triangulate(path);

                            path.dirty() = false;
                        }
                    }

                    // Batch Geometry into vertex and index buffers
                    for (size_t i = 0; i < renderQueue.size(); ++i)
                    {
                        auto &path = renderQueue.path(i);
                        auto &state = renderQueue.state(i);

                        Paint *paint;
                        switch(renderQueue.op(i))
                        {
                            case DRAW_FILL:
                                paint = &state.fillStyle;
                                break;
                            case DRAW_STROKE:
                                paint = &state.strokeStyle;
                                break;
                        }

                        glm::vec2 range = path.boundBottomRight() - path.boundTopLeft();

                        for(size_t j = 0; j < path.subPaths().size(); ++j)
                        {
                            MPEPolyContext &polyContext = path.subPaths().polyContext(j);

                            uint32_t vertexCount = polyContext.PointPoolCount;
                            uint16_t indexCount = polyContext.TriangleCount*3;

                            if (vertexCount < 3)
                            {
                                continue; // not enough vertices to make a fill. Skip
                            }


                            Vertex *verticies;
                            Index *indices;
                            uint16_t offset = addBatch(&default2DProgram,
                                                       textures.back(),
                                                       vertexCount,
                                                       indexCount,
                                                       &verticies,
                                                       &indices);

                            Color color = paint->innerColor();
                            color.a *= state.globalAlpha;

                            //populate the vertices
                            for (size_t vid = 0; vid < polyContext.PointPoolCount; ++vid)
                            {
                                MPEPolyPoint &Point = polyContext.PointsPool[vid];
                                Position pos(Point.X, Point.Y);
                                glm::vec2 tcoord = TCoord(((pos - path.boundTopLeft()) / range) * 16.0f / static_cast<float>(gfxStates.maxTexSize) * static_cast<float>(0xFFFF));
                                verticies[vid].pos = pos;
                                verticies[vid].tcoord.x = static_cast<uint16_t>(tcoord.s);
                                verticies[vid].tcoord.t = static_cast<uint16_t>(tcoord.t);
                                verticies[vid].color = color;
                            }

                            //populate the indicies
                            for (size_t tid = 0; tid < polyContext.TriangleCount; ++tid)
                            {
                                MPEPolyTriangle* triangle = polyContext.Triangles[tid];

                                // get the array index by pointer address arithmetic.
                                uint16_t p0 = static_cast<uint16_t>(triangle->Points[0] - polyContext.PointsPool);
                                uint16_t p1 = static_cast<uint16_t>(triangle->Points[1] - polyContext.PointsPool);
                                uint16_t p2 = static_cast<uint16_t>(triangle->Points[2] - polyContext.PointsPool);

                                size_t iid = tid * 3;
                                indices[iid+0] = offset+p2;
                                indices[iid+1] = offset+p1;
                                indices[iid+2] = offset+p0;
                            }
                        }
                    }

                    renderQueue.resize(0);
                }


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
                if ( batches.size() > 0)
                {
                    for (size_t i = 0; i < batches.size(); ++i)
                    {
                        batches.program(i)->useProgram();
                        batches.program(i)->setViewSizeUniform(viewWidth, viewHeight);
                        batches.texture(i).bind();


#if 1
                        glDrawElements(GL_TRIANGLES,
                                       static_cast<GLsizei>(batches.count(i)),
                                       GL_UNSIGNED_SHORT,
                                       reinterpret_cast<void*>(batches.offset(i) * sizeof(GLushort)));
#endif

#if 0
                        // Helpful code for debugging triangles.
                        for (size_t j = 0; j < batches.count(i)/3; ++j)
                        {
                            glDrawElements(GL_LINE_LOOP,
                                           3,
                                           GL_UNSIGNED_SHORT,
                                           reinterpret_cast<void*>((batches.offset(i)+(j*3)) * sizeof(GLushort)));
                        }
#endif

#if 0
                        // Helpful code for debugging contours.
                        glDrawElements(GL_LINE_STRIP,
                                       static_cast<GLsizei>(batches.count(i)),
                                       GL_UNSIGNED_SHORT,
                                       reinterpret_cast<void*>(batches.offset(i) * sizeof(GLushort)));
#endif

                    }

                    batches.resize(0);
                }
            }

            void addSubPath(SubPathArray &subPaths, glm::vec2 startPos)
            {
                size_t i = subPaths.size();
                subPaths.resize(i + 1);

                subPaths.mempool(i).resize(0);
                subPaths.outerPoints(i).resize(0);
                subPaths.innerPoints(i).resize(0);
                subPaths.closed(i) = false;

                subPaths.outerPoints(i).push(std::move(startPos), {}, {});
            }

            void addPoint(SubPathArray &subPaths, glm::vec2 pos)
            {
                auto &points = subPaths.outerPoints(subPaths.size()-1);

                if (points.size() > 0)
                {
                    if (glm::all(glm::epsilonEqual(points.pos(points.size() - 1),
                                                   pos,
                                                   distTol)))
                    {
                        return;
                    }
                }

                points.push(std::move(pos), {}, {});
            }


            // based of http://antigrain.com/__code/src/agg_curves.cpp.html by Maxim Shemanarev
            void bezier(SubPathArray &subPaths, float x1, float y1, float x2,
                        float y2, float x3, float y3, float x4, float y4)
            {
                addPoint(subPaths, glm::vec2(x1, y1));
                recursiveBezier(subPaths, x1, y1, x2, y2, x3, y3, x4, y4, 0);
                addPoint(subPaths, glm::vec2(x4, y4));
            }
            float calcSqrtDistance(float x1, float y1, float x2, float y2)
            {
                float dx = x2-x1;
                float dy = y2-y1;
                return dx * dx + dy * dy;
            }
            void recursiveBezier(SubPathArray &subPaths,
                                 float x1, float y1,
                                 float x2, float y2,
                                 float x3, float y3,
                                 float x4, float y4,
                                 int32_t level)
            {
                if(level > TUNIS_CURVE_RECURSION_LIMIT)
                {
                    return;
                }

                // Calculate all the mid-points of the line segments
                //----------------------
                float x12   = (x1 + x2) / 2;
                float y12   = (y1 + y2) / 2;
                float x23   = (x2 + x3) / 2;
                float y23   = (y2 + y3) / 2;
                float x34   = (x3 + x4) / 2;
                float y34   = (y3 + y4) / 2;
                float x123  = (x12 + x23) / 2;
                float y123  = (y12 + y23) / 2;
                float x234  = (x23 + x34) / 2;
                float y234  = (y23 + y34) / 2;
                float x1234 = (x123 + x234) / 2;
                float y1234 = (y123 + y234) / 2;

                // Try to approximate the full cubic curve by a single straight line
                //------------------
                float dx = x4-x1;
                float dy = y4-y1;

                float d2 = glm::abs(((x2 - x4) * dy - (y2 - y4) * dx));
                float d3 = glm::abs(((x3 - x4) * dy - (y3 - y4) * dx));
                float da1, da2, k;

                switch((int(d2 > glm::epsilon<float>()) << 1) + int(d3 > glm::epsilon<float>()))
                {
                    case 0:
                        // All collinear OR p1==p4
                        //----------------------
                        k = dx*dx + dy*dy;
                        if(glm::epsilonEqual(k, 0.0f, glm::epsilon<float>()))
                        {
                            d2 = calcSqrtDistance(x1, y1, x2, y2);
                            d3 = calcSqrtDistance(x4, y4, x3, y3);
                        }
                        else
                        {
                            k   = 1 / k;
                            da1 = x2 - x1;
                            da2 = y2 - y1;
                            d2  = k * (da1*dx + da2*dy);
                            da1 = x3 - x1;
                            da2 = y3 - y1;
                            d3  = k * (da1*dx + da2*dy);
                            if(d2 > 0 && d2 < 1 && d3 > 0 && d3 < 1)
                            {
                                // Simple collinear case, 1---2---3---4
                                // We can leave just two endpoints
                                return;
                            }
                            if(d2 <= 0) d2 = calcSqrtDistance(x2, y2, x1, y1);
                            else if(d2 >= 1) d2 = calcSqrtDistance(x2, y2, x4, y4);
                            else             d2 = calcSqrtDistance(x2, y2, x1 + d2*dx, y1 + d2*dy);

                            if(d3 <= 0) d3 = calcSqrtDistance(x3, y3, x1, y1);
                            else if(d3 >= 1) d3 = calcSqrtDistance(x3, y3, x4, y4);
                            else             d3 = calcSqrtDistance(x3, y3, x1 + d3*dx, y1 + d3*dy);
                        }
                        if(d2 > d3)
                        {
                            if(d2 < tessTol)
                            {
                                addPoint(subPaths, glm::vec2(x2, y2));
                                return;
                            }
                        }
                        else
                        {
                            if(d3 < tessTol)
                            {
                                addPoint(subPaths, glm::vec2(x3, y3));
                                return;
                            }
                        }
                        break;

                    case 1:
                        // p1,p2,p4 are collinear, p3 is significant
                        //----------------------
                        if(d3 * d3 <= tessTol * (dx*dx + dy*dy))
                        {
                            addPoint(subPaths, glm::vec2(x23, y23));
                            return;
                        }
                        break;

                    case 2:
                        // p1,p3,p4 are collinear, p2 is significant
                        //----------------------
                        if(d2 * d2 <= tessTol * (dx*dx + dy*dy))
                        {
                            addPoint(subPaths, glm::vec2(x23, y23));
                            return;
                        }
                        break;

                    case 3:
                        // Regular case
                        //-----------------
                        if((d2 + d3)*(d2 + d3) <= tessTol * (dx*dx + dy*dy))
                        {
                            addPoint(subPaths, glm::vec2(x23, y23));
                            return;
                        }
                        break;
                }

                // Continue subdivision
                //----------------------
                recursiveBezier(subPaths, x1, y1, x12, y12, x123, y123, x1234, y1234, level + 1);
                recursiveBezier(subPaths, x1234, y1234, x234, y234, x34, y34, x4, y4, level + 1);
            }

            void arc(SubPathArray &subPaths, float centerX, float centerY,
                     float radius, float startAngle, float endAngle,
                     bool anticlockwise)
            {
                float deltaAngle = endAngle - startAngle;

                if (anticlockwise)
                {
                    if (glm::abs(deltaAngle) < glm::two_pi<float>())
                    {
                        while (deltaAngle > 0.0f)
                        {
                            deltaAngle -= glm::two_pi<float>();
                        }
                    }
                    else
                    {
                        deltaAngle = -glm::two_pi<float>();
                    }
                }
                else
                {
                    if (glm::abs(deltaAngle) < glm::two_pi<float>())
                    {
                        while (deltaAngle < 0.0f)
                        {
                            deltaAngle += glm::two_pi<float>();
                        }
                    }
                    else
                    {
                        deltaAngle = glm::two_pi<float>();
                    }
                }

                int32_t segmentCount = static_cast<int32_t>(glm::ceil( glm::abs(deltaAngle) / glm::half_pi<float>()));

                float midAngle = (deltaAngle / segmentCount) * 0.5f;
                float tangentFactor = glm::abs(4.0f / 3.0f * (1.0f - glm::cos(midAngle)) / glm::sin(midAngle));

                if (anticlockwise)
                {
                    tangentFactor = -tangentFactor;
                }

                float deltaX = glm::cos(startAngle);
                float deltaY = glm::sin(startAngle);

                float prevX = centerX + deltaX * radius;
                float prevY = centerY + deltaY * radius;
                float prevTanX = -deltaY * tangentFactor * radius;
                float prevTanY = deltaX * tangentFactor * radius;

                size_t id = subPaths.size();

                if(id == 0)
                {
                    addSubPath(subPaths, glm::vec2(prevX, prevY));
                }
                else
                {
                    addPoint(subPaths, glm::vec2(prevX, prevY));
                }

                for (int32_t segment = 1; segment <= segmentCount; ++segment)
                {
                    float angle = startAngle + deltaAngle * (static_cast<float>(segment)/static_cast<float>(segmentCount));
                    deltaX = glm::cos(angle);
                    deltaY = glm::sin(angle);
                    float x = centerX + deltaX * radius;
                    float y = centerY + deltaY * radius;
                    float tanX = -deltaY * tangentFactor * radius;
                    float tanY = deltaX * tangentFactor * radius;

                    bezier(subPaths,
                           prevX, prevY, // start point
                           prevX + prevTanX, prevY + prevTanY, // control point 1
                           x - tanX, y - tanY, // control point 2
                           x, y); // end point

                    prevX = x;
                    prevY = y;
                    prevTanX = tanX;
                    prevTanY = tanY;
                }
            }

            float distPtSeg(const glm::vec2 &c, const glm::vec2 &p, const glm::vec2 &q)
            {
                glm::vec2 pq = q - p;
                glm::vec2 pc = c - p;
                float d = (pq.x * pq.x) + (pq.y * pq.y);
                d = glm::dot(pq, pq);
                float t = glm::dot(pq, pc);

                if (d > 0)
                {
                    t /= d;
                }

                t = glm::clamp(t, 0.0f, 1.0f);

                pc = p + (t*pq) - c;

                return glm::dot(pc, pc);
            }

            void arcTo(SubPathArray &subPaths, float x1, float y1, float x2, float y2, float radius)
            {
                auto &points = subPaths.outerPoints(subPaths.size()-1);

                glm::vec2 p0 = points.pos(points.size()-1);
                glm::vec2 p1(x1, y1);
                glm::vec2 p2(x2, y2);

                if (glm::all(glm::epsilonEqual(p0, p1, distTol)))
                {
                    addPoint(subPaths, p1);
                    return;
                }

                if (glm::all(glm::epsilonEqual(p1, p2, distTol)))
                {
                    addPoint(subPaths, p1);
                    return;
                }

                if (distPtSeg(p1, p0, p2) < (distTol * distTol))
                {
                    addPoint(subPaths, p1);
                    return;
                }

                if (radius < distTol)
                {
                    addPoint(subPaths, p1);
                    return;
                }

                glm::vec2 d0 = glm::normalize(p0 - p1);
                glm::vec2 d1 = glm::normalize(p2 - p1);
                float a = glm::acos(glm::dot(d0, d1));
                float d = radius / glm::tan(a * 0.5f);

                if (d > 10000.0f)
                {
                    addPoint(subPaths, p1);
                    return;
                }

                float cp = d1.x * d0.y - d0.x * d1.y;

                float cx, cy, a0, a1;
                bool anticlockwise;
                if (cp > 0.0f)
                {
                    cx = p1.x + d0.x * d + d0.y * radius;
                    cy = p1.y + d0.y * d + -d0.x * radius;
                    a0 = glm::atan(d0.x, -d0.y);
                    a1 = glm::atan(-d1.x, d1.y);
                    anticlockwise = false;
                }
                else
                {
                    cx = p1.x + d0.x * d + -d0.y * radius;
                    cy = p1.y + d0.y * d + d0.x * radius;
                    a0 = glm::atan(-d0.x, d0.y);
                    a1 = glm::atan(d1.x, -d1.y);
                    anticlockwise = true;
                }

                arc(subPaths, cx, cy, radius, a0, a1, anticlockwise);
            }


            void generateFillContour(Path2D &path)
            {

                SubPathArray &subPaths = path.subPaths();
                PathCommandArray &commands = path.commands();

                // reset to default.
                subPaths.resize(0);

                for(size_t i = 0; i < commands.size(); ++i)
                {
                    switch(commands.type(i))
                    {
                        case CLOSE:
                            if (subPaths.size() > 0)
                            {
                                subPaths.closed(subPaths.size()-1) = true;
                            }
                            break;
                        case MOVE_TO:
                            addSubPath(subPaths, glm::vec2(commands.param0(i), commands.param1(i)));
                            break;
                        case LINE_TO:
                            if (subPaths.size() == 0) { addSubPath(subPaths, glm::vec2(0.0f)); }
                            addPoint(subPaths, glm::vec2(commands.param0(i), commands.param1(i)));
                            break;
                        case BEZIER_TO:
                        {
                            if (subPaths.size() == 0) { addSubPath(subPaths, glm::vec2(0.0f)); }
                            auto &points = subPaths.outerPoints(subPaths.size()-1);
                            auto &prevPoint = points.pos(points.size()-1);
                            bezier(subPaths,
                                   prevPoint.x, prevPoint.y,
                                   commands.param0(i), commands.param1(i),
                                   commands.param2(i), commands.param3(i),
                                   commands.param4(i), commands.param5(i));

                            break;
                        }
                        case QUAD_TO:
                        {
                            if (subPaths.size() == 0) { addSubPath(subPaths, glm::vec2(0.0f)); }
                            auto &points = subPaths.outerPoints(subPaths.size()-1);
                            auto &prevPoint = points.pos(points.size()-1);
                            float x0 = prevPoint.x;
                            float y0 = prevPoint.y;
                            float cx = commands.param0(i);
                            float cy = commands.param1(i);
                            float x = commands.param2(i);
                            float y = commands.param3(i);
                            float c1x = x0 + 2.0f/3.0f*(cx - x0);
                            float c1y = y0 + 2.0f/3.0f*(cy - y0);
                            float c2x = x + 2.0f/3.0f*(cx - x);
                            float c2y = y + 2.0f/3.0f*(cy - y);
                            bezier(subPaths, x0, y0, c1x, c1y, c2x, c2y, x, y);
                            break;
                        }
                        case ARC:
                        {
                            arc(subPaths,
                                commands.param0(i),
                                commands.param1(i),
                                commands.param2(i),
                                commands.param3(i),
                                commands.param4(i),
                                commands.param5(i) > 0.5f);
                            break;
                        }
                        case ARC_TO:
                        {
                            if (subPaths.size() == 0) { addSubPath(subPaths, glm::vec2(0.0f)); }
                            arcTo(subPaths,
                                  commands.param0(i),
                                  commands.param1(i),
                                  commands.param2(i),
                                  commands.param3(i),
                                  commands.param4(i));
                            break;
                        }
                        case ELLIPSE:
                            // TODO
                            break;
                        case RECT:
                        {
                            float x = commands.param0(i);
                            float y = commands.param1(i);
                            float w = commands.param2(i);
                            float h = commands.param3(i);
                            size_t id = subPaths.size();
                            addSubPath(subPaths, glm::vec2(x, y));
                            addPoint(subPaths, glm::vec2(x, y+h));
                            addPoint(subPaths, glm::vec2(x+w, y+h));
                            addPoint(subPaths, glm::vec2(x+w, y));
                            subPaths.closed(id) = true;
                            break;
                        }
                    }
                }

                // validate.
                for (size_t i = 0; i < subPaths.size(); ++i)
                {
                    auto &points = subPaths.outerPoints(i);

                    // Check if the first and last point are the same. Get rid of
                    // the last point if that is the case, and close the subpath.
                    if (points.size() >= 2 &&
                        glm::all(glm::epsilonEqual(points.pos(0),
                                                   points.pos(points.size()-1),
                                                   distTol)))
                    {
                        points.resize(points.size()-1);
                        subPaths.closed(i) = true;
                    }
                }
            }

            void generateStrokeContour(Path2D &path, float lineWidth)
            {
                generateFillContour(path);

                float halfLineWidth = lineWidth * 0.5f;

                SubPathArray &subPaths = path.subPaths();
                for (size_t i = 0; i < subPaths.size(); ++i)
                {
                    auto &outerPoints = subPaths.outerPoints(i);
                    auto &innerPoints = subPaths.innerPoints(i);

                    if(subPaths.closed(i))
                    {
                        // Calculate direction vectors
                        for (size_t p0 = outerPoints.size() - 1, p1 = 0; p1 < outerPoints.size(); p0 = p1++)
                        {
                            outerPoints.dir(p0) = glm::normalize(outerPoints.pos(p1) - outerPoints.pos(p0));
                        }

                        // Calculate excrusion vectors
                        for (size_t p0 = outerPoints.size() - 1, p1 = 0; p1 < outerPoints.size(); p0 = p1++)
                        {
                            // rotate direction vector by 90degree CW
                            glm::vec2 dir0 = glm::vec2(outerPoints.dir(p0).y, -outerPoints.dir(p0).x);
                            glm::vec2 dir1 = glm::vec2(outerPoints.dir(p1).y, -outerPoints.dir(p1).x);
                            glm::vec2 exc = (dir0 + dir1) * 0.5f;
                            float dot = glm::dot(exc, exc);
                            if (dot > glm::epsilon<float>())
                            {
                                exc *= glm::clamp(1.0f / dot, 0.0f, 1000.0f);
                            }
                            outerPoints.exc(p1) = exc;
                        }

                        // create inner contour and re-adjust outer contour accordingly.
                        innerPoints.resize(outerPoints.size());
                        for (size_t p0 = 0; p0 < outerPoints.size(); ++p0)
                        {
                            innerPoints.pos(p0) = outerPoints.pos(p0) + (outerPoints.exc(p0) * halfLineWidth);
                            outerPoints.pos(p0) = outerPoints.pos(p0) - (outerPoints.exc(p0) * halfLineWidth);
                        }

                    }
                    else
                    {
                        // Calculate direction vectors
                        outerPoints.dir(0) = glm::normalize(outerPoints.pos(1) - outerPoints.pos(0)); // first point
                        outerPoints.dir(outerPoints.size()-2) = glm::normalize(outerPoints.pos(outerPoints.size()-1) - outerPoints.pos(outerPoints.size()-2)); // second last point
                        outerPoints.dir(outerPoints.size()-1) = outerPoints.dir(outerPoints.size()-2); // last point, which should be the same direction than the second last point.
                        for (size_t p0 = 1, p1 = 2; p0 < outerPoints.size()-2; ++p0, ++p1)
                        {
                            outerPoints.dir(p0) = glm::normalize(outerPoints.pos(p1) - outerPoints.pos(p0));
                        }

                        // Calculate excrusion vectors
                        outerPoints.exc(0) = glm::vec2(outerPoints.dir(0).y, -outerPoints.dir(0).x); // first point
                        outerPoints.exc(outerPoints.size()-1) = glm::vec2(outerPoints.dir(outerPoints.size()-1).y, -outerPoints.dir(outerPoints.size()-1).x); // last point
                        for (size_t p0 = 0, p1 = 1; p0 < outerPoints.size()-2; p0++, p1++)
                        {
                            // rotate direction vector by 90degree CW
                            glm::vec2 dir0 = glm::vec2(outerPoints.dir(p0).y, -outerPoints.dir(p0).x);
                            glm::vec2 dir1 = glm::vec2(outerPoints.dir(p1).y, -outerPoints.dir(p1).x);
                            glm::vec2 exc = (dir0 + dir1) * 0.5f;
                            float dot = glm::dot(exc, exc);
                            if (dot > glm::epsilon<float>())
                            {
                                exc *= glm::clamp(1.0f / dot, 0.0f, 1000.0f);
                            }
                            outerPoints.exc(p1) = exc;
                        }

                        const size_t outer0 = 0;
                        const size_t outerN = outerPoints.size();
                        outerPoints.resize(outerN*2);
                        const size_t inner0 = outerN;
                        const size_t innerN = outerPoints.size();

                        // Point[inner0] .. Point[innerN] populated by Point[outerN] .. Point[outer0] (reversed order)
                        for (size_t outerPt = outerN - 1, innerPt = inner0; innerPt < innerN; --outerPt, ++innerPt)
                        {
                            outerPoints.pos(innerPt) = outerPoints.pos(outerPt) + (outerPoints.exc(outerPt) * halfLineWidth);
                        }

                        // Point[outer0] .. Point[outerN]
                        for (size_t outerPt = outer0; outerPt < outerN; ++outerPt)
                        {
                            outerPoints.pos(outerPt) = outerPoints.pos(outerPt) - (outerPoints.exc(outerPt) * halfLineWidth);
                        }
                    }

                }

            }

            void triangulate(Path2D &path)
            {
                SubPathArray &subPaths = path.subPaths();
                glm::vec2 &boundTopLeft = path.boundTopLeft();
                glm::vec2 &boundBottomRight = path.boundBottomRight();
                boundTopLeft = glm::vec2(FLT_MAX);
                boundBottomRight = glm::vec2(-FLT_MAX);

                for (size_t i = 0; i < subPaths.size(); ++i)
                {
                    MPEPolyContext &polyContext = subPaths.polyContext(i);
                    MemPool &mempool = subPaths.mempool(i);
                    auto &outerPoints = subPaths.outerPoints(i);
                    auto &innerPoints = subPaths.innerPoints(i);

                    // The maximum number of points you expect to need
                    // This value is used by the library to calculate
                    // working memory required
                    uint32_t maxPointCount = static_cast<uint32_t>(outerPoints.size() + innerPoints.size());

                    // Request how much memory (in bytes) you should
                    // allocate for the library
                    size_t memoryRequired = MPE_PolyMemoryRequired(maxPointCount);

                    // Allocate a memory block of size MemoryRequired
                    // IMPORTANT: The memory must be zero initialized
                    mempool.resize(memoryRequired, 0);

                    // Initialize the poly context by passing the memory pointer,
                    // and max number of points from before
                    MPE_PolyInitContext(&polyContext, mempool.data(), maxPointCount);

                    // fill outer polypoints buffer.
                    if (outerPoints.size() > 2)
                    {
                        MPEPolyPoint* polyPoints = MPE_PolyPushPointArray(&polyContext, outerPoints.size());
                        for(size_t j = 0; j < outerPoints.size(); ++j)
                        {
                            glm::vec2 &point = outerPoints.pos(j);

                            polyPoints[j].X = point.x;
                            polyPoints[j].Y = point.y;

                            // update path bounds
                            boundTopLeft     = glm::min(boundTopLeft,     point);
                            boundBottomRight = glm::max(boundBottomRight, point);
                        }
                        MPE_PolyAddEdge(&polyContext);
                    }

                    // fill inner polypoints buffer.
                    if (innerPoints.size() > 2)
                    {
                        MPEPolyPoint* polyHoles = MPE_PolyPushPointArray(&polyContext, innerPoints.size());
                        for(size_t j = 0; j < innerPoints.size(); ++j)
                        {
                            glm::vec2 &point = innerPoints.pos(j);

                            polyHoles[j].X = point.x;
                            polyHoles[j].Y = point.y;

                            // update path bounds
                            boundTopLeft     = glm::min(boundTopLeft,     point);
                            boundBottomRight = glm::max(boundBottomRight, point);
                        }
                        MPE_PolyAddHole(&polyContext);
                    }

                    MPE_PolyTriangulate(&polyContext);
                }
            }

        };


    }

    Context::Context() :
        ctx(new detail::ContextPriv())
    {
    }

    Context::~Context()
    {
    }

    const char * Context::backendName() const
    {
        return "GL";
    }

    void Context::clearFrame(int32_t fbLeft, int32_t fbTop, int32_t fbWidth, int32_t fbHeight, Color backgroundColor)
    {
        ctx->clearFrame(std::move(fbLeft), std::move(fbTop),
                        std::move(fbWidth), std::move(fbHeight),
                        std::move(backgroundColor));
    }

    void Context::beginFrame(int32_t winWidth, int32_t winHeight, float devicePixelRatio)
    {
        ctx->beginFrame(std::move(winWidth), std::move(winHeight), std::move(devicePixelRatio));
    }

    void Context::endFrame()
    {
        ctx->endFrame();
    }

    void Context::save()
    {
        ctx->states.push_back(*this);
    }

    void Context::restore()
    {
        if (ctx->states.size() > 0)
        {
            *static_cast<ContextState*>(this) = ctx->states.back();
            ctx->states.pop_back();
        }
    }

    void Context::clearRect(float x, float y, float width, float height)
    {
        Paint origFillStyle = fillStyle;
        fillStyle = detail::gfxStates.backgroundColor;
        rect(x, y, width, height);
        fill();
        fillStyle = origFillStyle;
    }

    void Context::fill(Path2D &path, FillRule /*fillRule*/)
    {
        ctx->renderQueue.push(detail::DRAW_FILL,
                              path.clone<Path2D>(),
                              std::move(*this));
        path.reset();
    }

    void Context::stroke(Path2D &path)
    {
        ctx->renderQueue.push(detail::DRAW_STROKE,
                              path.clone<Path2D>(),
                              std::move(*this));
        path.reset();
    }


}
