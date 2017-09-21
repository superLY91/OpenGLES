attribute vec4 position;
attribute vec4 color;
attribute vec2 texcoord;
attribute vec4 normal;
uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 IT_ModelMatrix;
varying vec4 V_Color;
varying vec4 V_Normal;
void main() {
    V_Color = color;
    V_Normal = IT_ModelMatrix * normal;
    gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * position;
}
