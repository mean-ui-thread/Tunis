#if defined(_MSC_VER)
#include <basetsd.h>
#include <stdlib.h>
#include <search.h>
typedef SSIZE_T ssize_t;
#endif
#define MPE_POLY2TRI_IMPLEMENTATION
#include <MPE_fastpoly2tri.h>

#define TUNIS_GL_IMPLEMENTATION
#include <TunisGL.h>

#include <TunisContextData.h>
tunis::detail::GraphicStates tunis::detail::gfxStates;


#include <Tunis.h>

namespace tunis
{

namespace detail
{

void* poolAlloc( void* userData, uint32_t size )
{
    MemPool &pool = *reinterpret_cast<MemPool*>(userData);
    size = (size+0x7) & ~0x7;
    if (pool.size() + size < pool.capacity())
    {
        uint8_t* ptr = pool.data() + pool.size();
        pool.resize(pool.size() + size);
        return ptr;
    }
    printf("out of mem: %zu < %zu!\n", pool.size() + size, pool.capacity());
    return nullptr;
}

void poolFree( void* userData, void* ptr )
{
    // empty
    TESS_NOTUSED(userData);
    TESS_NOTUSED(ptr);
}

void ContextData::fill(Path2D &path, Fill fillRule, Paint &fillStyle)
{
    updatePath(path, fillRule);

    auto &subpaths = path.subpaths();

    glm::vec2 range = path.boundBottomRight() - path.boundTopLeft();

#if 1

    size_t vertexCount = 0;
    for(size_t i = 0; i < subpaths.size(); ++i)
    {
        auto &pts = subpaths.points(i);

        if (pts.size() == 0)
        {
            continue;
        }

        vertexCount += pts.size();

        size_t leftTurnCount = 0;
        size_t prevPointID = pts.size() - 1;
        for (size_t j = 0; j < pts.size(); j++)
        {
            glm::vec2 &dir = pts.dir(j);
            glm::vec2 &prevDir = pts.dir(prevPointID);

            float cross = dir.x * prevDir.y - prevDir.x * dir.y;
            if (cross > 0.0f)
            {
                ++leftTurnCount;
            }

            prevPointID = j;
        }

        // convexity test.
        if (leftTurnCount == pts.size()) // convex
        {
            Vertex *verticies;
            addBatch(&default2DProgram, textures.back(), pts.size(), &verticies);

            for (size_t j = 0; j < pts.size(); ++j)
            {
                glm::vec2 &pos = pts.pos(j);
                glm::vec2 tcoord = TCoord(((pos.x - path.boundTopLeft()) / range) * 16.0f / static_cast<float>(detail::gfxStates.maxTexSize) * static_cast<float>(0xFFFF));
                verticies[j].pos = pos;
                verticies[j].tcoord.x = static_cast<uint16_t>(tcoord.s);
                verticies[j].tcoord.t = static_cast<uint16_t>(tcoord.t);
                verticies[j].color = fillStyle.innerColor();
            }
        }
        else // concave
        {
            MPEPolyContext PolyContext;
            memset(pool.data(), 0, pool.size());
            if (MPE_PolyInitContext(&PolyContext, pool.data(), TUNIS_VERTEX_MAX))
            {
                MPEPolyPoint* FirstPoint = MPE_PolyPushPointArray(&PolyContext, static_cast<uint32_t>(pts.size()));
                for (size_t j = 0; j < pts.size(); ++j)
                {
                    FirstPoint[j].X = pts.pos(j).x;
                    FirstPoint[j].Y = pts.pos(j).y;
                }

                // Add the polyline for the edge. This will consume all points added so far.
                MPE_PolyAddEdge(&PolyContext);

                // Triangulate the shape
                MPE_PolyTriangulate(&PolyContext);

                Vertex *verticies;
                size_t vertexCount = PolyContext.TriangleCount * 3;
                addBatch(&default2DProgram, textures.back(), vertexCount, &verticies);

                // The resulting triangles can be used like so
                for (size_t TriangleIndex = 0; TriangleIndex < PolyContext.TriangleCount; ++TriangleIndex)
                {
                    MPEPolyTriangle* Triangle = PolyContext.Triangles[TriangleIndex];

                    for(size_t j = 0; j < 3; ++j)
                    {
                        MPEPolyPoint* Point = Triangle->Points[2-j];
                        Position pos(Point->X, Point->Y);
                        glm::vec2 tcoord = TCoord(((pos.x - path.boundTopLeft()) / range) * 16.0f / static_cast<float>(detail::gfxStates.maxTexSize) * static_cast<float>(0xFFFF));
                        size_t id = (TriangleIndex*3)+j;
                        verticies[id].pos = pos;
                        verticies[id].tcoord.x = static_cast<uint16_t>(tcoord.s);
                        verticies[id].tcoord.t = static_cast<uint16_t>(tcoord.t);
                        verticies[id].color = fillStyle.innerColor();
                    }
                }
            }
        }

    }
#else

    pool.resize(0); // reset pool
    TESStesselator* tess = tessNewTess(&ma);
    tessSetOption(tess, TESS_CONSTRAINED_DELAUNAY_TRIANGULATION, 1);

    const int nvp = 3;
    unsigned char* vflags = nullptr;
    int nvflags = 0;

    for (size_t i = 0; i < subpaths.size(); i++)
    {

        auto &pts = subpaths.points(i);

        tessAddContour(tess,
                       decltype(Vertex::pos)::length(),
                       &pts.pos(0),
                       sizeof(Vertex::pos),
                       static_cast<int>(pts.size()));


    }

    //TessWindingRule windingRule = fillRule == FillNonZero ? TESS_WINDING_NONZERO : TESS_WINDING_ODD;

    // First combine contours and then triangulate, this removes unnecessary inner vertices.
    if (tessTesselate(tess, TESS_WINDING_POSITIVE, TESS_BOUNDARY_CONTOURS, 0, 0, nullptr))
    {
        const float* verts = tessGetVertices(tess);
        const int* vinds = tessGetVertexIndices(tess);
        const int nverts = tessGetVertexCount(tess);
        const int* elems = tessGetElements(tess);
        const int nelems = tessGetElementCount(tess);

        if (nverts > nvflags)
        {
            if (vflags)
            {
                free(vflags);
            }
            nvflags = nverts;
            vflags = reinterpret_cast<unsigned char*>(malloc(static_cast<size_t>(nvflags)));
        }

        if (vflags)
        {
            // Vertex indices describe the order the indices were added and can be used
            // to map the tesselator output to input. Vertices marked as TESS_UNDEF
            // are the ones that were created at the intersection of segments.
            // That is, if vflags is set it means that the vertex comes from intersegment.
            for (int i = 0; i < nverts; ++i)
            {
                vflags[i] = vinds[i] == TESS_UNDEF ? 1 : 0;
            }
        }

        for (int i = 0; i < nelems; ++i)
        {
            int b = elems[i*2];
            int n = elems[i*2+1];
            tessAddContour(tess, 2, &verts[b*2], sizeof(float)*2, n);
        }
        if (!tessTesselate(tess, TESS_WINDING_POSITIVE, TESS_POLYGONS, nvp, 2, nullptr))
            tess = nullptr;
    }
    else
    {
        tess = nullptr;
    }

    // batch tesselated pieces.
    if (tess)
    {
        const float* verts = tessGetVertices(tess);
        const int nverts = tessGetVertexCount(tess);
        const int* vinds = tessGetVertexIndices(tess);
        const int* elems = tessGetElements(tess);
        const int nelems = tessGetElementCount(tess);

        Vertex *verticies;
        addBatch(&default2DProgram, textures.back(),
                 static_cast<size_t>(nverts), &verticies);

        for (int i = 0; i < nelems; ++i)
        {
            const int* p = &elems[i*3];
            for (int j = 0; j < 3 && p[j] != TESS_UNDEF; ++j)
            {
                glm::vec2 pos(verts[p[j]*2], verts[p[j]*2+1]);
                glm::vec2 tcoord = TCoord(((pos.x - path.boundTopLeft()) / range) * 16.0f / static_cast<float>(detail::gfxStates.maxTexSize) * static_cast<float>(0xFFFF));

                int id = (i*3)+j;

                verticies[id].pos = pos;
                verticies[id].tcoord.x = static_cast<uint16_t>(tcoord.s);
                verticies[id].tcoord.t = static_cast<uint16_t>(tcoord.t);
                verticies[id].color = fillStyle.innerColor();

            }
        }

    }


    tessDeleteTess(tess);
#endif

}

}

Context::Context()
{
    EASY_FUNCTION(profiler::colors::Coral);

    auto tunisGL_initialized = tunisGLInit();
    if (!tunisGL_initialized)
    {
        abort();
    }

#if 0
    ctx.pool.reserve(1024*1024);
    memset(&ctx.ma, 0, sizeof(ctx.ma));
    ctx.ma.memalloc = detail::poolAlloc;
    ctx.ma.memfree = detail::poolFree;
    ctx.ma.userData = &ctx.pool;
    ctx.ma.extraVertices = 256; // realloc not provided, allow 256 extra vertices.
#else
    // Request how much memory (in bytes) you should
    // allocate for the library
    size_t MemoryRequired = MPE_PolyMemoryRequired(TUNIS_VERTEX_MAX);

    // Allocate a memory block of size MemoryRequired
    // IMPORTANT: The memory must be zero initialized
    ctx.pool.resize(MemoryRequired, 0);

#endif

    // default state.
    ctx.states.push_back(*this);

    // Create a default texture atlas.
#ifdef TUNIS_MAX_TEXTURE_SIZE
    detail::gfxStates.maxTexSize = TUNIS_MAX_TEXTURE_SIZE;
#else
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &detail::global.maxTexSize);
#endif
    Texture tex = Texture(detail::gfxStates.maxTexSize, detail::gfxStates.maxTexSize);
    ctx.textures.push_back(tex); // retain

