//
// Created by Ajay Raj Nelapudi on 2019-09-15.
//

#ifndef FILEBACKUPANDRESTORE_BACKUPANDRESTORE_H
#define FILEBACKUPANDRESTORE_BACKUPANDRESTORE_H

#include <vector>
#include <map>
using namespace std;

#define MODIFY 0
#define APPEND 1
#define DELETE 2

typedef struct BackupInfo {
    unsigned int fileSequenceNumber;
    unsigned int blockIndex;
    unsigned int operationType;
} BACKUPINFO;

typedef struct Timestamp {
    unsigned int totalBlocks;
    vector<BACKUPINFO> backupInfo;
} TIMESTAMP;

typedef struct FileData {
    map<int, TIMESTAMP> timestamp;
} FILEDATA;

typedef struct FilesLog {
    map<string, FILEDATA> metadata;
} FILESLOG;

/*
 * COMMVAULT ROUND 2 MISTAKE:
 * Instead of clearly defining the Data Structure as above,
 * I declared it as below, effecting the readability in the long run
 * map<string, map<int, pair<int, vector<pair<int, int>>>>>
 *
 */

bool runFullBackup(unsigned int timestamp, string filename, char *fileContent);
bool runIncrementalBackup(unsigned int timestamp, string filename, char *fileContent);
void displayFileContents(string filename, unsigned int timestamp);

#endif //FILEBACKUPANDRESTORE_BACKUPANDRESTORE_H
