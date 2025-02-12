#define _CRT_SECURE_NO_WARNINGS

#include"header.h"

glm::mat4 cubeViewMatrix[6];
glm::mat4 cubeViewProjMatrix[6];
GLuint loc_cube_viewprojmat[6];
GLuint loc_cube_viewmat[6];
GLuint loc_phong_viewprojmat[6];
GLuint loc_phong_viewmat[6];

GLuint loc_env_texture; 

//시간 측정 - 1, no - 0
#define UseTimeCount 0

__int64 _start, _freq, _end;
#define CHECK_TIME_START QueryPerformanceFrequency((LARGE_INTEGER*)&_freq); QueryPerformanceCounter((LARGE_INTEGER*)&_start)
#define CHECK_TIME_END(a) QueryPerformanceCounter((LARGE_INTEGER*)&_end); a = (float)((float)(_end - _start) / (_freq * 1.0e-3f))
GLuint cubeDepthBuf,  cubeEnvironmentTex;
void setupFBO()
{
	// Create and bind the FBO
	glGenFramebuffers(1, &envFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, envFBO);
	
	// The depth buffer
	glGenTextures(1, &cubeDepthBuf);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeDepthBuf);
	for (int i = 0; i < 6; i++) {
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT24, scene_width, scene_width, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	}

	glActiveTexture(GL_TEXTURE0 + TEXTURE_INDEX_ENVIRONMENT);
	glGenTextures(1, &cubeEnvironmentTex);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeEnvironmentTex);
	//NULL means reserve texture memory, but texels are undefined
	for (int i = 0; i < 6; i++) {
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA8, scene_width, scene_width, 0, GL_BGRA, GL_UNSIGNED_BYTE, NULL);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	   	 
	glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, cubeDepthBuf, 0);
	glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, cubeEnvironmentTex, 0);
	
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void prepare_environmentMap() {
	float as;
	
	if (WINDOW_param.height == 0)
		as = scene_width / scene_height;
	else
		as = WINDOW_param.width / WINDOW_param.height;
	ProjectionMatrix = glm::perspective(90.0f*TO_RADIAN, as, 0.0001f, 15000.0f);

	cubeViewMatrix[0] = glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
	cubeViewMatrix[1] = glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
	cubeViewMatrix[2] = glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	cubeViewMatrix[3] = glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f,-1.0f));
	cubeViewMatrix[4] = glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
	cubeViewMatrix[5] = glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f));

	for (int i = 0; i < 6; i++) {
		cubeViewProjMatrix[i] = ProjectionMatrix * cubeViewMatrix[i];
	}

	loc_cube_viewprojmat[0] = glGetUniformLocation(h_ShaderProgram_Env, "cubeViewProjMat[0]");
	loc_cube_viewprojmat[1] = glGetUniformLocation(h_ShaderProgram_Env, "cubeViewProjMat[1]");
	loc_cube_viewprojmat[2] = glGetUniformLocation(h_ShaderProgram_Env, "cubeViewProjMat[2]");
	loc_cube_viewprojmat[3] = glGetUniformLocation(h_ShaderProgram_Env, "cubeViewProjMat[3]");
	loc_cube_viewprojmat[4] = glGetUniformLocation(h_ShaderProgram_Env, "cubeViewProjMat[4]");
	loc_cube_viewprojmat[5] = glGetUniformLocation(h_ShaderProgram_Env, "cubeViewProjMat[5]");


	loc_cube_viewmat[0] = glGetUniformLocation(h_ShaderProgram_Env, "cubeViewMat[0]");
	loc_cube_viewmat[1] = glGetUniformLocation(h_ShaderProgram_Env, "cubeViewMat[1]");
	loc_cube_viewmat[2] = glGetUniformLocation(h_ShaderProgram_Env, "cubeViewMat[2]");
	loc_cube_viewmat[3] = glGetUniformLocation(h_ShaderProgram_Env, "cubeViewMat[3]");
	loc_cube_viewmat[4] = glGetUniformLocation(h_ShaderProgram_Env, "cubeViewMat[4]");
	loc_cube_viewmat[5] = glGetUniformLocation(h_ShaderProgram_Env, "cubeViewMat[5]");



	loc_phong_viewprojmat[0] = glGetUniformLocation(h_ShaderProgram_TXPS , "cubeViewProjMat[0]");
	loc_phong_viewprojmat[1] = glGetUniformLocation(h_ShaderProgram_TXPS , "cubeViewProjMat[1]");
	loc_phong_viewprojmat[2] = glGetUniformLocation(h_ShaderProgram_TXPS , "cubeViewProjMat[2]");
	loc_phong_viewprojmat[3] = glGetUniformLocation(h_ShaderProgram_TXPS , "cubeViewProjMat[3]");
	loc_phong_viewprojmat[4] = glGetUniformLocation(h_ShaderProgram_TXPS , "cubeViewProjMat[4]");
	loc_phong_viewprojmat[5] = glGetUniformLocation(h_ShaderProgram_TXPS , "cubeViewProjMat[5]");
	loc_phong_viewmat[0] = glGetUniformLocation(h_ShaderProgram_TXPS, "cubeViewMat[0]");
	loc_phong_viewmat[1] = glGetUniformLocation(h_ShaderProgram_TXPS, "cubeViewMat[1]");
	loc_phong_viewmat[2] = glGetUniformLocation(h_ShaderProgram_TXPS, "cubeViewMat[2]");
	loc_phong_viewmat[3] = glGetUniformLocation(h_ShaderProgram_TXPS, "cubeViewMat[3]");
	loc_phong_viewmat[4] = glGetUniformLocation(h_ShaderProgram_TXPS, "cubeViewMat[4]");
	loc_phong_viewmat[5] = glGetUniformLocation(h_ShaderProgram_TXPS, "cubeViewMat[5]");


	for (int i = 0; i < 6; i++) {
		glUseProgram(h_ShaderProgram_Env);
		glUniformMatrix4fv(loc_cube_viewprojmat[i], 1, GL_FALSE, &cubeViewProjMatrix[i][0][0]);
		glUniformMatrix4fv(loc_cube_viewmat[i], 1, GL_FALSE, &cubeViewMatrix[i][0][0]);

		glUseProgram(h_ShaderProgram_TXPS);
		glUniformMatrix4fv(loc_phong_viewprojmat[i], 1, GL_FALSE, &cubeViewProjMatrix[i][0][0]);
		glUniformMatrix4fv(loc_phong_viewmat[i], 1, GL_FALSE, &cubeViewMatrix[i][0][0]);
	}
}

