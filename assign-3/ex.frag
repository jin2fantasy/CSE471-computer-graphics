//
// Passing example
//

#version 140
#extension GL_ARB_compatibility: enable

in vec3 perVertexColor;

void main()
{	
	gl_FragColor.xyz = perVertexColor;
}