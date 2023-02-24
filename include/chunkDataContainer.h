#ifndef CHUNK_DATA_CLASS_H
#define CHUNK_DATA_CLASS_H

#include <vector>
#include "VAO.h"

#define NUM_BLOCKS 5
#define RENDER_DISTANCE 24
#define CHUNK_SIZE 16

struct ChunkDataContainer {
	std::vector<short> chunkData;
	std::vector<unsigned short> lightData;
	VAO array;
	std::vector<short> mesh;
    std::vector<unsigned int> indices;
	int EBOsize = 0;
	int chunkID[3];
	float distance;
	bool inQueue = 0;
	bool emptyChunk = 1;
	bool unCompiledChunk = 1;
	bool unGeneratedChunk = 1;
	bool vaolck = 0;
	bool generatelck = 0;
	bool renderlck = 0;
	bool forUpdate = 0;
};

struct Blocks {
    std::string blockName;
	bool isSolid = 1;
};

#endif
