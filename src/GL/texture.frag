R"(

#if defined(GL_ES)
precision highp float;
#endif

varying vec2 v_texcoord;
varying vec4 v_color;

uniform sampler2D u_texture0;

void main()
{
    gl_FragColor = texture2D(u_texture0, v_texcoord) * v_color;
};

)"