// for tiger animation
int cur_frame_tiger = 0;
float rotation_angle_camera = 0.0f;
int rotation_speed_camera = 100;

float rotation_angle_object = TO_RADIAN;
int rotation_speed_object = 70;

int skybox_index = 3;

#include "Objects.h"

void draw_screen() {

	prepare_quad();

	ModelViewMatrix = glm::mat4(1.0f);
	ModelViewMatrixInvTrans = glm::mat3(1.0f);
	ModelViewProjectionMatrix = glm::mat4(1.0f);

	glUniformMatrix4fv(loc_ModelViewProjectionMatrix_TXPS, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	glUniformMatrix4fv(loc_ModelViewMatrix_TXPS, 1, GL_FALSE, &ModelViewMatrix[0][0]);
	glUniformMatrix3fv(loc_ModelViewMatrixInvTrans_TXPS, 1, GL_FALSE, &ModelViewMatrixInvTrans[0][0]);

	draw_quad();
}

void setModelMatrixes(glm::mat4 ModelMatrix) {

	ModelViewMatrix = ViewMatrix * ModelMatrix;
	ModelViewProjectionMatrix = ProjectionMatrix * ModelViewMatrix;
	ModelViewMatrixInvTrans = glm::inverseTranspose(glm::mat3(ModelViewMatrix));

	glUniformMatrix4fv(loc_ModelViewProjectionMatrix_TXPS, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	glUniformMatrix4fv(loc_ModelMatrix_TXPS, 1, GL_FALSE, &ModelMatrix[0][0]);
	glUniformMatrix4fv(loc_ModelViewMatrix_TXPS, 1, GL_FALSE, &ModelViewMatrix[0][0]);
	glUniformMatrix3fv(loc_ModelViewMatrixInvTrans_TXPS, 1, GL_FALSE, &ModelViewMatrixInvTrans[0][0]);

}

glm::mat4 model;
void setMatrices()
{
	setUniform("ModelMatrix", model);
	setUniform("MVP", ProjectionMatrix * ViewMatrix * model);
	setUniform("ViewMatrix", ViewMatrix);
	setUniform("ProjectionMatrix", ProjectionMatrix );
}

void draw_world(glm::vec3 addpos) {	

	glm::mat4 ModelMatrix;


	set_material_tiger();
	glUniform1i(loc_base_texture, TEXTURE_INDEX_TIGER);
	ModelMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.05f, 0.05f, 0.05f));
	ModelMatrix = glm::rotate(ModelMatrix, rotation_angle_object, glm::vec3(0.0f, 1.0f, 0.0f));
	ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-150.0f, 0.0f, -20.0f));
	ModelMatrix = glm::translate(ModelMatrix, addpos);
	ModelMatrix = glm::rotate(ModelMatrix, -90.0f*TO_RADIAN, glm::vec3(1.0f, 0.0f, 0.0f));

	setModelMatrixes(ModelMatrix);
	draw_tiger();


	set_material_object(OBJECT_OPTIMUS);
	ModelMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.005f, 0.005f, 0.005f));
	ModelMatrix = glm::rotate(ModelMatrix, rotation_angle_object, glm::vec3(0.0f, 1.0f, 0.0f));
	ModelMatrix = glm::translate(ModelMatrix, glm::vec3(1500.0f, 0.0f, 0.0f));
	ModelMatrix = glm::translate(ModelMatrix, addpos);
	ModelMatrix = glm::rotate(ModelMatrix, -90.0f*TO_RADIAN, glm::vec3(1.0f, 0.0f, 0.0f));
	
	setModelMatrixes(ModelMatrix);
	draw_object(OBJECT_OPTIMUS, 1.0f, 1.0f, 1.0f, GL_CCW);
	

	set_material_object(OBJECT_DRAGON);

	ModelMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.2f, 0.2f, 0.2f));
	ModelMatrix = glm::rotate(ModelMatrix, rotation_angle_object, glm::vec3(1.0f, 0.0f, 0.0f));
	ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.0f, 25.0f, 0.0f));
	ModelMatrix = glm::translate(ModelMatrix, addpos);
	ModelMatrix = glm::rotate(ModelMatrix, -90.0f*TO_RADIAN, glm::vec3(1.0f, 0.0f, 0.0f));
	ModelMatrix = glm::rotate(ModelMatrix, -90.0f*TO_RADIAN, glm::vec3(0.0f, 0.0f, 1.0f));
	
	setModelMatrixes(ModelMatrix);
	draw_object(OBJECT_DRAGON, 1.0f, 1.0f, 1.0f, GL_CCW);
}

int runcnt = 4;
#define CAM_HEIGHT 10.0f
float cam_height = 5.0f;

