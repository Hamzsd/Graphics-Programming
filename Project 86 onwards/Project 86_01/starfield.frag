#version 330

uniform sampler2D tex;

in vec4 colourOut;

out vec4 colourl;

void main()
{
	colour = texture(tex, gl_PointCoord) * colourOut;
}