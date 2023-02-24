#ifndef CHUNK_LIST_CLASS_H
#define CHUNK_LIST_CLASS_H

#include <bits/stdc++.h>
#include <math.h>
#include "fastFloat.h"
#include <vector>
#include <iostream>
#include <chrono>
#include <thread>
#include <mutex>
#include "glm/glm.hpp"

#include "chunkDataContainer.h"
#include "buffers.h"
#include "chunkGenerator.h"

#define NUM_THREADS 8

class BlockTemplate {
public:
	const std::vector<short> fr {
        1, 1, 1,		0b000010,
        0, 1, 1,		0b010010,
        0, 0, 1,		0b100010,
        1, 0, 1,		0b110010,
    };
    const std::vector<short> bk {
        0, 0, 0,		0b100100,
        0, 1, 0,		0b010100,
        1, 1, 0,		0b000100,
        1, 0, 0,		0b110100,
    };
    const std::vector<short> rt {
        1, 1, 1,		0b010110,
        1, 0, 1,		0b100110,
        1, 0, 0,		0b110110,
        1, 1, 0,		0b000110,
    };
    const std::vector<short> lt {
        0, 0, 0,		0b111000,
        0, 0, 1,		0b101000,
        0, 1, 1,		0b011000,
        0, 1, 0,		0b001000,
    };
    const std::vector<short> tp {
        0, 1, 0,		0b011010,
        0, 1, 1,		0b101010,
        1, 1, 1,		0b111010,
        1, 1, 0,		0b001010,
    };
    const std::vector<short> bt {
        1, 0, 1,		0b001100,
        0, 0, 1,		0b011100,
        0, 0, 0,		0b101100,
        1, 0, 0,		0b111100,
    };

    const std::vector<uint> dataEBO {
        0, 1, 2,
    	2, 3, 0,
    };
};

class ChunkList {
private:
    Blocks blocks[NUM_BLOCKS];

    uint call[NUM_THREADS] = {0, 0};
    bool discardChunk[NUM_THREADS] = {0, 0};

    bool organiselck = 0;

	bool firstRun = 0;
	int index[NUM_THREADS];
	//int currentIndex;

	ChunkGen generator;

	BlockTemplate solidBlock;

	int loadedChunkCoord[RENDER_DISTANCE * RENDER_DISTANCE * RENDER_DISTANCE][4];
    float chunkDistance[RENDER_DISTANCE * RENDER_DISTANCE * RENDER_DISTANCE];
    std::vector <int> updateQueue;

    bool alreadyIn(std::vector <int> queue, int element);
    
	void combineFace(int coordX, int coordY, int coordZ, int blockID, int threadID);
    int blockAt(int coordX, int coordY, int coordZ, int threadID);
    int globalBlockAt(int coordX, int coordY, int coordZ, int threadID);
    void doIndices(int threadID);
    int getIndex(int coordX, int coordY, int coordZ);

	std::vector<short> mesh;
    std::vector<uint> indices;

	int chunkX[NUM_THREADS];
    int chunkY[NUM_THREADS];
    int chunkZ[NUM_THREADS];

    double increment = 0.05;

    std::mutex chunkWorldContainerMutex;

    std::vector<float> highlightCube {
        1.0001f, 1.0001f, 1.0001f,
        -0.0001f, 1.0001f, 1.0001f,
        -0.0001f, -0.0001f, 1.0001f,
        1.0001f, -0.0001f, 1.0001f,
        -0.0001f, -0.0001f, -0.0001f,
        -0.0001f, 1.0001f, -0.0001f,
        1.0001f, 1.0001f, -0.0001f,
        1.0001f, -0.0001f, -0.0001f,
        1.0001f, 1.0001f, 1.0001f,
        1.0001f, -0.0001f, 1.0001f,
        1.0001f, -0.0001f, -0.0001f,
        1.0001f, 1.0001f, -0.0001f,
        -0.0001f, -0.0001f, -0.0001f,
        -0.0001f, -0.0001f, 1.0001f,
        -0.0001f, 1.0001f, 1.0001f,
        -0.0001f, 1.0001f, -0.0001f,
        -0.0001f, 1.0001f, -0.0001f,
        -0.0001f, 1.0001f, 1.0001f,
        1.0001f, 1.0001f, 1.0001f,
        1.0001f, 1.0001f, -0.0001f,
        1.0001f, -0.0001f, 1.0001f,
        -0.0001f, -0.0001f, 1.0001f,
        -0.0001f, -0.0001f, -0.0001f,
        1.0001f, -0.0001f, -0.0001f,
        /*1.0f, 1.0f, 1.0f,
        0.0f, 1.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 1.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 0.0f,
        1.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,*/
    };
    std::vector<uint> highlightEBO {
        0, 1, 2,
    	2, 3, 0,
        4, 5, 6,
    	6, 7, 4,
        8, 9, 10,
    	10, 11, 8,
        12, 13, 14,
    	14, 15, 12,
        16, 17, 18,
    	18, 19, 16,
        20, 21, 22,
    	22, 23, 20,
    };

public:
	double camPosX = 0.0;
	double camPosY = 0.0;
	double camPosZ = 0.0;

    glm::dvec3 camDir;

    glm::ivec3 blockPos;
    VAO highlightVAO;
    int EBOsize = 36;

    bool run = 1;

    bool isEdgeChunk(int coordX, int coordY, int coordZ);

	std::vector<ChunkDataContainer> chunkWorldContainer = std::vector<ChunkDataContainer>(RENDER_DISTANCE * RENDER_DISTANCE * RENDER_DISTANCE);

    void lookingAtBlock();
    void createHighlightVAO();

    void updateChunk(int ChunkX, int ChunkY, int ChunkZ, bool surroundings);

	void chunkManager();

    void calculateLoadedChunks();
    void assignChunkID();

	void organiseChunks(int threadID);
    void putInVAOs();
    void generateChunks();

    void updateLight(int ChunkX, int ChunkY, int ChunkZ, int threadID);

	int buildChunk(int threadID);

    void blockInit();
};

#endif
