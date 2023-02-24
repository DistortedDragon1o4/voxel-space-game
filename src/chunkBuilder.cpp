#include "../include/chunkList.h"

void ChunkList::doIndices(int threadID) {
    std::vector<uint> crntEBO;
    for (int i = 0; i < 6; i++) {
        crntEBO.push_back((4 * call[threadID]) + solidBlock.dataEBO[i]);
    }
    chunkWorldContainer[index[threadID]].indices.insert(chunkWorldContainer[index[threadID]].indices.end(), crntEBO.begin(), crntEBO.end());
    call[threadID]++;
}

int ChunkList::getIndex(int coordX, int coordY, int coordZ) {
    for (int i = 0; i < RENDER_DISTANCE * RENDER_DISTANCE * RENDER_DISTANCE; i++)
        if (chunkWorldContainer[i].chunkID[0] == coordX && chunkWorldContainer[i].chunkID[1] == coordY && chunkWorldContainer[i].chunkID[2] == coordZ && chunkWorldContainer[i].unGeneratedChunk == 0)
            return i;
    return -1;
}

int ChunkList::globalBlockAt(int coordX, int coordY, int coordZ, int threadID) {
    int currentChunkX = chunkX[threadID];
    int currentChunkY = chunkY[threadID];
    int currentChunkZ = chunkZ[threadID];
    int x = coordX;
    int y = coordY;
    int z = coordZ;
    if (coordX < 0) {
        currentChunkX = chunkX[threadID] - 1;
        x = 15;
    } else if (coordX >= CHUNK_SIZE) {
        currentChunkX = chunkX[threadID] + 1;
        x = 0;
    }
    if (coordY < 0) {
        currentChunkY = chunkY[threadID] - 1;
        y = 15;
    } else if (coordY >= CHUNK_SIZE) {
        currentChunkY = chunkY[threadID] + 1;
        y = 0;
    }
    if (coordZ < 0) {
        currentChunkZ = chunkZ[threadID] - 1;
        z = 15;
    } else if (coordZ >= CHUNK_SIZE) {
        currentChunkZ = chunkZ[threadID] + 1;
        z = 0;
    }

    bool match = 0;
    int currentIndex = getIndex(currentChunkX, currentChunkY, currentChunkZ);

    if (currentIndex >= 0 && currentIndex < RENDER_DISTANCE * RENDER_DISTANCE * RENDER_DISTANCE) {
        return chunkWorldContainer[currentIndex].chunkData[(x * CHUNK_SIZE * CHUNK_SIZE) + (y * CHUNK_SIZE) + z];
        return 0;
    } else {
        discardChunk[threadID] = 1;
    }
    return 0;
}

int ChunkList::blockAt(int coordX, int coordY, int coordZ, int threadID) {
    if (coordX < 0 || coordZ < 0 || coordY < 0 || coordX >= CHUNK_SIZE || coordZ >= CHUNK_SIZE || coordY >= CHUNK_SIZE) {
        return globalBlockAt(coordX, coordY, coordZ, threadID);
    } else {
        return chunkWorldContainer[index[threadID]].chunkData[(coordX * CHUNK_SIZE * CHUNK_SIZE) + (coordY * CHUNK_SIZE) + coordZ];
        return 0;
    }
}

