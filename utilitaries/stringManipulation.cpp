#include "utilitaries/stringManipulation.hpp"

void replaceCharInCharList(char* string, int stringSize, char to_replace, char replacement) {
	for (int i = 0; i < stringSize; i++)
		if (string[i] == to_replace) string[i] = replacement;
}

void addFileExtension(char* string, int allocatedChars, const char fileExtension[]) {

	int fileExtensionSize = getStringSize(fileExtension);
	int indexToAdd = getStringSize(string) - 1;
	if (allocatedChars - indexToAdd < fileExtensionSize) {
		printf("addFileExtension -> Error, extension name too big for the given array!");
		return;
	}

	for (int i = 0; i < fileExtensionSize; i++) {
		string[indexToAdd + i] = fileExtension[i];
	}


}

void setFileFolder(char* to_format, int allocatedChars, const char folderName[]) {

	int folderNameSize = getStringSize(folderName);
	if (allocatedChars < folderNameSize) {
		printf("setFileFolder -> Error, folder name too big for the given array!");
		return;
	}
	// Also copy the sentinel for good measure
	for (int i = 0; i <= folderNameSize; i++) to_format[i] = folderName[i];
}

int getStringSize(const char string[]) {
	int count = 0;
	char currentChar = ' ';
	while (currentChar != '\0') {
		currentChar = string[count++];
	}
	return (count);
}

void appendCharListToCharList(char* receiver, char* to_add, int size_receiver, int indexToAdd) {
	
	int adderSize = getStringSize(to_add);
	
	if ((size_receiver - indexToAdd) < adderSize) {
		printf("Error, tried to add a string bigger than the receiver!\n");
		return;
	}
	for (int i = 0; i <= adderSize; i++) {
		receiver[indexToAdd + i] = to_add[i];
		if (to_add[i] == '\0') return;
	}

}



bool writeTableToFile(int sizeY, int sizeX, float* table, const char folderName[]) {

	try {

		auto now = std::chrono::system_clock::now();
		const std::time_t end_time = std::chrono::system_clock::to_time_t(now);

		char currentDate[64];
		char fileName[128];

		for (int i = 0; i < 127; i++) fileName[i] = ' ';
		fileName[127] = '\0';

		ctime_s(currentDate, 64 * sizeof(char), &end_time);

		replaceCharInCharList(currentDate, 64, ' ', '_');
		replaceCharInCharList(currentDate, 64, ':', '_');
		replaceCharInCharList(currentDate, 64, '\n', '\0');
		setFileFolder(fileName, 128, folderName);

		int folderNameSize = getStringSize(folderName);
		appendCharListToCharList(fileName, currentDate, 128, folderNameSize - 1);

		addFileExtension(fileName, 128, ".txt");

		std::ofstream newFile(fileName);

		printf("File : %s opened\n", fileName);

		if (newFile.is_open()) {

			newFile << sizeX << " " << sizeY << "\n";

			for (int y = 0; y < sizeY; y++) {
				for (int x = 0; x < sizeX; x++) {
					newFile << table[y * sizeX + x];
					newFile << " ";
				}
				newFile << "\n";
			}
			printf("Wrote to file\n");
			newFile.close();
			printf("File closed\n");
		}

		else printf("Tried to write while file was not opened.\n");
		return true;
	}


	catch (...) {
		printf("Could not print to file!");
		return false;
	}
}

int* getTableDimensionFromFile(const char* filename) {

	std::ifstream readFile(filename);

	if (!readFile) {
		printf("Error, could not open file with filename : %s", filename);
		return NULL;
	}

	int* dimensions = new int[2];
	readFile >> dimensions[0] >> dimensions[1];

	readFile.close();
	return dimensions;

}


float* readFloatTableFromFile(const char* filename, int sizeX, int sizeY) {
	
	std::ifstream readFile(filename);
	
	if (!readFile) {
		printf("Error, could not open file with filename : %s", filename);
		return NULL;
	}

	float* readArray = new float[sizeX * sizeY];
	std::string readLine;
	readFile >> readLine;
	readFile >> readLine;

	int currentLine = 0;

	for (int y = 0; y < sizeY; y++) {
		for (int x = 0; x < sizeX; x++) {
			readFile >> readLine;
			//printf("%s\n", readLine.c_str());
			readArray[y * sizeX + x] = std::atof(readLine.c_str());
		}
	}

	readFile.close();
	return readArray;
}