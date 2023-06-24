#include "pch.h"
#include "vec4.h"
#include "hmat.h"
#include "pmat.h"
#include "point.h"
#include "vec.h"
#include "direction.h"
#define PI 3.141592f

using namespace hats;

template<space FROM, space TO>
static hmat<FROM, TO> look_at(const point<FROM>& eye, const direction<FROM>& look, const direction<FROM>& up)
{
	const direction<FROM>& i = look.cross_copy(up);
	const direction<FROM>& j = i.cross_copy(look);
	const direction<FROM>& k = -look;
	return hmat<FROM, TO>(i, j, k, eye);
}
static pmat<space::CAMERA, space::CLIP> projection(const f32 fovy, const f32 aspect_ratio, const f32 near, const f32 far)
{
	const f32 g = 1.f / tanf(.5f * (fovy * PI / 180.f));
	// y negated to move from OpenGL RH camera space to Direct3D LH clip space
	const f32 x = g / aspect_ratio, y = -g;
	// reversed projection (1 @ near, 0 @ far)
	const f32 a = near / (near - far), b = -(near * far) / (near - far);
	// not sure why this has to be negative, something with OpenGL
	const f32 w = -1.f;
	return pmat<space::CAMERA, space::CLIP>(x, y, a, b, w);
}

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
		-.5f, -.5f, .5f,			0, 0, 1, 
		.5f, -.5f, .5f,				0, 0, 1, 
		.5f, .5f, .5f,				0, 0, 1, 
		-.5f, .5f, .5f,				0, 0, 1, 
		// right
		.5f, -.5f, .5f,				1, 0, 0, 
		.5f, -.5f, -.5f,			1, 0, 0, 
		.5f, .5f, -.5f,				1, 0, 0, 
		.5f, .5f, .5f,				1, 0, 0, 
		// back
		.5f, -.5f, -.5f,			0, 0, -1,
		-.5f, -.5f, -.5f,			0, 0, -1,
		-.5f, .5f, -.5f,			0, 0, -1,
		.5f, .5f, -.5f,				0, 0, -1,
		// left
		-.5f, -.5f, -.5f,			-1, 0, 0,
		-.5f, -.5f, .5f,			-1, 0, 0,
		-.5f, .5f, .5f,				-1, 0, 0,
		-.5f, .5f, -.5f,			-1, 0, 0,
		// top
		-.5f, .5f, .5f,				0, 1, 0, 
		.5f, .5f, .5f,				0, 1, 0, 
		.5f, .5f, -.5f,				0, 1, 0, 
		-.5f, .5f, -.5f,			0, 1, 0, 
		// bottom
		-.5f, -.5f, -.5f,			0, -1, 0,
		.5f, -.5f, -.5f,			0, -1, 0,
		.5f, -.5f, .5f,				0, -1, 0,
		-.5f, -.5f, .5f,			0, -1, 0
	};
	glGenBuffers(1, &b.vbo);
	glBindBuffer(GL_ARRAY_BUFFER, b.vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// i_pos
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(f32), (void*)0);
	// i_norm
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(f32), (void*)(3 * sizeof(f32)));
	// i_tan
	/*glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(f32), (void*)(6 * sizeof(f32)));*/

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
		uniform mat4 u_mvp;
		out vec3 v_norm;
		void main()
		{
			gl_Position = u_mvp * vec4(i_pos, 1);
			v_norm = i_norm;
		}
	)";
	const char* frag_src = R"(
		#version 330 core
		layout(location = 0) out vec4 o_color;
		uniform mat4 u_normals;
		in vec3 v_norm;
		void main()
		{
			vec3 light = normalize(vec3(1, 2, 3));
			o_color = vec4(vec3(max(0, dot(light, mat3(u_normals) * v_norm))) + .2, 1);
		}
	)";

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
	const direction<space::WORLD> cam_dir = -direction<space::WORLD>::k_hat();
	const direction<space::WORLD> cam_up = direction<space::WORLD>::j_hat();
	const pmat<space::CAMERA, space::CLIP> proj = projection(45, 1920.f / 1080.f, .01f, 10.f);

	const auto& i = direction<space::WORLD>(1, 0, -1);
	const auto& j = direction<space::WORLD>(1, 1, 1);
	const auto& k = i.cross_copy(j);
	hmat<space::WORLD, space::OBJECT> obj(
		i, j, k,
		point<space::WORLD>(0, 0, 2)
	);
	const auto& scale = hmat<space::WORLD, space::WORLD>::make_scale(.5f, .5f, .5f);
	obj *= scale;
	const mat<space::OBJECT, space::WORLD>& normals = obj.normalize_copy().invert_copy();

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
		const hmat<space::WORLD, space::CAMERA> view = look_at<space::WORLD, space::CAMERA>(cam_pos, cam_dir, cam_up);
		// TODO
		const mat<space::OBJECT, space::CLIP> mvp = proj * view * (mat<space::OBJECT, space::WORLD>)obj.invert_copy();
		glUniformMatrix4fv(glGetUniformLocation(s.prog, "u_mvp"), 1, GL_FALSE, mvp.e);
		glUniformMatrix4fv(glGetUniformLocation(s.prog, "u_normals"), 1, GL_FALSE, normals.e);
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