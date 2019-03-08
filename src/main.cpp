#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <random>
#include <ctime>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "tools/App.h"
#include "tools/Camera.h"
#include "tools/Mesh.h"
#include "tools/Resource.h"
#include "tools/ShaderProgram.h"
#include "tools/FrameBuffer.h"

using namespace glm;
using namespace std;

void getError(int line) {
	GLenum err(glGetError());
	while (err != GL_NO_ERROR) {
		string error;

		switch (err) {
		case GL_INVALID_OPERATION:      error = "INVALID_OPERATION";      break;
		case GL_INVALID_ENUM:           error = "INVALID_ENUM";           break;
		case GL_INVALID_VALUE:          error = "INVALID_VALUE";          break;
		case GL_OUT_OF_MEMORY:          error = "OUT_OF_MEMORY";          break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:  error = "INVALID_FRAMEBUFFER_OPERATION";  break;
		}
		printf("GL_%s : %d", error.c_str(), line);
		err = glGetError();
	}
}
#define _getError() getError(__LINE__)

Mesh loadOBJ(string fn) {

	vector<vec3> verts_;
	vector<vec3> norms_;
	vector<vec2> uv_;
	vector<Vertex> verts;

	ifstream in;
	in.open(fn.c_str(), ifstream::in);
	if (in.fail()) return Mesh();
	string line;
	while (!in.eof()) {
		getline(in, line);
		istringstream iss(line.c_str());
		char trash;
		if (!line.compare(0, 2, "v ")) {
			iss >> trash;
			vec3 v;
			for (int i = 0; i < 3; i++) iss >> v[i];
			verts_.push_back(v);
		}
		else if (!line.compare(0, 3, "vn ")) {
			iss >> trash >> trash;
			vec3 n;
			for (int i = 0; i < 3; i++) iss >> n[i];
			norms_.push_back(n);
		}
		else if (!line.compare(0, 3, "vt ")) {
			iss >> trash >> trash;
			vec2 uv;
			for (int i = 0; i < 2; i++) iss >> uv[i];
			uv_.push_back(vec2(uv.x, 1 - uv.y));
		}
		else if (!line.compare(0, 2, "f ")) {
			vector<ivec3> f;
			int vert, uv, norm;
			iss >> trash;
			while (iss >> vert >> trash >> uv >> trash >> norm) {
				vert--; uv--; norm--;
				verts.push_back(Vertex{ verts_[vert],{1,1,1} ,uv_[uv], norms_[norm] });
			}
		}
	}
	return Mesh(verts);
}

struct Light {
	vec3 pos;
	vec3 color;
	float quad;
	void bind(Program *shader, int i) {
		shader->setUniform("lights[" + to_string(i) + "].pos", &pos);
		shader->setUniform("lights[" + to_string(i) + "].color", &color);
		shader->setUniform("lights[" + to_string(i) + "].quad", quad);
	}
};


class Game :public App {
	Resource *R;

	Camera cam;
	Mesh monkey,quad,lamp;
	Program pass,gbuff,lpass,simple;
	FrameBuffer passthrough,gBuffer;
	vector<Light> lights;

	GLuint ibo,instances;
	GLuint lamp_ibo, lamp_instances;

	GLuint ubo;

	int lightidx = 0,sstep=0;
	vector<vec3> colors{ {1,1,1},{1,0,0},{0,1,0},{0,0,1},{0,1,1},{1,0,1},{1,1,0} };

	void initGL() {
		glEnable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		glEnable(GL_CULL_FACE);
	}

