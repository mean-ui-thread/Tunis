R"(

#if defined(GL_ES)
precision highp float;
#endif

uniform vec2 u_viewSize;

attribute vec2 a_position;

void main()
{
    gl_Position  = vec4(2.0*a_position.x/u_viewSize.x - 1.0, 1.0 - 2.0*a_position.y/u_viewSize.y, 0, 1);
};

)"
