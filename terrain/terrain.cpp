#include "terrain/terrain.hpp"

sf::Color GetCorrespondingColor(float height, float sigmoidBlend) {

	sf::Color blend;

	for (int i = 0; i < COLOR_NUMBER - 1; i++) {
		if (height < COLOR_HEIGHT_LIST[i + 1]) {
			float percent = (height - COLOR_HEIGHT_LIST[i]) / (COLOR_HEIGHT_LIST[i + 1] - COLOR_HEIGHT_LIST[i]);
			percent = sigmoid(percent, 1.0f, 0.5f, sigmoidBlend);
			//percent *= 1.5f;
			blend.r = (sf::Uint8)((1.0f - percent) * COLOR_LIST[i].r + percent * COLOR_LIST[i + 1].r);
			blend.g = (sf::Uint8)((1.0f - percent) * COLOR_LIST[i].g + percent * COLOR_LIST[i + 1].g);
			blend.b = (sf::Uint8)((1.0f - percent) * COLOR_LIST[i].b + percent * COLOR_LIST[i + 1].b);
			return blend;
			//percent *= 1.5f;
			//float color_interp = 0.5f + 1.5f * percent;
		}
	}

	return SNOW;
	
}

void colorTerrain(int sizeX, int sizeY, sf::Uint8* pixelArray, float sigmoidBlend) {

	sf::Color buffer;

	for (int y = 0; y < sizeY; y++) {
		for (int x = 0; x < sizeX; x++) {
			buffer = GetCorrespondingColor(pixelArray[(y * sizeY + x) * 4], sigmoidBlend);
			pixelArray[(y * sizeY + x) * 4] = buffer.r;
			pixelArray[(y * sizeY + x) * 4 + 1] = buffer.g;
			pixelArray[(y * sizeY + x) * 4 + 2] = buffer.b;
		}
	}
}

Terrain::Terrain(int sizeX, int sizeY, int octaves, float bias, float blend) {
	this->sizeX = sizeX;
	this->sizeY = sizeY;
	this->heightMap = new float[sizeX * sizeY];
	this->colorMap = nullptr;
	this->blend = blend;
	this->octaves = octaves;
	this->bias = bias;
	this->drawHeightIndicators = false;
	this->heightIndicatorList = new bool[256];
	for (int i = 0; i < 256; i++) this->heightIndicatorList[i] = false;
	this->blendResolKeyboard = 5.0f;
	this->generateTerrain();
}

Terrain::~Terrain() {
	if(this->colorMap != nullptr) delete this->colorMap;
	delete this->heightMap;
	delete this->heightIndicatorList;
}

void Terrain::generateTerrain() {
	generateHeightMap(this->sizeX, this->sizeY, this->octaves, this->bias, this->heightMap);
	if (this->colorMap != nullptr) {
		delete this->colorMap;
		this->colorMap = nullptr;
	}
}

float* Terrain::getHeightMap() {
	return this->heightMap;
}

sf::Uint8* Terrain::getColorMap() {
	return this->colorMap;
}

int Terrain::getSizeX() {
	return this->sizeX;
}

int Terrain::getSizeY() {
	return this->sizeY;
}

int Terrain::getOctaves() {
	return this->octaves;
}

float Terrain::getBias() {
	return this->bias;
}

float Terrain::getBlend() {
	return this->blend;
}

float Terrain::getBlendResKeyboard() {
	return this->blendResolKeyboard;
}

void Terrain::setOctaves(int newOctaves) {
	this->octaves = newOctaves;
}

void Terrain::setBias(float newBias) {
	this->bias = newBias;
}

void Terrain::setBlend(float newBlend) {
	this->blend = newBlend;
}

void Terrain::setBlendResKeyboard(float newBlendRes) {
	this->blendResolKeyboard = newBlendRes;
}

void Terrain::setIndicatorList(int* list, int size) {
	for (int i = 0; i < 256; i++) this->heightIndicatorList[i] = false;
	for (int i = 0; i < size; i++) {
		this->heightIndicatorList[list[i]] = true;
	} 
}

void Terrain::toggleIndicatorDraw() {
	this->drawHeightIndicators = !this->drawHeightIndicators;
}

void Terrain::setHeightMap(float* newMap, int sizeX, int sizeY) {
	this->sizeX = sizeX;
	this->sizeY = sizeY;
	if (this->heightMap != nullptr) delete[] this->heightMap;
	this->heightMap = new float[sizeX * sizeY];

	for (int y = 0; y < sizeY; y++)
		for (int x = 0; x < sizeX; x++)
			this->heightMap[y * sizeX + x] = newMap[y * sizeX + x];
}

