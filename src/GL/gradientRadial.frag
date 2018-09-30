R"(

#if defined(GL_ES)
precision highp float;
#endif

varying vec2 v_texcoord;
varying vec4 v_color;

void main()
{
    vec2 start = gl_FragCoord.xy/vec2(100, 100);
    vec2 end = vec2(1,1);
    float t = distance(start, end);

    vec3 color1 = vec3(1.0, 0, 0);
    vec3 color2 = vec3(  0, 0, 1.0);

    vec3 color = mix(color1, color2, t);

    gl_FragColor = vec4(color, 1.0);
};

)"
