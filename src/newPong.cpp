#include "newPong.h"

Shader::Shader() {
	const char* vertexShaderSource = R"(#version 330 core
	layout(location = 0) in vec3 aPos;
	layout(location = 1) in vec3 aColor;

	out vec3 ourColor;

	uniform mat4 transform;

	void main()
	{
		//gl_Position = vec4(aPos, 1.0);
		gl_Position = transform * vec4(aPos, 1.0);
		//ourColor = aColor;
	}
	)";

	const char* fragmentShaderSource = R"(#version 330 core
	out vec4 FragColor;
	
	uniform vec4 vertexColor;

	void main()
	{
		FragColor = vertexColor;
	}
	)";

	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	int  success = 0;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glUseProgram(shaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

Shader::~Shader() {
	glDeleteProgram(shaderProgram);
}

void Shader::useProgram() {
	glUseProgram(shaderProgram);
}

void Shader::setVertexColor(float r, float g, float b) {
	useProgram();
	glUniform4f(glGetUniformLocation(shaderProgram, "vertexColor"), r, g, b, 1.0f);
}
//
//void Shader::setVertexScale(float b, float h, float v) {
//	useProgram();
//
//	auto transform = glm::scale(glm::mat4(1.0f), glm::vec3(b, h, v));
//	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "transform"), 1, GL_FALSE, glm::value_ptr(transform));
//}
//
//void Shader::setVertexTranslation(float b, float h, float v) {
//	useProgram();
//
//	auto transform = glm::translate(glm::mat4(1.0f), glm::vec3(b, h, v));
//	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "transform"), 1, GL_FALSE, glm::value_ptr(transform));
//}

void Shader::setTransform(const glm::mat4& transform) {
	useProgram();
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "transform"), 1, GL_FALSE, glm::value_ptr(transform));
}

Buffer::Buffer(vector<float>& vertices, vector<int>& indices) {
	unsigned int VBO, EBO;
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(float), indices.data(), GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute

	glBindVertexArray(0);
}

void Buffer::bind() {
	glBindVertexArray(vao);
}

Rectangle::Rectangle(Shader& shader, Buffer& buffer)
	: shaderProgram(shader), buffer(buffer)
{ }

void Rectangle::color(float r, float g, float b) {
	shaderProgram.setVertexColor(r, g, b);
}

void Rectangle::scale(float x, float y, float z) {
	s = glm::vec3(x, y, z);
}

//void Rectangle::rotate() {
	//transform = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
	//shaderProgram.setTransform(transform);
//}

void Rectangle::setPosition(float x, float y, float z) {
	t = glm::vec3(x, y, z);
}

void Rectangle::draw() {
	shaderProgram.useProgram();

	auto scale = glm::scale(glm::mat4(1.0f), s);
	auto trans = glm::translate(glm::mat4(1.0f), t);
	auto M = trans * scale;
	shaderProgram.setTransform(M);

	//auto transform = glm::scale(glm::mat4(1.0f), s);
	//transform = glm::translate(transform, t);
	//shaderProgram.setTransform(transform);

	buffer.bind();
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

Player::Player(Shader& shader, Buffer& buffer)
	: Rectangle(shader, buffer)
{ }