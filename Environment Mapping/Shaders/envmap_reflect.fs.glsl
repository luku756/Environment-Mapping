#version 430

in vec3 ReflectDir;

layout(binding = 10) uniform samplerCube SkyboxMap1;	//스카이박스
layout(binding = 11) uniform samplerCube SkyboxMap2;	//스카이박스
layout(binding = 12) uniform samplerCube SkyboxMap3;	//스카이박스
layout(binding = 13) uniform samplerCube SkyboxMap4;	//스카이박스
layout(binding = 14) uniform samplerCube SkyboxMap5;	//스카이박스
layout(binding = 19) uniform samplerCube EnvironmentMap;	//주변 물체 환경매핑

uniform bool DrawSkyBox;
uniform int SkyboxIndex;
uniform float ReflectFactor;
uniform vec4 MaterialColor;

layout(location = 0) out vec4 FragColor;

void main() {

	vec4 cubeMapColor;

	if (DrawSkyBox) {//스카이 박스만 그리는 경우, position 좌표를 텍스쳐 좌표로 사용.
		switch (SkyboxIndex+1) {
		case 1: FragColor = texture(SkyboxMap1, ReflectDir); break;
		case 2: FragColor = texture(SkyboxMap2, ReflectDir); break;
		case 3: FragColor = texture(SkyboxMap3, ReflectDir); break;
		case 4: FragColor = texture(SkyboxMap4, ReflectDir); break;
		case 5: FragColor = texture(SkyboxMap5, ReflectDir); break;
		}

		//FragColor = texture(SkyboxMap, ReflectDir);
	}
	else {//반사되는 물체(미리 랜더링한 환경 맵 사용)
		cubeMapColor = texture(EnvironmentMap, ReflectDir);
		FragColor = mix(MaterialColor, cubeMapColor, ReflectFactor);
	}

}