void draw_scene() {

	//메인 화면에 그리기
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);

	now_Shader_program = h_ShaderProgram_Env;
	glUseProgram(now_Shader_program);
	setUniform("genenvironment", false);	//환경 매핑 아님


	glm::vec3 cameraPos = glm::vec3(15.0f * cos(rotation_angle_camera), cam_height, 15.0f * sin(rotation_angle_camera));
	ViewMatrix = glm::lookAt(cameraPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	
	setUniform("WorldCameraPosition", cameraPos);

	setUniform("DrawSkyBox", true);
	setUniform("SkyboxIndex", skybox_index);
	model = glm::scale(glm::mat4(1.0f), glm::vec3(20.0f, 20.0f, 20.0f));
	setMatrices();
	draw_skybox();

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	setUniform("DrawSkyBox", false);

	setUniform("MaterialColor", glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
	setUniform("ReflectFactor", 0.85f);

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	setMatrices();
	vbosphere->render();


	glEnable(GL_DEPTH_TEST);


	//물체 그리기
	now_Shader_program = h_ShaderProgram_TXPS;
	glUseProgram(now_Shader_program);
	setUniform("genenvironment", false);

	//오브젝트 랜더링
	if (UseTimeCount == 1) {
		for (int i = 0; i < runcnt; i++)
			draw_world(glm::vec3(i, i, i));
	}
	else
		draw_world(glm::vec3(0, 0, 0));

	glUseProgram(0);
}

bool flag_require_envMap = true;

void create_environmentMap() {

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, envFBO);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	

	now_Shader_program = h_ShaderProgram_Env;
	glUseProgram(now_Shader_program);
	setUniform("genenvironment", true);

	//스카이 박스를 랜더링
	glDepthMask(GL_FALSE);
	glUseProgram(h_ShaderProgram_Env);
	now_Shader_program = h_ShaderProgram_Env;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//스카이 박스 그리기
	setUniform("DrawSkyBox", true);
	setUniform("SkyboxIndex", skybox_index);
	draw_skybox();

	setUniform("DrawSkyBox", false);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);


	now_Shader_program = h_ShaderProgram_TXPS;
	glUseProgram(now_Shader_program);
	setUniform("genenvironment", true);

	if (UseTimeCount == 1) {
		for (int i = 0; i < runcnt; i++)
			draw_world(glm::vec3(i,i,i));
	}
	else
		draw_world(glm::vec3(0, 0, 0));
	flag_require_envMap = false;

}

int count, bigcnt;
float timesum = 0;
float bigtimesum = 0;
// callbacks
void display(void) {
	
	CHECK_TIME_START;
	
	if (flag_require_envMap)
		create_environmentMap();
	draw_scene();
	glutSwapBuffers();
	glFinish();


	float time;
	CHECK_TIME_END(time);
	//printf("%f\n",time);
	timesum += time;
	count++;
	if (count == 10) {
		bigcnt++;
		bigtimesum += timesum / 10;
		count = 0;
	
		if(UseTimeCount == 1)
		printf("=================time : %f ms (%d)\n", timesum / 10, bigcnt);
		
		timesum = 0;
		if (bigcnt == 100) {
			bigcnt = 0;
			if (UseTimeCount == 1)
			printf("=================time for 100 frame : %f ms\n", bigtimesum );
			bigtimesum = 0;
		}
	}
}

void timer_scene(int timer_flag) {
	if (timer_flag) {
		//cur_frame_tiger = flag.tiger_timestamp % N_TIGER_FRAMES;
		rotation_angle_camera = (flag.camera_timestamp % 360)*TO_RADIAN;
		flag.camera_timestamp = flag.camera_timestamp++ % INT_MAX;
		glutTimerFunc(rotation_speed_camera, timer_scene, flag.camera_move);
		glutPostRedisplay();
	}
}

void timer_scene2(int timer_flag) {
	if (timer_flag) {
		cur_frame_tiger = (cur_frame_tiger + 1) % N_TIGER_FRAMES;
		rotation_angle_object = (flag.obj_timestamp % 360)*TO_RADIAN;
		flag.obj_timestamp = flag.obj_timestamp++ % INT_MAX;
		glutTimerFunc(rotation_speed_object, timer_scene2, flag.obj_move);
		flag_require_envMap = true;
		glutPostRedisplay();
	}
}

void keyboard(unsigned char key, int x, int y) {
	int i;
	int light_flag = -1;
	glm::vec4 position_EC;

	if ((key >= '0') && (key <= '0' + NUMBER_OF_LIGHT_SUPPORTED - 1)) {
		int light_ID = (int)(key - '0');

		glUseProgram(h_ShaderProgram_TXPS);
		light[light_ID].light_on = 1 - light[light_ID].light_on;
		glUniform1i(loc_light[light_ID].light_on, light[light_ID].light_on);
		glUseProgram(0);

		glutPostRedisplay();
		return;
	}

	switch (key) {
	case 'p':
		flag.polygon_fill = 1 - flag.polygon_fill;
		if (flag.polygon_fill)
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glutPostRedisplay();
		break;
	case 't':
		flag.texture_mapping = 1 - flag.texture_mapping;
		glUseProgram(h_ShaderProgram_TXPS);
		glUniform1i(loc_flag_texture_mapping, flag.texture_mapping);
		glUseProgram(0);
		glutPostRedisplay();
		break;
	case 's': // Currently, shadow is cast for light 0 only.		
		skybox_index++;
		skybox_index %= TEXTURE_SIZE_SKYBOX;
		glutPostRedisplay();
		break;
	case 'c':
		flag.cull_face = (flag.cull_face + 1) % 3;
		switch (flag.cull_face) {
		case 0:
			fprintf(stdout, "^^^ Face culling OFF.\n");
			glDisable(GL_CULL_FACE);
			glutPostRedisplay();
			break;
		case 1: // cull back faces;
			fprintf(stdout, "^^^ BACK face culled.\n");
			glCullFace(GL_BACK);
			glEnable(GL_CULL_FACE);
			glutPostRedisplay();
			break;
		case 2: // cull front faces;
			fprintf(stdout, "^^^ FRONT face culled.\n");
			glCullFace(GL_FRONT);
			glEnable(GL_CULL_FACE);
			glutPostRedisplay();
			break;
		}
		break;
	case 'v':
		if(cam_height > -CAM_HEIGHT)
		cam_height -= 0.2f;
		glutPostRedisplay();
		break;
	case 'b':
		if (cam_height < CAM_HEIGHT)
			cam_height += 0.2f;
		glutPostRedisplay();
		break;
	case 'm':
		flag.camera_move = 1 - flag.camera_move;
		if (flag.camera_move)
			glutTimerFunc(rotation_speed_camera, timer_scene, 1);
		//glutPostRedisplay();
		break;
	case 'n':
		flag.obj_move = 1 - flag.obj_move;
		if (flag.obj_move) {
			printf("move start, generate env map\n");
			glutTimerFunc(rotation_speed_object, timer_scene2, 1);
		}
		else
			printf("move fin. no generate env map\n");
		//glutPostRedisplay();
		break;
	case 'r':
		flag.camera_speed = (++flag.camera_speed) % 3;
		switch (flag.camera_speed) {
		case 0:
			rotation_speed_camera = 10;
			rotation_speed_object = 7;
			break;
		case 1:
			rotation_speed_camera = 100;
			rotation_speed_object = 70;
			break;
		case 2:
			rotation_speed_camera = 1000;
			rotation_speed_object = 700;
			break;
		}
		break;
	//case 'l':
	//	if (light[0].light_on) {
	//		light0_lerp_t += 0.025f;
	//		if (light0_lerp_t > 1.0001f)  // for numerical error
	//			light0_lerp_t = 0.0f;
	//		for (i = 0; i < 4; i++)
	//			light[0].position[i] = (1.0f - light0_lerp_t)*light0_position_WC[0][i]
	//			+ light0_lerp_t * light0_position_WC[1][i];
	//		ViewMatrix_SHADOW[0] = glm::lookAt(glm::vec3(light[0].position[0], light[0].position[1], light[0].position[2]),
	//			glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	//		glUseProgram(h_ShaderProgram_TXPS);
	//		position_EC = ViewMatrix * glm::vec4(light[0].position[0], light[0].position[1], light[0].position[2], light[0].position[3]);
	//		glUniform4fv(loc_light[0].position, 1, &position_EC[0]);
	//		glUseProgram(0);
	//		glutPostRedisplay();
	//	}
	//	break;
	case 27: // ESC key
		glutLeaveMainLoop(); // Incur destuction callback for cleanups
		break;
	}

	flag_require_envMap = true;
}

