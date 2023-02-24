R"(
#version 460 core
#extension GL_ARB_gpu_shader_fp64 : enable

layout (location = 0) in int aData;

uniform mat4 lightProj;

void main() {
  int x = aData / 1024;
  int y = (aData / 32) - (32 * x);
  int z = aData - (32 * (aData / 32));
  
  dvec4 mesh = dvec4(x + double(chunkIDx * 16), y + double(chunkIDy * 16), z + double(chunkIDz * 16), 1.0f);

  gl_Position = vec4(lightProj * mesh);
}
)"
