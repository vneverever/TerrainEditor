#include "Terrain.h"
#include "Manager.h"
#include <qthread.h>
#include "qmath.h"



Terrain::Terrain()
{
	
}

void Terrain::Init(Manager* manager)
{
	M = manager;
	calculatehMapIndex();
	for (int i = 0; i < 256*256; i++){
		heightTable[i] = i /(256.f*256.f);
	}
	setDataStructure(tSize);
	calculateGeometry();
	initGround();
	
	//t1.pause();
	threadBreak[0] = true;
	threadBreak[1] = true;
	threadBreak[2] = true;
	threadBreak[3] = true;
	threadBreak[4] = true;
	threads[0] = &Terrain::water;//QtConcurrent::run(this, &Terrain::water);
	threads[1] = &Terrain::volcano;
	threads[2] = &Terrain::diamondSquare;//QtConcurrent::run(this, &Terrain::water);
	threads[3] = &Terrain::fluid;//QtConcurrent::run(this, &Terrain::water);
	threads[4] = &Terrain::olsen;
}

int Terrain::getTSize(){
	return tSize;
}

void Terrain::setDataStructure(int size){
	//vertices.clear();
	//vertices.resize(size*size);
}

void Terrain::setScaleH(float f){
	if (f <= 0.0f) return;
	for (int i = 0; i < tSize*tSize; i++){
		//vertices[i].setY((vertices[i].y() / scaleH) * f);
	}
	scaleH = f;
	updateGround();
};

void Terrain::invert(){
	inverted = !inverted;
	for (int i = 0; i < tSize*tSize; i++){
		//vertices[i].setY((1 - (vertices[i].y() / scaleH)) * scaleH);
	}
	updateGround();
}
void Terrain::Render()
{
	
}
void Terrain::updateTerrain(QPoint start, QPoint end){
	if (inverted){
		for (int i = start.y(); i < end.y(); i++){
			for (int j = start.x(); j < end.x(); j++){
				//vertices[i*tSize + j].setY((1 - (heights[i*tSize + j] / 0xFFFF)) * scaleH);
			}
		}
	}
	else{
		for (int i = start.y(); i < end.y(); i++){
			for (int j = start.x(); j < end.x(); j++){
				//vertices[i*tSize + j].setY((heights[i*tSize + j] / 0xFFFF) * scaleH);
			}
		}
	}
	updateGround();

}
void Terrain::calculateGeometry(){
	QVector2D p1;
	QVector2D p2;
	QVector2D p3;
	QVector2D p4;

	QVector3D v1;
	QVector3D v2;
	QVector3D v3;

	QVector3D cross1;
	QVector3D cross2;



	int indexCounter=0;
	int indexCounter2 = 0;


	for (int i = 0; i < tSize; i++){
		for (int j = 0; j < tSize; j++){

			
			p1 = QVector2D(j *terrainScale, i * -terrainScale);
			vertices[i*tSize + j] = p1;

			texCoord[tSize*i+j] = QVector2D((float)(i+1.f)/32, (float)(j + 1.f) / 32);

			if(j+1 >= tSize || i+1 >=tSize)continue;
			if (indexRowLength == 0 && i == 1) indexRowLength = indexCounter;

			index[indexCounter++] = i*tSize + j;
			index[indexCounter++] = i*tSize + j+1;
			index[indexCounter++] = (i+1)*tSize + j;

			index[indexCounter++] = (i+1)*tSize + j;
			index[indexCounter++] = i*tSize + j+1;
			index[indexCounter++] = (i+1)*tSize + j+1;

			if (j % 2 == 0 || i % 2 == 0){
				indexTest[indexCounter2++] = i*tSize + j;
				indexTest[indexCounter2++] = i*tSize + j+1;
				indexTest[indexCounter2++] = (i+1)*tSize + j;

				indexTest[indexCounter2++] = (i+1)*tSize + j;
				indexTest[indexCounter2++] = i*tSize + j+1;
				indexTest[indexCounter2++] = (i+1)*tSize + j+1;
			}
			else{
				indexTest[indexCounter2++] = (i+1)*tSize + j;
				indexTest[indexCounter2++] = i*tSize + j;
				indexTest[indexCounter2++] = (i+1)*tSize + j+1;

				indexTest[indexCounter2++] = (i + 1)*tSize + j + 1;
				indexTest[indexCounter2++] = i*tSize + j;
				indexTest[indexCounter2++] = i*tSize + j+1;

			}

		}
	}

	indexCounter = 0;

	
	for (int i = 0; i < 255; i++){
		for (int j = 0; j < 255; j++){
			
			indexLower[indexCounter++] = (i*2)*tSize + j*2;

			if (j == 254) indexLower[indexCounter++] = (i * 2) * tSize + j * 2 + 3;
			else indexLower[indexCounter++] = (i * 2) * tSize + j * 2 + 2;

			if (i == 254)indexLower[indexCounter++] = (i * 2 + 3) * tSize + j * 2;
			else indexLower[indexCounter++] = (i * 2 + 2) * tSize + j * 2;


			if (i == 254)indexLower[indexCounter++] = (i * 2 + 3) * tSize + j * 2;
			else indexLower[indexCounter++] = (i * 2 + 2) * tSize + j * 2;

			if (j == 254) indexLower[indexCounter++] = (i * 2) * tSize + j * 2 + 3;
			else indexLower[indexCounter++] = (i * 2) * tSize + j * 2 + 2;

			if (j == 254) {
				if (i == 254) indexLower[indexCounter++] = (i * 2 + 3) * tSize + j * 2 + 3;
				else indexLower[indexCounter++] = (i * 2 + 2) * tSize + j * 2 + 3;

			}
			else{
				if (i == 254) indexLower[indexCounter++] = (i * 2 + 3) * tSize + j * 2 + 2;
				else indexLower[indexCounter++] = (i * 2 + 2) * tSize + j * 2 + 2;

			}
		}
	}

	indexCounter = 0;
	for (int i = 0; i < 127; i++){
		for (int j = 0; j < 127; j++){
			
			indexLowest[indexCounter++] = (i * 4)*tSize + j * 4;
			indexLowest[indexCounter++] = (i * 4)*tSize + j * 4 + 4;
			indexLowest[indexCounter++] = (i * 4 + 4)*tSize + j * 4;

			indexLowest[indexCounter++] = (i * 4 + 4)*tSize + j * 4;
			indexLowest[indexCounter++] = (i * 4)*tSize + j * 4 + 4;
			indexLowest[indexCounter++] = (i * 4 + 4)*tSize + j * 4 + 4;

		}
	}
}

