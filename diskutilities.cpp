//
// Created by Ajay Raj Nelapudi on 2019-09-15.
//

#include "diskutilities.h"

#include <string>
#include <vector>
using namespace std;

bool allocatedBlock[NO_OF_BLOCKS];

void formatDisk() {
    // marking all blocks as unallocated
    for (int i = 0; i < NO_OF_BLOCKS; i++) {
        allocatedBlock[i] = false;
    }
}

unsigned int getAvailableSpaceOnDisk() {
    unsigned int availableSpace = 0;

    // counting allocated space
    for (unsigned int blockIndex = 0; blockIndex < NO_OF_BLOCKS; blockIndex++) {
        if (!allocatedBlock[blockIndex]) {
            availableSpace++;
        }
    }

    return availableSpace;
}

vector<unsigned int> getFreeDiskBlocks() {
    vector<unsigned int> freeDiskBlocks;

    for (unsigned int diskBlockIndex = 0; diskBlockIndex < NO_OF_BLOCKS; diskBlockIndex++) {
        if (!allocatedBlock[diskBlockIndex]) {
            freeDiskBlocks.push_back(diskBlockIndex);
        }
    }

    return freeDiskBlocks;
}

bool eraseBlock(unsigned int blockIndex) {
    if (blockIndex >= NO_OF_BLOCKS) {
        return false;
    }

    // marking block as un-allocated
    allocatedBlock[blockIndex] = false;
    return true;
}

bool readBlock(unsigned int blockIndex, char *buffer, unsigned int bufferSize) {
    // checking overflow condition
    if (blockIndex >= NO_OF_BLOCKS || bufferSize < BLOCK_SIZE) {
        return false;
    }

    // calculating offset to access the disk
    unsigned int diskOffset = blockIndex * BLOCK_SIZE;

    FILE *disk = fopen(DISK, "rb");
    if (disk == NULL) {
        return false;
    }

    // copying from disk to buffer
    fseek(disk, diskOffset, SEEK_SET);
    fread(buffer, sizeof(char), BLOCK_SIZE, disk);

    fclose(disk);
    return true;
}

bool writeBlock(unsigned int blockIndex, char *buffer, unsigned int bufferSize) {
    // checking overflow condition
    if (blockIndex >= NO_OF_BLOCKS) {
        return false;
    }

    if (allocatedBlock[blockIndex]) {
        return false;
    }

    // calculating offset to access the disk
    unsigned int diskOffset = blockIndex * BLOCK_SIZE;

    FILE *disk = fopen(DISK, "rb+");
    if (disk == NULL) {
        return false;
    }

    // copying from buffer to disk
    fseek(disk, diskOffset, SEEK_SET);
    fwrite(buffer, sizeof(char), BLOCK_SIZE, disk);

    // marking as allocated
    allocatedBlock[blockIndex] = true;

    fclose(disk);
    return true;
}