	void loading() {
		GLuint text = R->addTexture("loading","test.png");
		pass.bind();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, text);
		pass.setUniform("passthrough", 0);
		pass.setUniform("model", &mat4(1));
		quad.renderVertices(GL_QUADS);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	void init() {
		R = &Resource::getInstance();
		Shader passv = R->addShader("pass.vert"),
			passf = R->addShader("pass.frag"),
			defv = R->addShader("def.vert"),
			deff = R->addShader("def.frag"),
			lpassv = R->addShader("Lpass.vert"),
			lpassf = R->addShader("Lpass.frag"),
			simpv = R->addShader("simple.vert"),
			simpf = R->addShader("simple.frag");

		pass = Program(passv, passf);
		gbuff = Program(defv, deff);
		lpass = Program(lpassv, lpassf);
		simple = Program(simpv,simpf);

		quad = Mesh({
				Vertex{ {1,1,0},{1,1,0},{1,1}},
				Vertex{ {-1,1,0},{1,0,1},{0,1}},
				Vertex{ {-1,-1,0},{0,1,1},{0,0}},
				Vertex{ {1,-1,0},{1,1,1},{1,0}}
			});

		loading();

		R->addTexture("uvmap", "uvmap.bmp");

		monkey = loadOBJ(R->path+"less_face_monkey.obj");
		lamp = loadOBJ(R->path+"sphere.obj");

		cam = Camera(vec3(0,3,3), vec3(0,0,0)- vec3(0, 3, 3), vec3(0,1,0));
		cam.perspective(window, 45, .1, 100);

		vector<vec3> positions;
		int num = 13;
		float sep = 3;
		for (int r = 0; r < num; r++)
			for (int c = 0; c < num; c++)
				positions.push_back(vec3(r*sep,0, c*sep)- vec3((num/2)*sep,0, (num / 2)*sep));


		instances = positions.size();
		glBindVertexArray(monkey.vao);
		glGenBuffers(1, &ibo);
		glBindBuffer(GL_ARRAY_BUFFER, ibo);
		glBufferData(GL_ARRAY_BUFFER, positions.size()*sizeof(vec3), &positions[0], GL_STATIC_DRAW);
		glVertexAttribPointer(6, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), (void*)0);
		glEnableVertexAttribArray(6);
		glVertexAttribDivisor(6, 1);
		glBindVertexArray(0);


		lights = vector<Light>(100, Light{vec3(0),vec3(0),0});
		lights[0] = (Light{ {0,2,0},{1,1,1},(1 / 2) });

		lamp_instances = lights.size();
		glBindVertexArray(lamp.vao);
		glGenBuffers(1, &lamp_ibo);
		glBindBuffer(GL_ARRAY_BUFFER, lamp_ibo);
		glBufferData(GL_ARRAY_BUFFER, lights.size() * sizeof(Light), &lights[0], GL_DYNAMIC_DRAW);
		glVertexAttribPointer(6, 3, GL_FLOAT, GL_FALSE, sizeof(Light), (void*)offsetof(Light,pos));
		glVertexAttribPointer(7, 3, GL_FLOAT, GL_FALSE, sizeof(Light), (void*)offsetof(Light,color));
		glEnableVertexAttribArray(6);
		glEnableVertexAttribArray(7);
		glVertexAttribDivisor(6, 1);
		glVertexAttribDivisor(7, 1);
		glBindVertexArray(0);


		passthrough=FrameBuffer(1920,1080);
		passthrough.addTexture("passthrough", GL_RGBA, GL_RGBA, GL_COLOR_ATTACHMENT0);
		passthrough.addDepth();
		passthrough.drawBuffers();


		gBuffer = FrameBuffer(1920, 1080);
		gBuffer.addTexture("position", GL_RGB16F, GL_RGB, GL_COLOR_ATTACHMENT0);
		gBuffer.addTexture("normal", GL_RGB16F, GL_RGB, GL_COLOR_ATTACHMENT1);
		gBuffer.addTexture("albedo-spec", GL_RGBA, GL_RGBA, GL_COLOR_ATTACHMENT2);
		gBuffer.addDepth();
		gBuffer.drawBuffers();
		gBuffer.check();


