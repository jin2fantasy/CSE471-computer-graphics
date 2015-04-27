#version 140
//#define GL_compatibility_profile 1
#extension GL_ARB_compatibility: enable

out vec3 normal, lightDir;
out vec4 p;
uniform mat4 mvp; //model view projection matrix
uniform mat4 mv; // model view matrix
uniform mat4 norm; // normal matrix

void main()
{	
	// ToDo
//	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
//	p = gl_ModelViewMatrix * gl_Vertex;
	gl_Position = mvp * mv * gl_Vertex; 
	p = mv * gl_Vertex;

	normal = normalize(gl_NormalMatrix * gl_Normal);

	lightDir = gl_LightSource[0].position.xyz - p.xyz;
}