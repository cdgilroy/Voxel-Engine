#version 330

flat in vec4 vertexColour;
out vec4 col;

void main()
{
	col = vertexColour;
}