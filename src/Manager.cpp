#include "Manager.h"
#include "RenderWindow.h"
#include "PainterWindow.h"
#include "Controls.h"
#include "Loader.h"
#include "Transform.h"
#include "Terrain.h"
#include "TextureUtil.h"
#include "QtConcurrent\qtconcurrentrun.h"
#include "qobject.h"

void Manager::init(RenderWindow *RWindow, PainterWindow *PWindow){
	tra = new Transform();
	ter = new Terrain();
	tex = new TextureUtil();
	loa = new Loader();
	con = new Controls();


	con->Init(this);
	ter->Init(this);
	tex->init(this);
	loa->init(this);
	RW = RWindow;
	PW = PWindow;
}

void Manager::setHMap(QString HMapPath){
	hMap = loa->loadHMap(HMapPath);
	PW->changeSize();
	PW->setImage(hMap);
	tra->init(this);
	PW->update();
	updateArea();
	RW->rdy = true;
}
void Manager::clearMap(){
	PW->clearTempImage();
	PW->clearHMap();
	PW->update();
	updateArea();
}
void Manager::updateArea(){
	ter->init = true;
	refreshHmap = true;
}

void Manager::updateHeightsToImage(){
	refreshHmap = true;
}