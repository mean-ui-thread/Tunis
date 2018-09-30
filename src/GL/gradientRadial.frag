R"(

#if defined(GL_ES)
precision highp float;
#endif

uniform vec4 u_frag[2];

//#define u_resolution     u_frag[0].xy
//#define u_gradientCenter u_frag[0].zw
//#define u_radius         u_frag[1].x

void main()
{
    vec2 u_resolution = vec2(200, 200);
    vec2 u_gradientCenter = vec2(100, 100);
    float u_radius = 25.0;

    vec2 fragPos = gl_FragCoord.xy / u_resolution;
    float t = distance(fragPos, u_gradientCenter) * u_radius;

    vec4 color1 = vec4(1, 0, 0, 1);
    vec4 color2 = vec4(0, 0, 1, 1);

    vec4 color = mix(color1, color2, t);

    gl_FragColor = color;
};

)"
