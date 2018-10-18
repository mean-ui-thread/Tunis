#include "TunisGlyphLoader.h"

#include FT_OUTLINE_H

using namespace msdfgen;

struct FtContext {
    Point2 position;
    Shape *shape;
    Contour *contour;
};

static Point2 ftPoint2(const FT_Vector &vector) {
    return Point2(vector.x/64., vector.y/64.);
}

static int ftMoveTo(const FT_Vector *to, void *user) {
    FtContext *context = reinterpret_cast<FtContext *>(user);
    context->contour = &context->shape->addContour();
    context->position = ftPoint2(*to);
    return 0;
}

static int ftLineTo(const FT_Vector *to, void *user) {
    FtContext *context = reinterpret_cast<FtContext *>(user);
    context->contour->addEdge(new LinearSegment(context->position, ftPoint2(*to)));
    context->position = ftPoint2(*to);
    return 0;
}

static int ftConicTo(const FT_Vector *control, const FT_Vector *to, void *user) {
    FtContext *context = reinterpret_cast<FtContext *>(user);
    context->contour->addEdge(new QuadraticSegment(context->position, ftPoint2(*control), ftPoint2(*to)));
    context->position = ftPoint2(*to);
    return 0;
}

static int ftCubicTo(const FT_Vector *control1, const FT_Vector *control2, const FT_Vector *to, void *user) {
    FtContext *context = reinterpret_cast<FtContext *>(user);
    context->contour->addEdge(new CubicSegment(context->position, ftPoint2(*control1), ftPoint2(*control2), ftPoint2(*to)));
    context->position = ftPoint2(*to);
    return 0;
}

bool tunis::loadGlyph(Shape &output, FT_Face face, FT_ULong unicode)
{
    FT_Error error = FT_Load_Char(face, unicode, FT_LOAD_NO_SCALE);
    if (error)
        return false;

    return loadGlyph(output, face->glyph);
}

bool tunis::loadGlyph(Shape &output, FT_GlyphSlot glyph)
{
    if (!glyph)
        return false;
    output.contours.clear();
    output.inverseYAxis = false;

    FtContext context = { };
    context.shape = &output;
    FT_Outline_Funcs ftFunctions;
    ftFunctions.move_to = &ftMoveTo;
    ftFunctions.line_to = &ftLineTo;
    ftFunctions.conic_to = &ftConicTo;
    ftFunctions.cubic_to = &ftCubicTo;
    ftFunctions.shift = 0;
    ftFunctions.delta = 0;
    FT_Error error = FT_Outline_Decompose(&glyph->outline, &ftFunctions, &context);
    if (error)
        return false;
    return true;
}
