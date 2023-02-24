#include "../include/chunkList.h"
#include <chrono>
#include <ratio>
#include <thread>

void ChunkList::chunkManager() {
    if (int(camPosX) % CHUNK_SIZE == 0 || int(camPosY) % CHUNK_SIZE == 0 || int(camPosZ) % CHUNK_SIZE == 0) {
        organiselck = 1;
        assignChunkID();
        organiselck = 0;
        putInVAOs();
    }
}

void ChunkList::calculateLoadedChunks() {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    while (run == 1) {
        for (int i = 0; i < (RENDER_DISTANCE * RENDER_DISTANCE * RENDER_DISTANCE); i++) {
            float distance = sqrt(pow((chunkWorldContainer[i].chunkID[0] * CHUNK_SIZE) - camPosX, 2) + pow((chunkWorldContainer[i].chunkID[1] * CHUNK_SIZE) - camPosY, 2) + pow((chunkWorldContainer[i].chunkID[2] * CHUNK_SIZE) - camPosZ, 2));
            //float distance = abs((chunkWorldContainer[i].chunkID[0] * CHUNK_SIZE) - camPosX) + abs(2 * ((chunkWorldContainer[i].chunkID[1] * CHUNK_SIZE) - camPosY)) + abs((chunkWorldContainer[i].chunkID[2] * CHUNK_SIZE) - camPosZ);
            chunkWorldContainer[i].distance = distance;
        }
    }
}

void ChunkList::assignChunkID() {
    while (run == 1) {
        for (int i = 0; i < RENDER_DISTANCE; i++) {
            for (int j = 0; j < RENDER_DISTANCE; j++) {
                for (int k = 0; k < RENDER_DISTANCE; k++) {
                    loadedChunkCoord[(i * RENDER_DISTANCE * RENDER_DISTANCE) + (j * RENDER_DISTANCE) + k][0] = fastFloat::fastFloor(camPosX / CHUNK_SIZE) + i - (RENDER_DISTANCE / 2);
                    loadedChunkCoord[(i * RENDER_DISTANCE * RENDER_DISTANCE) + (j * RENDER_DISTANCE) + k][1] = fastFloat::fastFloor(camPosY / CHUNK_SIZE) + j - (RENDER_DISTANCE / 2);
                    loadedChunkCoord[(i * RENDER_DISTANCE * RENDER_DISTANCE) + (j * RENDER_DISTANCE) + k][2] = fastFloat::fastFloor(camPosZ / CHUNK_SIZE) + k - (RENDER_DISTANCE / 2);
                    loadedChunkCoord[(i * RENDER_DISTANCE * RENDER_DISTANCE) + (j * RENDER_DISTANCE) + k][3] = 0;
                }
            }
        }
        if (firstRun == 0) {
            for (int i = 0; i < (RENDER_DISTANCE * RENDER_DISTANCE * RENDER_DISTANCE); i++) {
                chunkWorldContainer[i].chunkID[0] = loadedChunkCoord[i][0];
                chunkWorldContainer[i].chunkID[1] = loadedChunkCoord[i][1];
                chunkWorldContainer[i].chunkID[2] = loadedChunkCoord[i][2];
                chunkWorldContainer[i].emptyChunk = 0;
                chunkWorldContainer[i].unGeneratedChunk = 1;
                chunkWorldContainer[i].unCompiledChunk = 1;
                chunkWorldContainer[i].renderlck = 1;
            }
            firstRun = 1;
        } else {
            for (int i = 0; i < (RENDER_DISTANCE * RENDER_DISTANCE * RENDER_DISTANCE); i++) {
                bool chunkLoaded = 0;
                for (int j = 0; j < (RENDER_DISTANCE * RENDER_DISTANCE * RENDER_DISTANCE); j++) {
                    if (chunkWorldContainer[i].chunkID[0] == loadedChunkCoord[j][0] && chunkWorldContainer[i].chunkID[1] == loadedChunkCoord[j][1] && chunkWorldContainer[i].chunkID[2] == loadedChunkCoord[j][2]) {
                        chunkLoaded = 1;
                        loadedChunkCoord[j][3] = 1;
                    }
                }
                if (chunkLoaded == 0) {
                    chunkWorldContainer[i].emptyChunk = 1;
                }
            }
            for (int i = 0; i < (RENDER_DISTANCE * RENDER_DISTANCE * RENDER_DISTANCE); i++) {
                if (chunkWorldContainer[i].emptyChunk == 1) {
                    for (int j = 0; j < (RENDER_DISTANCE * RENDER_DISTANCE * RENDER_DISTANCE); j++) {
                        if (loadedChunkCoord[j][3] == 0) {
                            loadedChunkCoord[j][3] = 1;

                            chunkWorldContainer[i].chunkID[0] = loadedChunkCoord[j][0];
                            chunkWorldContainer[i].chunkID[1] = loadedChunkCoord[j][1];
                            chunkWorldContainer[i].chunkID[2] = loadedChunkCoord[j][2];
                            chunkWorldContainer[i].unGeneratedChunk = 1;
                            chunkWorldContainer[i].unCompiledChunk = 1;
                            chunkWorldContainer[i].EBOsize = 0;
                            chunkWorldContainer[i].chunkData.clear();
                            chunkWorldContainer[i].generatelck = 0;
                            chunkWorldContainer[i].emptyChunk = 0;
                            break;
                        }
                    }
                }
            }
        }
    }
}