void reshape(int width, int height) {
	float aspect_ratio;
	scene_height = height;
	scene_width = width;
	WINDOW_param.width = width;
	WINDOW_param.height = height;

	glViewport(0, 0, width, height);

	aspect_ratio = (float)width / height;
	ProjectionMatrix = glm::perspective(90.0f*TO_RADIAN, aspect_ratio, 0.0001f, 15000.0f);
	prepare_environmentMap();
	glutPostRedisplay();
}

void cleanup(void) {
	printf("clear up!\n");
	glDeleteVertexArrays(1, &axes_VAO);
	glDeleteBuffers(1, &axes_VBO);

	glDeleteVertexArrays(1, &tiger_VAO);
	glDeleteBuffers(1, &tiger_VBO);

	glDeleteVertexArrays(1, &rectangle_VAO);
	glDeleteBuffers(1, &rectangle_VBO);

	//skybox
	glDeleteVertexArrays(1, &skybox_VAO);
	glDeleteBuffers(1, &skybox_VBO);

	//optimmus
	glDeleteVertexArrays(1, &object_VAO[0]);
	glDeleteBuffers(1, &object_VBO[0]);

	//dragon
	glDeleteVertexArrays(1, &object_VAO[1]);
	glDeleteBuffers(1, &object_VBO[1]);

	//framebuffer
	glDeleteFramebuffers(1, &envFBO);
	glDeleteFramebuffers(1, &ShadowMapping.FBO_ID[0]);

	//texture
	glDeleteTextures(N_NORMAL_TEXTURES_USED, texture_names);
	glDeleteTextures(1, &ShadowMapping.shadow_map_ID[0]);

	glDeleteTextures(1, &cubeDepthBuf);
	glDeleteTextures(1, &cubeEnvironmentTex);
	glDeleteTextures(TEXTURE_SIZE_SKYBOX, skyboxTex);

}

void register_callbacks(void) {
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(reshape);
	glutTimerFunc(rotation_speed_camera, timer_scene, 1);
	glutCloseFunc(cleanup);
}

