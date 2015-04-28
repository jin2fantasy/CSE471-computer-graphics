#version 140
//#define GL_compatibility_profile 1
#extension GL_ARB_compatibility: enable

out vec3 normal, lightDir;
out vec4 p;

uniform mat4 mvp; //model view projection matrix
uniform mat4 mv; // model view matrix
uniform mat4 norm; // normal matrix

uniform vec4 lightpos; // light position

void main()
{	
	// ToDo
	gl_Position = mvp * gl_Vertex; 
	p = mv * gl_Vertex;
	normal = normalize(gl_NormalMatrix * gl_Normal);
//	normal = normalize(vec3(norm * vec4(gl_Normal, 0.0)));
	lightDir = lightpos.xyz - p.xyz;
}