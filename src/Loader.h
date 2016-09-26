#pragma once
#include <qmap.h>
#include <qimage.h>
#include <qimagereader.h>
#include <qpoint.h>
#include "Manager.h"
#include <qgraphicsscene.h>
#include <qpixmap.h>
#include "qgraphicsitem.h"
#include <qgraphicseffect.h>

class Loader : QObject{
public:
	void init(Manager *manager);
	QImage loadHMap(QString HMapPath);
	QImage blur(QImage img);
	bool transformImageToHeights(const QImage &img, QPoint start, QPoint end);
	QImage applyEffectToImage(QImage src, QGraphicsEffect *effect, int extent = 0);
	void transformHeightsToImage();
	Manager *M;
	QImageReader reader;
};