bool ChunkList::isEdgeChunk(int coordX, int coordY, int coordZ) {
    if (coordX - fastFloat::fastFloor(camPosX / CHUNK_SIZE) == RENDER_DISTANCE / 2 - 1 || fastFloat::fastFloor(camPosX / CHUNK_SIZE) - coordX == RENDER_DISTANCE / 2 || coordY - fastFloat::fastFloor(camPosY / CHUNK_SIZE) == RENDER_DISTANCE / 2 - 1 || fastFloat::fastFloor(camPosY / CHUNK_SIZE) - coordY == RENDER_DISTANCE / 2 || coordZ - fastFloat::fastFloor(camPosZ / CHUNK_SIZE) == RENDER_DISTANCE / 2 - 1 || fastFloat::fastFloor(camPosZ / CHUNK_SIZE) - coordZ == RENDER_DISTANCE / 2) {
        return 1;
    }
    return 0;
}

void ChunkList::organiseChunks(int threadID) {
    std::this_thread::sleep_for(std::chrono::seconds(3));
    //int threadID = 0;
    while (run == 1) {
        if (organiselck == 0) {
            int leastIndex = 0;
            do {
                for (int i = 0; i < (RENDER_DISTANCE * RENDER_DISTANCE * RENDER_DISTANCE); i++) {
                    if (chunkWorldContainer[i].distance <= chunkWorldContainer[leastIndex].distance && (chunkWorldContainer[i].unCompiledChunk == 1 || chunkWorldContainer[i].forUpdate == 1) && chunkWorldContainer[i].inQueue == 0) {
                        leastIndex = i;
                    }
                }
            } while (chunkWorldContainer[leastIndex].inQueue == 1);
            chunkWorldContainer[leastIndex].inQueue = 1;
            index[threadID] = leastIndex;
            //std::cout << chunkWorldContainer[index[threadID]].unCompiledChunk << " ThreadID: " << threadID << " || Index: " << index[threadID] << " || X Y Z: " << chunkWorldContainer[index[threadID]].chunkID[0] << " " << chunkWorldContainer[index[threadID]].chunkID[1] << " " << chunkWorldContainer[index[threadID]].chunkID[2] << " || Distance: " << chunkWorldContainer[index[threadID]].distance << "\n";
            chunkX[threadID] = chunkWorldContainer[index[threadID]].chunkID[0];
            chunkY[threadID] = chunkWorldContainer[index[threadID]].chunkID[1];
            chunkZ[threadID] = chunkWorldContainer[index[threadID]].chunkID[2];
            std::unique_lock<std::mutex> lock(chunkWorldContainerMutex);
            if ((chunkWorldContainer[index[threadID]].unCompiledChunk == 1 || chunkWorldContainer[index[threadID]].forUpdate == 1) && chunkWorldContainer[index[threadID]].unGeneratedChunk == 0) {
                chunkWorldContainer[index[threadID]].generatelck = 1;
                chunkWorldContainer[index[threadID]].vaolck = 1;
                chunkWorldContainer[index[threadID]].EBOsize = buildChunk(threadID);
                chunkWorldContainer[index[threadID]].vaolck = 0;
            } else {
                chunkWorldContainer[index[threadID]].vaolck = 1;
            }
            lock.unlock();
            chunkWorldContainer[index[threadID]].inQueue = 0;
        }
    }
}

void ChunkList::generateChunks() {
    std::this_thread::sleep_for(std::chrono::milliseconds(250));
    while (run == 1) {
        if (organiselck == 0) {
            int index = 0;
            for (int i = 0; i < (RENDER_DISTANCE * RENDER_DISTANCE * RENDER_DISTANCE); i++) {
                if (chunkWorldContainer[i].distance <= chunkWorldContainer[index].distance && chunkWorldContainer[i].unGeneratedChunk == 1) {
                    index = i;
                }
            }
            //std::cout << chunkWorldContainer[index].unGeneratedChunk <<  " Gen || Index: " << index << " || X Y Z: " << chunkWorldContainer[index].chunkID[0] << " " << chunkWorldContainer[index].chunkID[1] << " " << chunkWorldContainer[index].chunkID[2] << " || Distance: " << chunkWorldContainer[index].distance << "\n";
            if (chunkWorldContainer[index].unGeneratedChunk == 1) {
                generator.initChunk(chunkWorldContainer[index].chunkData);
                generator.generateChunk(chunkWorldContainer[index].chunkData, chunkWorldContainer[index].chunkID[0], chunkWorldContainer[index].chunkID[1], chunkWorldContainer[index].chunkID[2]);
                chunkWorldContainer[index].unGeneratedChunk = 0;
            }
        }
    }
}

