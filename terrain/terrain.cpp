#include "terrain/terrain.hpp"
#include "Path/path.hpp"
#include "Solver/solver.hpp"


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

	// Set RNG with EPOCH

	const auto GLOBAL_TIME = std::chrono::system_clock::now();
	srand(std::chrono::duration_cast<std::chrono::seconds>(GLOBAL_TIME.time_since_epoch()).count());

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

bool Terrain::isInScreen(Point* a) {
	return !(a->x < 0 || a->x >= this->getSizeX() || a->y < 0 || a->y >= this->getSizeY());
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


void logPoint(Point* p) {
	printf("Point : x = %f, y = %f, z = %f\n", p->x, p->y, p->z);
}


f32 Terrain::surfaceFunction(Point* p) {

	if (!this->isInScreen(p)) return 0.0f;

	if (this->heightMap == nullptr) this->generateTerrain();

	//logPoint(p);

	i32 round_x = (i32)floor(p->x);
	i32 round_y = (i32)floor(p->y);

	f32 float_x = p->x - round_x;
	f32 float_y = p->y - round_y;

	i32 coord = round_y * this->sizeX + round_x;

	if (float_x <= 0 && float_y <= 0) return this->heightMap[coord];

	f32 corner_A = this->heightMap[coord];
	f32 corner_B = this->heightMap[coord + this->sizeX];
	f32 corner_C = this->heightMap[coord + 1];
	f32 corner_D =this->heightMap[coord + this->sizeX + 1];

	if (float_x >= float_y) {

		f32 x_edgeZ = (1 - float_x) * corner_A + float_x * corner_C;
		f32 mid_edgeZ = (1 - float_x) * corner_A + float_x * corner_D;
		f32 y_percent = float_y / float_x;

		return (1 - y_percent) * x_edgeZ + y_percent * mid_edgeZ;
	}

	else {

		f32 y_edgeZ = (1 - float_y) * corner_A + float_y * corner_B;
		f32 mid_edgeZ = (1 - float_y) * corner_A + float_y * corner_D;
		f32 x_percent = float_x / float_y;

		return (1 - x_percent) * y_edgeZ + x_percent * mid_edgeZ;

	}
}


void Terrain::fillPoint(Point* p) {
	f32 height = surfaceFunction(p);;
	if (height < CUTTING_HEIGHT) p->z = height - DOWN_OFFSET;
	else p->z = height;
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
					
					if(this->heightMap[y * this->sizeX + x] < CUTTING_HEIGHT)
						buffer << "v " << std::to_string(y) << " " << std::to_string((heightScale * this->heightMap[y * this->sizeX + x]) - 500) << " " << std::to_string(x) << "\n";
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


void Terrain::setPoints(Point* a, Point* b) {
	this->pointA = a;
	this->pointB = b;
}

Point* Terrain::getPointA() {
	return this->pointA;
}

Point* Terrain::getPointB() {
	return this->pointB;
}

void Terrain::drawLine(Point* a, Point* b) {

	Point difference = *b - *a;
	f32 minCoord, maxCoord;


	if (difference.x == 0.0f) {
		if (difference.y >= 0) {
			minCoord = a->y;
			maxCoord = b->y;
		}

		else {
			minCoord = b->y;
			maxCoord = a->y;
		}

		for (int y = (int)floor(minCoord); y <= maxCoord; y++) {
			i32 coord = (y * this->sizeX + (i32)floor(b->x)) * 4;
			this->colorMap[coord] = PATH_COLOR.r;
			this->colorMap[coord + 1] = PATH_COLOR.g;
			this->colorMap[coord + 2] = PATH_COLOR.b;
			this->colorMap[coord + 3] = 255;
		}
	}

	else {
		f32 dy = difference.y / difference.x;

		if (difference.x >= 0) {
			minCoord = a->x;
			maxCoord = b->x;
		}

		else {
			minCoord = b->x;
			maxCoord = a->x;
		}

		f32 minY = ((a->x <= b->x) ? a->y : b->y);

		for (int x = (i32)floor(minCoord) + 1; x <= maxCoord; x++) {
			i32 coord = (((i32)floor(minY + dy * (x - minCoord))) * this->sizeX + x) * 4;
			this->colorMap[coord] = PATH_COLOR.r;
			this->colorMap[coord + 1] = PATH_COLOR.g;
			this->colorMap[coord + 2] = PATH_COLOR.b;
			this->colorMap[coord + 3] = 255;
		}
	}
	
 }

void Terrain::drawPath(void* p) {
	Path* np = (Path*)p;
	for (int i = 0; i < np->precision - 1; i++) this->drawLine(&np->points[i], &np->points[i + 1]);
}

void Terrain::generatePath() {
	fillPoint(this->pointA);
	fillPoint(this->pointB);
	Path* path = initializePath(PATH_PRECISION, *this->pointA, *this->pointB, this);
	if (!path) return;
	this->generateColorMap(); // RESET IN CASE OF PATH

	for (int point = 1; point < path->precision - 1; point++) {
		i32 coord = ((int)floor(path->points[point].y) * this->sizeX + (int)floor(path->points[point].x)) * 4;
		//printf("Point : %d, Coord : %d, Point.x = %f, Point.y = %f\n", point, coord, path->points[point].x, path->points[point].y);
		this->colorMap[coord] = PATH_COLOR.r;
		this->colorMap[coord + 1] = PATH_COLOR.g;
		this->colorMap[coord + 2] = PATH_COLOR.b;
		this->colorMap[coord + 3] = 255;	//ALPHA
	}
}

void Terrain::naivePath() {

	fillPoint(this->pointA);
	fillPoint(this->pointB);

	pointList pathList = makePointList();
	pathList->p = *this->pointB;
	pathList = pointListAddHead(pathList, *this->pointA);
	
	f64 pathLength = naive_optimisation(pathList, this);
	Path* path = emptyPath(this);
	path->precision = getPointListSize(pathList);
	path->points = pointListToArray(pathList);
	path->startingPoint = path->points[0];
	path->startingPoint = path->points[path->precision - 1];

	//printf("Precision : %d", path->precision);

	this->generateColorMap(); // RESET IN CASE OF PATH

	for (int point = 1; point < path->precision - 1; point++) {
		//printPoint(&path->points[point]);
		i32 coord = ((int)floor(path->points[point].y) * this->sizeX + (int)floor(path->points[point].x)) * 4;
		//printf("Point : %d, Coord : %d, Point.x = %f, Point.y = %f\n", point, coord, path->points[point].x, path->points[point].y);
		this->colorMap[coord] = PATH_COLOR.r;
		this->colorMap[coord + 1] = PATH_COLOR.g;
		this->colorMap[coord + 2] = PATH_COLOR.b;
		this->colorMap[coord + 3] = 255;	//ALPHA
	}

	/*for (int point = 0; point < path->precision - 1; point++) {
		printPoint(&path->points[point]);
		this->drawLine(&path->points[point], &path->points[point + 1]);
	}*/

	printf("Generated the Naïve solution! Total length : %f\n", pathLength);
}


void Terrain::drawBezier(bezier3 b) {
	fillPoint(this->pointA);
	fillPoint(this->pointB);

	Path* path = initializePath(PATH_PRECISION, *this->pointA, *this->pointB, this);

	for (int i = 1; i < PATH_PRECISION - 1; i++) {
		path->points[i] = b.evaluate((f32)((f32)i / (f32)PATH_PRECISION));
		this->fillPoint(&path->points[i]);
	}

	for (int point = 0; point < path->precision - 1; point++) {
		//printPoint(&path->points[point]);
		this->drawLine(&path->points[point], &path->points[point + 1]);
	}
}

void Terrain::bezierPath() {

	fillPoint(this->pointA);
	fillPoint(this->pointB);

	Path* path = initializePath(PATH_PRECISION, *this->pointA, *this->pointB, this);

	f64 pathLength = bezier_approximation(path, *this->pointA, *this->pointB, this);

	this->generateColorMap(); // RESET IN CASE OF PATH

	/*for (int point = 1; point < path->precision - 1; point++) {
		//printPoint(&path->points[point]);
		i32 coord = ((int)floor(path->points[point].y) * this->sizeX + (int)floor(path->points[point].x)) * 4;
		//printf("Point : %d, Coord : %d, Point.x = %f, Point.y = %f\n", point, coord, path->points[point].x, path->points[point].y);
		this->colorMap[coord] = PATH_COLOR.r;
		this->colorMap[coord + 1] = PATH_COLOR.g;
		this->colorMap[coord + 2] = PATH_COLOR.b;
		this->colorMap[coord + 3] = 255;	//ALPHA
	}*/

	for (int point = 0; point < path->precision - 1; point++) {
		//printPoint(&path->points[point]);
		this->drawLine(&path->points[point], &path->points[point + 1]);
	}

	printf("Generated a Bezier Approximation! Total length : %f\n", pathLength);
}

void Terrain::best_path() {
	fillPoint(this->pointA);
	fillPoint(this->pointB);

	this->generateColorMap(); // RESET IN CASE OF PATH

	Path* path = try_solution(this, *this->pointA, *this->pointB);

	if (path == nullptr) {
		printf("No solution were found :( \n");
		return;
	}

	this->generateColorMap(); // RESET IN CASE OF PATH
	f64 length = computePathLength(path);

	for (int point = 0; point < path->precision - 1; point++) {
		//printPoint(&path->points[point]);
		this->drawLine(&path->points[point], &path->points[point + 1]);
	}

	printf("Generated a solution! Total length : %f\n", length);

	
}