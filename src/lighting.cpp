#include "../include/chunkList.h"

void ChunkList::updateLight(int ChunkX, int ChunkY, int ChunkZ, int threadID) {
    chunkWorldContainer[index[threadID]].lightData = generator.light;
    
}