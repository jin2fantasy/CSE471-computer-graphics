#version 140
#extension GL_ARB_compatibility: enable

in vec4 p;
in vec3 normal, lightDir;

void main()
{	
	vec4 color;
	
	// ToDo
	vec3 v = normalize(vec3(-p));
	vec3 n = normalize(normal);
	vec3 l = normalize(vec3(gl_LightSource[0].position) - vec3(p));
	vec3 r = normalize(reflect(-l,n));
	vec4 diffuse = gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse;
	vec4 ambient = gl_FrontMaterial.ambient * gl_LightSource[0].ambient;
	ambient += gl_LightModel.ambient * gl_FrontMaterial.ambient;
	gl_FragColor = max(dot(l,n),0)*diffuse + ambient;
}