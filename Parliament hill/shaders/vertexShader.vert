#version 460
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 colourVertex;

uniform mat4 mvpIn;

out vec3 colourFrag;

void main()
{
    gl_Position = mvpIn * vec4(position.x, position.y, position.z, 1.0);
    colourFrag = colourVertex;
}