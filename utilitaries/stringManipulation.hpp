#pragma once
#include <iostream>
#include <fstream>
#include <chrono>
#include <format>
#include <iostream>
#include <time.h>
#include <string>
#include <cstring>


void replaceCharInCharList(char* string, int stringSize, char to_replace, char replacement);
int getStringSize(const char string[]);
void addFileExtension(char* string, int allocatedChars, const char fileExtension[]);
void setFileFolder(char* to_format, int allocatedChars, const char folderName[]);
bool writeTableToFile(int sizeY, int sizeX, float* table, const char folderName[]);
int* getTableDimensionFromFile(const char* filename);
void appendCharListToCharList(char* receiver, char* to_add, int size_receiver, int indexToAdd);
float* readFloatTableFromFile(const char* filename, int sizeX, int sizeY);