QElapsedTimer timer;

void Terrain::test(){

}

void Terrain::avgHeights(){
	//timer.start();
	uint h[9];
	//printf("START: %dms\n", timer.elapsed());
	QImage copy = M->PW->hImage.copy();
	uint* buffer_copy = (uint*)copy.bits();
	uint* buffer = (uint*)M->PW->hImage.bits();

	//printf("After copy avg verts: %dms\n", timer.elapsed());
	for (int i = 0; i < tSize; i++){
		for (int j = 0; j < tSize; j++){
			int temp = i*tSize+j;
			int tempAdd = temp + tSize;
			int tempSub = temp - tSize;
			if (i < tSize - 1){
				if(j>0)	h[6] = buffer_copy[tempAdd - 1] & 0xFFFF;
				h[7] = buffer_copy[temp+tSize] & 0xFFFF;
				if (j< tSize - 1)h[8] = buffer_copy[tempAdd + 1] & 0xFFFF;
			}
			if (j>0)h[3] = buffer_copy[temp - 1] & 0xFFFF;
			
			h[4] = buffer_copy[temp] & 0xFFFF;
			h[5] = buffer_copy[temp + 1] & 0xFFFF;
			
			if (i > 0){
				if (j > 0)
					h[0] = buffer_copy[tempSub - 1] & 0xFFFF;
				h[1] = buffer_copy[tempSub] & 0xFFFF;
				h[2] = buffer_copy[tempSub + 1] & 0xFFFF;
			}


			if (i > 0 && i + 1 <tSize && j>0 && j + 1 < tSize) buffer[temp] = (h[0] + h[1] + h[2] + h[3] + h[4] + h[5] + h[6] + h[7] + h[8]) / 9;

			else if (i > 0 && i + 1 <tSize && j == 0) buffer[temp] = (h[1] + h[2] + h[4] + h[5] + h[7] + h[8]) / 6;
			else if (i > 0 && i + 1 <tSize && j + 1 == tSize) buffer[temp]= (h[0] + h[1] + h[3] + h[4] + h[6] + h[7]) / 6;

			else if (i == 0 && j == 0) buffer[temp] = (h[4] + h[5] + h[7] + h[8]) / 4;
			else if (i == 0 && j > 0 && j + 1 < tSize) buffer[temp] = (h[3] + h[4] + h[5] + h[6] + h[7] + h[8]) / 6;
			else if (i == 0 && j + 1 == tSize) buffer[temp] = (h[3] + h[4] + h[6] + h[7]) / 4;

			else if (i + 1 == tSize && j == 0) buffer[temp] = (h[4] + h[5] + h[1] + h[2]) / 4;
			else if (i + 1 == tSize && j > 0 && j + 1 < tSize) buffer[temp] = (h[3] + h[4] + h[5] + h[0] + h[1] + h[2]) / 6;
			else if (i + 1 == tSize && j + 1 == tSize) buffer[temp] = (h[3] + h[4] + h[0] + h[1]) / 4;
			buffer[temp] = 0xFF000000 | buffer[temp];
		}
	}
	//printf("After Averaging: %dms\n", timer.elapsed());
	M->updateHeightsToImage();
	//printf("After Update Heights: %dms\n", timer.elapsed());
	//updateGround();
	//printf("After update ground: %dms\n", timer.elapsed());
}


