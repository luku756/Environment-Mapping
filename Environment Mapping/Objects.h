// axes object
GLuint axes_VBO, axes_VAO;
GLfloat axes_vertices[6][3] = {
	{ 0.0f, 0.0f, 0.0f },{ 1.0f, 0.0f, 0.0f },{ 0.0f, 0.0f, 0.0f },{ 0.0f, 1.0f, 0.0f },
{ 0.0f, 0.0f, 0.0f },{ 0.0f, 0.0f, 1.0f }
};
GLfloat axes_color[3][3] = { { 1.0f, 0.0f, 0.0f },{ 0.0f, 1.0f, 0.0f },{ 0.0f, 0.0f, 1.0f } };

void prepare_axes(void) { // draw coordinate axes
						  // initialize vertex buffer object
	glGenBuffers(1, &axes_VBO);

	glBindBuffer(GL_ARRAY_BUFFER, axes_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(axes_vertices), &axes_vertices[0][0], GL_STATIC_DRAW);

	// initialize vertex array object
	glGenVertexArrays(1, &axes_VAO);
	glBindVertexArray(axes_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, axes_VBO);
	glVertexAttribPointer(LOC_VERTEX, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}


#define NUMBER_OF_OBJECT 8
#define OBJECT_ID_FLOOR 2
#define OBJECT_ID_TIGER 3
#define OBJECT_ID_OPTIMUS 0
#define OBJECT_ID_DRAGON 1

void draw_axes(void) {
	// assume ShaderProgram_simple is used
	glBindVertexArray(axes_VAO);
	glUniform3fv(loc_primitive_color, 1, axes_color[0]);
	glDrawArrays(GL_LINES, 0, 2);
	glUniform3fv(loc_primitive_color, 1, axes_color[1]);
	glDrawArrays(GL_LINES, 2, 2);
	glUniform3fv(loc_primitive_color, 1, axes_color[2]);
	glDrawArrays(GL_LINES, 4, 2);
	glBindVertexArray(0);
}

void draw_axes(glm::mat4 mvp, float scale) {
	glUseProgram(h_ShaderProgram_simple);
	//ModelViewMatrix = glm::scale(ViewMatrix, glm::vec3(scale, scale, scale));
	//ModelViewProjectionMatrix = ProjectionMatrix * ModelViewMatrix;
	ModelViewProjectionMatrix = glm::scale(mvp, glm::vec3(20.0f, 20.0f, 20.0f));
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix_simple, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	glLineWidth(3.0f);
	draw_axes();
	glLineWidth(1.0f);
}


// floor object
GLuint rectangle_VBO, rectangle_VAO;
GLfloat rectangle_vertices[6][8] = {  // vertices enumerated counterclockwise
{ 0.0f, 0.0f, 0.0f,		 0.0f, 0.0f, 1.0f,		 0.0f, 0.0f },
{ 1.0f, 0.0f, 0.0f,		 0.0f, 0.0f, 1.0f,		 1.0f, 0.0f },
{ 1.0f, 1.0f, 0.0f,		 0.0f, 0.0f, 1.0f,		 1.0f, 1.0f },
{ 0.0f, 0.0f, 0.0f,		 0.0f, 0.0f, 1.0f,		 0.0f, 0.0f },
{ 1.0f, 1.0f, 0.0f,		 0.0f, 0.0f, 1.0f,		 1.0f, 1.0f },
{ 0.0f, 1.0f, 0.0f,		 0.0f, 0.0f, 1.0f,		 0.0f, 1.0f }
};


GLuint quad_VBO, quad_VAO;
GLfloat quad_vertices[6][8] = {  // vertices enumerated counterclockwise
	
{ -1.0f, -1.0f, 0.0f,		 0.0f, 0.0f, 1.0f,		 0.0f, 0.0f },
{ 1.0f, -1.0f, 0.0f,		 0.0f, 0.0f, 1.0f,		 1.0f, 0.0f },
{ 1.0f, 1.0f, 0.0f,		 0.0f, 0.0f, 1.0f,		 1.0f, 1.0f },
{ -1.0f, -1.0f, 0.0f,		 0.0f, 0.0f, 1.0f,		 0.0f, 0.0f },
{ 1.0f, 1.0f, 0.0f,		 0.0f, 0.0f, 1.0f,		 1.0f, 1.0f },
{ -1.0f, 1.0f, 0.0f,		 0.0f, 0.0f, 1.0f,		 0.0f, 1.0f }
};

Material_Parameters material_floor;

void prepare_floor(void) { // Draw coordinate axes.
						   // Initialize vertex buffer object.
	glGenBuffers(1, &rectangle_VBO);

	glBindBuffer(GL_ARRAY_BUFFER, rectangle_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rectangle_vertices), &rectangle_vertices[0][0], GL_STATIC_DRAW);

	// Initialize vertex array object.
	glGenVertexArrays(1, &rectangle_VAO);
	glBindVertexArray(rectangle_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, rectangle_VBO);
	glVertexAttribPointer(LOC_VERTEX, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), BUFFER_OFFSET(0));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(LOC_NORMAL, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), BUFFER_OFFSET(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(LOC_TEXCOORD, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), BUFFER_OFFSET(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	material_floor.ambient_color[0] = 0.0f;
	material_floor.ambient_color[1] = 0.05f;
	material_floor.ambient_color[2] = 0.0f;
	material_floor.ambient_color[3] = 1.0f;

	material_floor.diffuse_color[0] = 0.2f;
	material_floor.diffuse_color[1] = 0.5f;
	material_floor.diffuse_color[2] = 0.2f;
	material_floor.diffuse_color[3] = 1.0f;

	material_floor.specular_color[0] = 0.24f;
	material_floor.specular_color[1] = 0.5f;
	material_floor.specular_color[2] = 0.24f;
	material_floor.specular_color[3] = 1.0f;

	material_floor.specular_exponent = 2.5f;

	material_floor.emissive_color[0] = 0.0f;
	material_floor.emissive_color[1] = 0.0f;
	material_floor.emissive_color[2] = 0.0f;
	material_floor.emissive_color[3] = 1.0f;

	glActiveTexture(GL_TEXTURE0 + TEXTURE_INDEX_FLOOR);
	glBindTexture(GL_TEXTURE_2D, texture_names[TEXTURE_INDEX_FLOOR]);

	My_glTexImage2D_from_file("Data/grass_tex.jpg");

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

}

void set_material_floor(void) {
	// assume ShaderProgram_TXPS is used
	glUniform1i(loc_object_id, OBJECT_ID_FLOOR);
	glUniform4fv(loc_material.ambient_color, 1, material_floor.ambient_color);
	glUniform4fv(loc_material.diffuse_color, 1, material_floor.diffuse_color);
	glUniform4fv(loc_material.specular_color, 1, material_floor.specular_color);
	glUniform1f(loc_material.specular_exponent, material_floor.specular_exponent);
	glUniform4fv(loc_material.emissive_color, 1, material_floor.emissive_color);
}

void draw_floor(void) {
	glFrontFace(GL_CCW);

	glBindVertexArray(rectangle_VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}


void prepare_quad(void) { // Draw coordinate axes.
						   // Initialize vertex buffer object.
	glGenBuffers(1, &quad_VBO);

	glBindBuffer(GL_ARRAY_BUFFER, quad_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quad_vertices), &quad_vertices[0][0], GL_STATIC_DRAW);

	// Initialize vertex array object.
	glGenVertexArrays(1, &quad_VAO);
	glBindVertexArray(quad_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, quad_VBO);
	glVertexAttribPointer(LOC_VERTEX, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), BUFFER_OFFSET(0));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(LOC_NORMAL, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), BUFFER_OFFSET(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(LOC_TEXCOORD, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), BUFFER_OFFSET(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}

void draw_quad(void) {
	glFrontFace(GL_CCW);

	glBindVertexArray(quad_VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

// tiger object
#define N_TIGER_FRAMES 12
GLuint tiger_VBO, tiger_VAO;
int tiger_n_triangles[N_TIGER_FRAMES];
int tiger_vertex_offset[N_TIGER_FRAMES];
GLfloat *tiger_vertices[N_TIGER_FRAMES];

Material_Parameters material_tiger;

int read_geometry(GLfloat **object, int bytes_per_primitive, const char *filename) {
	int n_triangles;
	FILE *fp;

	// fprintf(stdout, "Reading geometry from the geometry file %s...\n", filename);
	fp = fopen(filename, "rb");
	if (fp == NULL) {
		fprintf(stderr, "Cannot open the object file %s ...", filename);
		return -1;
	}
	fread(&n_triangles, sizeof(int), 1, fp);
	*object = (float *)malloc(n_triangles*bytes_per_primitive);
	if (*object == NULL) {
		fprintf(stderr, "Cannot allocate memory for the geometry file %s ...", filename);
		return -1;
	}

	fread(*object, bytes_per_primitive, n_triangles, fp);
	// fprintf(stdout, "Read %d primitives successfully.\n\n", n_triangles);
	fclose(fp);

	return n_triangles;
}

void prepare_tiger(void) { // vertices enumerated clockwise
	glUniform1i(loc_object_id, OBJECT_ID_TIGER);
	int i, n_bytes_per_vertex, n_bytes_per_triangle, tiger_n_total_triangles = 0;
	char filename[512];

	n_bytes_per_vertex = 8 * sizeof(float); // 3 for vertex, 3 for normal, and 2 for texcoord
	n_bytes_per_triangle = 3 * n_bytes_per_vertex;

	for (i = 0; i < N_TIGER_FRAMES; i++) {
		sprintf(filename, "Data/Tiger_%d%d_triangles_vnt.geom", i / 10, i % 10);
		tiger_n_triangles[i] = read_geometry(&tiger_vertices[i], n_bytes_per_triangle, filename);
		// assume all geometry files are effective
		tiger_n_total_triangles += tiger_n_triangles[i];

		if (i == 0)
			tiger_vertex_offset[i] = 0;
		else
			tiger_vertex_offset[i] = tiger_vertex_offset[i - 1] + 3 * tiger_n_triangles[i - 1];
	}

	// initialize vertex buffer object
	glGenBuffers(1, &tiger_VBO);

	glBindBuffer(GL_ARRAY_BUFFER, tiger_VBO);
	glBufferData(GL_ARRAY_BUFFER, tiger_n_total_triangles*n_bytes_per_triangle, NULL, GL_STATIC_DRAW);

	for (i = 0; i < N_TIGER_FRAMES; i++)
		glBufferSubData(GL_ARRAY_BUFFER, tiger_vertex_offset[i] * n_bytes_per_vertex,
			tiger_n_triangles[i] * n_bytes_per_triangle, tiger_vertices[i]);

	// as the geometry data exists now in graphics memory, ...
	for (i = 0; i < N_TIGER_FRAMES; i++)
		free(tiger_vertices[i]);

	// initialize vertex array object
	glGenVertexArrays(1, &tiger_VAO);
	glBindVertexArray(tiger_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, tiger_VBO);
	glVertexAttribPointer(LOC_VERTEX, 3, GL_FLOAT, GL_FALSE, n_bytes_per_vertex, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(LOC_NORMAL, 3, GL_FLOAT, GL_FALSE, n_bytes_per_vertex, BUFFER_OFFSET(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(LOC_TEXCOORD, 2, GL_FLOAT, GL_FALSE, n_bytes_per_vertex, BUFFER_OFFSET(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	material_tiger.ambient_color[0] = 0.24725f;
	material_tiger.ambient_color[1] = 0.1995f;
	material_tiger.ambient_color[2] = 0.0745f;
	material_tiger.ambient_color[3] = 1.0f;

	material_tiger.diffuse_color[0] = 0.75164f;
	material_tiger.diffuse_color[1] = 0.60648f;
	material_tiger.diffuse_color[2] = 0.22648f;
	material_tiger.diffuse_color[3] = 1.0f;

	material_tiger.specular_color[0] = 0.728281f;
	material_tiger.specular_color[1] = 0.655802f;
	material_tiger.specular_color[2] = 0.466065f;
	material_tiger.specular_color[3] = 1.0f;

	material_tiger.specular_exponent = 51.2f;

	material_tiger.emissive_color[0] = 0.1f;
	material_tiger.emissive_color[1] = 0.1f;
	material_tiger.emissive_color[2] = 0.0f;
	material_tiger.emissive_color[3] = 1.0f;

	glActiveTexture(GL_TEXTURE0 + TEXTURE_INDEX_TIGER);
	glBindTexture(GL_TEXTURE_2D, texture_names[TEXTURE_INDEX_TIGER]);

	My_glTexImage2D_from_file("Data/tiger_tex2.jpg");

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

void set_material_tiger(void) {
	glUniform1i(loc_object_id, OBJECT_ID_TIGER);
	// assume ShaderProgram_TXPS is used
	glUniform4fv(loc_material.ambient_color, 1, material_tiger.ambient_color);
	glUniform4fv(loc_material.diffuse_color, 1, material_tiger.diffuse_color);
	glUniform4fv(loc_material.specular_color, 1, material_tiger.specular_color);
	glUniform1f(loc_material.specular_exponent, material_tiger.specular_exponent);
	glUniform4fv(loc_material.emissive_color, 1, material_tiger.emissive_color);
}

void draw_tiger(void) {
	glFrontFace(GL_CW);

	glBindVertexArray(tiger_VAO);
	glDrawArrays(GL_TRIANGLES, tiger_vertex_offset[cur_frame_tiger], 3 * tiger_n_triangles[cur_frame_tiger]);
	glBindVertexArray(0);
}



// other objects

#define N_OBJECTS	3 // objects other than the moving tiger
#define OBJECT_OPTIMUS 0
#define OBJECT_DRAGON 1

GLuint object_VBO[N_OBJECTS], object_VAO[N_OBJECTS];
int object_n_triangles[N_OBJECTS];
GLfloat *object_vertices[N_OBJECTS];

Material_Parameters material_object[N_OBJECTS];

void set_up_object(int object_ID, const char *filename, int n_bytes_per_vertex) {
	object_n_triangles[object_ID] = read_geometry(&object_vertices[object_ID],
		3 * n_bytes_per_vertex, filename);
	// Error checking is needed here.

	// Initialize vertex buffer object.
	glGenBuffers(1, &object_VBO[object_ID]);

	glBindBuffer(GL_ARRAY_BUFFER, object_VBO[object_ID]);
	glBufferData(GL_ARRAY_BUFFER, object_n_triangles[object_ID] * 3 * n_bytes_per_vertex,
		object_vertices[object_ID], GL_STATIC_DRAW);

	// As the geometry data exists now in graphics memory, ...
	free(object_vertices[object_ID]);

	// Initialize vertex array object.
	glGenVertexArrays(1, &object_VAO[object_ID]);
	glBindVertexArray(object_VAO[object_ID]);

	glBindBuffer(GL_ARRAY_BUFFER, object_VBO[object_ID]);
	glVertexAttribPointer(LOC_VERTEX, 3, GL_FLOAT, GL_FALSE, n_bytes_per_vertex, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(LOC_VERTEX);
	glVertexAttribPointer(LOC_NORMAL, 3, GL_FLOAT, GL_FALSE, n_bytes_per_vertex, BUFFER_OFFSET(3 * sizeof(float)));
	glEnableVertexAttribArray(LOC_NORMAL);
	//if (n_bytes_per_vertex == 8 * sizeof(float)) {
		glVertexAttribPointer(LOC_TEXCOORD, 2, GL_FLOAT, GL_FALSE, n_bytes_per_vertex, BUFFER_OFFSET(6 * sizeof(float)));
		glEnableVertexAttribArray(LOC_TEXCOORD);
	//}
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void push_obj_mat(int object_ID, Material_Parameters material_object);

void initialize_object_material(int object_ID,
	float a_r, float a_g, float a_b, float a_a,
	float d_r, float d_g, float d_b, float d_a,
	float s_r, float s_g, float s_b, float s_a, float s_e,
	float e_r, float e_g, float e_b, float e_a) {
	material_object[object_ID].ambient_color[0] = a_r;
	material_object[object_ID].ambient_color[1] = a_g;
	material_object[object_ID].ambient_color[2] = a_b;
	material_object[object_ID].ambient_color[3] = a_a;

	material_object[object_ID].diffuse_color[0] = d_r;
	material_object[object_ID].diffuse_color[1] = d_g;
	material_object[object_ID].diffuse_color[2] = d_b;
	material_object[object_ID].diffuse_color[3] = d_a;

	material_object[object_ID].specular_color[0] = s_r;
	material_object[object_ID].specular_color[1] = s_g;
	material_object[object_ID].specular_color[2] = s_b;
	material_object[object_ID].specular_color[3] = s_a;

	material_object[object_ID].specular_exponent = s_e;

	material_object[object_ID].emissive_color[0] = e_r;
	material_object[object_ID].emissive_color[1] = e_g;
	material_object[object_ID].emissive_color[2] = e_b;
	material_object[object_ID].emissive_color[3] = e_a;

	push_obj_mat(object_ID, material_object[object_ID]);
}

void set_material_object(int object_ID) {
	glUniform1i(loc_object_id, object_ID);
	// assume ShaderProgram_PS is used
	glUniform4fv(loc_material.ambient_color, 1, material_object[object_ID].ambient_color);
	glUniform4fv(loc_material.diffuse_color, 1, material_object[object_ID].diffuse_color);
	glUniform4fv(loc_material.specular_color, 1, material_object[object_ID].specular_color);
	glUniform1f(loc_material.specular_exponent, material_object[object_ID].specular_exponent);
	glUniform4fv(loc_material.emissive_color, 1, material_object[object_ID].emissive_color);
}

void draw_object(int object_ID, float r, float g, float b, unsigned int polygon_orientation) {
	glFrontFace(polygon_orientation);

	glUniform3f(loc_primitive_color, r, g, b);
	glBindVertexArray(object_VAO[object_ID]);
	glDrawArrays(GL_TRIANGLES, 0, 3 * object_n_triangles[object_ID]);
	glBindVertexArray(0);
}

void prepare_OPTIMUS(void) {
	set_up_object(OBJECT_OPTIMUS, "Data/optimus_vnt.geom", 8 * sizeof(float));
	initialize_object_material(OBJECT_OPTIMUS,
		0.1745f, 0.01175f, 0.01175f, 1.0f,
		0.61424f, 0.04136f, 0.04136f, 1.0f,
		0.727811f, 0.626959f, 0.626959f, 1.0f, 76.8f,
		0.1f, 0.1f, 0.1f, 1.0f);
}


void prepare_dragon(void) {
	set_up_object(OBJECT_DRAGON, "Data/dragon_vnt.geom", 8 * sizeof(float));
	initialize_object_material(OBJECT_DRAGON,
		0.329412f, 0.223529f, 0.027451f, 1.0f,
		0.780392f, 0.568627f, 0.113725f, 1.0f,
		0.992157f, 0.941176f, 0.807843f, 1.0f, 76.8f,
		0.0f, 0.0f, 0.0f, 1.0f);
}

int readShort(std::ifstream & stream) {
	unsigned char ic;     // input char
	int result;           // result

	ic = stream.get();
	result = ic;
	ic = stream.get();
	result |= ((unsigned int)ic << 8);
	return result;
}
GLubyte * readTexture(const char * fName, int & width, int & height) {
	//using namespace std;

	// Open file for reading

	std::ifstream inFile(fName, std::ios::binary);

	if (!inFile) {
		std::string msg = std::string("Error: can't open ") + fName;
		printf("%s\n", msg.c_str());
		return NULL;
		//throw IOException(msg);
	}

	int idLen = inFile.get();      // Length of image ID field
	int mapType = inFile.get();    // Color map type (expect 0 == no color map)
	int typeCode = inFile.get();   // Image type code (expect 2 == uncompressed)
	inFile.ignore(5);              // Color map info (ignored)
	int xOrigin = readShort(inFile);  // X origin
	int yOrigin = readShort(inFile);  // Y origin
	width = readShort(inFile);        // Image width
	height = readShort(inFile);       // Image height
	int bpp = inFile.get();               // Bits per pixel (expect 24 or 32)
	inFile.ignore();                      // Image descriptor (expect 0 for 24bpp and 8 for 32bpp)

	if (typeCode != 2 || mapType != 0) {
		//throw IOException("File does not appear to be a non-color-mapped, uncompressed TGA image");
		printf("File does not appear to be a non-color-mapped, uncompressed TGA image");
		return NULL;
	}

	if (bpp != 24 && bpp != 32) {
		//throw IOException("File must be 24 or 32 bits per pixel.\n");
		printf("File must be 24 or 32 bits per pixel.\n");
		return NULL;

	}

	// Skip the image ID data
	if (idLen > 0) inFile.ignore(idLen);

	// Color map data would be here, but we assume no color map

//	printf("%s: (%d x %d) %d bpp origin (%d, %d)\n", fName, width, height, bpp, xOrigin, yOrigin);

	// Read pixel data
	GLubyte *p = new GLubyte[width * height * 4];
	// 24 bpp -- Blue, Green, Red
	// 32 bpp -- Blue, Green, Red, Alpha
	// p -- stored as RGBA
	for (unsigned int i = 0; i < (unsigned int)(width * height); i++)
	{
		p[i * 4 + 2] = inFile.get();  // Blue
		p[i * 4 + 1] = inFile.get();  // Green
		p[i * 4] = inFile.get();  // Red
		if (bpp == 32)
			p[i * 4 + 3] = inFile.get();
		else
			p[i * 4 + 3] = 0xFF;
	}

	inFile.close();
	return p;
}

GLuint skyboxTex[TEXTURE_SIZE_SKYBOX];

void loadCubeMap(const char * baseFileName, int cubeindex)
{
	glActiveTexture(GL_TEXTURE0 + cubeindex);

	GLuint texID;
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texID);
	skyboxTex[cubeindex - TEXTURE_INDEX_SKYBOX1] = texID;
	//const char * suffixes[] = { "posx", "negx", "posy", "negy", "posz", "negz" };
	const char * suffixes[] = { "lf", "rt", "up", "dn", "bk", "ft" };
	//const char * suffixes[] = { "rt", "lf", "dn", "up", "bk", "ft" };
	GLuint targets[] = {
		GL_TEXTURE_CUBE_MAP_POSITIVE_X, GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Y, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Z, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
	};

	GLint w, h;
	// Allocate immutable storage for the cube map texture
	//glTexStorage2D(GL_TEXTURE_CUBE_MAP, 1, GL_RGBA8, 256, 256);
	glTexStorage2D(GL_TEXTURE_CUBE_MAP, 1, GL_RGBA8, 512, 512);

	// Load each cube-map face
	for (int i = 0; i < 6; i++) {
		std::string texName = std::string(baseFileName) + "_" + suffixes[i] + ".tga";
		GLubyte * data = readTexture(texName.c_str(), w, h);

		glTexSubImage2D(targets[i], 0, 0, 0, w, h,
			GL_RGBA, GL_UNSIGNED_BYTE, data);

		delete[] data;
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

}
//SkyBox *sky;

GLfloat skyboxVertices[] = {
	// positions          
	-100.0f,  100.0f, -100.0f,
	-100.0f, -100.0f, -100.0f,
	 100.0f, -100.0f, -100.0f,
	 100.0f, -100.0f, -100.0f,
	 100.0f,  100.0f, -100.0f,
	-100.0f,  100.0f, -100.0f,

	-100.0f, -100.0f,  100.0f,
	-100.0f, -100.0f, -100.0f,
	-100.0f,  100.0f, -100.0f,
	-100.0f,  100.0f, -100.0f,
	-100.0f,  100.0f,  100.0f,
	-100.0f, -100.0f,  100.0f,

	 100.0f, -100.0f, -100.0f,
	 100.0f, -100.0f,  100.0f,
	 100.0f,  100.0f,  100.0f,
	 100.0f,  100.0f,  100.0f,
	 100.0f,  100.0f, -100.0f,
	 100.0f, -100.0f, -100.0f,

	-100.0f, -100.0f,  100.0f,
	-100.0f,  100.0f,  100.0f,
	 100.0f,  100.0f,  100.0f,
	 100.0f,  100.0f,  100.0f,
	 100.0f, -100.0f,  100.0f,
	-100.0f, -100.0f,  100.0f,

	-100.0f,  100.0f, -100.0f,
	 100.0f,  100.0f, -100.0f,
	 100.0f,  100.0f,  100.0f,
	 100.0f,  100.0f,  100.0f,
	-100.0f,  100.0f,  100.0f,
	-100.0f,  100.0f, -100.0f,

	-100.0f, -100.0f, -100.0f,
	-100.0f, -100.0f,  100.0f,
	 100.0f, -100.0f, -100.0f,
	 100.0f, -100.0f, -100.0f,
	-100.0f, -100.0f,  100.0f,
	 100.0f, -100.0f,  100.0f
};
GLuint skybox_VBO, skybox_VAO;

void prepare_skybox(void) { // Draw coordinate axes.
						   // Initialize vertex buffer object.
	
	glGenVertexArrays(1, &skybox_VAO);
	glGenBuffers(1, &skybox_VBO);
	glBindVertexArray(skybox_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, skybox_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	loadCubeMap("Data/skybox/mp_marvelous/bloody-marvelous", TEXTURE_INDEX_SKYBOX1);
	loadCubeMap("Data/skybox/mp_majestic/majestic", TEXTURE_INDEX_SKYBOX2);
	loadCubeMap("Data/skybox/mp_orbital/orbital-element", TEXTURE_INDEX_SKYBOX3);
	loadCubeMap("Data/skybox/mp_crimimpact/criminal-impact", TEXTURE_INDEX_SKYBOX4);
	loadCubeMap("Data/skybox/mp_blood/blood", TEXTURE_INDEX_SKYBOX5);
}

void draw_skybox(void) {

	glFrontFace(GL_CCW);

	glBindVertexArray(skybox_VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}


//void prepare_skybox()
//{
//	sky = new SkyBox();
//
//	//loadCubeMap("Data/skybox/cubemap_night/night");
//	//loadCubeMap("Data/skybox/mp_blood/blood");
//	loadCubeMap("Data/skybox/mp_marvelous/bloody-marvelous");
//
//
//}


class VBOSphere {

	unsigned int vaoHandle;
	GLuint nVerts, elements;
	float radius;
	GLuint slices, stacks;

public:
	VBOSphere(float rad, GLuint sl, GLuint st) :
		radius(rad), slices(sl), stacks(st)
	{

		nVerts = (slices + 1) * (stacks + 1);
		elements = (slices * 2 * (stacks - 1)) * 3;

		// Verts
		float * v = new float[3 * nVerts];
		// Normals
		float * n = new float[3 * nVerts];
		// Tex coords
		float * tex = new float[2 * nVerts];
		// Elements
		unsigned int * el = new unsigned int[elements];

		// Generate the vertex data
		generateVerts(v, n, tex, el);

		// Create and populate the buffer objects
		unsigned int handle[4];
		glGenBuffers(4, handle);

		glBindBuffer(GL_ARRAY_BUFFER, handle[0]);
		glBufferData(GL_ARRAY_BUFFER, (3 * nVerts) * sizeof(float), v, GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, handle[1]);
		glBufferData(GL_ARRAY_BUFFER, (3 * nVerts) * sizeof(float), n, GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, handle[2]);
		glBufferData(GL_ARRAY_BUFFER, (2 * nVerts) * sizeof(float), tex, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle[3]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, elements * sizeof(unsigned int), el, GL_STATIC_DRAW);

		delete[] v;
		delete[] n;
		delete[] el;
		delete[] tex;

		// Create the VAO
		glGenVertexArrays(1, &vaoHandle);
		glBindVertexArray(vaoHandle);

		glEnableVertexAttribArray(0);  // Vertex position
		glBindBuffer(GL_ARRAY_BUFFER, handle[0]);
		glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glEnableVertexAttribArray(1);  // Vertex normal
		glBindBuffer(GL_ARRAY_BUFFER, handle[1]);
		glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glBindBuffer(GL_ARRAY_BUFFER, handle[2]);
		glEnableVertexAttribArray(2);  // Texture coords
		glVertexAttribPointer((GLuint)2, 2, GL_FLOAT, GL_FALSE, 0, 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle[3]);

		glBindVertexArray(0);
	}

	void render() const {
		glBindVertexArray(vaoHandle);
		glDrawElements(GL_TRIANGLES, elements, GL_UNSIGNED_INT, ((GLubyte *)NULL + (0)));
	}

	void generateVerts(float * verts, float * norms, float * tex,
		unsigned int * el)
	{
		// Generate positions and normals
		GLfloat theta, phi;
		GLfloat thetaFac = glm::two_pi<float>() / slices;
		GLfloat phiFac = glm::pi<float>() / stacks;
		GLfloat nx, ny, nz, s, t;
		GLuint idx = 0, tIdx = 0;
		for (GLuint i = 0; i <= slices; i++) {
			theta = i * thetaFac;
			s = (GLfloat)i / slices;
			for (GLuint j = 0; j <= stacks; j++) {
				phi = j * phiFac;
				t = (GLfloat)j / stacks;
				nx = sinf(phi) * cosf(theta);
				ny = sinf(phi) * sinf(theta);
				nz = cosf(phi);
				verts[idx] = radius * nx; verts[idx + 1] = radius * ny; verts[idx + 2] = radius * nz;
				norms[idx] = nx; norms[idx + 1] = ny; norms[idx + 2] = nz;
				idx += 3;

				tex[tIdx] = s;
				tex[tIdx + 1] = t;
				tIdx += 2;
			}
		}

		// Generate the element list
		idx = 0;
		for (GLuint i = 0; i < slices; i++) {
			GLuint stackStart = i * (stacks + 1);
			GLuint nextStackStart = (i + 1) * (stacks + 1);
			for (GLuint j = 0; j < stacks; j++) {
				if (j == 0) {
					el[idx] = stackStart;
					el[idx + 1] = stackStart + 1;
					el[idx + 2] = nextStackStart + 1;
					idx += 3;
				}
				else if (j == stacks - 1) {
					el[idx] = stackStart + j;
					el[idx + 1] = stackStart + j + 1;
					el[idx + 2] = nextStackStart + j;
					idx += 3;
				}
				else {
					el[idx] = stackStart + j;
					el[idx + 1] = stackStart + j + 1;
					el[idx + 2] = nextStackStart + j + 1;
					el[idx + 3] = nextStackStart + j;
					el[idx + 4] = stackStart + j;
					el[idx + 5] = nextStackStart + j + 1;
					idx += 6;
				}
			}
		}
	}

	int getVertexArrayHandle() {
		return this->vaoHandle;
	}
};

VBOSphere *vbosphere;

//obj 파일을 읽는다. 단 하나의 obj 가 있는 파일만 읽음. face가 quad가 아닌 triangle 인 경우에만 유효하다.
int read_obj_file(GLfloat **object, const char *filename) {
	int n_triangles;
	FILE *fp;

	char buf[1000], tmp[256];
	char* c;

	fp = fopen(filename, "rb");

	//meterial 정보 및 이름.
	char mtllib[100];
	char matname[100];

	//읽어 온 정보
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> texcoords;
	float x, y, z;

	//face 라인에서 인덱스를 담을 배열
	int indexes_vertex[4];
	int indexes_normal[4];
	int indexes_texcod[4];

	//실제 face를 보고 순서대로 정렬된 정점들.
	std::vector < glm::vec3 >  tri_vertices;
	std::vector < glm::vec2 >  tri_uvs;
	std::vector < glm::vec3 >  tri_normals;

	char type[100];//그 줄의 타입(헤더). 어떤 데이터를 담고 있는지
	int res;

	while (1) {
		c = fgets(buf, 1000, fp);
		if (c == NULL) {
			break;
		}
		res = fscanf(fp, "%s", type);

		if (strcmp(type, "v") == 0) {//위치 정보
			fscanf(fp, "%f %f %f", &x, &y, &z);
			vertices.push_back(glm::vec3(x, y, z));
		}
		else if (strcmp(type, "vn") == 0) {//노말벡터 정보
			fscanf(fp, "%f %f %f", &x, &y, &z);
			normals.push_back(glm::vec3(x, y, z));
		}
		else if (strcmp(type, "vt") == 0) {//uv (텍스쳐 좌표) 정보
			fscanf(fp, "%f %f", &x, &y);
			texcoords.push_back(glm::vec2(x, y));
		}
		else if (strcmp(type, "f") == 0) {//삼각형 구성 정보
			fscanf(fp, "%d/%d/%d %d/%d/%d %d/%d/%d",
				&indexes_vertex[0], &indexes_texcod[0], &indexes_normal[0],
				&indexes_vertex[1], &indexes_texcod[1], &indexes_normal[1],
				&indexes_vertex[2], &indexes_texcod[2], &indexes_normal[2]);
			for (int i = 0; i < 3; i++) {
				tri_vertices.push_back(vertices[indexes_vertex[i] - 1]);
				tri_normals.push_back(normals[indexes_normal[i] - 1]);
				tri_uvs.push_back(texcoords[indexes_texcod[i] - 1]);
			}
		}
		else if (strcmp(type, "mtllib") == 0) {
			fscanf(fp, "%s", mtllib);
		}
		else if (strcmp(type, "usemtl") == 0) {
			fscanf(fp, "%s", matname);
		}
	}


	fclose(fp);

	//삼각형의 수.
	n_triangles = tri_vertices.size() / 3;

	//실제 총 데이터의 크기
	int size = sizeof(glm::vec3) * tri_vertices.size() + sizeof(glm::vec3) * tri_normals.size() + sizeof(glm::vec2) * tri_uvs.size();

	GLfloat *object_vertices;
	object_vertices = (float*)malloc(size);

	int id = 0;
	for (int i = 0; i < tri_vertices.size(); i++) {
		object_vertices[id++] = tri_vertices[i].x;
		object_vertices[id++] = tri_vertices[i].y;
		object_vertices[id++] = tri_vertices[i].z;
		object_vertices[id++] = tri_normals[i].x;
		object_vertices[id++] = tri_normals[i].y;
		object_vertices[id++] = tri_normals[i].z;
		object_vertices[id++] = tri_uvs[i].x;
		object_vertices[id++] = tri_uvs[i].y;
	}

	*object = object_vertices;

	return n_triangles;
}


GLuint videocam_VBO, videocam_VAO;
GLfloat *videocam_vertices;
int videocam_vertex_size;

GLuint videodisplay_VBO, videodisplay_VAO;
GLfloat *videodisplay_vertices;
int videodisplay_vertex_size;

Material_Parameters material_camera;

void set_material_videocam(void) {
	// assume ShaderProgram_TXPS is used
	glUniform4fv(loc_material.ambient_color, 1, material_camera.ambient_color);
	glUniform4fv(loc_material.diffuse_color, 1, material_camera.diffuse_color);
	glUniform4fv(loc_material.specular_color, 1, material_camera.specular_color);
	glUniform1f(loc_material.specular_exponent, material_camera.specular_exponent);
	glUniform4fv(loc_material.emissive_color, 1, material_camera.emissive_color);
}



void prepare_video() {
	videocam_vertex_size = read_obj_file(&videocam_vertices, "Data/Video_camera.obj");
	videocam_vertex_size *= 3;//삼각형 수가 아닌 정점의 수가 필요.
	int size = videocam_vertex_size * 8 * sizeof(float);

	glGenBuffers(1, &videocam_VBO);

	glBindBuffer(GL_ARRAY_BUFFER, videocam_VBO);
	glBufferData(GL_ARRAY_BUFFER, size, videocam_vertices, GL_STATIC_DRAW);

	// Initialize vertex array object.
	glGenVertexArrays(1, &videocam_VAO);
	glBindVertexArray(videocam_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, videocam_VBO);
	glVertexAttribPointer(LOC_VERTEX, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), BUFFER_OFFSET(0));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(LOC_NORMAL, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), BUFFER_OFFSET(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(LOC_TEXCOORD, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), BUFFER_OFFSET(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	material_camera.ambient_color[0] = 0.5882f;
	material_camera.ambient_color[1] = 0.5882f;
	material_camera.ambient_color[2] = 0.5882f;
	material_camera.ambient_color[3] = 1.0f;

	material_camera.diffuse_color[0] = 0.5882f;
	material_camera.diffuse_color[1] = 0.5882f;
	material_camera.diffuse_color[2] = 0.5882f;
	material_camera.diffuse_color[3] = 1.0f;

	material_camera.specular_color[0] = 0.0f;
	material_camera.specular_color[1] = 0.0f;
	material_camera.specular_color[2] = 0.0f;
	material_camera.specular_color[3] = 1.0f;

	material_camera.specular_exponent = 2.5f;

	material_camera.emissive_color[0] = 0.0f;
	material_camera.emissive_color[1] = 0.0f;
	material_camera.emissive_color[2] = 0.0f;
	material_camera.emissive_color[3] = 1.0f;

	glActiveTexture(GL_TEXTURE0 + TEXTURE_INDEX_CAMERA);
	glBindTexture(GL_TEXTURE_2D, texture_names[TEXTURE_INDEX_CAMERA]);

	My_glTexImage2D_from_file("Data/video_camera.jpg");

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);


	videodisplay_vertex_size = read_obj_file(&videodisplay_vertices, "Data/video_display.obj");
	videodisplay_vertex_size *= 3;//삼각형 수가 아닌 정점의 수가 필요.
	size = videodisplay_vertex_size * 8 * sizeof(float);

	glGenBuffers(1, &videodisplay_VBO);

	glBindBuffer(GL_ARRAY_BUFFER, videodisplay_VBO);
	glBufferData(GL_ARRAY_BUFFER, size, videodisplay_vertices, GL_STATIC_DRAW);

	// Initialize vertex array object.
	glGenVertexArrays(1, &videodisplay_VAO);
	glBindVertexArray(videodisplay_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, videodisplay_VBO);
	glVertexAttribPointer(LOC_VERTEX, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), BUFFER_OFFSET(0));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(LOC_NORMAL, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), BUFFER_OFFSET(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(LOC_TEXCOORD, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), BUFFER_OFFSET(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}
void draw_video() {

	glUniform1i(loc_base_texture, TEXTURE_INDEX_CAMERA);
	//glUniform1i(loc_objectId, OBJECT_CAMERA);
	set_material_videocam();

	glFrontFace(GL_CCW);

	glBindVertexArray(videocam_VAO);
	glDrawArrays(GL_TRIANGLES, 0, videocam_vertex_size);
	glBindVertexArray(0);

	glBindVertexArray(videodisplay_VAO);
	glDrawArrays(GL_TRIANGLES, 0, videodisplay_vertex_size);
	glBindVertexArray(0);
}




GLuint now_Shader_program;

void setUniform(const char *name, float x, float y, float z)
{
	GLint loc = glGetUniformLocation(now_Shader_program, name);
	glUniform3f(loc, x, y, z);
}

void setUniform(const char *name, const glm::vec3 & v)
{
	setUniform(name, v.x, v.y, v.z);
}

void setUniform(const char *name, const glm::vec4 & v)
{
	GLint loc = glGetUniformLocation(now_Shader_program, name);
	glUniform4f(loc, v.x, v.y, v.z, v.w);
}

void setUniform(const char *name, const glm::vec2 & v)
{
	GLint loc = glGetUniformLocation(now_Shader_program, name);
	glUniform2f(loc, v.x, v.y);
}

void setUniform(const char *name, const glm::mat4 & m)
{
	GLint loc = glGetUniformLocation(now_Shader_program, name);
	glUniformMatrix4fv(loc, 1, GL_FALSE, &m[0][0]);
}

void setUniform(const char *name, const glm::mat3 & m)
{
	GLint loc = glGetUniformLocation(now_Shader_program, name);
	glUniformMatrix3fv(loc, 1, GL_FALSE, &m[0][0]);
}

void setUniform(const char *name, float val)
{
	GLint loc = glGetUniformLocation(now_Shader_program, name);
	glUniform1f(loc, val);
}

void setUniform(const char *name, int val)
{
	GLint loc = glGetUniformLocation(now_Shader_program, name);
	glUniform1i(loc, val);
}

void setUniform(const char *name, GLuint val)
{
	GLint loc = glGetUniformLocation(now_Shader_program, name);
	glUniform1ui(loc, val);
}

void setUniform(const char *name, bool val)
{
	int loc = glGetUniformLocation(now_Shader_program, name);
	glUniform1i(loc, val);
}