void ChunkList::putInVAOs() {
    for (int i = 0; i < (RENDER_DISTANCE * RENDER_DISTANCE * RENDER_DISTANCE); i++) {
        if (chunkWorldContainer[i].unCompiledChunk == 0 && chunkWorldContainer[i].EBOsize != 0 && chunkWorldContainer[i].vaolck == 0) {
            chunkWorldContainer[i].array.Bind();

            VBO VBO1;
            EBO EBO1;

            VBO1.Gen(chunkWorldContainer[i].mesh);
            EBO1.Gen(chunkWorldContainer[i].indices);

            chunkWorldContainer[i].array.LinkAttribI(VBO1, 0, 1, GL_SHORT, 2 * sizeof(short), (void*)0);
		    chunkWorldContainer[i].array.LinkAttribI(VBO1, 1, 1, GL_SHORT, 2 * sizeof(short), (void*)(1 * sizeof(short)));

            chunkWorldContainer[i].array.Unbind();
            VBO1.Unbind();
            EBO1.Unbind();

            VBO1.Delete();
            EBO1.Delete();
            chunkWorldContainer[i].renderlck = 0;
            chunkWorldContainer[i].vaolck = 1;
        }
    }
}

bool ChunkList::alreadyIn(std::vector <int> queue, int element) {
    for (int i = 0; i < queue.size(); i++) {
        if (queue.at(i) == element) {
            return 0;
            break;
        }
    }
    return 1;
}

void ChunkList::updateChunk(int ChunkX, int ChunkY, int ChunkZ, bool surroundings) {
    if (surroundings) {
        for (int i = 0; i < RENDER_DISTANCE * RENDER_DISTANCE * RENDER_DISTANCE; i++) {
            if (chunkWorldContainer[i].chunkID[0] == ChunkX && chunkWorldContainer[i].chunkID[1] == ChunkY && chunkWorldContainer[i].chunkID[2] == ChunkZ + 1 && chunkWorldContainer[i].chunkData.size() == CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE) {
                chunkWorldContainer[i].forUpdate = 1;
            }
            if (chunkWorldContainer[i].chunkID[0] == ChunkX && chunkWorldContainer[i].chunkID[1] == ChunkY && chunkWorldContainer[i].chunkID[2] == ChunkZ - 1 && chunkWorldContainer[i].chunkData.size() == CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE) {
                chunkWorldContainer[i].forUpdate = 1;
            }
            if (chunkWorldContainer[i].chunkID[0] == ChunkX && chunkWorldContainer[i].chunkID[1] == ChunkY + 1 && chunkWorldContainer[i].chunkID[2] == ChunkZ && chunkWorldContainer[i].chunkData.size() == CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE) {
                chunkWorldContainer[i].forUpdate = 1;
            }
            if (chunkWorldContainer[i].chunkID[0] == ChunkX && chunkWorldContainer[i].chunkID[1] == ChunkY - 1 && chunkWorldContainer[i].chunkID[2] == ChunkZ && chunkWorldContainer[i].chunkData.size() == CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE) {
                chunkWorldContainer[i].forUpdate = 1;
            }
            if (chunkWorldContainer[i].chunkID[0] == ChunkX + 1 && chunkWorldContainer[i].chunkID[1] == ChunkY && chunkWorldContainer[i].chunkID[2] == ChunkZ && chunkWorldContainer[i].chunkData.size() == CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE) {
                chunkWorldContainer[i].forUpdate = 1;
            }
            if (chunkWorldContainer[i].chunkID[0] == ChunkX - 1 && chunkWorldContainer[i].chunkID[1] == ChunkY && chunkWorldContainer[i].chunkID[2] == ChunkZ && chunkWorldContainer[i].chunkData.size() == CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE) {
                chunkWorldContainer[i].forUpdate = 1;
            }
        }
    }
    for (int i = 0; i < RENDER_DISTANCE * RENDER_DISTANCE * RENDER_DISTANCE; i++) {
        if (chunkWorldContainer[i].chunkID[0] == ChunkX && chunkWorldContainer[i].chunkID[1] == ChunkY && chunkWorldContainer[i].chunkID[2] == ChunkZ && chunkWorldContainer[i].chunkData.size() == CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE) {
            chunkWorldContainer[i].forUpdate = 1;
            break;
        }
    }
}