		glGenBuffers(1, &ubo);
		glBindBuffer(GL_UNIFORM_BUFFER, ubo);
		glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(mat4), NULL, GL_DYNAMIC_DRAW);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(cam.P()));
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(cam.V()));

		GLuint loc = glGetUniformBlockIndex(gbuff.getProgramID(), "camera");
		glUniformBlockBinding(gbuff.getProgramID(), loc, 0);
		glBindBufferBase(GL_UNIFORM_BUFFER, 0, ubo);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);




		addInput(GLFW_KEY_E, [&](int action, int mods) {
			if (action == GLFW_PRESS) {
				unsigned int index = lightidx % lights.size();
				lights[index] = Light{ 
					vec4(cam.getPosition(),1),
					vec4(colors[lightidx%colors.size()],1),
					1/1.75
				};
				glBindVertexArray(lamp.vao);
				glBindBuffer(GL_ARRAY_BUFFER, lamp_ibo);
				glBufferSubData(GL_ARRAY_BUFFER, sizeof(Light)*(index)+offsetof(Light, pos), sizeof(vec3), &lights[index].pos);
				glBufferSubData(GL_ARRAY_BUFFER, sizeof(Light)*(index)+offsetof(Light, color), sizeof(vec3), &lights[index].color);
				glBindVertexArray(0);

				lightidx = lightidx + 1;
			}
		});
		addInput(GLFW_KEY_Q, [&](int action, int mods) {
			if (action == GLFW_PRESS) 
				sstep++;
		});

	}

	void onClose() {
		glDeleteBuffers(1, &lamp_ibo);
		glDeleteBuffers(1, &ibo);
		glDeleteBuffers(1, &ubo);
		passthrough.cleanup();
		gBuffer.    cleanup();
		monkey.     cleanup();
		quad.       cleanup();
		lamp.       cleanup();
		simple.     cleanup();
		gbuff.      cleanup();
		lpass.      cleanup();
		pass.       cleanup();
		R->         cleanup();
	}

	void update(float delta) {
		cam.perspective(window, 45, .1, 100);
		cam.apply(window, delta);

		glBindBuffer(GL_UNIFORM_BUFFER, ubo);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(cam.P()));
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(cam.V()));
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		//printf("%d\n", fps);
		glfwSetWindowTitle(window, to_string(fps).c_str());

	}

	void render(float delta) {
		gBuffer.bind(); 
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		gbuff.bind();
		gbuff.setUniform("diffuse", R->bindTexture("uvmap", GL_TEXTURE0));
		glBindVertexArray(monkey.vao);
		glDrawArraysInstanced(GL_TRIANGLES, 0, monkey.vertices.size(), instances);
		
		
		
		passthrough.bind();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		lpass.bind();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, gBuffer.getTexture("position"));
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, gBuffer.getTexture("normal"));
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, gBuffer.getTexture("albedo-spec"));
		for (int i = 0; i < lights.size(); i++)
			lights[i].bind(&lpass, i);
		lpass.setUniform("position", 0);
		lpass.setUniform("normal", 1);
		lpass.setUniform("albedospec", 2); 
		quad.renderVertices(GL_QUADS); 

		gBuffer.read();
		passthrough.draw();
		glBlitFramebuffer(0, 0, gBuffer.getWidth(), gBuffer.getHeight(),
			0, 0, passthrough.getWidth(), passthrough.getHeight(),
			GL_DEPTH_BUFFER_BIT, GL_NEAREST);

		simple.bind();
		simple.setUniform("model", &(scale(vec3(.25))));
		glBindVertexArray(lamp.vao); 
		glDrawArraysInstanced(GL_TRIANGLES, 0, lamp.vertices.size(), lamp_instances);

		FrameBuffer::bindDefualt();
		viewportinit(window);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		pass.bind();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, gBuffer.getTexture("position"));
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, gBuffer.getTexture("normal"));
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, gBuffer.getTexture("albedo-spec"));
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, passthrough.getTexture("passthrough"));

		pass.setUniform("model", &(scale(vec3(1./3.))*translate(vec3(2, 2, 0))));
		pass.setUniform("passthrough", sstep%4);
		quad.renderVertices(GL_QUADS);
		pass.setUniform("model", &(scale(vec3(1. / 3.))*translate(vec3(0, 2, 0))));
		pass.setUniform("passthrough", (sstep+1)%4);
		quad.renderVertices(GL_QUADS);
		pass.setUniform("model", &(scale(vec3(1. / 3.))*translate(vec3(-2, 2, 0))));
		pass.setUniform("passthrough", (sstep + 2) % 4);
		quad.renderVertices(GL_QUADS);
		pass.setUniform("model", &mat4(1));
		pass.setUniform("passthrough", (sstep + 3) % 4);
		quad.renderVertices(GL_QUADS);

	}

	void inputListener(float delta) {
		running = glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS;
	}
public:

	Game(int width, int height, const char *title) :App(width, height, title) {

	}
};

int main() {
	Game game(700, 700, "");
	game.start();
}