void Terrain::avgHeights(int x, int y){
	uint h[9];
	//printf("START: %dms\n", timer.elapsed());
	uint* buffer = (uint*)M->PW->hImage.bits();

	//printf("After copy avg verts: %dms\n", timer.elapsed());
			int temp = y*tSize + x;
			int tempAdd = temp + tSize;
			int tempSub = temp - tSize;
			if (y < tSize - 1){
				if (x>0)	h[6] = buffer[tempAdd - 1] & 0xFFFF;
				h[7] = buffer[temp + tSize] & 0xFFFF;
				if (x< tSize - 1)h[8] = buffer[tempAdd + 1] & 0xFFFF;
			}
			if (x>0)h[3] = buffer[temp - 1] & 0xFFFF;

			h[4] = buffer[temp] & 0xFFFF;
			h[5] = buffer[temp + 1] & 0xFFFF;

			if (y > 0){
				if (x > 0)
					h[0] = buffer[tempSub - 1] & 0xFFFF;
				h[1] = buffer[tempSub] & 0xFFFF;
				h[2] = buffer[tempSub + 1] & 0xFFFF;
			}


			if (y > 0 && y + 1 <tSize && x>0 && x + 1 < tSize) buffer[temp] = (h[0] + h[1] + h[2] + h[3] + h[4] + h[5] + h[6] + h[7] + h[8]) / 9;

			else if (y > 0 && y + 1 <tSize && x == 0) buffer[temp] = (h[1] + h[2] + h[4] + h[5] + h[7] + h[8]) / 6;
			else if (y > 0 && y + 1 <tSize && x + 1 == tSize) buffer[temp] = (h[0] + h[1] + h[3] + h[4] + h[6] + h[7]) / 6;

			else if (y == 0 && x == 0) buffer[temp] = (h[4] + h[5] + h[7] + h[8]) / 4;
			else if (y == 0 && x > 0 && x + 1 < tSize) buffer[temp] = (h[3] + h[4] + h[5] + h[6] + h[7] + h[8]) / 6;
			else if (y == 0 && x + 1 == tSize) buffer[temp] = (h[3] + h[4] + h[6] + h[7]) / 4;

			else if (y + 1 == tSize && x == 0) buffer[temp] = (h[4] + h[5] + h[1] + h[2]) / 4;
			else if (y + 1 == tSize && x > 0 && x + 1 < tSize) buffer[temp] = (h[3] + h[4] + h[5] + h[0] + h[1] + h[2]) / 6;
			else if (y + 1 == tSize && x + 1 == tSize) buffer[temp] = (h[3] + h[4] + h[0] + h[1]) / 4;
			buffer[temp] = 0xFF000000 | buffer[temp];
	//printf("After Averaging: %dms\n", timer.elapsed());
	M->updateHeightsToImage();
	//printf("After Update Heights: %dms\n", timer.elapsed());
	//updateGround();
	//printf("After update ground: %dms\n", timer.elapsed());
}



void Terrain::calculatehMapIndex(){
	for (int i = 0; i < tSize; i++){
		for (int j = 0; j < tSize; j++){
			hMapIndex[i*tSize + j] = QVector2D(j, i);
		}
	}
}

