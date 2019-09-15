#include <iostream>
#include "backupandrestore.h"
using namespace std;

int main() {
    runFullBackup(1, "FILE1", "AJAY RAJ");
    runFullBackup(2, "FILE2", "RANDOM DATA");
    runFullBackup(3, "FILE1", "1234567890");

    displayFileContents("FILE1", 3);
    displayFileContents("FILE2", 2);
    displayFileContents("FILE1", 1);

    return 0;
}