    Paint::reserve(1024);
    Path2D::reserve(1024);
    ctx.batches.reserve(1024);

    ctx.vertexBuffer.reserve(TUNIS_VERTEX_MAX);

    // Initialize our default shader.
    ctx.default2DProgram.init();

    if (tunisGLSupport(GL_VERSION_3_0))
    {
        // Create a dummy vertex array object (mandatory since GL Core profile)
        glGenVertexArrays(1, &ctx.vao);
        glBindVertexArray(ctx.vao);
    }

    // Create vertex and index buffer objects for the batches
    glGenBuffers(2, ctx.buffers);
    glBindBuffer(GL_ARRAY_BUFFER, ctx.buffers[detail::ContextData::VBO]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ctx.buffers[detail::ContextData::IBO]);

    // Configure our vertex attributes
    glVertexAttribPointer(static_cast<GLuint>(ctx.default2DProgram.a_position), decltype(Vertex::pos)::length(),    GL_FLOAT,          GL_FALSE, sizeof(Vertex), reinterpret_cast<const void *>(0));
    glVertexAttribPointer(static_cast<GLuint>(ctx.default2DProgram.a_texcoord), decltype(Vertex::tcoord)::length(), GL_UNSIGNED_SHORT, GL_TRUE,  sizeof(Vertex), reinterpret_cast<const void *>(sizeof(Vertex::pos)));
    glVertexAttribPointer(static_cast<GLuint>(ctx.default2DProgram.a_color),    decltype(Vertex::color)::length(),  GL_UNSIGNED_BYTE,  GL_TRUE,  sizeof(Vertex), reinterpret_cast<const void *>(sizeof(Vertex::pos) + sizeof(Vertex::tcoord)));
    glEnableVertexAttribArray(static_cast<GLuint>(ctx.default2DProgram.a_position));
    glEnableVertexAttribArray(static_cast<GLuint>(ctx.default2DProgram.a_texcoord));
    glEnableVertexAttribArray(static_cast<GLuint>(ctx.default2DProgram.a_color));

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
    EASY_FUNCTION(profiler::colors::Coral);

    // unload texture data by deleting every potential texture holders.
    ctx.textures.resize(0);
    ctx.batches.resize(0);
    fillStyle.reset();
    strokeStyle.reset();

    // unload shader program
    ctx.default2DProgram.shutdown();

    // unload vertex and index buffers
    glDeleteBuffers(2, ctx.buffers);
    ctx.buffers[detail::ContextData::VBO] = 0;
    ctx.buffers[detail::ContextData::IBO] = 0;

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



}