void Terrain::initGround(){
	int counter = 0;
	int indexCounter = 0;
		for (int j = 0; j < tSize-1; j++){
			verticesGround[counter++] = QVector2D(j *terrainScale, 0);
			verticesGround[counter++] = vertices[j];

			
		}
		for (int j = 0; j < tSize; j++){
			verticesGround[counter++] = QVector2D((tSize-1)*terrainScale, -j*terrainScale);
			verticesGround[counter++] = vertices[tSize-1+j*tSize];

		}
		
		for (int j = tSize-2; j >=0; j--){
			verticesGround[counter++] = QVector2D(j *terrainScale, -(tSize-1)*terrainScale);
			verticesGround[counter++] = vertices[(tSize)*(tSize-1)+j];
		}
		for (int j = tSize-2; j >= 1; j--){
			verticesGround[counter++] = QVector2D(0, -j*terrainScale);
			verticesGround[counter++] = vertices[j*tSize];
		}

		
		

		int offset = 0;
		for (int j = 0; j < (tSize-1)*4; j++){
			offset = j * 2;

			indexGround[indexCounter++] = 0 + offset;
			indexGround[indexCounter++] = (2 + offset)%8176;
			indexGround[indexCounter++] = 1 + offset;

			indexGround[indexCounter++] = (2 + offset) % 8176;
			indexGround[indexCounter++] = (3 + offset) % 8176;
			indexGround[indexCounter++] = 1 + offset;

		}

		indexGround[indexCounter++] = 0;
		indexGround[indexCounter++] = (tSize-1)*2;
		indexGround[indexCounter++] = (tSize-1)*4;
		
		indexGround[indexCounter++] = (tSize - 1) * 4;
		indexGround[indexCounter++] = (tSize - 1) * 6;
		indexGround[indexCounter++] = 0;		

		indexCounter = 0;
		for (int j = 0; j < 255 * 4+2; j++){
			offset = j * 4;
			indexGroundLower[indexCounter++] = 0 + offset;
			indexGroundLower[indexCounter++] = (4 + offset) % 8176;
			indexGroundLower[indexCounter++] = 1 + offset;

			indexGroundLower[indexCounter++] = 1 + offset;
			indexGroundLower[indexCounter++] = (4 + offset) % 8176;
			indexGroundLower[indexCounter++] = (5 + offset) % 8176;

		}
		indexGroundLower[indexCounter++] = 0;
		indexGroundLower[indexCounter++] = (tSize - 1) * 2;
		indexGroundLower[indexCounter++] = (tSize - 1) * 4;

		indexGroundLower[indexCounter++] = (tSize - 1) * 4;
		indexGroundLower[indexCounter++] = (tSize - 1) * 6;
		indexGroundLower[indexCounter++] = 0;


}

void Terrain::updateGround(){
	int counter = 1;
	for (int j = 0; j < tSize - 1; j++){
		
		verticesGround[counter] = vertices[j];
		counter += 2;
	}
	for (int j = 0; j < tSize; j++){
		
		verticesGround[counter] = vertices[(tSize - 1) + j*tSize];
		counter += 2;
	}

	for (int j = tSize-2; j >= 0; j--){
		
		verticesGround[counter] = vertices[(tSize)*(tSize - 1) + j];
		counter += 2;
	}

	for (int j = tSize-2; j >= 1; j--){
		
		verticesGround[counter] = vertices[j*tSize];
		counter += 2;
	}



}

void Terrain::water(){
	
	buf_ = (uint*)M->PW->hImage.bits();
	int *xVel = new int[tSize*tSize];
	int *yVel = new int[tSize*tSize];
	int a = 5;

	for (int i = 0; i < tSize * tSize; i++){
		xVel[i] = 0;
		yVel[i] = 0;
	}
	while (!threadBreak[0]){
		//timer.start();

		//printf("START: %dms\n", timer.elapsed());

		

		//printf("After copy avg verts: %dms\n", timer.elapsed());
		for (int y = 0; y < tSize-1; y++){
			int index = y * tSize;
			for (int x = 0; x < tSize-1; x++){
				int index2 = index + x;
				int indexY = index2;
				int indexX = index2 + 1;
				//int cell = M->tra->cellIndex(x, y);
				//printf("index1: %d\n", (int)(buffer[index] & 0xFFFF) - (int)(buffer[indexX] & 0xFFFF));
				//printf("index2: %d\n", (int)buffer[index] & 0xFFFF - (int)buffer[indexX] & 0xFFFF);
				//printf("index3: %d\n", int(buffer[index]- buffer[indexX]));
				//int force_x = int(buffer[index] - buffer[indexX]);
				//int force_y = int(buffer[index] - buffer[indexY]);
				int force_x = int(buf_[index2] - buf_[indexX]);
				int force_y = int(buf_[index2] - buf_[indexY]);
				xVel[index2] += force_x/a;
				xVel[indexX] += force_x/a;
				yVel[index2] += force_y/a;
				yVel[indexY] += force_y/a;
				//printf("xVel: %d\n", xVel[cell]);
			}
		}
		//printf("After First: %dms\n", timer.elapsed());
		for (int y = 0; y < tSize-1; y++){
			int index = y * tSize;
			for (int x = 0; x < tSize - 1; x++){
				int index2 = index + x;
				//int cell = M->tra->cellIndex(x, y);
				
				if (true){
					increaseValue(x + xVel[index2], y + yVel[index2], 100);
					decreaseValue(x, y, 100);
				}
				else
				{
					decreaseValue(x, y, std::abs(xVel[index2] + yVel[index2]));
				}
				xVel[index2] = 0;
				yVel[index2] = 0;
			}
		}
		//printf("After 2nd: %dms\n", timer.elapsed());
		//printf("After Averaging: %dms\n", timer.elapsed());
		M->updateHeightsToImage();
		//printf("After Update Heights: %dms\n", timer.elapsed());
		//Sleep((uint)10);
	}
	delete(xVel);
	delete(yVel);
	//updateGround();
	//printf("After update ground: %dms\n", timer.elapsed());
}

