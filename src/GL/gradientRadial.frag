R"(

#if defined(GL_ES)
precision highp float;
#endif

uniform vec2 u_viewSize;
uniform vec4 u_frag[7];

#define u_x0         u_frag[0].x
#define u_y0         u_frag[0].y
#define u_x1         u_frag[0].z
#define u_y1         u_frag[0].w
#define u_radius0    u_frag[1].x
#define u_radius1    u_frag[1].y
#define U_UNUSED0    u_frag[1].z
#define U_UNUSED1    u_frag[1].w
#define u_colorStop0 u_frag[2].x
#define u_colorStop1 u_frag[2].y
#define u_colorStop2 u_frag[2].z
#define u_colorStop3 u_frag[2].w
#define u_color0     u_frag[3]
#define u_color1     u_frag[4]
#define u_color2     u_frag[5]
#define u_color3     u_frag[6]

void main()
{
    vec2 center = vec2(u_x0, u_viewSize.y - u_y0);
    vec2 focal = vec2(u_x1, u_viewSize.y - u_y1);

    vec2 centerDir = center - focal;
    vec2 dir = center - gl_FragCoord.xy;
    vec2 N = normalize(vec2(-dir.y, dir.x));
    float focalDist = abs(dot(centerDir, N) / length(N));
    float cutoff = sqrt(u_radius1 * u_radius1 - focalDist * focalDist) + (step(dot(dir, centerDir) / distance(dir , centerDir) , 0.0) * -2.0 + 1.0 ) * abs(dot(centerDir, dir) / length(dir));


    float expand = u_radius0 / u_radius1;
    float dist = (min(1.0, abs(length(gl_FragCoord.xy - center)) / cutoff) - expand) / (1.0 - expand);

    vec4 color;
    color = mix(u_color0, u_color1, smoothstep(u_colorStop0, u_colorStop1, dist));
    color = mix(color, u_color2, smoothstep(u_colorStop1, u_colorStop2, dist));
    //color = mix(color, u_color3, smoothstep(u_colorStop2, u_colorStop3, dist));

    gl_FragColor = color;

};

)"
