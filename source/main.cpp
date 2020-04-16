#include <iostream>
#include <glad/glad.h>
#include <glfw/glfw3.h>

#include <cmath>
#include <ShaderProgram.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

std::string proc_path = "";
std::string proc_name = "";

static float filter = 0.2f;

void LOG(std::initializer_list<std::string> log)
{
	for (auto iter = log.begin(); iter != log.end(); ++iter)
	{
		std::cout << *iter << std::endl;
	}
}

int screenWdt = 640;
int screenHgh = 640;
void frame_buffer_size(GLFWwindow* window, int width, int height)
{
	screenWdt = width;
	screenHgh = height;
	glViewport(0, 0, width, height);
}

glm::vec3 cameraPos(0.0f);

void precess_event(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		filter += 0.05;
		if (filter > 1.0f) filter = 1.0f;
		cameraPos.z -= 0.01f;
	}
		

	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		filter -= 0.05;
		if (filter < 0.0f) filter = 0.0f;
		cameraPos.z += 0.01f;
	}

	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		cameraPos.x -= 0.01f;
	}

	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		cameraPos.x += 0.01f;
	}
}

static const char* vshader = "#version 330 core\n\
layout (location = 0) in vec3 aPos; \n\
void main() \n\
{ \n\
	gl_Position = vec4(aPos, 1.0f);\n\
} \n\
";

static const char* fshader = "#version 330 core\n\
out vec4 aColor; \n\
void main() \n\
{ \n\
	aColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);\n\
} \n\
";

unsigned int genProgram()
{
	int success;
	char log[512];
	unsigned int vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vshader, NULL);
	glCompileShader(vs);

	glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vs, 512, NULL, log);
		LOG({"vshader compile error: ", log});
	}

	unsigned int fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fshader, NULL);
	glCompileShader(fs);

	glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fs, 512, NULL, log);
		LOG({"fshader compile error: ", log});
	}

	unsigned int program = glCreateProgram();
	glAttachShader(program, vs);
	glAttachShader(program, fs);

	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(program, 512, NULL, log);
		LOG({"program link error:", log});
	}

	glDeleteShader(vs);
	glDeleteShader(fs);
	return program;
}

void definePath(std::string arg)
{
#if defined(_WIN32) || defined(WIN32)
    int dP = arg.rfind('\\');
#else
    int dP = arg.rfind('/');
#endif
	
	proc_path = arg.substr(0, dP);
	proc_name = arg.substr(dP - 1);

	std::cout << "workpath is :" << proc_path << std::endl;
}


