#version 330

// Input vertex attributes
in vec3 vertexPosition;
in vec2 vertexTexCoord;
in vec3 vertexNormal;
in vec4 vertexColor;

// Input uniform values
uniform mat4 mvp;

// Output vertex attributes (to fragment shader)
out vec2 uv;
out vec4 color;

void main()
{
    uv = vertexTexCoord;
    color = vertexColor;

    gl_Position = mvp*vec4(vertexPosition, 1.0);
}