void Terrain::volcano(){
	int counter = 0;
	std::uniform_int_distribution<uint> unif((tSize * 500), (tSize * 600));
	std::mt19937 re;
	buf_ = (uint*)M->PW->hImage.bits();
	while (!threadBreak[1]){
		//uint* buf_ = (uint*)M->PW->hImage.bits();

		
		//if (r > tSize * (M->ter->getTSize() - 1) || r % (M->ter->getTSize() - 1) == 0) r -= tSize;
		//int index = r%(500*500);
		//r = rand() + 250 + 250 * 250;
		uint index = unif(re);
		if (index < tSize) index += tSize;
		if (index % (M->ter->getTSize() - 1) == 0)index += 1;
		if (index % 1022 == 0) index -=1;
		while (index % tSize < 510) index += 50;
		while (index % tSize > 590) index -= 50;
		
		//buf_[r%r2] = 0xFF00FFFF & (250 + buf_[r%r2]);
		//unsigned int current = buf_[index];
		//int dif = 10;
		/*for (int i = 0; i < 3; i++){
			for (int j = 0; j < 3; j++){
				if (buf_)
			}
		}
		if (buf_[index - 1]		> current + dif)	buf_[index + 1]			= 0xFF00FFFF & (dif + buf_[index + 1]);
		else if (buf_[index + 1]		> current + dif)buf_[index + 1]		= 0xFF00FFFF & (dif + buf_[index + 1]);
		else if (buf_[index + tSize]	> current + dif)buf_[index + tSize]	= 0xFF00FFFF & (dif + buf_[index + tSize]);
		else if (buf_[index - tSize]	> current + dif)buf_[index - tSize]	= 0xFF00FFFF & (dif + buf_[index - tSize]);
		else buf_[index] = 0xFF00FFFF & (dif + buf_[index]);
		*/
		//printf("NEW");
		//mtx.lock();
		//printf("index");
		volcanoRec(index%tSize,index/tSize,0);
		//mtx.unlock();

		//printf("WTF");
		//printf("RAND: %d\n", r%r2);
		//counter++;
		//if (counter == 0){
			//avgHeights(index%(M->ter->getTSize() - 1), index/(M->ter->getTSize() - 1));
			//avgHeights((index+1) % (M->ter->getTSize() - 1), index / (M->ter->getTSize() - 1));
			//avgHeights((index-1) % (M->ter->getTSize() - 1), index / (M->ter->getTSize() - 1));
			//avgHeights(index % (M->ter->getTSize() - 1), (index + 1) / (M->ter->getTSize() - 1));
			//avgHeights(index % (M->ter->getTSize() - 1), (index-1) / (M->ter->getTSize() - 1));
		//	counter = 0;
		//}
		//Sleep(1);
		M->refreshHmap = true;
		
	}
}