void prepare_shader_program(void) {
	int i;
	char string[256];
	ShaderInfo shader_info_simple[3] = {
		{ GL_VERTEX_SHADER, "Shaders/simple.vert" },
	{ GL_FRAGMENT_SHADER, "Shaders/simple.frag" },
	{ GL_NONE, NULL }
	};
	ShaderInfo shader_info_TXPS[4] = {
			{ GL_VERTEX_SHADER, "Shaders/cube_phong.vert.glsl" },
			{ GL_GEOMETRY_SHADER, "Shaders/cube_phong.geom.glsl" },
			{ GL_FRAGMENT_SHADER, "Shaders/cube_phong.frag.glsl" },
		{ GL_NONE, NULL }
	};

	ShaderInfo shader_info_shadow[3] = {
		{ GL_VERTEX_SHADER, "Shaders/Shadow.vert" },
	{ GL_FRAGMENT_SHADER, "Shaders/Shadow.frag" },
	{ GL_NONE, NULL }
	};


	ShaderInfo shader_info_Env[4] = {
		{ GL_VERTEX_SHADER, "Shaders/envmap_reflect.vs.glsl" },
		{ GL_GEOMETRY_SHADER, "Shaders/envmap_reflect.geom.glsl" },
	{ GL_FRAGMENT_SHADER, "Shaders/envmap_reflect.fs.glsl" },
	{ GL_NONE, NULL }
	};
	

	h_ShaderProgram_Env = LoadShaders(shader_info_Env);

	//////////////////
	h_ShaderProgram_simple = LoadShaders(shader_info_simple);
	loc_primitive_color = glGetUniformLocation(h_ShaderProgram_simple, "u_primitive_color");
	loc_ModelViewProjectionMatrix_simple = glGetUniformLocation(h_ShaderProgram_simple, "u_ModelViewProjectionMatrix");

	//////////////////
	h_ShaderProgram_TXPS = LoadShaders(shader_info_TXPS);

	loc_PASS = glGetUniformLocation(h_ShaderProgram_TXPS, "Pass");
	loc_posTex = glGetUniformLocation(h_ShaderProgram_TXPS, "PositionTex");
	loc_normTex = glGetUniformLocation(h_ShaderProgram_TXPS, "NormalTex");

	loc_objIDTex = glGetUniformLocation(h_ShaderProgram_TXPS, "objIDTex");
	loc_diffTex = glGetUniformLocation(h_ShaderProgram_TXPS, "diffTex");
	loc_sampleKernal = glGetUniformLocation(h_ShaderProgram_TXPS, "SampleKernel");

	loc_ao = glGetUniformLocation(h_ShaderProgram_TXPS, "AoTex");
	loc_rand = glGetUniformLocation(h_ShaderProgram_TXPS, "RandTex");
	loc_ProjMat = glGetUniformLocation(h_ShaderProgram_TXPS, "ProjectionMatrix");

	loc_object_id = glGetUniformLocation(h_ShaderProgram_TXPS, "object_id");

	loc_base_texture = glGetUniformLocation(h_ShaderProgram_TXPS, "u_base_texture");
	loc_ModelViewProjectionMatrix_TXPS = glGetUniformLocation(h_ShaderProgram_TXPS, "u_ModelViewProjectionMatrix");
	loc_ModelMatrix_TXPS = glGetUniformLocation(h_ShaderProgram_TXPS, "u_ModelMatrix");
	loc_ModelViewMatrix_TXPS = glGetUniformLocation(h_ShaderProgram_TXPS, "u_ModelViewMatrix");
	loc_ModelViewMatrixInvTrans_TXPS = glGetUniformLocation(h_ShaderProgram_TXPS, "u_ModelViewMatrixInvTrans");

	for (int i = 0; i < NUMBER_OF_LIGHT_COUNT; i++) {
		sprintf(string, "u_ShadowMatrix[%d]", i);
		loc_ShadowMatrix_TXPS[i] = glGetUniformLocation(h_ShaderProgram_TXPS, string);
	}

	loc_global_ambient_color = glGetUniformLocation(h_ShaderProgram_TXPS, "u_global_ambient_color");
	for (i = 0; i < NUMBER_OF_LIGHT_SUPPORTED; i++) {
		sprintf(string, "u_light[%d].light_on", i);
		loc_light[i].light_on = glGetUniformLocation(h_ShaderProgram_TXPS, string);
		sprintf(string, "u_light[%d].shadow_on", i);
		loc_light[i].shadow_on = glGetUniformLocation(h_ShaderProgram_TXPS, string);
		sprintf(string, "u_light[%d].position", i);
		loc_light[i].position = glGetUniformLocation(h_ShaderProgram_TXPS, string);
		sprintf(string, "u_light[%d].ambient_color", i);
		loc_light[i].ambient_color = glGetUniformLocation(h_ShaderProgram_TXPS, string);
		sprintf(string, "u_light[%d].diffuse_color", i);
		loc_light[i].diffuse_color = glGetUniformLocation(h_ShaderProgram_TXPS, string);
		sprintf(string, "u_light[%d].specular_color", i);
		loc_light[i].specular_color = glGetUniformLocation(h_ShaderProgram_TXPS, string);
		sprintf(string, "u_light[%d].spot_direction", i);
		loc_light[i].spot_direction = glGetUniformLocation(h_ShaderProgram_TXPS, string);
		sprintf(string, "u_light[%d].spot_exponent", i);
		loc_light[i].spot_exponent = glGetUniformLocation(h_ShaderProgram_TXPS, string);
		sprintf(string, "u_light[%d].spot_cutoff_angle", i);
		loc_light[i].spot_cutoff_angle = glGetUniformLocation(h_ShaderProgram_TXPS, string);
		sprintf(string, "u_light[%d].light_attenuation_factors", i);
		loc_light[i].light_attenuation_factors = glGetUniformLocation(h_ShaderProgram_TXPS, string);
	}

	loc_material.ambient_color = glGetUniformLocation(h_ShaderProgram_TXPS, "u_material.ambient_color");
	loc_material.diffuse_color = glGetUniformLocation(h_ShaderProgram_TXPS, "u_material.diffuse_color");
	loc_material.specular_color = glGetUniformLocation(h_ShaderProgram_TXPS, "u_material.specular_color");
	loc_material.emissive_color = glGetUniformLocation(h_ShaderProgram_TXPS, "u_material.emissive_color");
	loc_material.specular_exponent = glGetUniformLocation(h_ShaderProgram_TXPS, "u_material.specular_exponent");

	loc_base_texture = glGetUniformLocation(h_ShaderProgram_TXPS, "u_base_texture");
	for (i = 0; i < NUMBER_OF_LIGHT_COUNT; i++) {
		sprintf(string, "u_shadow_texture[%d]", i);
		loc_shadow_texture[i] = glGetUniformLocation(h_ShaderProgram_TXPS, string);
	}
	loc_flag_texture_mapping = glGetUniformLocation(h_ShaderProgram_TXPS, "u_flag_texture_mapping");
	loc_flag_ambient_occlusion = glGetUniformLocation(h_ShaderProgram_TXPS, "u_flag_ambient_occlusion");
	loc_flag_fog = glGetUniformLocation(h_ShaderProgram_TXPS, "u_flag_fog");

	//////////////////
	h_ShaderProgram_shadow = LoadShaders(shader_info_shadow);
	loc_ModelViewProjectionMatrix_simple = glGetUniformLocation(h_ShaderProgram_shadow, "u_ModelViewProjectionMatrix");

	/////////////////
}

