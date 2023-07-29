#include "pch.h"
#include "vec4.h"
#include "tmat.h"
#include "pmat.h"
#include "point.h"
#include "vec.h"
#include "direction.h"
#include "tmat_util.h"
#include "direction_util.h"
#include "vec_util.h"
#include "pmat_util.h"

using namespace hats;

GLFWwindow* init(const s32 w, const s32 h, const std::string& title)
{
	bool success = glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	GLFWwindow* win = glfwCreateWindow(w, h, title.c_str(), nullptr, nullptr);
	glfwMakeContextCurrent(win);
	glfwSwapInterval(1);
	success = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.f, 0.f, 1.f, 1.f);
	// Direct3D clip space (z in [0, 1], y-axis down)
	glClipControl(GL_UPPER_LEFT, GL_ZERO_TO_ONE);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);

	return win;
}
struct bufs
{
	u32 vao, vbo, ibo;
};
bufs gen_buffers()
{
	bufs b;
	glGenVertexArrays(1, &b.vao);
	glBindVertexArray(b.vao);

	constexpr f32 vertices[] =
	{
		// front
		-.5f, -.5f, .5f,	 0,  0,  1,		 1,  0,  0,
		 .5f, -.5f, .5f,	 0,  0,  1,		 1,  0,  0,
		 .5f,  .5f, .5f,	 0,  0,  1,		 1,  0,  0,
		-.5f,  .5f, .5f,	 0,  0,  1,		 1,  0,  0,
		// right			  	 	 		 
		.5f, -.5f,  .5f,	 1,  0,  0,		 0,  0, -1,
		.5f, -.5f, -.5f,	 1,  0,  0,		 0,  0, -1,
		.5f,  .5f, -.5f,	 1,  0,  0,		 0,  0, -1,
		.5f,  .5f,  .5f,	 1,  0,  0,		 0,  0, -1,
		// back				 	 
		 .5f, -.5f, -.5f,	 0,  0, -1,		-1,  0,  0,
		-.5f, -.5f, -.5f,	 0,  0, -1,		-1,  0,  0,
		-.5f,  .5f, -.5f,	 0,  0, -1,		-1,  0,  0,
		 .5f,  .5f, -.5f,	 0,  0, -1,		-1,  0,  0,
		// left					 
		-.5f, -.5f, -.5f,	-1,  0,  0,		 0,  0,  1,
		-.5f, -.5f,  .5f,	-1,  0,  0,		 0,  0,  1,
		-.5f,  .5f,  .5f,	-1,  0,  0,		 0,  0,  1,
		-.5f,  .5f, -.5f,	-1,  0,  0,		 0,  0,  1,
		// top					     
		-.5f,  .5f,  .5f,	 0,  1,  0,		 1,  0,  0,
		 .5f,  .5f,  .5f,	 0,  1,  0,		 1,  0,  0,
		 .5f,  .5f, -.5f,	 0,  1,  0,		 1,  0,  0,
		-.5f,  .5f, -.5f,	 0,  1,  0,		 1,  0,  0,
		// bottom			 	     
		-.5f, -.5f, -.5f,	 0, -1,  0,		 1,  0,  0,
		 .5f, -.5f, -.5f,	 0, -1,  0,		 1,  0,  0,
		 .5f, -.5f,  .5f,	 0, -1,  0,		 1,  0,  0,
		-.5f, -.5f,  .5f,	 0, -1,  0,		 1,  0,  0,
	};
	glGenBuffers(1, &b.vbo);
	glBindBuffer(GL_ARRAY_BUFFER, b.vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// i_pos
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(f32), (void*)0);
	// i_norm
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(f32), (void*)(3 * sizeof(f32)));
	// i_tan
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(f32), (void*)(6 * sizeof(f32)));

	const u32 indices[] =
	{
		// front
		0, 1, 2, 0, 2, 3,
		// right
		4, 5, 6, 4, 6, 7,
		// back
		8, 9, 10, 8, 10, 11,
		// left
		12, 13, 14, 12, 14, 15,
		// top
		16, 17, 18, 16, 18, 19,
		// bottom
		20, 21, 22, 20, 22, 23
	};
	glGenBuffers(1, &b.ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, b.ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	return b;
}
void destroy_buffers(bufs* const b)
{
	glDeleteBuffers(1, &b->vbo);
	glDeleteBuffers(1, &b->ibo);
	glDeleteVertexArrays(1, &b->vao);
}
struct shader
{
	u32 vert, frag, prog;
};
shader gen_shader()
{
	const char* vert_src = R"(
		#version 330 core
		layout(location = 0) in vec3 i_pos;
		layout(location = 1) in vec3 i_norm;
		uniform mat4 u_model2world, u_mvp;
		out vec3 v_pos, v_norm;
		void main()
		{
			gl_Position = u_mvp * vec4(i_pos, 1);
			v_pos = (u_model2world * vec4(i_pos, 1)).xyz;
			v_norm = mat3(u_model2world) * i_norm;
		}
	)";
	//const char* vert_src = R"(
	//	#version 330 core
	//	layout(location = 0) in vec3 i_pos;
	//	layout(location = 1) in vec3 i_norm;
	//	layout(location = 2) in vec3 i_tan;
	//	uniform mat4 u_mvp, u_model2world;
	//	out vec3 v_pos;
	//	out mat3 v_tbn;
	//	void main()
	//	{
	//		gl_Position = u_mvp * vec4(i_pos, 1);
	//		v_pos = (u_model2world * vec4(i_pos, 1)).xyz;

	//		vec3 T = normalize(vec3(u_model2world * vec4(i_tan, 0)));
	//		vec3 B = normalize(vec3(u_model2world * vec4(cross(i_tan, i_norm), 0)));
	//		vec3 N = normalize(vec3(u_model2world * vec4(i_norm, 0)));
	//		// world->tangent
	//		v_tbn = mat3(T, B, N);
	//	}
	//)";
	const char* frag_src = R"(
		#version 330 core
		layout(location = 0) out vec4 o_color;
		in vec3 v_norm;
		void main()
		{
			vec3 light = normalize(vec3(1, 2, 3));
			o_color = vec4(vec3(max(0, dot(light, v_norm))) + .2, 1);
		}
	)";
	/*const char* frag_src = R"(
		#version 330 core
		layout(location = 0) out vec4 o_color;
		uniform vec3 u_camera, u_kd, u_ks, u_light_pos, u_light_color;
		uniform float u_f0, u_alpha, u_m, u_s, u_d;
		in vec3 v_pos, v_norm;
		void main()
		{
			const float PI = 3.14159265;
			vec3 V = normalize(u_camera - v_pos);
			vec3 L = normalize(u_light_pos - v_pos);
			vec3 N = v_norm;
			vec3 H = normalize(V + L);

			float NdH = max(0, dot(N, H));
			float NdV = max(0, dot(N, V));
			float NdL = max(0, dot(N, L));
			float VdH = max(0, dot(V, H));
			float NdH2 = NdH * NdH, NdH4 = NdH2 * NdH2;
			float m2 = u_m * u_m;

			float F = u_f0 + (1 - u_f0) * pow((1 - VdH), 5);
			float G = min(1, min(2 * NdH * NdV / VdH, 2 * NdH * NdL / VdH));
			float D = 1 / (m2 * NdH4) * exp((NdH2 - 1) / (m2 * NdH));

			vec3 Rs = min(vec3(1), max(vec3(0), u_ks * (F * D * G) / (PI * NdL * NdV)));
			vec3 Rd = min(vec3(1), max(vec3(0), u_kd));
			vec3 Ra = min(vec3(1), max(vec3(0), vec3(1)));
			o_color = vec4(0 * Ra + u_light_color * NdL * (u_s * Rs + u_d * Rd), 1);
		}
	)";*/
	/*const char* frag_src = R"(
		#version 330 core
		layout(location = 0) out vec4 o_color;
		uniform vec3 u_camera, u_kd, u_ks, u_light_pos, u_light_color, u_albedo;
		uniform float u_f0, u_alpha, u_mx, u_my;
		in vec3 v_pos;
		in mat3 v_tbn;
		void main()
		{
			const float PI = 3.14159265;
			vec3 V = v_tbn * normalize(u_camera - v_pos);
			vec3 L = v_tbn * normalize(u_light_pos - v_pos);
			vec3 N = vec3(0, 0, 1);
			vec3 H = v_tbn * normalize(V + L);

			float NdH = max(0, dot(N, H));
			float VdH = max(0, dot(V, H));
			float NdL = max(0, dot(N, L));
			float NdV = max(0, dot(N, V));
			float NdH2 = NdH * NdH, NdH4 = NdH2 * NdH2;

			float F = u_f0 + (1 - u_f0) * pow(1 - VdH, 5);
			float st2 = 1 - NdH2;
			float tt2 = st2 / NdH2;
			float sp2 = (H.y * H.y) / st2;
			float cp2 = (H.x * H.x) / st2;
			float D = 1 / (PI * u_mx * u_my * NdH4) * exp(-tt2 * (cp2 / (u_mx * u_mx) + sp2 / (u_my * u_my)));

			vec3 Rd = u_kd * (1 - F) / PI;
			vec3 Rs = u_ks * F * D / (4 * VdH * pow(NdL * NdV, u_alpha));

			o_color = vec4(u_albedo * (vec3(.2) + NdL * (Rd + Rs)), 1);
		}
	)";*/

	shader s;
	s.prog = glCreateProgram();
	s.vert = glCreateShader(GL_VERTEX_SHADER);
	s.frag = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(s.vert, 1, &vert_src, nullptr);
	glShaderSource(s.frag, 1, &frag_src, nullptr);
	glCompileShader(s.vert);
	int result = 0;
	glGetShaderiv(s.vert, GL_COMPILE_STATUS, &result);
	if (!result)
		printf("Vert error\n");
	result = 0;
	glCompileShader(s.frag);
	glGetShaderiv(s.frag, GL_COMPILE_STATUS, &result);
	if (!result)
		printf("Frag error\n");

	glAttachShader(s.prog, s.vert);
	glAttachShader(s.prog, s.frag);
	glLinkProgram(s.prog);
	glGetProgramiv(s.prog, GL_LINK_STATUS, &result);
	if (!result)
	{
		char buf[1024] = { 0 };
		glGetProgramInfoLog(s.prog, sizeof(buf), nullptr, buf);
		printf("Link error: %s\n", buf);
	}
	glValidateProgram(s.prog);
	glDeleteShader(s.vert);
	glDeleteShader(s.frag);

	return s;
}

