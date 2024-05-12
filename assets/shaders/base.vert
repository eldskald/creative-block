#version 100

// Input vertex attributes
attribute vec3 vertexPosition;
attribute vec2 vertexTexCoord;
attribute vec3 vertexNormal;
attribute vec4 vertexColor;

// Input uniform values
uniform mat4 mvp;

// Output vertex attributes (to fragment shader)
varying vec2 uv;
varying vec4 color;

void main()
{
    uv = vertexTexCoord;
    color = vertexColor;

    gl_Position = mvp*vec4(vertexPosition, 1.0);
}
