#include "../include/camera.h"
#include <GLFW/glfw3.h>

Camera::Camera(int width, int height, glm::dvec3 position) {
	Camera::width = width;
	Camera::height = height;
	Position = position;
}

void Camera::matrix(double FOVdeg, double nearPlane, double farPlane, Shader& shader, const char* uniform) {
	glm::dmat4 view = glm::dmat4(1.0f);
	glm::dmat4 projection = glm::dmat4(1.0f);

	view = glm::lookAt(Position, Position + Orientation, Up);
	projection = glm::perspective(glm::radians(FOVdeg), double(width / height), nearPlane, farPlane);

	glUniformMatrix4dv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(projection * view));
}

void Camera::inputs(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		Position += speed * glm::normalize(glm::dvec3(Orientation.x, 0.0f, Orientation.z));
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		Position -= speed * glm::normalize(glm::cross(Orientation, Up));
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		Position -= speed * glm::normalize(glm::dvec3(Orientation.x, 0.0f, Orientation.z));
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		Position += speed * glm::normalize(glm::cross(Orientation, Up));
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		Position += speed * Up;
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		Position -= speed * Up;
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
		speed = 0.7f;
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_RELEASE)
		speed = 0.1f;

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) != GLFW_PRESS)
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

		if (firstClick && glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) != GLFW_PRESS) {
			glfwSetCursorPos(window, (width / 2), (height / 2));
			firstClick = false;
		}

		double mouseX;
		double mouseY;
		glfwGetCursorPos(window, &mouseX, &mouseY);

		double rotX = -sensitivity * double((mouseY - (height / 2)) / height);
		double rotY = sensitivity * double((mouseX - (width / 2)) / width);

		glm::dvec3 newOrientation = glm::rotate(Orientation, glm::radians(rotX), glm::normalize(glm::cross(Orientation, Up)));

		if (!(sqrt(pow((newOrientation.x - Up.x), 2) + pow((newOrientation.y - Up.y), 2) + pow((newOrientation.z - Up.z), 2)) < 1.001f) && !(sqrt(pow((newOrientation.x - Up.x), 2) + pow((newOrientation.y - Up.y), 2) + pow((newOrientation.z - Up.z), 2)) > 2.999f))
			Orientation = newOrientation;

		Orientation = glm::rotate(Orientation, glm::radians(-rotY), Up);

		if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) != GLFW_PRESS)
			glfwSetCursorPos(window, (width / 2), (height / 2));
	} else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		firstClick = true;
	}

	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
		std::cout << "X: " << Position.x << "\n";
		std::cout << "Y: " << Position.y << "\n";
		std::cout << "Z: " << Position.z << "\n";
		std::cout << "\n";
		std::cout << "chunkX: " << fastFloat::fastFloor(Position.x / 16) << "\n";
		std::cout << "chunkY: " << fastFloat::fastFloor(Position.y / 16) << "\n";
		std::cout << "chunkZ: " << fastFloat::fastFloor(Position.z / 16) << "\n";
		std::cout << "\n\n\n";
	}
}