void Terrain::volcanoRec(int x, int y, float velocity){
	static const int xVal[8] = { 1,	 1,  0, -1,-1, -1,  0,  1 };
	static const int yVal[8] = { 0, -1, -1, -1, 0,  1,  1,  1 };
	int dif,toPlace;
	int initDif = 50;
	int heightDif[8];
	int maxVal = -1000;
	int position;
	QList<int> tempPos;
	int initValue = 200;
	int direction = -1;
	toPlace = initValue;
	Transform *helper = M->tra;

	while (!threadBreak[1]){
		
		if (helper->getValue(x,y) == 0xFFFF){
			break;
		}

		if (x == 0 || y == 0 || x == (M->ter->getTSize() - 1) || y == (M->ter->getTSize() - 1)){
			break;
		}
		dif = initDif;
		//if (current < dif) {
		//	uint result = 0xFF000000 | (toPlace/16 + current);
		//	if (result > 0xFF00FFFF) result = 0xFF00FFFF;
		//	buf_[index] = result;
		//	return;
		//}

		//printf("HEX: %X", (uint)(0.0001f*0xFFFFFFFF));
		
		
		maxVal = -toPlace;
		position = -1;
		int tempVel = 0;


		float hl, hr, hu, hd, c;

		hr = helper->getValue(x + 1, y) / 0xFFFF;
		hl = helper->getValue(x - 1, y) / 0xFFFF;
		hu = helper->getValue(x, y + 1) / 0xFFFF;
		hd = helper->getValue(x, y - 1) / 0xFFFF;
		c = helper->getValue(x, y) / 0xFFFF;



		for (int i = 0; i < 8; i++){
			heightDif[i] = helper->getValue(x, y) - helper->getValue(x + xVal[i], y + yVal[i]);
			if (i == direction) dif = toPlace - (velocity * toPlace)/2;
			else if (i == (direction + 7) % 8 || i == (direction + 1) % 8){
				dif = toPlace - (velocity * (toPlace))*10;
			}
			else dif = toPlace;
			//if (i % 2 == 1) dif *= 1.4f;
			if (heightDif[i] > dif * 5 && heightDif[i] - dif*5>= maxVal){
				if (heightDif[i]> maxVal){
					tempPos.clear();
					tempPos.append(i);
				}
				else tempPos.append(i);
				maxVal = heightDif[i] - dif*5;
				position = i;
			}
		}
		
		if (tempPos.size() > 0){
			position = tempPos.at(qrand() % tempPos.size());
			//printf("SIZE %d", tempPos.size());
			//if (heightDif[position] > 4000) velocity += 100;
			//else if (heightDif[position] > 2000) velocity += 300;
			//else if (heightDif[position] > 1200) velocity += 200;
			if (heightDif[position] > 30) velocity += .1f;
			else if (heightDif[position] < 30) velocity -= .2f;
			//else if (heightDif[position] > toPlace) velocity += 0.01f;
			//else if (heightDif[position] > toPlace/2) velocity += 0.05f;
			//else if (heightDif[position] > 30) velocity += 10;
			//else if (heightDif[position] > 0) velocity -= 0.01f;
			//else if (heightDif[position] > -toPlace / 8 - 20) velocity -= 0.02f;
			//else if (heightDif[position] > -toPlace / 4 - 20) velocity -= 0.05f;
			//else if (heightDif[position] > -toPlace / 2 - 20) velocity -= 0.2f;
			//else if (heightDif[position] > -toPlace*2 - 20) velocity -= 0.4f;
			//else if (heightDif[position] >= -toPlace/16 - 100) velocity -= 10;
			//else if (heightDif[position] >= -toPlace / 8 - 10) velocity -= 20;
			//else if (heightDif[position] > -toPlace / 4 - 10) velocity -= 100;
			//else if (heightDif[position] > -toPlace / 2 - 10) velocity -= 500;
			//else if (heightDif[position] > -toPlace - 10) velocity -= -2000;

			if (velocity > 5.f) velocity = 5.f;
			else if (velocity < 0) velocity = 0.f;
			//printf("velocity %f\n",velocity);
			//velocity = std::min(velocity + std::min(std::max(heightDif[position]-5, -0x1FF), 0xFF), 0x3FFF);// + maxVal/4;
			//printf("HeightDif: %d\n", heightDif[position]);
			//printf("velocity: %d\n", velocity);
			//printf("index: %d\n", index);
			if (position != -1){
				
				if (velocity < 0.2f){
					//int addVal = 0;
					//int addValMid = 1;
					//if (toPlace >= addVal * 4 + addValMid){
					//	helper->increaseValue(x, y, addValMid);
						//helper->increaseValue(x + 1, y, addVal);
						//helper->increaseValue(x - 1, y, addVal);
						//helper->increaseValue(x, y + 1, addVal);
						//helper->increaseValue(x, y - 1, addVal);

					//	toPlace -= addValMid + 4 * addVal;
					//}
					//else{
						//helper->increaseValue(x, y, toPlace);
					//	break;
					//}
				}
				else if (velocity > 0.6f){
					int removeVal = 4;
					int removeValMid = 2;
					//if (velocity > 0x3FF) removeVal = 5;
					if (toPlace + removeVal * 3 + removeValMid < 0x0FFF){
						//helper->decreaseValue(x, y, removeValMid);
						/*
						helper->decreaseValue(x + 1, y + 1, removeVal);
						helper->decreaseValue(x + 1, y - 1, removeVal);
						helper->decreaseValue(x + 1, y    , removeVal);
						helper->decreaseValue(x - 1, y + 1, removeVal);
						helper->decreaseValue(x - 1, y - 1, removeVal);
						helper->decreaseValue(x - 1, y    , removeVal);
						helper->decreaseValue(x    , y + 1, removeVal);
						helper->decreaseValue(x    , y - 1, removeVal);
						*/
						//printf("direction: %d\n", direction);
						//if (heightDif[direction + 2]);
						//for (int i = 0; i < 3; i++){
						//	int tempDir = (direction + i + 3) % 8;
						//	int tempHeight = helper->getValue(x, y);
						//	//int neighbourHeight = helper->getValue(x + tempDir, y + tempDir);
						//	if (heightDif[tempDir] < 0){
						//		helper->decreaseValue(x + tempDir, y + tempDir, removeVal);
								//printf("HI");
						//	}
						//}
						/*helper->decreaseValue(x + xVal[(direction + 3) % 8], y + yVal[(direction + 3) % 8], removeVal*2);
						helper->decreaseValue(x + xVal[(direction + 5) % 8], y + yVal[(direction + 5) % 8], removeVal*2);
						helper->decreaseValue(x + xVal[(direction + 2) % 8], y + yVal[(direction + 2) % 8], removeVal);
						helper->decreaseValue(x + xVal[(direction + 6) % 8], y + yVal[(direction + 6) % 8], removeVal);
						//helper->decreaseValue(x + xVal[(direction + 1) % 8], y + yVal[(direction + 1) % 8], removeVal);
						//helper->decreaseValue(x + xVal[(direction + 7) % 8], y + yVal[(direction + 7) % 8], removeVal);
						helper->decreaseValue(x + xVal[(direction + 4) % 8], y + yVal[(direction + 4) % 8], removeVal*2);
						//helper->decreaseValue(x + xVal[direction + 1], y + yVal[direction + 2], removeVal);
						//helper->decreaseValue(x + xVal[direction - 2], y + yVal[direction - 2], removeVal);
						//printf("Mod: %d", (position +7 1) % 8);
						*/
						//toPlace += removeValMid + 3 * removeVal;
					}
				}
				direction = position;
			}
		}
		tempPos.clear();
		
		if (position == -1){
			direction = -1;
			helper->increaseValue(x, y, toPlace);

			uint result = (toPlace / 32);
			for (int i = 0; i < 8; i++){
				//helper->increaseValue(x + xVal[i], y + yVal[i], 0);
			}
			toPlace = initValue;
			break;
		}
		else{
			x += xVal[position];
			y += yVal[position];
		}
			
		
		
	}
}