void initialize_lights_and_material(void) { // follow OpenGL conventions for initialization
	int i;

	glUseProgram(h_ShaderProgram_TXPS);

	glUniform4f(loc_global_ambient_color, 0.115f, 0.115f, 0.115f, 1.0f);
	for (i = 0; i < NUMBER_OF_LIGHT_SUPPORTED; i++) {
		glUniform1i(loc_light[i].light_on, 0); // turn off all lights initially
		glUniform1i(loc_light[i].shadow_on, 0); // turn off all shadows initially
		glUniform4f(loc_light[i].position, 0.0f, 0.0f, 1.0f, 0.0f);
		glUniform4f(loc_light[i].ambient_color, 0.0f, 0.0f, 0.0f, 1.0f);
		if (i == 0) {
			glUniform4f(loc_light[i].diffuse_color, 1.0f, 1.0f, 1.0f, 1.0f);
			glUniform4f(loc_light[i].specular_color, 1.0f, 1.0f, 1.0f, 1.0f);
		}
		else {
			glUniform4f(loc_light[i].diffuse_color, 0.0f, 0.0f, 0.0f, 1.0f);
			glUniform4f(loc_light[i].specular_color, 0.0f, 0.0f, 0.0f, 1.0f);
		}
		glUniform3f(loc_light[i].spot_direction, 0.0f, 0.0f, -1.0f);
		glUniform1f(loc_light[i].spot_exponent, 0.0f); // [0.0, 128.0]
		glUniform1f(loc_light[i].spot_cutoff_angle, 180.0f); // [0.0, 90.0] or 180.0 (180.0 for no spot light effect)
		glUniform4f(loc_light[i].light_attenuation_factors, 1.0f, 0.0f, 0.0f, 0.0f); // .w != 0.0f for no ligth attenuation
	}

	glUniform4f(loc_material.ambient_color, 0.2f, 0.2f, 0.2f, 1.0f);
	glUniform4f(loc_material.diffuse_color, 0.8f, 0.8f, 0.8f, 1.0f);
	glUniform4f(loc_material.specular_color, 0.0f, 0.0f, 0.0f, 1.0f);
	glUniform4f(loc_material.emissive_color, 0.0f, 0.0f, 0.0f, 1.0f);
	glUniform1f(loc_material.specular_exponent, 0.0f); // [0.0, 128.0]

	for (i = 0; i < NUMBER_OF_LIGHT_COUNT; i++) {
		glUniform1i(loc_shadow_texture[i], ShadowMapping.texture_unit[i]);
	}
	glUseProgram(0);
}

void initialize_flags(void) {
	flag.texture_mapping = 0;
	flag.fog = 0;
	flag.show_shadow_map = 0;
	flag.cull_face = 0;
	flag.polygon_fill = 1;
	flag.camera_move = 1;
	flag.camera_speed = 1;
	flag.obj_move = 0;
	flag.obj_timestamp = 0;

	glUseProgram(h_ShaderProgram_TXPS);
	glUniform1i(loc_flag_fog, flag.fog);
	glUniform1i(loc_flag_texture_mapping, flag.texture_mapping);
	glUseProgram(0);
}

void initialize_OpenGL(void) {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);

	glFrontFace(GL_CCW);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

#define CAMERA_COORDINATE 400.0f
	ViewMatrix = glm::lookAt(glm::vec3(CAMERA_COORDINATE, CAMERA_COORDINATE, CAMERA_COORDINATE), glm::vec3(0.0f, 30.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));

	//ViewMatrix = glm::lookAt(glm::vec3(CAMERA_COORDINATE, CAMERA_COORDINATE, CAMERA_COORDINATE), glm::vec3(0.0f, 30.0f, 0.0f),	glm::vec3(0.0f, 1.0f, 0.0f));


	initialize_lights_and_material();
	initialize_flags();

	glGenTextures(N_NORMAL_TEXTURES_USED, texture_names);
}

