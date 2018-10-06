R"(

#if defined(GL_ES)
precision highp float;
#endif

uniform vec2 u_viewSize;
uniform vec4 u_uniforms[7];

#define u_start            u_uniforms[0].xy
#define u_dt              u_uniforms[0].zw
#define u_lenSq            u_uniforms[1].x
#define u_colorStopCount   int(u_uniforms[1].w)
#define u_colorStop(INDEX) u_uniforms[2][INDEX]
#define u_color(INDEX)     u_uniforms[3+(INDEX)]

void main()
{
    float t = dot(gl_FragCoord.xy - u_start, u_dt)/ u_lenSq;

    vec4 color = mix(u_color(0), u_color(1), clamp((t - u_colorStop(0))/(u_colorStop(1) - u_colorStop(0)), 0.0, 1.0));
    for (int i = 2; i < u_colorStopCount; ++i)
    {
        color = mix(color, u_color(i), clamp((t - u_colorStop(i-1))/(u_colorStop(i) - u_colorStop(i-1)), 0.0, 1.0));
    }

    gl_FragColor = color;
};

)"