void Terrain::diamondSquare(){
	while (!threadBreak[2]) {
		int size = 1025;

		M->tra->diamondSquare(size);
		M->refreshHmap = true;
		Sleep(2000);
		
	}
}

void Terrain::olsen() {
	while (!threadBreak[3]) {
		int size = 1025;

		M->tra->diamondSquare(size);
		M->refreshHmap = true;
		Sleep(2000);

	}
}

void Terrain::toggle(int id){
			bool toBreak = !threadBreak[id];
			threadBreak[id] = toBreak;
			if (!toBreak){
				QtConcurrent::run(this, threads[id]);
			}
}

void Terrain::fluid(){
	//QElapsedTimer t;
	buf_ = (uint*)M->PW->hImage.bits();
	for (int i = 1; i < tSize * tSize-1; i++){
		M->tra->dens[i] = (buf_[i] & 0xFFFF) / (float)0xFFFF;
		M->tra->dens_prev[i] = (buf_[i] & 0xFFFF) / (float)0xFFFF;
	}
	while (!threadBreak[3]){
		//t.start();
		//printf("Start %d\n", t.elapsed());
		M->tra->fluid();
		//printf("ELAPSED %d\n", t.elapsed());
	}
}

/*DECREASE
void Terrain::volcanoRec(int index){
	uint* buf_ = (uint*)M->PW->hImage.bits();
	uint current = 0xFFFF & buf_[index];
	uint dif = 0x00000400;
	uint toPlace = 0x00000020;
	//printf("Index: %d\n", index);
	if (index < tSize){
		index += tSize;
		//buf_[index] = 0xFF00FFFF & (toPlace + current);
		return;
	}
	if (index % tSize == 0){
		index += 1;
		//buf_[index] = 0xFF00FFFF & (toPlace + current);
		return;
	}
	if (index % (M->ter->getTSize() - 1) == 0){
		index -= 1;
		//buf_[index] = 0xFF00FFFF & (toPlace + current);
		return;
	}
	if (index > tSize * (M->ter->getTSize() - 1)){
		index -= tSize;
		//buf_[index] = 0xFF00FFFF & (toPlace + current);
		return;
	}

	if (current < dif) {
		uint result = 0xFF000000 | (toPlace + current);
		if (result > 0xFF00FFFF) result = 0xFF00FFFF;
		buf_[index] = result;
		return;
	}


	uint h[8];
	h[0] = buf_[index - tSize - 1] & 0xFFFF;
	h[1] = buf_[index - tSize] & 0xFFFF;
	h[2] = buf_[index - tSize + 1] & 0xFFFF;
	h[3] = buf_[index + 1] & 0xFFFF;
	h[4] = buf_[index + tSize + 1] & 0xFFFF;
	h[5] = buf_[index + tSize] & 0xFFFF;
	h[6] = buf_[index + tSize - 1] & 0xFFFF;
	h[7] = buf_[index - 1] & 0xFFFF;
	uint heightDif[8];
	uint maxVal = 0x00000000;
	int position = -1;
	for (int i = 0; i < 8; i++){
		//printf("Current: %X\n", current);
		//printf("H: %X\n", h[i]);
		//printf("HeighDif: %X\n", heightDif[i]);
		heightDif[i] = current - h[i];
		//printf("HeighDif: %X\n", heightDif[i]);
		//printf("Dif: %X\n", dif);
		//printf("HeighDif: %X\n", heightDif[i]);
		if (heightDif[i] > dif && heightDif[i] <= 0xFFFF && heightDif[i] > maxVal){
			maxVal = heightDif[i];
			position = i;
			//printf("POSITION: %d\n", position);
		}
	}
	//printf("maxVal: %X\n", maxVal);
	//printf("heightDif: %X\n", heightDif[position]);
	if (position == -1){
		uint result = 0xFF000000 | (toPlace + current);
		if (result > 0xFF00FFFF) result = 0xFF00FFFF;
		buf_[index] = result;

		result = 0xFF000000 | (toPlace / 4 + current);
		if (result > 0xFF00FFFF) result = 0xFF00FFFF;
		buf_[index + 1] = result;
		buf_[index - 1] = result;
		buf_[index + tSize] = result;
		buf_[index - tSize] = result;

		result = 0xFF000000 | (toPlace / 8 + current);
		if (result > 0xFF00FFFF) result = 0xFF00FFFF;
		buf_[index + 1 + 1024] = result;
		buf_[index + 1 - 1024] = result;
		buf_[index - 1 + 1024] = result;
		buf_[index - 1 - 1024] = result;
		//printf("POSITION: %X\n", 0xFF000000 | (toPlace + current));
	}
	else if (position == 0){
		//printf("POSITION: %d\n", position);
		volcanoRec(index - 1024 - 1);
	}
	else if (position == 1){
		//printf("POSITION: %d\n", position);
		volcanoRec(index - 1024);
	}
	else if (position == 2){
		//printf("POSITION: %d\n", position);
		volcanoRec(index - 1024 + 1);
	}
	else if (position == 3){
		//printf("POSITION: %d\n", position);
		volcanoRec(index + 1);
	}
	else if (position == 4){
		//printf("POSITION: %d\n", position);
		volcanoRec(index + 1 + 1024);
	}
	else if (position == 5){
		//printf("POSITION: %d\n", position);
		volcanoRec(index + 1024);
	}
	else if (position == 6){
		//printf("POSITION: %d\n", position);
		volcanoRec(index + 1024 - 1);
	}
	else if (position == 7){
		//printf("POSITION: %d\n", position);
		volcanoRec(index - 1);
	}

}*/

int inline Terrain::getValue(int x, int y){
	static int size = M->ter->getTSize();
	return buf_[(x & (size - 1)) + (y&(size - 1)) * M->ter->getTSize()] & 0xFFFF;
}

void inline Terrain::setValue(int x, int y, uint val){
	static int size = M->ter->getTSize();
	if (val > 0xFFFF) val = 0xFFFF;
	buf_[(x & (size - 1)) + (y&(size - 1)) * M->ter->getTSize()] = 0xFF000000 | val;
}

void inline Terrain::increaseValue(int x, int y, uint val){
	uint temp = getValue(x, y);

	if (val > 0xFFFF) val = 0xFFFF;

	if (val + temp > 0xFFFF) setValue(x, y, 0xFFFF);
	else setValue(x, y, val + temp);
}

void inline Terrain::decreaseValue(int x, int y, uint val){
	uint temp = getValue(x, y);

	if (val > 0xFFFF) val = 0xFFFF;

	if (temp - val> 0xFFFF) setValue(x, y, 0x0);
	else setValue(x, y, temp - val);
}