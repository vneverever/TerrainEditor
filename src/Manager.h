#pragma once
#include "RenderWindow.h"
#include "Terrain.h"
#include "Controls.h"
#include "TextureUtil.h"
#include "Loader.h"
#include "PainterWindow.h"
#include "Transform.h"
class RenderWindow;
class PainterWindow;
class Terrain;
class Controls;
class TextureUtil;
class Loader;
class Transform;

class Manager{
public:
	Terrain *ter;
	Controls *con;
	TextureUtil *tex;
	Loader *loa;
	RenderWindow *RW;
	PainterWindow *PW;
	Transform *tra;

	void init(RenderWindow *RW, PainterWindow *PW);
	void setHMap(QString HMapPath);
	void updateArea();
	void updateHeightsToImage();
	void clearMap();
	QImage hMap;
	boolean refreshHmap = true;
	GLuint* mPixels;
private:
	QElapsedTimer timer;
};