void set_up_scene_lights(void) {
	int i;
	glm::vec4 position_EC;

#define SF 1.0f
	light0_position_WC[0][0] = -SF * 500.0f;
	light0_position_WC[0][1] = SF * 600.0f;
	light0_position_WC[0][2] = SF * 400.0f;
	light0_position_WC[0][3] = 1.0f;
	light0_position_WC[1][0] = SF * 500.0f;
	light0_position_WC[1][1] = SF * 600.0f;
	light0_position_WC[1][2] = SF * 400.0f;
	light0_position_WC[1][3] = 1.0f;
	light0_lerp_t = 0.0f;

	// point_light_WC: use light 0
	light[0].light_on = 1;
	light[0].shadow_on = 0;
	for (i = 0; i < 4; i++)
		light[0].position[i] = (1.0f - light0_lerp_t)*light0_position_WC[0][i]
		+ light0_lerp_t * light0_position_WC[1][i]; // point light position in WC

	light[0].ambient_color[0] = 0.13f; light[0].ambient_color[1] = 0.13f;
	light[0].ambient_color[2] = 0.13f; light[0].ambient_color[3] = 1.0f;

	light[0].diffuse_color[0] = 0.5f; light[0].diffuse_color[1] = 0.5f;
	light[0].diffuse_color[2] = 0.5f; light[0].diffuse_color[3] = 1.5f;

	light[0].specular_color[0] = 0.8f; light[0].specular_color[1] = 0.8f;
	light[0].specular_color[2] = 0.8f; light[0].specular_color[3] = 1.0f;

	// spot_light_WC: do not use light 1 initially
	light[1].light_on = 0;
	light[1].shadow_on = 0;
	light[1].position[0] = 400.0f; light[1].position[1] = 550.0f; // spot light position in WC
	light[1].position[2] = -400.0f; light[1].position[3] = 1.0f;

	light[1].ambient_color[0] = 0.152f; light[1].ambient_color[1] = 0.152f;
	light[1].ambient_color[2] = 0.152f; light[1].ambient_color[3] = 1.0f;

	light[1].diffuse_color[0] = 0.572f; light[1].diffuse_color[1] = 0.572f;
	light[1].diffuse_color[2] = 0.572f; light[1].diffuse_color[3] = 1.0f;

	light[1].specular_color[0] = 0.772f; light[1].specular_color[1] = 0.772f;
	light[1].specular_color[2] = 0.772f; light[1].specular_color[3] = 1.0f;

	light[1].spot_direction[0] = -300.0f; light[1].spot_direction[1] = -500.0f; // spot light direction in WC
	light[1].spot_direction[2] = 400.0f;
	light[1].spot_cutoff_angle = 10.0f;
	light[1].spot_exponent = 8.0f;


	// spot_light_MC:

	light[2].light_on = 0;
	light[2].shadow_on = 0;
	light[2].position[0] = 30; light[2].position[1] = 250.0f; // spot light position in MC
	light[2].position[2] = 00; light[2].position[3] = 1.0f;

	light[2].ambient_color[0] = 0.152f; light[2].ambient_color[1] = 0.152f;
	light[2].ambient_color[2] = 0.152f; light[2].ambient_color[3] = 1.0f;

	light[2].diffuse_color[0] = 0.572f; light[2].diffuse_color[1] = 0.572f;
	light[2].diffuse_color[2] = 0.572f; light[2].diffuse_color[3] = 1.0f;

	light[2].specular_color[0] = 0.772f; light[2].specular_color[1] = 0.772f;
	light[2].specular_color[2] = 0.772f; light[2].specular_color[3] = 1.0f;

	light[2].spot_direction[0] = 300.0f; light[2].spot_direction[1] = -500.0f; // spot light direction in MC
	light[2].spot_direction[2] = 00.0f;
	light[2].spot_cutoff_angle = 20.0f;
	light[2].spot_exponent = 8.0f;


	glUseProgram(h_ShaderProgram_TXPS);
	glUniform1i(loc_light[0].light_on, light[0].light_on);
	glUniform1i(loc_light[0].shadow_on, light[0].shadow_on);
	// need to supply position in EC for shading
	position_EC = ViewMatrix * glm::vec4(light[0].position[0], light[0].position[1], light[0].position[2],
		light[0].position[3]);
	glUniform4fv(loc_light[0].position, 1, &position_EC[0]);
	glUniform4fv(loc_light[0].ambient_color, 1, light[0].ambient_color);
	glUniform4fv(loc_light[0].diffuse_color, 1, light[0].diffuse_color);
	glUniform4fv(loc_light[0].specular_color, 1, light[0].specular_color);

	glUniform1i(loc_light[1].light_on, light[1].light_on);
	glUniform1i(loc_light[1].shadow_on, light[1].shadow_on);
	// need to supply position in EC for shading
	position_EC = ViewMatrix * glm::vec4(light[1].position[0], light[1].position[1], light[1].position[2], light[1].position[3]);
	glUniform4fv(loc_light[1].position, 1, &position_EC[0]);
	glUniform4fv(loc_light[1].ambient_color, 1, light[1].ambient_color);
	glUniform4fv(loc_light[1].diffuse_color, 1, light[1].diffuse_color);
	glUniform4fv(loc_light[1].specular_color, 1, light[1].specular_color);
	// need to supply direction in EC for shading in this example shader
	// note that the viewing transform is a rigid body transform
	// thus transpose(inverse(mat3(ViewMatrix)) = mat3(ViewMatrix)
	glm::vec3 direction_EC = glm::mat3(ViewMatrix) * glm::vec3(light[1].spot_direction[0], light[1].spot_direction[1],
		light[1].spot_direction[2]);
	glUniform3fv(loc_light[1].spot_direction, 1, &direction_EC[0]);
	glUniform1f(loc_light[1].spot_cutoff_angle, light[1].spot_cutoff_angle);
	glUniform1f(loc_light[1].spot_exponent, light[1].spot_exponent);



	glUniform1i(loc_light[2].light_on, light[2].light_on);
	glUniform1i(loc_light[2].shadow_on, light[2].shadow_on);
	// need to supply position in EC for shading
	position_EC = ViewMatrix * glm::vec4(light[2].position[0], light[2].position[1], light[2].position[2], light[2].position[3]);
	glUniform4fv(loc_light[2].position, 1, &position_EC[0]);
	glUniform4fv(loc_light[2].ambient_color, 1, light[2].ambient_color);
	glUniform4fv(loc_light[2].diffuse_color, 1, light[2].diffuse_color);
	glUniform4fv(loc_light[2].specular_color, 1, light[2].specular_color);
	// need to supply direction in EC for shading in this example shader
	// note that the viewing transform is a rigid body transform
	// thus transpose(inverse(mat3(ViewMatrix)) = mat3(ViewMatrix)
	glm::vec3 direction_EC2 = glm::mat3(ViewMatrix) * glm::vec3(light[2].spot_direction[0], light[2].spot_direction[1],
		light[2].spot_direction[2]);
	glUniform3fv(loc_light[2].spot_direction, 1, &direction_EC2[0]);
	glUniform1f(loc_light[2].spot_cutoff_angle, light[2].spot_cutoff_angle);
	glUniform1f(loc_light[2].spot_exponent, light[2].spot_exponent);





	glUseProgram(0);
}

//하드코딩으로 해놧음!!!
Material_Parameters object_mat_list[NUMBER_OF_OBJECT];

 

void initialize_materials() {

	glUseProgram(h_ShaderProgram_TXPS);

	char string[256];
	GLint loc_ambient, loc_diff, loc_spec, loc_emissive, loc_spec_expo, loc;
	float* t;
	int len = 4;//지금 오브젝트가 네개 뿐이니까...
	
	for (int i = 0; i < len; i++) {
		sprintf(string, "object_materials[%d].ambient_color", i);
		loc = glGetUniformLocation(h_ShaderProgram_TXPS, string);

		t = object_mat_list[i].ambient_color;
		glUniform4f(loc, t[0],t[1],t[2],t[3]);


		sprintf(string, "object_materials[%d].diffuse_color", i);
		loc = glGetUniformLocation(h_ShaderProgram_TXPS, string);

		 t = object_mat_list[i].diffuse_color;
		glUniform4f(loc, t[0], t[1], t[2], t[3]);



		sprintf(string, "object_materials[%d].specular_color", i);
		loc = glGetUniformLocation(h_ShaderProgram_TXPS, string);

		t = object_mat_list[i].specular_color;
		glUniform4f(loc, t[0], t[1], t[2], t[3]);


		sprintf(string, "object_materials[%d].emissive_color", i);
		loc = glGetUniformLocation(h_ShaderProgram_TXPS, string);

		t = object_mat_list[i].emissive_color;
		glUniform4f(loc, t[0], t[1], t[2], t[3]);



		sprintf(string, "object_materials[%d].specular_exponent", i);
		loc = glGetUniformLocation(h_ShaderProgram_TXPS, string);

		glUniform1f(loc, object_mat_list[i].specular_exponent);


	}


	glUseProgram(0);
}

//오브젝트의 마테리얼 정보를 하나의 배열로 저장한다.
void push_obj_mat(int object_ID, Material_Parameters material_object) {
	object_mat_list[object_ID] = material_object;
}
class Random {
private:
	std::mt19937 generator;
	std::uniform_real_distribution<float> distr01;

public:
	Random() : distr01(0.0f, 1.0f) {
		std::random_device rd;
		generator.seed(rd());
	}

	float nextFloat() {
		return distr01(generator);
	}

