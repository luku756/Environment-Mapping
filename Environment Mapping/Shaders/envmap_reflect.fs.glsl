#version 430

in vec3 ReflectDir;

layout(binding = 10) uniform samplerCube SkyboxMap1;	//��ī�̹ڽ�
layout(binding = 11) uniform samplerCube SkyboxMap2;	//��ī�̹ڽ�
layout(binding = 12) uniform samplerCube SkyboxMap3;	//��ī�̹ڽ�
layout(binding = 13) uniform samplerCube SkyboxMap4;	//��ī�̹ڽ�
layout(binding = 14) uniform samplerCube SkyboxMap5;	//��ī�̹ڽ�
layout(binding = 19) uniform samplerCube EnvironmentMap;	//�ֺ� ��ü ȯ�����

uniform bool DrawSkyBox;
uniform int SkyboxIndex;
uniform float ReflectFactor;
uniform vec4 MaterialColor;

layout(location = 0) out vec4 FragColor;

void main() {

	vec4 cubeMapColor;

	if (DrawSkyBox) {//��ī�� �ڽ��� �׸��� ���, position ��ǥ�� �ؽ��� ��ǥ�� ���.
		switch (SkyboxIndex+1) {
		case 1: FragColor = texture(SkyboxMap1, ReflectDir); break;
		case 2: FragColor = texture(SkyboxMap2, ReflectDir); break;
		case 3: FragColor = texture(SkyboxMap3, ReflectDir); break;
		case 4: FragColor = texture(SkyboxMap4, ReflectDir); break;
		case 5: FragColor = texture(SkyboxMap5, ReflectDir); break;
		}

		//FragColor = texture(SkyboxMap, ReflectDir);
	}
	else {//�ݻ�Ǵ� ��ü(�̸� �������� ȯ�� �� ���)
		cubeMapColor = texture(EnvironmentMap, ReflectDir);
		FragColor = mix(MaterialColor, cubeMapColor, ReflectFactor);
	}

}
