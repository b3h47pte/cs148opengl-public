#version 330

layout(location = 0) in vec4 vertexPosition;

void main()
{
    vec4 modifiedVertexPosition = vertexPosition;

    // Insert your code for "Slightly-More Advanced Shaders" here.

    gl_Position = modifiedVertexPosition;
}
