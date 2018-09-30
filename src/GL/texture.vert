R"(

#if defined(GL_ES)
precision highp float;
#endif

uniform vec2 u_viewSize;

attribute vec2 a_position;
attribute vec2 a_texcoord;
attribute vec4 a_color;

varying vec2 v_texcoord;
varying vec4 v_color;

void main()
{
    v_texcoord   = a_texcoord;
    v_color      = a_color;
    gl_Position  = vec4(2.0*a_position.x/u_viewSize.x - 1.0, 1.0 - 2.0*a_position.y/u_viewSize.y, 0, 1);
};

)"
