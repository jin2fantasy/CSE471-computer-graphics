//
// Passing example
//

#version 140
#extension GL_ARB_compatibility: enable


void main()
{		
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}