void ChunkList::combineFace(int coordX, int coordY, int coordZ, int blockID, int threadID) {
    std::vector<short> locfr {0, 0, 0, 0, 0, 0, 0, 0};
    std::vector<short> locbk {0, 0, 0, 0, 0, 0, 0, 0};
    std::vector<short> locrt {0, 0, 0, 0, 0, 0, 0, 0};
    std::vector<short> loclt {0, 0, 0, 0, 0, 0, 0, 0};
    std::vector<short> loctp {0, 0, 0, 0, 0, 0, 0, 0};
    std::vector<short> locbt {0, 0, 0, 0, 0, 0, 0, 0};

    std::vector<bool> cull {1, 1, 1, 1, 1, 1};
    if (blocks[blockAt(coordX, coordY, coordZ + 1, threadID)].isSolid || blockAt(coordX, coordY, coordZ + 1, threadID) == blockID)
        cull[0] = 0;
    if (blocks[blockAt(coordX, coordY, coordZ - 1, threadID)].isSolid || blockAt(coordX, coordY, coordZ - 1, threadID) == blockID)
        cull[1] = 0;
    if (blocks[blockAt(coordX + 1, coordY, coordZ, threadID)].isSolid || blockAt(coordX + 1, coordY, coordZ, threadID) == blockID)
        cull[2] = 0;
    if (blocks[blockAt(coordX - 1, coordY, coordZ, threadID)].isSolid || blockAt(coordX - 1, coordY, coordZ, threadID) == blockID)
        cull[3] = 0;
    if (blocks[blockAt(coordX, coordY + 1, coordZ, threadID)].isSolid || blockAt(coordX, coordY + 1, coordZ, threadID) == blockID)
        cull[4] = 0;
    if (blocks[blockAt(coordX, coordY - 1, coordZ, threadID)].isSolid || blockAt(coordX, coordY - 1, coordZ, threadID) == blockID)
        cull[5] = 0;

    for (int i = 0; i < 15; i += 4) {
        if (cull[0] == 1) {
            bool darken = 0;
            locfr[i / 2] = (1024 * (coordX + solidBlock.fr[i])) + (32 * (coordY + solidBlock.fr[i + 1])) + coordZ + solidBlock.fr[i + 2];
            if (blocks[blockID].isSolid) {
                if (i == 0) {
                    if (blocks[blockAt(coordX + 1, coordY, coordZ + 1, threadID)].isSolid || blocks[blockAt(coordX, coordY + 1, coordZ + 1, threadID)].isSolid || blocks[blockAt(coordX + 1, coordY + 1, coordZ + 1, threadID)].isSolid)
                        darken = 1;
                } else if (i == 4) {
                    if (blocks[blockAt(coordX - 1, coordY, coordZ + 1, threadID)].isSolid || blocks[blockAt(coordX, coordY + 1, coordZ + 1, threadID)].isSolid || blocks[blockAt(coordX - 1, coordY + 1, coordZ + 1, threadID)].isSolid)
                        darken = 1;
                } else if (i == 8) {
                    if (blocks[blockAt(coordX - 1, coordY, coordZ + 1, threadID)].isSolid || blocks[blockAt(coordX, coordY - 1, coordZ + 1, threadID)].isSolid || blocks[blockAt(coordX - 1, coordY - 1, coordZ + 1, threadID)].isSolid)
                        darken = 1;
                } else if (i == 12) {
                    if (blocks[blockAt(coordX + 1, coordY, coordZ + 1, threadID)].isSolid || blocks[blockAt(coordX, coordY - 1, coordZ + 1, threadID)].isSolid || blocks[blockAt(coordX + 1, coordY - 1, coordZ + 1, threadID)].isSolid)
                        darken = 1;
                }
            }
            if (darken == 1)
                locfr[(i / 2) + 1] = solidBlock.fr[i + 3] + 1 + (blockID * 0b1000000);
            else
                locfr[(i / 2) + 1] = solidBlock.fr[i + 3] + (blockID * 0b1000000);
        }
        if (cull[1] == 1) {
            bool darken = 0;
            locbk[i / 2] = (1024 * (coordX + solidBlock.bk[i])) + (32 * (coordY + solidBlock.bk[i + 1])) + coordZ + solidBlock.bk[i + 2];
            if (blocks[blockID].isSolid) {
                if (i == 0) {
                    if (blocks[blockAt(coordX - 1, coordY, coordZ - 1, threadID)].isSolid || blocks[blockAt(coordX, coordY - 1, coordZ - 1, threadID)].isSolid || blocks[blockAt(coordX - 1, coordY - 1, coordZ - 1, threadID)].isSolid)
                        darken = 1;
                } else if (i == 4) {
                    if (blocks[blockAt(coordX - 1, coordY, coordZ - 1, threadID)].isSolid || blocks[blockAt(coordX, coordY + 1, coordZ - 1, threadID)].isSolid || blocks[blockAt(coordX - 1, coordY + 1, coordZ - 1, threadID)].isSolid)
                        darken = 1;
                } else if (i == 8) {
                    if (blocks[blockAt(coordX + 1, coordY, coordZ - 1, threadID)].isSolid || blocks[blockAt(coordX, coordY + 1, coordZ - 1, threadID)].isSolid || blocks[blockAt(coordX + 1, coordY + 1, coordZ - 1, threadID)].isSolid)
                        darken = 1;
                } else if (i == 12) {
                    if (blocks[blockAt(coordX + 1, coordY, coordZ - 1, threadID)].isSolid || blocks[blockAt(coordX, coordY - 1, coordZ - 1, threadID)].isSolid || blocks[blockAt(coordX + 1, coordY - 1, coordZ - 1, threadID)].isSolid)
                        darken = 1;
                }
            }
            if (darken == 1)
                locbk[(i / 2) + 1] = solidBlock.bk[i + 3] + 1 + (blockID * 0b1000000);
            else
                locbk[(i / 2) + 1] = solidBlock.bk[i + 3] + (blockID * 0b1000000);
        }
        if (cull[2] == 1) {
            bool darken = 0;
            locrt[i / 2] = (1024 * (coordX + solidBlock.rt[i])) + (32 * (coordY + solidBlock.rt[i + 1])) + coordZ + solidBlock.rt[i + 2];
            if (blocks[blockID].isSolid) {
                if (i == 0) {
                    if (blocks[blockAt(coordX + 1, coordY + 1, coordZ, threadID)].isSolid || blocks[blockAt(coordX + 1, coordY, coordZ + 1, threadID)].isSolid || blocks[blockAt(coordX + 1, coordY + 1, coordZ + 1, threadID)].isSolid)
                        darken = 1;
                } else if (i == 4) {
                    if (blocks[blockAt(coordX + 1, coordY - 1, coordZ, threadID)].isSolid || blocks[blockAt(coordX + 1, coordY, coordZ + 1, threadID)].isSolid || blocks[blockAt(coordX + 1, coordY - 1, coordZ + 1, threadID)].isSolid)
                        darken = 1;
                } else if (i == 8) {
                    if (blocks[blockAt(coordX + 1, coordY - 1, coordZ, threadID)].isSolid || blocks[blockAt(coordX + 1, coordY, coordZ - 1, threadID)].isSolid || blocks[blockAt(coordX + 1, coordY - 1, coordZ - 1, threadID)].isSolid)
                        darken = 1;
                } else if (i == 12) {
                    if (blocks[blockAt(coordX + 1, coordY + 1, coordZ, threadID)].isSolid || blocks[blockAt(coordX + 1, coordY, coordZ - 1, threadID)].isSolid || blocks[blockAt(coordX + 1, coordY + 1, coordZ - 1, threadID)].isSolid)
                        darken = 1;
                }
            }
            if (darken == 1)
                locrt[(i / 2) + 1] = solidBlock.rt[i + 3] + 1 + (blockID * 0b1000000);
            else
                locrt[(i / 2) + 1] = solidBlock.rt[i + 3] + (blockID * 0b1000000);
        }
        if (cull[3] == 1) {
            bool darken = 0;
            loclt[i / 2] = (1024 * (coordX + solidBlock.lt[i])) + (32 * (coordY + solidBlock.lt[i + 1])) + coordZ + solidBlock.lt[i + 2];
            if (blocks[blockID].isSolid) {
                if (i == 0) {
                    if (blocks[blockAt(coordX - 1, coordY - 1, coordZ, threadID)].isSolid || blocks[blockAt(coordX - 1, coordY, coordZ - 1, threadID)].isSolid || blocks[blockAt(coordX - 1, coordY - 1, coordZ - 1, threadID)].isSolid)
                        darken = 1;
                } else if (i == 4) {
                    if (blocks[blockAt(coordX - 1, coordY - 1, coordZ, threadID)].isSolid || blocks[blockAt(coordX - 1, coordY, coordZ + 1, threadID)].isSolid || blocks[blockAt(coordX - 1, coordY - 1, coordZ + 1, threadID)].isSolid)
                        darken = 1;
                } else if (i == 8) {
                    if (blocks[blockAt(coordX - 1, coordY + 1, coordZ, threadID)].isSolid || blocks[blockAt(coordX - 1, coordY, coordZ + 1, threadID)].isSolid || blocks[blockAt(coordX - 1, coordY + 1, coordZ + 1, threadID)].isSolid)
                        darken = 1;
                } else if (i == 12) {
                    if (blocks[blockAt(coordX - 1, coordY + 1, coordZ, threadID)].isSolid || blocks[blockAt(coordX - 1, coordY, coordZ - 1, threadID)].isSolid || blocks[blockAt(coordX - 1, coordY + 1, coordZ - 1, threadID)].isSolid)
                        darken = 1;
                }
            }
            if (darken == 1)
                loclt[(i / 2) + 1] = solidBlock.lt[i + 3] + 1 + (blockID * 0b1000000);
            else
                loclt[(i / 2) + 1] = solidBlock.lt[i + 3] + (blockID * 0b1000000);
        }
        if (cull[4] == 1) {
            bool darken = 0;
            loctp[i / 2] = (1024 * (coordX + solidBlock.tp[i])) + (32 * (coordY + solidBlock.tp[i + 1])) + coordZ + solidBlock.tp[i + 2];
            if (blocks[blockID].isSolid) {
                if (i == 0) {
                    if (blocks[blockAt(coordX - 1, coordY + 1, coordZ, threadID)].isSolid || blocks[blockAt(coordX, coordY + 1, coordZ - 1, threadID)].isSolid || blocks[blockAt(coordX - 1, coordY + 1, coordZ - 1, threadID)].isSolid)
                        darken = 1;
                } else if (i == 4) {
                    if (blocks[blockAt(coordX - 1, coordY + 1, coordZ, threadID)].isSolid || blocks[blockAt(coordX, coordY + 1, coordZ + 1, threadID)].isSolid || blocks[blockAt(coordX - 1, coordY + 1, coordZ + 1, threadID)].isSolid)
                        darken = 1;
                } else if (i == 8) {
                    if (blocks[blockAt(coordX + 1, coordY + 1, coordZ, threadID)].isSolid || blocks[blockAt(coordX, coordY + 1, coordZ + 1, threadID)].isSolid || blocks[blockAt(coordX + 1, coordY + 1, coordZ + 1, threadID)].isSolid)
                        darken = 1;
                } else if (i == 12) {
                    if (blocks[blockAt(coordX + 1, coordY + 1, coordZ, threadID)].isSolid || blocks[blockAt(coordX, coordY + 1, coordZ - 1, threadID)].isSolid || blocks[blockAt(coordX + 1, coordY + 1, coordZ - 1, threadID)].isSolid)
                        darken = 1;
                }
            }
            if (darken == 1)
                loctp[(i / 2) + 1] = solidBlock.tp[i + 3] + 1 + (blockID * 0b1000000);
            else
                loctp[(i / 2) + 1] = solidBlock.tp[i + 3] + (blockID * 0b1000000);
        }
        if (cull[5] == 1) {
            bool darken = 0;
            locbt[i / 2] = (1024 * (coordX + solidBlock.bt[i])) + (32 * (coordY + solidBlock.bt[i + 1])) + coordZ + solidBlock.bt[i + 2];
            if (blocks[blockID].isSolid) {
                if (i == 0) {
                    if (blocks[blockAt(coordX + 1, coordY - 1, coordZ, threadID)].isSolid || blocks[blockAt(coordX, coordY - 1, coordZ + 1, threadID)].isSolid || blocks[blockAt(coordX + 1, coordY - 1, coordZ + 1, threadID)].isSolid)
                        darken = 1;
                } else if (i == 4) {
                    if (blocks[blockAt(coordX - 1, coordY - 1, coordZ, threadID)].isSolid || blocks[blockAt(coordX, coordY - 1, coordZ + 1, threadID)].isSolid || blocks[blockAt(coordX - 1, coordY - 1, coordZ + 1, threadID)].isSolid)
                        darken = 1;
                } else if (i == 8) {
                    if (blocks[blockAt(coordX - 1, coordY - 1, coordZ, threadID)].isSolid || blocks[blockAt(coordX, coordY - 1, coordZ - 1, threadID)].isSolid || blocks[blockAt(coordX - 1, coordY - 1, coordZ - 1, threadID)].isSolid)
                        darken = 1;
                } else if (i == 12) {
                    if (blocks[blockAt(coordX + 1, coordY - 1, coordZ, threadID)].isSolid || blocks[blockAt(coordX, coordY - 1, coordZ - 1, threadID)].isSolid || blocks[blockAt(coordX + 1, coordY - 1, coordZ - 1, threadID)].isSolid)
                        darken = 1;
                }
            }
            if (darken == 1)
                locbt[(i / 2) + 1] = solidBlock.bt[i + 3] + 1 + (blockID * 0b1000000);
            else
                locbt[(i / 2) + 1] = solidBlock.bt[i + 3] + (blockID * 0b1000000);
        }
    }
    
    if (cull[0] == 1)
        chunkWorldContainer[index[threadID]].mesh.insert(chunkWorldContainer[index[threadID]].mesh.end(), locfr.begin(), locfr.end());
    if (cull[1] == 1)
        chunkWorldContainer[index[threadID]].mesh.insert(chunkWorldContainer[index[threadID]].mesh.end(), locbk.begin(), locbk.end());
    if (cull[2] == 1)
        chunkWorldContainer[index[threadID]].mesh.insert(chunkWorldContainer[index[threadID]].mesh.end(), locrt.begin(), locrt.end());
    if (cull[3] == 1)
        chunkWorldContainer[index[threadID]].mesh.insert(chunkWorldContainer[index[threadID]].mesh.end(), loclt.begin(), loclt.end());
    if (cull[4] == 1)
        chunkWorldContainer[index[threadID]].mesh.insert(chunkWorldContainer[index[threadID]].mesh.end(), loctp.begin(), loctp.end());
    if (cull[5] == 1)
        chunkWorldContainer[index[threadID]].mesh.insert(chunkWorldContainer[index[threadID]].mesh.end(), locbt.begin(), locbt.end());

    for (int i = 0; i < 6; i++)
        if (cull[i] == 1)
            doIndices(threadID);
}