int main(int argc, char* argv[])
{
	definePath(argv[0]);

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	#endif //__APPLE__

	GLFWwindow* window = glfwCreateWindow(640, 640, "Hello World", NULL, NULL);
	if (NULL == window)
	{
		glfwTerminate();
		LOG({"create window error"});
		return -1;
	}
	glfwSetFramebufferSizeCallback(window, frame_buffer_size);
	glfwMakeContextCurrent(window);
	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		glfwTerminate();
		LOG({"load glad error"});
		return -1;
	}
	glEnable(GL_DEPTH_TEST);

	float vertices[] = {
		//---坐标---------     -----颜色-------       ------纹理坐标-------  --------纹理坐标2
		-0.5f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f,      0.0f, 0.0f,           0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,    0.0f, 1.0f, 0.0f,      1.0f, 0.0f,           1.0f, 0.0f,
		0.5f, 0.5f, 0.0f,     0.0f, 0.0f, 1.0f,      1.0f, 1.0f,		   1.0f, 1.0f,
		-0.5f, 0.5f, 0.0f,    1.0f, 1.0f, 1.0f,      0.0f, 1.0f,		   0.0f, 1.0f,
	};

	unsigned int indices[] = {
		0, 1, 2, 
		2, 3, 0
	};

	unsigned VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(8 * sizeof(float)));
	glEnableVertexAttribArray(3);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	ShaderProgram sp("default-shader");

	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	float borderColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	stbi_set_flip_vertically_on_load(true);

	std::string texPath = proc_path + "/res/textures/container.jpg";
	int width, height, nrChannels;
	unsigned char *data = stbi_load(texPath.c_str(), &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(data);
	}
	else
	{
		std::cout << "error on loade image" << std::endl;
	}

	unsigned int texture1;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	texPath = proc_path + "/res/textures/awesomeface.png";
	data = stbi_load(texPath.c_str(), &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		stbi_image_free(data);
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	float verticesBox[] = {
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

		-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f};
	
	unsigned int boxVAO, boxVBO;
	glGenVertexArrays(1, &boxVAO);
	glBindVertexArray(boxVAO);

	glGenBuffers(1, &boxVBO);
	glBindBuffer(GL_ARRAY_BUFFER, boxVBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesBox), verticesBox, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	ShaderProgram boxShader("box");
	unsigned int boxTex;
	glGenTextures(1, &boxTex);
	glBindTexture(GL_TEXTURE_2D, boxTex);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


	texPath = proc_path + "/res/textures/container.jpg";
	int bW, bH, bC;
	data = stbi_load(texPath.c_str(), &bW, &bH, &bC, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bW, bH, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	stbi_image_free(data);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);



	glm::mat4 trans(1.0);

	while (!glfwWindowShouldClose(window))
	{
		glfwSwapBuffers(window);
		glfwPollEvents();
		glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		float timeValue =  (float)(glfwGetTime());
		float greenValue = (sin(timeValue) / 2.0f) + 0.5f;

		
		// draw first
		glBindVertexArray(VAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture1);

		sp.use();
		sp.ui("ourTexture", 0);
		sp.ui("myTexture", 1);
		double time = glfwGetTime();
		float gVal = (sin(time) / 2.0f) + 0.5f;
		sp.u4f("ourColor", 0.1f, gVal, 0.1f, 1.0f);
		sp.uf("filr", filter);
		sp.umat4("transform", trans);

		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -0.5f, 0.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

		glm::mat4 view(1.0f);
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

		glm::mat4 perspective(1.0f);
		perspective = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.f);

		sp.umat4("model", model);
		sp.umat4("view", view);
		sp.umat4("perspective", perspective);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// glm::mat4 trans1;
		// trans1 = glm::translate(trans1, glm::vec3(-0.75f, 0.75f, 0.0f));
		// trans1 = glm::scale(trans1, glm::vec3(0.5f * sinf(glfwGetTime()), 0.5f * sinf(glfwGetTime()), 1.0f));
		// sp.umat4("transform", trans1);
		// glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindTexture(GL_TEXTURE_2D, 0);

		glBindVertexArray(boxVAO);
		glBindBuffer(GL_ARRAY_BUFFER, boxVBO);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, boxTex);

		glm::vec3 cubePositions[] = {
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(2.0f, 5.0f, -15.0f),
			glm::vec3(-1.5f, -2.2f, -2.5f),
			glm::vec3(-3.8f, -2.0f, -12.3f),
			glm::vec3(2.4f, -0.4f, -3.5f),
			glm::vec3(-1.7f, 3.0f, -7.5f),
			glm::vec3(1.3f, -2.0f, -2.5f),
			glm::vec3(1.5f, 2.0f, -2.5f),
			glm::vec3(1.5f, 0.2f, -1.5f),
			glm::vec3(-1.3f, 1.0f, -1.5f)};


		for (int i = 0; i < 10; ++i)
		{
			boxShader.use();
			boxShader.ui("uTexture", 0);
			glm::mat4 bTrans(1.0f);
			bTrans = glm::translate(bTrans, cubePositions[i]);
			//bTrans = glm::rotate(bTrans, (float)(20.0f * i), glm::vec3(0.5f, 1.0f, 0.0f));
			if ((i % 3) == 0)
			{
				bTrans = glm::rotate(bTrans, (float)glfwGetTime() * (float)(glm::radians(20.0f)), glm::vec3(0.0f, 0.0f, 1.0f));
			}
			glm::mat4 bModel(1.0f);
			glm::mat4 bView(1.0f);
			bView = glm::translate(bView, cameraPos * -1.0f);
			
			glm::mat4 bPerspective = glm::perspective(glm::radians(45.0f), (float)screenWdt / (float)screenHgh, 0.1f, 100.0f);
			boxShader.umat4("transform", bTrans);
			boxShader.umat4("model", bModel);
			boxShader.umat4("view", bView);
			boxShader.umat4("perspective", bPerspective);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		precess_event(window);
	}
	
	glfwTerminate();
	return 0;
}