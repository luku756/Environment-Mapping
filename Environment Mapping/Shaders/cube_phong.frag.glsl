#version 430


struct LIGHT {
	vec4 position; // assume point or direction in EC in this example shader
	vec4 ambient_color, diffuse_color, specular_color;
	vec4 light_attenuation_factors; // compute this effect only if .w != 0.0f
	vec3 spot_direction;
	float spot_exponent;
	float spot_cutoff_angle;
	bool light_on;
	bool shadow_on;
};

struct MATERIAL {
	vec4 ambient_color;
	vec4 diffuse_color;
	vec4 specular_color;
	vec4 emissive_color;
	float specular_exponent;
};

#define NUMBER_OF_OBJECT 8
uniform MATERIAL object_materials[NUMBER_OF_OBJECT];
uniform int object_id;

uniform vec4 u_global_ambient_color;
#define NUMBER_OF_LIGHTS_SUPPORTED 4
#define NUMBER_OF_LIGHT_COUNT 4 
uniform LIGHT u_light[NUMBER_OF_LIGHTS_SUPPORTED];

uniform sampler2D u_base_texture;
uniform sampler2DShadow  u_shadow_texture[NUMBER_OF_LIGHT_COUNT];


uniform bool u_flag_texture_mapping = true;
uniform bool u_flag_fog = false;

const float zero_f = 0.0f;
const float one_f = 1.0f;

in vec3 v_position_EC;
in vec3 v_normal_EC;
in vec2 v_tex_coord;

layout (location = 0) out vec4 final_color;

uniform mat4 ProjectionMatrix;

vec4 lighting_equation_textured_defferd_id_ao(in vec3 P_EC, in vec3 N_EC, in int obj_id, in vec4 base_color, in float ao) {
	//그림자 효과


	vec4 color_sum;
	float local_scale_factor, tmp_float, shadow_factor;
	vec3 L_EC;

	color_sum = object_materials[obj_id].emissive_color + u_global_ambient_color * ao * base_color;
	float f;
	for (int i = 0; i < NUMBER_OF_LIGHTS_SUPPORTED; i++) {
		if (!u_light[i].light_on) continue;

		shadow_factor = 1.0f;		

		local_scale_factor = one_f;
		if (u_light[i].position.w != zero_f) { // point light source
			L_EC = u_light[i].position.xyz - P_EC.xyz;

			if (u_light[i].light_attenuation_factors.w != zero_f) {
				vec4 tmp_vec4;

				tmp_vec4.x = one_f;
				tmp_vec4.z = dot(L_EC, L_EC);
				tmp_vec4.y = sqrt(tmp_vec4.z);
				tmp_vec4.w = zero_f;
				local_scale_factor = one_f / dot(tmp_vec4, u_light[i].light_attenuation_factors);
			}

			L_EC = normalize(L_EC);

			if (u_light[i].spot_cutoff_angle < 180.0f) { // [0.0f, 90.0f] or 180.0f
				float spot_cutoff_angle = clamp(u_light[i].spot_cutoff_angle, zero_f, 90.0f);
				vec3 spot_dir = normalize(u_light[i].spot_direction);

				tmp_float = dot(-L_EC, spot_dir);
				if (tmp_float >= cos(radians(u_light[i].spot_cutoff_angle))) {
					tmp_float = pow(tmp_float, u_light[i].spot_exponent);
				}
				else
					tmp_float = zero_f;
				local_scale_factor *= tmp_float;
			}
		}
		else {  // directional light source
			L_EC = normalize(u_light[i].position.xyz);
		}

		if (local_scale_factor > zero_f) {
			vec4 local_color_sum = u_light[i].ambient_color* ao * object_materials[obj_id].ambient_color;

			if (shadow_factor > zero_f) {
				tmp_float = dot(N_EC, L_EC);
				if (tmp_float > zero_f) {
					local_color_sum += u_light[i].diffuse_color*base_color*tmp_float;

					vec3 H_EC = normalize(L_EC - normalize(P_EC));
					tmp_float = dot(N_EC, H_EC);
					if (tmp_float > zero_f) {
						local_color_sum += u_light[i].specular_color *
							object_materials[obj_id].specular_color * pow(tmp_float, object_materials[obj_id].specular_exponent);
					}
				}
			}

			color_sum += local_scale_factor * local_color_sum;
		}
	}

	//return vec4(0, 0, f, 1.0f);
	return color_sum;
}

void main() {

	vec4 base_color, shaded_color;
	float fog_factor;

	if (u_flag_texture_mapping)
		base_color = texture(u_base_texture, v_tex_coord);
	else
		base_color = object_materials[object_id].diffuse_color;

	shaded_color = lighting_equation_textured_defferd_id_ao(v_position_EC, normalize(v_normal_EC), object_id, base_color, 1);

	final_color = shaded_color;

}

