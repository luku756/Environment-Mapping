#version 430

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;
layout (location = 2) in vec2 VertexTexCoord;

out vec3 ReflectDirection;
out vec3 originalPos;

out vec3 VNormal;
out vec2 VTEXCOORD;

uniform bool DrawSkyBox;
uniform vec3 WorldCameraPosition;

uniform mat4 ModelMatrix;
uniform mat4 MVP;

void main()
{
	vec3 worldPos;
	if (DrawSkyBox) {
		ReflectDirection = VertexPosition;
	}
	else if (!DrawSkyBox) {
		worldPos = vec3(ModelMatrix * vec4(VertexPosition, 1.0));
		vec3 worldNorm = vec3(ModelMatrix * vec4(VertexNormal, 0.0));
		vec3 worldView = normalize(WorldCameraPosition - worldPos);
		ReflectDirection = reflect(-worldView, worldNorm);
	}

	originalPos = worldPos;

	gl_Position = MVP * vec4(VertexPosition, 1.0);
}