int main()
{
	point<space::WORLD> cam_pos(0, 0, 5);
	const direction<space::WORLD> cam_dir = -direction_util::k_hat<space::WORLD>();
	const direction<space::WORLD> cam_up = direction_util::j_hat<space::WORLD>();
	const pmat<space::CAMERA, space::CLIP> proj = pmat_util::projection(108 * 1080.f / 1920.f, 1920.f / 1080.f, .01f, 10.f);
	tmat<space::WORLD, space::OBJECT> obj =
		/*hmat_util::scale<space::WORLD, space::OBJECT>(1.f, 1.f, 1.f) *
		hmat_util::rotation_xyz<space::WORLD>(0, c::PI / 4, -c::PI / 4)**/
		tmat_util::translation<space::WORLD, space::OBJECT>(0, 0, .7f);

	GLFWwindow* window = init(1920, 1080, "HATS");
	bufs b = gen_buffers();
	shader s = gen_shader();
	const u32 keycodes[] = { GLFW_KEY_W, GLFW_KEY_A, GLFW_KEY_S, GLFW_KEY_D, GLFW_KEY_SPACE, GLFW_KEY_LEFT_SHIFT, GLFW_KEY_ESCAPE };
	bool keys[7] = { false };
	f32 last = 0;
	const f32 speed = 1.f;
	while (!glfwWindowShouldClose(window))
	{
		const f32 now = (f32)glfwGetTime();
		const f32 delta = now - last;
		last = now;
		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		for (int i = 0; i < sizeof(keys) / sizeof(bool); i++)
			keys[i] = glfwGetKey(window, keycodes[i]);
		if (keys[6])
			break;
		cam_pos.x += (keys[3] - keys[1]) * speed * delta;
		cam_pos.y += (keys[4] - keys[5]) * speed * delta;
		cam_pos.z += (keys[2] - keys[0]) * speed * delta;

		glUseProgram(s.prog);
		const tmat<space::WORLD, space::CAMERA> view =
			tmat_util::look_at<space::WORLD, space::CAMERA>(cam_pos, cam_dir, cam_up);
		const tmat<space::OBJECT, space::CAMERA> mv = view * obj.invert_copy();
		const mat<space::OBJECT, space::CLIP> mvp = proj * mv;
		f32 asdf = delta * 10.f;
		obj = tmat_util::rotation_xyz_about_point<space::OBJECT>(
			asdf, asdf / 3, -asdf / 2,
			point<space::OBJECT>()
		) * obj;
		const mat<space::OBJECT, space::WORLD>& model2world = obj.normalize_copy().invert_copy();
		glUniformMatrix4fv(glGetUniformLocation(s.prog, "u_mvp"), 1, GL_FALSE, mvp.e);
		glUniformMatrix4fv(glGetUniformLocation(s.prog, "u_model2world"), 1, GL_FALSE, model2world.e);

		glUniform3f(glGetUniformLocation(s.prog, "u_camera"), cam_pos.x, cam_pos.y, cam_pos.z);
		glUniform3f(glGetUniformLocation(s.prog, "u_light_pos"), 0, 0, 5);
		glUniform3f(glGetUniformLocation(s.prog, "u_light_color"), 1, 1, 1);
		// red velvet
		glUniform3f(glGetUniformLocation(s.prog, "u_kd"), .0048f, .0005f, 0.f);
		glUniform3f(glGetUniformLocation(s.prog, "u_ks"), .1938f, .0333f, .0267f);
		glUniform1f(glGetUniformLocation(s.prog, "u_f0"), .041f);
		glUniform1f(glGetUniformLocation(s.prog, "u_alpha"), 0.f);
		glUniform1f(glGetUniformLocation(s.prog, "u_mx"), 2.337f);
		glUniform1f(glGetUniformLocation(s.prog, "u_my"), 2.644f);
		// brushed aluminum
		glUniform3f(glGetUniformLocation(s.prog, "u_albedo"), 1, 1, 1);
		/*glUniform3f(glGetUniformLocation(s.prog, "u_kd"), .0036f, .0034f, .0026f);
		glUniform3f(glGetUniformLocation(s.prog, "u_ks"), .0115f, .0105f, .0075f);
		glUniform1f(glGetUniformLocation(s.prog, "u_f0"), .999f);
		glUniform1f(glGetUniformLocation(s.prog, "u_mx"), .035f);
		glUniform1f(glGetUniformLocation(s.prog, "u_my"), .129f);
		glUniform1f(glGetUniformLocation(s.prog, "u_alpha"),.005f);*/

		glBindVertexArray(b.vao);
		glBindBuffer(GL_ARRAY_BUFFER, b.vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, b.ibo);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);

		glfwSwapBuffers(window);
	}
	destroy_buffers(&b);
	glfwDestroyWindow(window);

	return 0;
}