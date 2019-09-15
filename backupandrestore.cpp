//
// Created by Ajay Raj Nelapudi on 2019-09-15.
//

#include "backupandrestore.h"

#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <vector>
#include <map>
#include <string>
#include "diskutilities.h"
using namespace std;

FILESLOG filesLog;

bool areSameBlocks(char *blockA, char *blockB) {
    if (blockA == NULL || blockB == NULL) {
        return false;
    }

    for (int i = 0; i < BLOCK_SIZE; i++) {
        if (blockA[i] != blockB[i]) {
            return false;
        }
    }

    return true;
}

unsigned int getBlockCount(unsigned int timestamp, FILEDATA fileData) {
    unsigned int totalBlocks = 0;
    unsigned int currentTimestamp = timestamp;
    while (currentTimestamp > 0) {
        if (fileData.timestamp.find(currentTimestamp) == fileData.timestamp.end()) {
            currentTimestamp--;
            continue;
        }

        totalBlocks = fileData.timestamp.find(currentTimestamp)->second.totalBlocks;
        break;
    }

    return totalBlocks;
}

void buildFileSequence(
        unsigned int timestamp, FILEDATA fileData,
        unsigned int totalBlocks, bool *fileBlockFound, unsigned int *fileSequenceBlockIndex) {
    unsigned int currentTimestamp = timestamp;
    unsigned int blocksRestored = 0;

    // iterating from the current timestamp to the initial full backup to build the file sequence
    while (currentTimestamp > 0 && blocksRestored < totalBlocks) {
        if (fileData.timestamp.find(currentTimestamp) == fileData.timestamp.end()) {
            currentTimestamp--;
            continue;
        }

        vector<BACKUPINFO> backupInfoVector = fileData.timestamp.find(currentTimestamp)->second.backupInfo;
        for (int i = 0; i < backupInfoVector.size(); i++) {
            BACKUPINFO backupInfo = backupInfoVector[i];

            if (backupInfo.fileSequenceNumber >= totalBlocks || backupInfo.operationType == DELETE) {
                continue;
            }

            if (fileBlockFound[i]) {
                continue;
            }

            fileSequenceBlockIndex[backupInfo.fileSequenceNumber] = backupInfo.blockIndex;
            fileBlockFound[backupInfo.fileSequenceNumber] = true;
            blocksRestored++;
        }
        currentTimestamp--;
    }
}

unsigned int getLatestTimestamp(FILEDATA fileData) {
    unsigned int latestTimestamp = 0;

    for (map<int, TIMESTAMP>::iterator currentTimestamp = fileData.timestamp.begin();
         currentTimestamp != fileData.timestamp.end(); currentTimestamp++) {
        if (currentTimestamp->first > latestTimestamp) {
            latestTimestamp = currentTimestamp->first;
        }
    }

    return latestTimestamp;
}

bool runFullBackup(unsigned int timestamp, string filename, char *fileContent) {
    unsigned int availableSpace = getAvailableSpaceOnDisk();
    if (strlen(fileContent) > availableSpace) {
        return false;
    }

    FILEDATA fileData;

    // checking whether file is already backed-up or not
    if (filesLog.metadata.find(filename) != filesLog.metadata.end()) {
        fileData = filesLog.metadata.find(filename)->second;
        filesLog.metadata.erase(filename);
    }

    unsigned int fileSequenceNumber = 0;
    int fileSize = (int)strlen(fileContent);
    TIMESTAMP timestampData;

    for (unsigned int blockIndex = 0; blockIndex < NO_OF_BLOCKS && fileSize > 0; blockIndex++) {
        bool blockWriteStatus = writeBlock(
                blockIndex,
                fileContent + (fileSequenceNumber * BLOCK_SIZE),
                fileSize
        );
        if (!blockWriteStatus) {
            continue;
        }

        BACKUPINFO backupInfo = {fileSequenceNumber, blockIndex, APPEND};
        timestampData.backupInfo.push_back(backupInfo);

        fileSize -= BLOCK_SIZE;
        fileSequenceNumber++;
    }

    timestampData.totalBlocks = fileSequenceNumber;
    fileData.timestamp.insert(pair<int, TIMESTAMP>(timestamp, timestampData));
    filesLog.metadata.insert(pair<string, FILEDATA>(filename, fileData));

    return true;
}

void displayFileContents(string filename, unsigned int timestamp) {
    if (filesLog.metadata.find(filename) == filesLog.metadata.end()) {
        return;
    }

    unsigned int fileSequenceBlockIndex[NO_OF_BLOCKS];
    bool fileBlockFound[NO_OF_BLOCKS] = {0};

    // finding the no of blocks for that version of file
    FILEDATA fileData = filesLog.metadata.find(filename)->second;
    unsigned int totalBlocks = getBlockCount(timestamp, fileData);

    buildFileSequence(timestamp, fileData, totalBlocks, fileBlockFound, fileSequenceBlockIndex);

    char buffer[NO_OF_BLOCKS];
    for (unsigned int blockIndex = 0; blockIndex < NO_OF_BLOCKS; blockIndex++) {
        if (!fileBlockFound[blockIndex]) {
            continue;
        }

        readBlock(fileSequenceBlockIndex[blockIndex], buffer, BLOCK_SIZE);
        printf("%s", buffer);
    }
    printf("\n");
}
