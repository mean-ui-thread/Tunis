R"(

/*******************************************************************************
 * Special thanks to
 *
 *   Alex Sabourin
 *   Zack Dawson
 *
 * For helping me figure this out.
 ******************************************************************************/

#if defined(GL_ES)
precision highp float;
#endif

uniform vec2 u_viewSize;
uniform vec4 u_uniforms[7];

#define u_dt               u_uniforms[0].xy
#define u_focal            u_uniforms[0].zw
#define u_r0               u_uniforms[1].x
#define u_dr               u_uniforms[1].y
#define u_a                u_uniforms[1].z
#define u_colorStopCount   int(u_uniforms[1].w)
#define u_colorStop(INDEX) u_uniforms[2][INDEX]
#define u_color(INDEX)     u_uniforms[3+(INDEX)]

void main()
{
    float x = u_focal.x - gl_FragCoord.x;
    float y = u_focal.y - gl_FragCoord.y;
    float b = -2.0 * (y * u_dt.y + x * u_dt.x + u_r0 * u_dr);
    float c = x*x + y*y - u_r0*u_r0;
    float t = 1.0 - (0.5/u_a) * (-b + sqrt(b*b - 4.0*u_a*c));

    vec4 color = mix(u_color(0), u_color(1), clamp((t - u_colorStop(0))/(u_colorStop(1) - u_colorStop(0)), 0.0, 1.0));
    for (int i = 2; i < u_colorStopCount; ++i)
    {
        color = mix(color, u_color(i), clamp((t - u_colorStop(i-1))/(u_colorStop(i) - u_colorStop(i-1)), 0.0, 1.0));
    }

    gl_FragColor = color;

};

)"
