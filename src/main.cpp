#include <string>
#include <vector>
#include <fstream>
#include <sstream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include "tools/App.h"
#include "tools/Camera.h"
#include "tools/Mesh.h"
#include "tools/Resource.h"
#include "tools/ShaderProgram.h"
#include "tools/FrameBuffer.h"

using namespace glm;
using namespace std;

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
	
	void bind(Program *shader,int i) {
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

		monkey = loadOBJ(R->path+"monkey.obj");
		lamp = loadOBJ(R->path+"sphere.obj");

		cam = Camera(vec3(0,3,0), vec3(0,0,0), vec3(0,1,0));
		cam.perspective(window, 45, .1, 100);

		vector<vec3> positions;
		int num = 7;
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


		passthrough=FrameBuffer(1920,1080);
		passthrough.addTexture("passthrough", GL_RGB, GL_RGB, GL_COLOR_ATTACHMENT0);
		passthrough.addDepth();
		passthrough.drawBuffers();

		gBuffer = FrameBuffer(1920, 1080);
		gBuffer.addTexture("position", GL_RGB16F, GL_RGB, GL_COLOR_ATTACHMENT0);
		gBuffer.addTexture("normal", GL_RGB16F, GL_RGB, GL_COLOR_ATTACHMENT1);
		gBuffer.addTexture("albedo-spec", GL_RGBA, GL_RGBA, GL_COLOR_ATTACHMENT2);
		gBuffer.addDepth();
		gBuffer.drawBuffers();
		gBuffer.check();

		lights = vector<Light>(10);
		lights[0]=(Light{{0,2,0},{1,1,1},(1/2)});
		addInput(GLFW_KEY_E, [&](int action, int mods) {
			if (action == GLFW_PRESS) {
				lights[lightidx%10] = Light{ 
					cam.getPosition(),
					colors[lightidx%colors.size()],
					1/1.75
				};
				lightidx = lightidx + 1;
			}
		});
		addInput(GLFW_KEY_Q, [&](int action, int mods) {
			if (action == GLFW_PRESS) 
				sstep++;
		});
	}

	void onClose() {
		passthrough.cleanup();
		gBuffer.cleanup();
		monkey.cleanup();
		pass.cleanup();
		R->cleanup();
	}

	void update(float delta) {
		cam.perspective(window, 45, .1, 100);
		cam.apply(window, delta);
		//printf("%d\n", fps);
		glfwSetWindowTitle(window, to_string(fps).c_str());
	}

	void render(float delta) {
		gBuffer.bind(); 
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		gbuff.bind();
		gbuff.setUniform("projection", &cam.P());
		gbuff.setUniform("view", &cam.V());

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
		simple.setUniform("view", &cam.V());
		simple.setUniform("projection", &cam.P());
		for (int i = 0; i < lights.size(); i++) {
			simple.setUniform("color", &lights[i].color);
			if(lights[i].pos==vec3(0))
				simple.setUniform("model", &(translate(vec3(100))));
			else simple.setUniform("model", &(translate(lights[i].pos)*scale(vec3(.5))));
			lamp.renderVertices(GL_QUADS);
		}

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