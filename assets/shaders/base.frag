#version 330

// Input vertex attributes (from vertex shader)
in vec2 uv;
in vec4 color;

// Input uniform values
uniform sampler2D texture0;
uniform vec4 colDiffuse;

// Output fragment color
out vec4 output;

void main()
{
    output = texture(texture0, uv) * color * colDiffuse;
}
