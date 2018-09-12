#define MPE_POLY2TRI_IMPLEMENTATION
#define TUNIS_GL_IMPLEMENTATION

#ifndef TUNIS_VERTEX_MAX
#define TUNIS_VERTEX_MAX 16384
#endif

#ifdef _WIN32
#define NOMINMAX
#endif

//#ifndef TUNIS_MAX_TEXTURE_SIZE
#define TUNIS_MAX_TEXTURE_SIZE 2048
//#endif

#ifndef TUNIS_CURVE_RECURSION_LIMIT
#define TUNIS_CURVE_RECURSION_LIMIT 32
#endif

#include <cstdlib>

#include <Tunis.h>
#include <TunisGL.h>
#include <TunisVertex.h>
#include <TunisSpinLock.h>
#include <TunisGraphicStates.h>
#include <easy/profiler.h>

#include <glm/gtc/epsilon.hpp>

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

        class ShaderProgram
        {
        public:
            ShaderProgram() = default;
            virtual ~ShaderProgram() = default;
            virtual void init();
            virtual void shutdown();

            void useProgram();
            void setTexture0Uniform(int32_t value);
            void setViewSizeUniform(int32_t width, int32_t height);

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

            constexpr static uint16_t edgeLUT[] = {0, 1, 2, 0, 1, 2, 3};

            uint16_t addBatch(ShaderProgram *program, Texture texture, uint32_t vertexCount, uint32_t indexCount, Vertex **vout, Index **iout);
            void pushColorRect(float x, float y, float w, float h, const Color &color);
            void renderViewport(int w, int h, float devicePixelRatio);
            void addSubPath(SubPathArray &subPaths, float startX, float startY);
            MPEPolyPoint* addPoint(MPEPolyContext &ctx, float x, float y);
            float calcSqrtDistance(float x1, float y1, float x2, float y2);
            void bezier(MPEPolyContext &ctx, float x1, float y1, float x2, float y2,
                        float x3, float y3, float x4, float y4);
            void recursiveBezier(MPEPolyContext &ctx, float x1, float y1, float x2,
                                 float y2, float x3, float y3, float x4, float y4,
                                 int32_t level);
            void arc(SubPathArray &subPaths, float centerX, float centerY, float radius,
                     float angleStart, float angleEnd, bool anticlockwise);
            float distPtSeg(const glm::vec2 &c, const glm::vec2 &p, const glm::vec2 &q);
            void arcTo(SubPathArray &subPaths, float x1, float y1, float x2, float y2, float radius);
            void generateFillContour(Path2D &path);
            void triangulate(Path2D &path);
            void generateStrokeContour(Path2D &path);
        };

        void ShaderProgram::useProgram()
        {
            EASY_FUNCTION(profiler::colors::Red);
            if (gfxStates.programId != programId)
            {
                glUseProgram(programId);
                gfxStates.programId = programId;
            }
        }

        void ShaderProgram::setTexture0Uniform(int32_t value)
        {
            EASY_FUNCTION(profiler::colors::Red);
            assert(value >= 0 && value < 32);
            assert(gfxStates.programId == programId);

            if (texture0 != value)
            {
                glActiveTexture(GL_TEXTURE0 + static_cast<GLenum>(value));
                glUniform1i(u_texture0, value);
                texture0 = value;
            }
        }

        void ShaderProgram::setViewSizeUniform(int32_t width, int32_t height)
        {
            EASY_FUNCTION(profiler::colors::Red);
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

        void ShaderProgram::init()
        {
            EASY_FUNCTION(profiler::colors::Red);

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

        void ShaderProgram::shutdown()
        {
            EASY_FUNCTION(profiler::colors::Red);

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

        uint16_t ContextPriv::addBatch(ShaderProgram *program, Texture texture, uint32_t vertexCount, uint32_t indexCount, Vertex **vout, Index **iout)
        {
            EASY_FUNCTION(profiler::colors::DarkRed);

            assert(vertexCount >= 3);

            size_t istart = indexBuffer.size();
            size_t iend = istart + indexCount;
            indexBuffer.resize(iend);
            *iout = &indexBuffer[istart];

            size_t vstart = vertexBuffer.size();
            size_t vend = vstart + vertexCount;
            vertexBuffer.resize(vend);
            *vout = &vertexBuffer[vstart];

            if (batches.size() > 0)
            {
                size_t id = batches.size() - 1;

                if (batches.program(id) == program && batches.texture(id) == texture)
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

        void ContextPriv::pushColorRect(float x, float y, float w, float h, const Color &color)
        {
            EASY_FUNCTION(profiler::colors::DarkRed);

            /*
             * [0]<----------[3]
             *  | \           ^
             *  |   \  tri 2  |
             *  |     \       |
             *  |       \     |
             *  | tri 1   \   |
             *  V           \ |
             * [1]---------->[2]
             */

            Vertex *vertices;
            Index *indices;
            uint16_t offset = addBatch(&default2DProgram, textures.back(), 4, 6, &vertices, &indices);

            vertices[0] = {{x,   y  }, {0, 0}, color}; // top left
            vertices[1] = {{x,   y+h}, {0, 1}, color}; // bottom left
            vertices[2] = {{x+w, y+h}, {1, 1}, color}; // bottom right
            vertices[3] = {{x+w, y  }, {1, 0}, color}; // top right

            indices[0] = offset+0;
            indices[1] = offset+1;
            indices[2] = offset+2;
            indices[3] = offset+0;
            indices[4] = offset+2;
            indices[5] = offset+3;
        }

        void ContextPriv::renderViewport(int w, int h, float devicePixelRatio)
        {
            EASY_FUNCTION(profiler::colors::DarkRed);

            viewWidth = std::move(w);
            viewHeight = std::move(h);
            tessTol = 0.25f / devicePixelRatio;
            distTol = 0.01f / devicePixelRatio;

        }

        void ContextPriv::addSubPath(SubPathArray &subPaths, float startX, float startY)
        {
            EASY_FUNCTION(profiler::colors::DarkRed);

            size_t i = subPaths.size();
            subPaths.resize(i + 1);

            MPEPolyContext &polyContext = subPaths.polyContext(i);
            MemPool &mempool = subPaths.mempool(i);
            subPaths.closed(i) = false;

            // The maximum number of points you expect to need
            // This value is used by the library to calculate
            // working memory required
            uint32_t maxPointCount = 128;

            // Request how much memory (in bytes) you should
            // allocate for the library
            size_t memoryRequired = MPE_PolyMemoryRequired(maxPointCount);

            // Allocate a memory block of size MemoryRequired
            mempool.resize(memoryRequired);

            // IMPORTANT: The memory must be zero initialized
            std::fill(mempool.begin(), mempool.end(), 0);

            // Initialize the poly context by passing the memory pointer,
            // and max number of points from before
            MPE_PolyInitContext(&polyContext, mempool.data(), maxPointCount);

            MPEPolyPoint* startingPoint = MPE_PolyPushPoint(&polyContext);
            startingPoint->X = std::move(startX);
            startingPoint->Y = std::move(startY);
        }

        MPEPolyPoint* ContextPriv::addPoint(MPEPolyContext &ctx, float x, float y)
        {
            EASY_FUNCTION(profiler::colors::DarkRed);
            size_t i = ctx.PointPoolCount;
            if (i > 0)
            {
                --i;

                if (glm::epsilonEqual(ctx.PointsPool[i].X, x, distTol) && glm::epsilonEqual(ctx.PointsPool[i].Y, y, distTol))
                {
                    return &ctx.PointsPool[i];
                }
            }

            MPEPolyPoint* point = MPE_PolyPushPoint(&ctx);
            point->X = std::move(x);
            point->Y = std::move(y);
            point->FirstEdge = nullptr;
            return point;
        }


        // based of http://antigrain.com/__code/src/agg_curves.cpp.html by Maxim Shemanarev
        void ContextPriv::bezier(MPEPolyContext &ctx, float x1, float y1, float x2,
                                 float y2, float x3, float y3, float x4, float y4)
        {
            EASY_FUNCTION(profiler::colors::DarkRed);
            addPoint(ctx, x1, y1);
            recursiveBezier(ctx, x1, y1, x2, y2, x3, y3, x4, y4, 0);
            addPoint(ctx, x4, y4);
        }
        float ContextPriv::calcSqrtDistance(float x1, float y1, float x2, float y2)
        {
            EASY_FUNCTION(profiler::colors::DarkRed);
            float dx = x2-x1;
            float dy = y2-y1;
            return dx * dx + dy * dy;
        }
        void ContextPriv::recursiveBezier(MPEPolyContext &ctx,
                                          float x1, float y1,
                                          float x2, float y2,
                                          float x3, float y3,
                                          float x4, float y4,
                                          int32_t level)
        {
            EASY_FUNCTION(profiler::colors::DarkRed);

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
                            addPoint(ctx, x2, y2);
                            return;
                        }
                    }
                    else
                    {
                        if(d3 < tessTol)
                        {
                            addPoint(ctx, x3, y3);
                            return;
                        }
                    }
                    break;

                case 1:
                    // p1,p2,p4 are collinear, p3 is significant
                    //----------------------
                    if(d3 * d3 <= tessTol * (dx*dx + dy*dy))
                    {
                        addPoint(ctx, x23, y23);
                        return;
                    }
                    break;

                case 2:
                    // p1,p3,p4 are collinear, p2 is significant
                    //----------------------
                    if(d2 * d2 <= tessTol * (dx*dx + dy*dy))
                    {
                        addPoint(ctx, x23, y23);
                        return;
                    }
                    break;

                case 3:
                    // Regular case
                    //-----------------
                    if((d2 + d3)*(d2 + d3) <= tessTol * (dx*dx + dy*dy))
                    {
                        addPoint(ctx, x23, y23);
                        return;
                    }
                    break;
            }

            // Continue subdivision
            //----------------------
            recursiveBezier(ctx, x1, y1, x12, y12, x123, y123, x1234, y1234, level + 1);
            recursiveBezier(ctx, x1234, y1234, x234, y234, x34, y34, x4, y4, level + 1);
        }

        void ContextPriv::arc(SubPathArray &subPaths, float centerX, float centerY,
                              float radius, float startAngle, float endAngle,
                              bool anticlockwise)
        {
            EASY_FUNCTION(profiler::colors::DarkRed);
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
                addSubPath(subPaths, prevX, prevY);
            }
            else
            {
                addPoint(subPaths.polyContext(--id), prevX, prevY);
            }

            auto &ctx = subPaths.polyContext(id);

            for (int32_t segment = 1; segment <= segmentCount; ++segment)
            {
                float angle = startAngle + deltaAngle * (static_cast<float>(segment)/static_cast<float>(segmentCount));
                deltaX = glm::cos(angle);
                deltaY = glm::sin(angle);
                float x = centerX + deltaX * radius;
                float y = centerY + deltaY * radius;
                float tanX = -deltaY * tangentFactor * radius;
                float tanY = deltaX * tangentFactor * radius;

                bezier(ctx,
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

        float ContextPriv::distPtSeg(const glm::vec2 &c, const glm::vec2 &p, const glm::vec2 &q)
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

        void ContextPriv::arcTo(SubPathArray &subPaths, float x1, float y1, float x2, float y2, float radius)
        {
            auto &ctx = subPaths.polyContext(subPaths.size()-1);
            auto &prevPoint = ctx.PointsPool[ctx.PointPoolCount - 1];

            glm::vec2 p0(prevPoint.X, prevPoint.Y);
            glm::vec2 p1(x1, y1);
            glm::vec2 p2(x2, y2);

            if (glm::all(glm::epsilonEqual(p0, p1, distTol)))
            {
                addPoint(subPaths.polyContext(subPaths.size()-1), p1.x, p1.y);
                return;
            }

            if (glm::all(glm::epsilonEqual(p1, p2, distTol)))
            {
                addPoint(subPaths.polyContext(subPaths.size()-1), p1.x, p1.y);
                return;
            }

            if (distPtSeg(p1, p0, p2) < (distTol * distTol))
            {
                addPoint(subPaths.polyContext(subPaths.size()-1), p1.x, p1.y);
                return;
            }

            if (radius < distTol)
            {
                addPoint(subPaths.polyContext(subPaths.size()-1), p1.x, p1.y);
                return;
            }

            glm::vec2 d0 = glm::normalize(p0 - p1);
            glm::vec2 d1 = glm::normalize(p2 - p1);
            float a = glm::acos(glm::dot(d0, d1));
            float d = radius / glm::tan(a * 0.5f);

            if (d > 10000.0f) {
                addPoint(subPaths.polyContext(subPaths.size()-1), p1.x, p1.y);
                return;
            }

            float cp = d1.x * d0.y - d0.x*d1.y;

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


        void ContextPriv::generateFillContour(Path2D &path)
        {
            EASY_FUNCTION(profiler::colors::DarkRed);

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
                        addSubPath(subPaths, commands.param0(i), commands.param1(i));
                        break;
                    case LINE_TO:
                        if (subPaths.size() == 0) { addSubPath(subPaths, 0, 0); }
                        addPoint(subPaths.polyContext(subPaths.size()-1), commands.param0(i), commands.param1(i));
                        break;
                    case BEZIER_TO:
                    {
                        if (subPaths.size() == 0) { addSubPath(subPaths, 0, 0); }
                        auto &ctx = subPaths.polyContext(subPaths.size()-1);
                        auto &prevPoint = ctx.PointsPool[ctx.PointPoolCount - 1];
                        bezier(ctx,
                               prevPoint.X, prevPoint.Y,
                               commands.param0(i), commands.param1(i),
                               commands.param2(i), commands.param3(i),
                               commands.param4(i), commands.param5(i));

                        break;
                    }
                    case QUAD_TO:
                    {
                        if (subPaths.size() == 0) { addSubPath(subPaths, 0, 0); }
                        auto &ctx = subPaths.polyContext(subPaths.size()-1);
                        auto &prevPoint = ctx.PointsPool[ctx.PointPoolCount - 1];
                        float x0 = prevPoint.X;
                        float y0 = prevPoint.Y;
                        float cx = commands.param0(i);
                        float cy = commands.param1(i);
                        float x = commands.param2(i);
                        float y = commands.param3(i);
                        float c1x = x0 + 2.0f/3.0f*(cx - x0);
                        float c1y = y0 + 2.0f/3.0f*(cy - y0);
                        float c2x = x + 2.0f/3.0f*(cx - x);
                        float c2y = y + 2.0f/3.0f*(cy - y);
                        bezier(ctx, x0, y0, c1x, c1y, c2x, c2y, x, y);
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
                        if (subPaths.size() == 0) { addSubPath(subPaths, 0, 0); }
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
                        addSubPath(subPaths, x, y);
                        auto &ctx = subPaths.polyContext(id);
                        addPoint(ctx, x, y+h);
                        addPoint(ctx, x+w, y+h);
                        addPoint(ctx, x+h, y);
                        subPaths.closed(id) = true;
                        break;
                    }
                }
            }

        }

        void ContextPriv::generateStrokeContour(Path2D &path)
        {
            // TODO
            generateFillContour(path); // temporary
        }

        void ContextPriv::triangulate(Path2D &path)
        {
            SubPathArray &subPaths = path.subPaths();
            glm::vec2 &boundTopLeft = path.boundTopLeft();
            glm::vec2 &boundBottomRight = path.boundBottomRight();
            boundTopLeft = glm::vec2(FLT_MAX);
            boundBottomRight = glm::vec2(-FLT_MAX);

            for (size_t i = 0; i < subPaths.size(); ++i)
            {
                auto &polyContext = subPaths.polyContext(i);
                auto &points = polyContext.PointsPool;

                // Check if the first and last point are the same. Get rid of
                // the last point if that is the case, and close the subpath.
                if (glm::epsilonEqual(points[0].X, points[polyContext.PointPoolCount-1].X, distTol) &&
                    glm::epsilonEqual(points[0].Y, points[polyContext.PointPoolCount-1].Y, distTol))
                {
                    --polyContext.PointPoolCount;
                    subPaths.closed(i) = true;
                }

                // check if we're still a polygon, reset the subpath if we lost
                // this status. we need a minimum of 3 points to do a proper fill.
                if (polyContext.PointPoolCount >= 3)
                {
                    MPE_PolyAddEdge(&polyContext);
                    MPE_PolyTriangulate(&polyContext);
                }

                // update path bounds
                for(uint32_t j = 0; j < polyContext.PointCount; ++j)
                {
                    boundTopLeft     = glm::min(boundTopLeft,     glm::vec2(polyContext.Points[j]->X, polyContext.Points[j]->Y));
                    boundBottomRight = glm::max(boundBottomRight, glm::vec2(polyContext.Points[j]->X, polyContext.Points[j]->Y));
                }

            }
        }


    }

    Context::Context() :
        ctx(new detail::ContextPriv())
    {
        EASY_FUNCTION(profiler::colors::RichRed);

        auto tunisGL_initialized = tunisGLInit();
        if (!tunisGL_initialized)
        {
            abort();
        }

        // default state.
        ctx->states.push_back(*this);

        // Create a default texture atlas.
#ifdef TUNIS_MAX_TEXTURE_SIZE
        detail::gfxStates.maxTexSize = TUNIS_MAX_TEXTURE_SIZE;
#else
        glGetIntegerv(GL_MAX_TEXTURE_SIZE, &detail::global.maxTexSize);
#endif
        Texture tex = Texture(detail::gfxStates.maxTexSize, detail::gfxStates.maxTexSize);
        ctx->textures.push_back(tex); // retain

        Paint::reserve(64);
        Path2D::reserve(64);
        ctx->renderQueue.reserve(1024);
        ctx->batches.reserve(1024);

        ctx->vertexBuffer.reserve(TUNIS_VERTEX_MAX);
        ctx->indexBuffer.reserve((TUNIS_VERTEX_MAX-2)*3);

        // Initialize our default shader.
        ctx->default2DProgram.init();

        if (tunisGLSupport(GL_VERSION_3_0))
        {
            // Create a dummy vertex array object (mandatory since GL Core profile)
            glGenVertexArrays(1, &ctx->vao);
            glBindVertexArray(ctx->vao);
        }

        // Create vertex and index buffer objects for the batches
        glGenBuffers(2, ctx->buffers);
        glBindBuffer(GL_ARRAY_BUFFER, ctx->buffers[detail::ContextPriv::VBO]);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ctx->buffers[detail::ContextPriv::IBO]);

        // Configure our vertex attributes
        glVertexAttribPointer(static_cast<GLuint>(ctx->default2DProgram.a_position), decltype(Vertex::pos)::length(),    GL_FLOAT,          GL_FALSE, sizeof(Vertex), reinterpret_cast<const void *>(0));
        glVertexAttribPointer(static_cast<GLuint>(ctx->default2DProgram.a_texcoord), decltype(Vertex::tcoord)::length(), GL_UNSIGNED_SHORT, GL_TRUE,  sizeof(Vertex), reinterpret_cast<const void *>(sizeof(Vertex::pos)));
        glVertexAttribPointer(static_cast<GLuint>(ctx->default2DProgram.a_color),    decltype(Vertex::color)::length(),  GL_UNSIGNED_BYTE,  GL_TRUE,  sizeof(Vertex), reinterpret_cast<const void *>(sizeof(Vertex::pos) + sizeof(Vertex::tcoord)));
        glEnableVertexAttribArray(static_cast<GLuint>(ctx->default2DProgram.a_position));
        glEnableVertexAttribArray(static_cast<GLuint>(ctx->default2DProgram.a_texcoord));
        glEnableVertexAttribArray(static_cast<GLuint>(ctx->default2DProgram.a_color));

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

    Context::~Context()
    {
        EASY_FUNCTION(profiler::colors::RichRed);

        // unload texture data by deleting every potential texture holders.
        ctx->textures.resize(0);
        ctx->batches.resize(0);
        fillStyle.reset();
        strokeStyle.reset();

        // unload shader program
        ctx->default2DProgram.shutdown();

        // unload vertex and index buffers
        glDeleteBuffers(2, ctx->buffers);
        ctx->buffers[detail::ContextPriv::VBO] = 0;
        ctx->buffers[detail::ContextPriv::IBO] = 0;

        // reset global states.
        detail::gfxStates = detail::GraphicStates();

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



    void Context::clearFrame(int32_t fbLeft, int32_t fbTop, int32_t fbWidth, int32_t fbHeight, Color backgroundColor)
    {
        EASY_FUNCTION(profiler::colors::RichRed);

        // update the clear color if necessary
        if (detail::gfxStates.backgroundColor != backgroundColor)
        {
            glClearColor(backgroundColor.r/255.0f,
                         backgroundColor.g/255.0f,
                         backgroundColor.b/255.0f,
                         backgroundColor.a/255.0f);

            detail::gfxStates.backgroundColor = backgroundColor;
        }

        // update the viewport if necessary
        Viewport viewport(fbLeft, fbTop, fbWidth, fbHeight);
        if (detail::gfxStates.viewport != viewport)
        {
            glViewport(fbLeft, fbTop, fbWidth, fbHeight);
            detail::gfxStates.viewport = viewport;
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    }

    void Context::beginFrame(int32_t winWidth, int32_t winHeight, float devicePixelRatio)
    {
        EASY_FUNCTION(profiler::colors::RichRed);

        // reset the states.
        ctx->states.resize(0);
        *static_cast<ContextState*>(this) = ContextState();

        ctx->renderViewport(std::move(winWidth), std::move(winHeight), std::move(devicePixelRatio));
    }

    void Context::endFrame()
    {
        EASY_FUNCTION(profiler::colors::RichRed);

        #pragma omp parallel for num_threads(std::thread::hardware_concurrency())
        for (int i = 0; i < ctx->renderQueue.size(); ++i)
        {
            auto &path = ctx->renderQueue.path(i);
            if (path.dirty())
            {
                switch(ctx->renderQueue.op(i))
                {
                    case detail::DRAW_FILL:
                        ctx->generateFillContour(path);
                        break;
                    case detail::DRAW_STROKE:
                        ctx->generateStrokeContour(path);
                        break;
                }

                ctx->triangulate(path);

                path.dirty() = false;
            }
        }

        for (size_t i = 0; i < ctx->renderQueue.size(); ++i)
        {
            auto &path = ctx->renderQueue.path(i);
            auto &state = ctx->renderQueue.state(i);

            glm::vec2 range = path.boundBottomRight() - path.boundTopLeft();

            for(size_t j = 0; j < path.subPaths().size(); ++j)
            {
                MPEPolyContext &polyContext = path.subPaths().polyContext(j);

                uint32_t vertexCount = polyContext.PointPoolCount;
                uint32_t indexCount = polyContext.TriangleCount*3;

                if (vertexCount < 3)
                {
                    continue; // not enough vertices to make a fill. Skip
                }

                Vertex *verticies;
                Index *indices;
                uint16_t offset = ctx->addBatch(&ctx->default2DProgram, ctx->textures.back(), vertexCount, indexCount, &verticies, &indices);

                //populate the vertices
                for (size_t i = 0; i < polyContext.PointPoolCount; ++i)
                {
                    MPEPolyPoint &Point = polyContext.PointsPool[i];
                    Position pos(Point.X, Point.Y);
                    glm::vec2 tcoord = TCoord(((pos - path.boundTopLeft()) / range) * 16.0f / static_cast<float>(detail::gfxStates.maxTexSize) * static_cast<float>(0xFFFF));
                    verticies[i].pos = pos;
                    verticies[i].tcoord.x = static_cast<uint16_t>(tcoord.s);
                    verticies[i].tcoord.t = static_cast<uint16_t>(tcoord.t);
                    verticies[i].color = state.fillStyle.innerColor();
                }

                //populate the indicies
                for (size_t i = 0; i < polyContext.TriangleCount; ++i)
                {
                    MPEPolyTriangle* triangle = polyContext.Triangles[i];

                    // get the array index by pointer address arithmetic.
                    uint16_t p0 = static_cast<uint16_t>(triangle->Points[0] - polyContext.PointsPool);
                    uint16_t p1 = static_cast<uint16_t>(triangle->Points[1] - polyContext.PointsPool);
                    uint16_t p2 = static_cast<uint16_t>(triangle->Points[2] - polyContext.PointsPool);

                    size_t j = i * 3;
                    indices[j+0] = offset+p2;
                    indices[j+1] = offset+p1;
                    indices[j+2] = offset+p0;
                }
            }
        }

        ctx->renderQueue.resize(0);

        // flush the vertex buffer.
        if (ctx->vertexBuffer.size() > 0) {
            glBufferData(GL_ARRAY_BUFFER,
                         static_cast<GLsizeiptr>(ctx->vertexBuffer.size() * sizeof(Vertex)),
                         &ctx->vertexBuffer.front(),
                         GL_STREAM_DRAW);
            ctx->vertexBuffer.resize(0);
        }


        // flush the index buffer.
        if (ctx->indexBuffer.size() > 0) {
            glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                         static_cast<GLsizeiptr>(ctx->indexBuffer.size() * sizeof(uint16_t)),
                         &ctx->indexBuffer.front(),
                         GL_STREAM_DRAW);
            ctx->indexBuffer.resize(0);
        }


        // flush the batches
        for (size_t i = 0; i < ctx->batches.size(); ++i)
        {
            ctx->batches.program(i)->useProgram();
            ctx->batches.program(i)->setViewSizeUniform(ctx->viewWidth, ctx->viewHeight);
            ctx->batches.texture(i).bind();


#if 1
            glDrawElements(GL_TRIANGLES,
                           static_cast<GLsizei>(ctx->batches.count(i)),
                           GL_UNSIGNED_SHORT,
                           reinterpret_cast<void*>(ctx->batches.offset(i) * sizeof(GLushort)));
#else
            // Helpful code for debugging triangles.
            for (size_t j = 0; j < ctx->batches.count(i)/3; ++j)
            {
                glDrawElements(GL_LINE_LOOP,
                               3,
                               GL_UNSIGNED_SHORT,
                               reinterpret_cast<void*>((ctx->batches.offset(i)+(j*3)) * sizeof(GLushort)));
            }
#endif
        }

        ctx->batches.resize(0);
    }

    void Context::save()
    {
        EASY_FUNCTION(profiler::colors::RichRed);
        ctx->states.push_back(*this);
    }

    void Context::restore()
    {
        EASY_FUNCTION(profiler::colors::RichRed);
        if (ctx->states.size() > 0)
        {
            *static_cast<ContextState*>(this) = ctx->states.back();
            ctx->states.pop_back();
        }
    }

    void Context::fillRect(float x, float y, float width, float height)
    {
        EASY_FUNCTION(profiler::colors::RichRed);
        ctx->pushColorRect(std::move(x), std::move(y), std::move(width), std::move(height), fillStyle.innerColor());
    }

    void Context::strokeRect(float x, float y, float width, float height)
    {
        EASY_FUNCTION(profiler::colors::RichRed);

        // helps to get the lines a bit sharper when the line width is not divisible by two.
        float offset = fmodf(lineWidth, 2.0f) / 2.0f;
        x+=offset;
        y+=offset;

        float hlw = lineWidth * 0.5f;

        // top line
        ctx->pushColorRect(x-hlw, y-hlw, width+lineWidth-1, lineWidth, strokeStyle.innerColor());

        // bottom line
        ctx->pushColorRect(x-hlw, y+height-hlw, width+lineWidth-1, lineWidth, strokeStyle.innerColor());

        // left line
        ctx->pushColorRect(x-hlw, y+hlw, lineWidth, height-lineWidth, strokeStyle.innerColor());

        // right line
        ctx->pushColorRect(x+width-hlw-1, y+hlw, lineWidth, height-lineWidth, strokeStyle.innerColor());
    }

    void Context::clearRect(float x, float y, float width, float height)
    {
        EASY_FUNCTION(profiler::colors::RichRed);
        ctx->pushColorRect(std::move(x), std::move(y), std::move(width), std::move(height), detail::gfxStates.backgroundColor);
    }

    void Context::fill(Path2D &path, FillRule /*fillRule*/)
    {
        EASY_FUNCTION(profiler::colors::RichRed);
        ctx->renderQueue.push(detail::DRAW_FILL,
                              path.clone<Path2D>(),
                              std::move(*this));
    }

    void Context::stroke(Path2D &path)
    {
        EASY_FUNCTION(profiler::colors::RichRed);
        ctx->renderQueue.push(detail::DRAW_STROKE,
                              path.clone<Path2D>(),
                              std::move(*this));
    }


}