void Terrain::generateColorMap() {
	if (this->colorMap == nullptr) this->colorMap = new sf::Uint8[this->sizeX * this->sizeY * 4];
	sf::Color buffer;

	for (int y = 0; y < this->sizeY; y++) {
		for (int x = 0; x < this->sizeX; x++) {
			if (this->drawHeightIndicators && this->heightIndicatorList[(int)this->heightMap[y * this->sizeX + x]]) {
				this->colorMap[(y * sizeX + x) * 4] = 0;
				this->colorMap[(y * sizeX + x) * 4 + 1] = 0;
				this->colorMap[(y * sizeX + x) * 4 + 2] = 0;
				this->colorMap[(y * sizeX + x) * 4 + 3] = 255;
			}
			else {
				buffer = GetCorrespondingColor(this->heightMap[y * this->sizeX + x], this->blend);
				this->colorMap[(y * sizeX + x) * 4] = buffer.r;
				this->colorMap[(y * sizeX + x) * 4 + 1] = buffer.g;
				this->colorMap[(y * sizeX + x) * 4 + 2] = buffer.b;
				this->colorMap[(y * sizeX + x) * 4 + 3] = 255;

			}

		}

	}
}


void Terrain::makeSprite(sf::Sprite sprite) {
	if (this->colorMap == nullptr) this->generateColorMap();
	this->img.create(this->sizeX, this->sizeY, this->colorMap);
	this->tex.loadFromImage(img);

	sprite.setTexture(tex);
}


void Terrain::generateFromFile(const char* fileName) {
	int* terrainDim = new int[2];
	terrainDim = getTableDimensionFromFile(fileName);
	
	this->sizeX = terrainDim[0];
	this->sizeY = terrainDim[1];
	if (this->heightMap != NULL) delete[] this->heightMap;
	this->heightMap = readFloatTableFromFile(fileName, terrainDim[0], terrainDim[1]);

	delete[] terrainDim;
} 


void Terrain::generateOBJFile(const char* folderName, float heightScale) {
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

		addFileExtension(fileName, 128, ".obj");

		std::ofstream newFile(fileName);

		printf("File : %s opened\n", fileName);

		std::stringstream buffer(std::stringstream::out | std::stringstream::binary);

		if (newFile.is_open()) {

			int completionPercent = 0;
			int previous_completionPercent = 0;

			// Writing the vertices

			std::cout << "Writing the Vertices : \n";

			for (int y = 0; y < sizeY; y++) {

				buffer.str(std::string());
				for (int x = 0; x < sizeX; x++) {
					
					if(this->heightMap[y * this->sizeX + x] < COLOR_HEIGHT_LIST[4])
						buffer << "v " << std::to_string(y) << " " << std::to_string((heightScale * this->heightMap[y * this->sizeX + x]) - 1000) << " " << std::to_string(x) << "\n";
					else
						buffer << "v " << std::to_string(y) << " " << std::to_string(heightScale * this->heightMap[y * this->sizeX + x]) << " " << std::to_string(x) << "\n";
				}

				completionPercent = (int) (((float)y / (float)sizeY) * 23.0f) + 1;
				newFile.write(buffer.str().c_str(), buffer.str().length());
				if (completionPercent > previous_completionPercent) {
					previous_completionPercent = completionPercent;
					std::cout << "\33[2K\r[" << std::string(completionPercent, 'X') << std::string(23 - completionPercent, '.') << "] - " << ((float)completionPercent / 23.0f) * 100.0f << "% Done!" << std::flush;
				}
			}

			std::cout << std::endl;
			std::cout << "Writing the Faces : \n";
			previous_completionPercent = 0;


			// Writing the faces

			for (int y = 0; y < sizeY - 1; y++) {
				buffer.str(std::string());
				for (int x = 0; x < sizeX - 1; x++) {
					buffer << "f " << std::to_string(y * (this->sizeX) + x + 1) << " " << std::to_string(y * (this->sizeX) + x + 2) << " " << std::to_string((y + 1) * (this->sizeX) + x + 1) << "\n";
					buffer << "f " << std::to_string(y * (this->sizeX) + x + 2) << " " << std::to_string((y+1) * (this->sizeX) + x + 2) << " " << std::to_string((y + 1) * (this->sizeX) + x + 1) << "\n";
				}

				completionPercent = (int)(((float)y / (float)sizeY) * 23.0f) + 1;

				newFile.write(buffer.str().c_str(), buffer.str().length());
				if (completionPercent > previous_completionPercent) {
					previous_completionPercent = completionPercent;
					std::cout << "\33[2K\r[" << std::string(completionPercent, 'X') << std::string(23 - completionPercent, '.') << "] - " << ((float)completionPercent / 23.0f) * 100.0f << "% Done!" << std::flush;
				}
			}

			std::cout << std::endl;

			printf("Wrote to file\n");
			newFile.close();
			printf("File closed\n");
		}

		else printf("Tried to write while file was not opened.\n");
		return;
	}


	catch (...) {
		printf("Could not print to file!");
		return;
	}
}