int ChunkList::buildChunk(int threadID) {

    discardChunk[threadID] = 0;

    combineFace(0, 0, 0, chunkWorldContainer[index[threadID]].chunkData[0], threadID);
    combineFace(CHUNK_SIZE - 1, CHUNK_SIZE - 1, CHUNK_SIZE - 1, chunkWorldContainer[index[threadID]].chunkData[(CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE) - 1], threadID);
    if (discardChunk[threadID] == 1) {
        chunkWorldContainer[index[threadID]].unCompiledChunk = 1;
        chunkWorldContainer[index[threadID]].forUpdate = 1;
        return 0;
    }

    chunkWorldContainer[index[threadID]].indices.clear();
    chunkWorldContainer[index[threadID]].mesh.clear();

    call[threadID] = 0;

    for (int i = 0; i < CHUNK_SIZE; i++)
        for (int j = 0; j < CHUNK_SIZE; j++)
            for (int k = 0; k < CHUNK_SIZE; k++)
                if (chunkWorldContainer[index[threadID]].chunkData[(i * CHUNK_SIZE * CHUNK_SIZE) + (j * CHUNK_SIZE) + k] > 0)
                    combineFace(i, j, k, chunkWorldContainer[index[threadID]].chunkData[(i * CHUNK_SIZE * CHUNK_SIZE) + (j * CHUNK_SIZE) + k], threadID);
    

    if (discardChunk[threadID] == 1) {
        chunkWorldContainer[index[threadID]].unCompiledChunk = 1;
        chunkWorldContainer[index[threadID]].forUpdate = 1;
        return 0;
    } else {
        chunkWorldContainer[index[threadID]].unCompiledChunk = 0;
        chunkWorldContainer[index[threadID]].forUpdate = 0;
    }
    
    return chunkWorldContainer[index[threadID]].indices.size();
}