#version 460
out vec4 FragColor;
in vec3 colourFrag;
void main()
{
    FragColor = vec4(colourFrag, 1.0f);
}