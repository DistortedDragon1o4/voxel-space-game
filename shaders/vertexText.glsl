R"(
#version 460 core
#extension GL_ARB_gpu_shader_fp64 : enable
#extension GL_KHR_vulkan_glsl : enable

layout (location = 0) in vec2 aData;
layout (location = 1) in vec3 aColor;

out vec3 color;

void main() {
  color = aColor;
  // vec3 data;
  // if (aData == 0)
  //   data = vec3(0.0f, 0.0f, 1.0f);
  // if (aData == 1)
  //   data = vec3(1.0f, 0.0f, 1.0f);
  // if (aData == 2)
  //   data = vec3(0.0f, 1.0f, 1.0f);
  gl_Position = vec4(aData, 0.0f, 1.0f);
}
)"
