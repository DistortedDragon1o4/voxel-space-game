#include "../include/chunkList.h"

void ChunkList::lookingAtBlock() {
    glm::dvec3 camPos = glm::dvec3(camPosX, camPosY, camPosZ);
    camDir = glm::normalize(camDir) * increment;
    for (int i = 0; i < 100; i++) {
        glm::ivec3 blockPosa;
        blockPosa = glm::ivec3(fastFloat::fastFloor(camPos.x + (camDir.x * i)), fastFloat::fastFloor(camPos.y + (camDir.y * i)), fastFloat::fastFloor(camPos.z + (camDir.z * i)));
        int x = fastFloat::fastFloor(float(blockPosa.x) / CHUNK_SIZE);
        int y = fastFloat::fastFloor(float(blockPosa.y) / CHUNK_SIZE);
        int z = fastFloat::fastFloor(float(blockPosa.z) / CHUNK_SIZE);
        int x1 = blockPosa.x - (CHUNK_SIZE * x);
        int y1 = blockPosa.y - (CHUNK_SIZE * y);
        int z1 = blockPosa.z - (CHUNK_SIZE * z);
        bool yes = 0;
        for (int j = 0; j < RENDER_DISTANCE * RENDER_DISTANCE * RENDER_DISTANCE; j++) {
            if (chunkWorldContainer[j].chunkID[0] == x && chunkWorldContainer[j].chunkID[1] == y && chunkWorldContainer[j].chunkID[2] == z && chunkWorldContainer[j].chunkData.size() == CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE) {
                if (chunkWorldContainer[j].chunkData.at((x1 * CHUNK_SIZE * CHUNK_SIZE) + (y1 * CHUNK_SIZE) + z1) > 0) {
                    //std::cout << blockPosa.x << " " << blockPosa.y << " " << blockPosa.z << "\n";
                    blockPos = blockPosa;
                    yes = 1;
                    chunkWorldContainer[j].chunkData.at((x1 * CHUNK_SIZE * CHUNK_SIZE) + (y1 * CHUNK_SIZE) + z1) = 0;
                    // if (x1 == 0 || x1 == CHUNK_SIZE || y1 == 0 || y1 == CHUNK_SIZE || z1 == 0 || z1 == CHUNK_SIZE)
                        updateChunk(x, y, z, 1);
                    // else
                    //     updateChunk(x, y, z, 0);
                    break;
                }
            }
        }
        if(yes)
            break;
        else
            blockPos = glm::ivec3(2147483647, 2147483647, 2147483647);
    }
}

void ChunkList::createHighlightVAO() {
    highlightVAO.Bind();

    VBO VBO1;
    EBO EBO1;
    
    VBO1.Gen(highlightCube);
    EBO1.Gen(highlightEBO);

    highlightVAO.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);

    highlightVAO.Unbind();
    VBO1.Unbind();
    EBO1.Unbind();

    VBO1.Delete();
    EBO1.Delete();
}