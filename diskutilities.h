//
// Created by Ajay Raj Nelapudi on 2019-09-15.
//

#ifndef FILEBACKUPANDRESTORE_DISKUTILITIES_H
#define FILEBACKUPANDRESTORE_DISKUTILITIES_H

#include <vector>
using namespace std;

#define DISK "/Users/ajayraj/Documents/BackupAndRestore/BackupAndRestore/disk.hdd"
#define DISK_SIZE 1024
#define BLOCK_SIZE 8
#define NO_OF_BLOCKS (DISK_SIZE / BLOCK_SIZE)

void formatDisk();
unsigned int getAvailableSpaceOnDisk();
vector<unsigned int> getFreeDiskBlocks();
bool eraseBlock(unsigned int blockIndex);
bool readBlock(unsigned int blockIndex, char *buffer, unsigned int bufferSize);
bool writeBlock(unsigned int blockIndex, char *buffer, unsigned int bufferSize);


#endif //FILEBACKUPANDRESTORE_DISKUTILITIES_H
