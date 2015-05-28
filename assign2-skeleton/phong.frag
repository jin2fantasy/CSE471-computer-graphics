#version 140
#extension GL_ARB_compatibility: enable

in vec4 p;
in vec3 normal, lightDir;

uniform colorBlock {
	vec4 lightSourceDiffuse, lightSourceAmbient, lightSourceSpecular,
		lightModelDiffuse, lightModelAmbient, lightModelSpecular,
		frontMaterialDiffuse, frontMaterialAmbient, frontMaterialSpecular;
};

void main()
{	
	// ToDo
	float alpha = 100;
	vec3 v = normalize(-p.xyz);
	vec3 n = normalize(normal);
	vec3 l = normalize(lightDir);
	vec3 r = normalize(reflect(-l,n));
	vec4 diffuse = frontMaterialDiffuse * lightSourceDiffuse;
	vec4 ambient = frontMaterialAmbient * lightSourceAmbient;
	ambient += lightModelAmbient * frontMaterialAmbient;
	vec4 specular;
	if (dot(normal, lightDir) < 0.0) specular = vec4(0); // no specular when the light source is on the wrong side
	else specular = pow(max(dot(r,v), 0), alpha) * frontMaterialSpecular * lightSourceSpecular;
	gl_FragColor = max(dot(l,n), 0)*diffuse + ambient + specular;	
}