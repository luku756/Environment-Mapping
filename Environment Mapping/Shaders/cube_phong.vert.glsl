#version 430

#define NUMBER_OF_LIGHT_COUNT 4 
uniform mat4 u_ModelViewProjectionMatrix;
uniform mat4 u_ModelViewMatrix;
uniform mat4 u_ModelMatrix;

uniform bool genenvironment;

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec2 a_tex_coord;

out vec3 ReflectDir;

out vec3 VPosition;
out vec3 VNormal;
out vec2 VTEXCOORD;

void main(void) {
	if(genenvironment){
		VPosition = vec3(u_ModelMatrix*vec4(a_position, 1.0f));
		VNormal = normalize((u_ModelMatrix*vec4(a_normal, 0.0f)).xyz);
		VTEXCOORD = a_tex_coord;

		gl_Position = u_ModelMatrix * vec4(a_position, 1.0f);
	}
	else {

		VPosition = vec3(u_ModelViewMatrix*vec4(a_position, 1.0f));
		VNormal = normalize((u_ModelViewMatrix*vec4(a_normal,0.0f)).xyz);
		VTEXCOORD = a_tex_coord;

		gl_Position = u_ModelViewProjectionMatrix*vec4(a_position, 1.0f);
	}





}