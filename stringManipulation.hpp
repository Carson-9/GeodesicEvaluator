#pragma once
#include <iostream>
#include <fstream>
#include <chrono>
#include <format>
#include <iostream>
#include <time.h>
#include <string>


void replaceCharInCharList(char* string, int stringSize, char to_replace, char replacement);
int getStringSize(const char string[]);
void addFileExtension(char* string, int allocatedChars, const char fileExtension[]);
void setFileFolder(char* to_format, int allocatedChars, const char folderName[]);
bool writeTableToFile(int sizeY, int sizeX, float* table, const char folderName[]);
int* getTableDimensionFromFile(char* filename);
void appendCharListToCharList(char* receiver, char* to_add, int size_receiver, int size_adder);
void convertStringLineToFloats(std::string readLine, float* readArray, int currentLine, int sizeX);
float* readFloatTableFromFile(char* filename, int sizeX, int sizeY);