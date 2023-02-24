#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

#define GLM_ENABLE_EXPERIMENTAL

#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/rotate_vector.hpp"
#include "glm/gtx/vector_angle.hpp"
#include <math.h>
#include "fastFloat.h"

#include <iostream>

#include "shaderCompiler.h"

class Camera {
public:
    glm::dvec3 Position;
    glm::dvec3 Orientation = glm::dvec3(0.0f, 0.0f, -2.0f);
    glm::dvec3 Up = glm::dvec3(0.0f, 1.0f, 0.0f);

    bool firstClick = true;

    double width;
    double height;

    double speed = 0.1f;
    double sensitivity = 100.0f;

    Camera(int width, int height, glm::dvec3 position);

    void matrix(double FOVdeg, double nearPlane, double farPlane, Shader& shader, const char* uniform);
    void inputs(GLFWwindow* window);
};

#endif