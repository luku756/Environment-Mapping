#version 400

layout(triangles, invocations = 6) in;
layout(triangle_strip, max_vertices = 15) out;

uniform bool genenvironment;
uniform mat4 cubeViewProjMat[6];
uniform mat4 cubeViewMat[6];
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

in vec3 VNormal[];
in vec3 VPosition[];
in vec2 VTEXCOORD[];

out vec3 v_position_EC;
out vec3 v_normal_EC;
out vec2 v_tex_coord;

void main()
{
	//invocation_id에 따라 다른 view mat 사용 가능.

	gl_Layer = gl_InvocationID;
	for (int i = 0; i < 3; i++) {
		if (genenvironment) {
			v_position_EC = vec3(cubeViewMat[gl_InvocationID] * vec4(VPosition[i], 1.0f));
			v_normal_EC = normalize((cubeViewMat[gl_InvocationID] * vec4(VNormal[i], 0.0f)).xyz);
			v_tex_coord = VTEXCOORD[i];
			gl_Position = cubeViewProjMat[gl_InvocationID] * gl_in[i].gl_Position;

		}
		else {
			v_normal_EC = VNormal[i];
			v_position_EC = VPosition[i];
			v_tex_coord = VTEXCOORD[i];
			gl_Position = gl_in[i].gl_Position;
		}
		EmitVertex();
	}



	EndPrimitive();

}
