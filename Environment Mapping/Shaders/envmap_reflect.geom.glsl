#version 400

layout(triangles, invocations = 6) in;
layout(triangle_strip, max_vertices = 15) out;

uniform mat4 cubeViewProjMat[6];
uniform bool genenvironment;

in vec3 ReflectDirection[];
in vec3 originalPos[];

out vec3 ReflectDir;

void main()
{
	gl_Layer = gl_InvocationID;
	//invocation_id�� ���� �ٸ� view mat ��� ����.
	for (int i = 0; i < 3; i++)
	{
		ReflectDir = ReflectDirection[i];
		if (genenvironment) {//ȯ�� �� ���۽�.
			gl_Position = cubeViewProjMat[gl_InvocationID] * vec4(originalPos[i], 1.0f);
		}
		else {
			gl_Position = gl_in[i].gl_Position;
		}
		EmitVertex();
	}
	EndPrimitive();

}