	static void shuffle(std::vector<GLfloat> & v) {
		std::random_device rd;
		std::mt19937 g(rd());

		std::shuffle(v.begin(), v.end(), g);
	}

	glm::vec3 uniformHemisphere() {
		glm::vec3 result;
		
		result.x = (rand() % 1000) - 500;
		result.y = (rand() % 1000) - 500;
		result.z = rand() % 500;
		result = glm::normalize(result);
		return result;
	}

	glm::vec3 uniformCircle() {
		glm::vec3 result(0.0f);

		result.x = (rand() % 1000) - 500;
		result.y = (rand() % 1000) - 500;
		result.z = 0;
		result = glm::normalize(result);

		return result;
	}
};

void prepare_scene(void) {
	prepare_axes();
	prepare_floor();
	prepare_tiger();
	prepare_OPTIMUS();
	prepare_dragon();
	prepare_skybox();
	vbosphere = new VBOSphere(2.0f, 50, 50);
	prepare_environmentMap();

	push_obj_mat(OBJECT_ID_FLOOR, material_floor);
	push_obj_mat(OBJECT_ID_TIGER, material_tiger);

	srand(time(NULL));
	set_up_scene_lights();
	initialize_materials();
	
}

void prepare_shadow_mapping(int index) {

	ShadowMapping.texture_unit[index] = TEXTURE_INDEX_SHADOW + index;
	ShadowMapping.shadow_map_width = SHADOW_MAP_WIDTH;
	ShadowMapping.shadow_map_height = SHADOW_MAP_HEIGHT;
	ShadowMapping.near_dist = SHADOW_MAP_NEAR_DIST;
	ShadowMapping.far_dist = SHADOW_MAP_FAR_DIST;
	ShadowMapping.shadow_map_border_color[0] = 1.0f;
	ShadowMapping.shadow_map_border_color[1] = 0.0f;
	ShadowMapping.shadow_map_border_color[2] = 0.0f;
	ShadowMapping.shadow_map_border_color[3] = 0.0f;

	glGenTextures(1, &ShadowMapping.shadow_map_ID[index]);

	glActiveTexture(GL_TEXTURE0 + ShadowMapping.texture_unit[index]);
	glBindTexture(GL_TEXTURE_2D, ShadowMapping.shadow_map_ID[index]);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH_COMPONENT32, ShadowMapping.shadow_map_width, ShadowMapping.shadow_map_height);

	// Initialize the shadow map
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, ShadowMapping.shadow_map_border_color);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LESS); // or GL_LESS

	//변수 설정

	glUseProgram(h_ShaderProgram_TXPS);
	glUniform1i(loc_shadow_texture[index], ShadowMapping.texture_unit[index]);
	glUseProgram(0);

	// Initialize the Frame Buffer Object for rendering shadows
	glGenFramebuffers(1, &ShadowMapping.FBO_ID[index]);
	glBindFramebuffer(GL_FRAMEBUFFER, ShadowMapping.FBO_ID[index]);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, ShadowMapping.shadow_map_ID[index], 0);
	glDrawBuffer(GL_NONE);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		fprintf(stderr, "Error: the framebuffer object for shadow mapping is not complete...\n");
		exit(-1);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	ViewMatrix_SHADOW[index] = glm::lookAt(glm::vec3(light[index].position[0], light[index].position[1], light[index].position[2]),
		glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	if (index == 0) {

		ProjectionMatrix_SHADOW = glm::perspective(TO_RADIAN*90.0f, 1.0f, ShadowMapping.near_dist, ShadowMapping.far_dist);
		BiasMatrix = glm::mat4(0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.5f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.5f, 0.0f, 0.5f, 0.5f, 0.5f, 1.0f);

		ModelViewProjectionMatrix = glm::ortho(0.0f, 1.0f, 0.0f, 1.0f, -1.0f, 1.0f);

	}

}


void initialize_renderer(void) {
	register_callbacks();
	prepare_shader_program();
	initialize_OpenGL();
	prepare_scene();

	for (int i = 0; i < NUMBER_OF_LIGHT_COUNT; i++) {
		prepare_shadow_mapping(i);
	}
	//initialize_ssao();
	setupFBO();

}

void initialize_glew(void) {
	GLenum error;

	glewExperimental = GL_TRUE;

	error = glewInit();
	if (error != GLEW_OK) {
		fprintf(stderr, "Error: %s\n", glewGetErrorString(error));
		exit(-1);
	}
	fprintf(stdout, "*********************************************************\n");
	fprintf(stdout, " - GLEW version supported: %s\n", glewGetString(GLEW_VERSION));
	fprintf(stdout, " - OpenGL renderer: %s\n", glGetString(GL_RENDERER));
	fprintf(stdout, " - OpenGL version supported: %s\n", glGetString(GL_VERSION));
	fprintf(stdout, "*********************************************************\n\n");
}

void greetings(char *program_name, char messages[][256], int n_message_lines) {
	fprintf(stdout, "**************************************************************\n\n");
	fprintf(stdout, "  PROGRAM NAME: %s\n\n", program_name);
	fprintf(stdout, "    This program was coded for CSE3170 students\n");
	fprintf(stdout, "      of Dept. of Comp. Sci. & Eng., Sogang University.\n\n");

	for (int i = 0; i < n_message_lines; i++)
		fprintf(stdout, "%s\n", messages[i]);
	fprintf(stdout, "\n**************************************************************\n\n");

	initialize_glew();
}

#define N_MESSAGE_LINES 1
void main(int argc, char *argv[]) {
	// Phong Shading
	char program_name[64] = "Environment Mapping - GLSL";
	char messages[N_MESSAGE_LINES][256] = { "    - Keys used: 'v'(up), 'b'(down), 's'(sky change), 'n'(rotate object), 'm'(rotate cam), 'r'(rotate speed),\n                 't'(Texture toggle), 'p'(polygon mode), 'c'(Face culling mode)" };

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);
	// glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(scene_width, scene_height);
	glutInitContextVersion(4, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow(program_name);

	greetings(program_name, messages, N_MESSAGE_LINES);
	initialize_renderer();

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	glutMainLoop();
}