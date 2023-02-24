R"(
#version 460 core
#extension GL_ARB_gpu_shader_fp64 : enable

layout (location = 0) in int aData;
layout (location = 1) in int aTex;
//layout (location = 2) in float aBrightness;

out vec2 texCoord;
out float ambientOcc;

out float camDistance;

out float blockID;

out vec3 normal;

uniform dmat4 cameraMatrix;

uniform vec3 camPos;

uniform int chunkIDx;
uniform int chunkIDy;
uniform int chunkIDz;

void main() {
  int x = aData / 1024;
  int y = (aData / 32) - (32 * x);
  int z = aData - (32 * (aData / 32));

  int aTexCoord = aTex / 16;

  int iBrightness = aTex - (2 * (aTex / 2));

  int iNormal = (aTex / 2) - (8 * aTexCoord);
  if (iNormal == 1)
    normal = vec3(0.0f, 0.0f, 1.0f);
  else if (iNormal == 2)
    normal = vec3(0.0f, 0.0f, -1.0f);
  else if (iNormal == 3)
    normal = vec3(1.0f, 0.0f, 0.0f);
  else if (iNormal == 4)
    normal = vec3(-1.0f, 0.0f, 0.0f);
  else if (iNormal == 5)
    normal = vec3(0.0f, 1.0f, 0.0f);
  else if (iNormal == 6)
    normal = vec3(0.0f, -1.0f, 0.0f);

  aTexCoord = aTexCoord % 4;

  blockID = int(aTex / 64);

  vec2 tex;
  if (aTexCoord == 0) {
    tex = vec2(1.0f, 1.0f);
  } else if (aTexCoord == 1) {
    tex = vec2(0.0f, 1.0f);
  } else if (aTexCoord == 2) {
    tex = vec2(0.0f, 0.0f);
  } else if (aTexCoord == 3) {
    tex = vec2(1.0f, 0.0f);
  }

  dvec4 mesh = dvec4(x + double(chunkIDx * 16), y + double(chunkIDy * 16), z + double(chunkIDz * 16), 1.0f);

  gl_Position = vec4(cameraMatrix * mesh);
  texCoord = tex;

  camDistance = sqrt(pow(float(mesh.x - camPos.x), 2) + pow(float(mesh.y - camPos.y), 2) + pow(float(mesh.z - camPos.z), 2));

  /*if (x == 0 || x == 16 || y == 0 || y == 16 || z == 0 || z == 16) {
    brightness = 0.8f;
  } else {*/
    ambientOcc = float(iBrightness) / 10.0f;
  //}
}
)"
