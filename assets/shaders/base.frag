#version 100

precision mediump float;

// Input vertex attributes (from vertex shader)
varying vec2 uv;
varying vec4 color;

// Input uniform values
uniform sampler2D texture0;
uniform vec4 colDiffuse;

void main()
{
    // Put on gl_FragColor the final color of the fragment
    gl_FragColor = texture2D(texture0, uv) * color * colDiffuse;
}
