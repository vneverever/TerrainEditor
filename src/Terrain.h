#pragma once
#include <qmap.h>
#include <qvector3d.h>
#include <qvector2d.h>
#include "Manager.h"
#include <QtConcurrent\qtconcurrentrun.h>


class Manager;
class Terrain : QObject
{
public:
	Terrain();
	void Init(Manager* manager);
	void Render();
	void move();

	void setLightIntesity(int number){ m_LightIntesity = (float)number / 100.f; };

	void setScaleH(float f);
	float getScaleH(){ return scaleH; };

	void invert();

	void calculateGeometry();

	void testChangeData();
	void setDataStructure(int size);

	int getTSize();
	int setTSize(int size){};
	void avgHeights();
	void inline avgHeights(int x, int y);
	void avgNormals();
	void updateTerrain(QPoint start, QPoint end);
	void updateGround();
	void updateTextures();
	void updateTextureTable(float start, float end, QVector3D *tex);
	void calculatehMapIndex();
	void inline water();
	void olsen();
	void toggle(int id);
	


	void test();

	boolean init = true;
	int indexRowLength = 0;
	GLfloat getHeightScale(){ return scaleH; };
	bool isTerrainInverted(){ return inverted; };
	QMap<int, float> heightTable;
	QMap<int, QVector3D> colors;
	QMutex mtx;
	//QFuture<void> t1;
	
	GLfloat scaleH = 20.f;
	static const int tSize = 1024;
	float alphaDist = 0.5f;
	QVector2D vertices[tSize * tSize];
	QVector2D verticesGround[(tSize-1) * 4 * 2];
	QVector3D verticesAvg[512 * 512];
	QVector3D normals[512 * 512];
	QVector3D normalsAvg[512 * 512];
	QVector2D texCoord[tSize*tSize*2];
	QVector2D hMapIndex[tSize * tSize];
	GLuint index[(tSize-1) * 6 * (tSize-1)];
	GLuint indexGround[(tSize-1) * 6 * 4 + 6];
	GLuint indexGroundLower[255 * 6 * 4 + 18];

	GLuint indexTest[(tSize-1) * 6 * (tSize-1)];
	GLuint indexLower[256 * 6 * 256];
	GLuint indexLowest[128 * 6 * 128];
private:


	void calculateNormals();
	void calculateLight(float intesity);
	bool inverted = false;
	void initGround();
	void inline volcano();
	void inline volcanoRec(int x, int y, float velocity);
	void inline diamondSquare();
	void fluid();
	
	int ttestest;
	uint* buf_;

	void inline increaseValue(int x, int y, unsigned int val);
	void inline decreaseValue(int x, int y, unsigned int val);
	int inline getValue(int x, int y);
	void inline setValue(int x, int y, uint val);
	
	QMap<int, void (Terrain::*) (void)> threads;
	QMap<int, bool> threadBreak;


	float m_LightIntesity = 0;

	float distToCenter;
	bool indexChanged[tSize * tSize];
	float terrainScale = 0.1f;
	Manager *M;
	//parameter for the sphere rotations
	float rotationAngle1 = 0.f;
	float rotationAngle2 = 0.f;
	float rotationAngle3 = 0.f;

};