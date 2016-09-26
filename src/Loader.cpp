#include "Loader.h"
#include "qpixmap.h"
#include <qcolor.h>
#include "QtGui\qrgb.h"
#include "Manager.h"
void Loader::init(Manager* manager){
	M = manager;
	reader.setScaledSize(QSize(M->ter->getTSize(), M->ter->getTSize()));
}

QImage Loader::loadHMap(QString HMapPath){
	reader.setFileName(HMapPath);
	QImage img = reader.read();
	//img = img.convertToFormat(QImage::Format::Format_RGB32);
	return img;
}

bool Loader::transformImageToHeights(const QImage &img, QPoint start, QPoint end){
	if (start.y() < 0) start.setY(0);
	if (start.x() < 0) start.setX(0);
	if (end.y() > M->ter->getTSize()) end.setY(M->ter->getTSize());
	if (end.x() > M->ter->getTSize()) end.setX(M->ter->getTSize());
	int* buffer_;
	buffer_ = (int*)img.bits();
	for (int i = start.y(); i < end.y(); i++){
		for (int j = start.x(); j < end.x(); j++){
			//M->ter->heights[i * M->ter->getTSize() + j] = buffer_[i * M->ter->getTSize() + j] & 0x00FFFF;
		}
	}
	return true;
}

void Loader::transformHeightsToImage(){
	/*
	int* buffer_;
	int* bufferH;
	buffer_ = (int*)M->PW->theImage.bits();
	bufferH = (int*)M->PW->hImage.bits();
	uint pixel;
	for (int i = 0; i < M->ter->getTSize(); i++){
		for (int j = 0; j < M->ter->getTSize(); j++){
			pixel = (M->ter->vertices[i * M->ter->getTSize() + j].y()/M->ter->getScaleH()) * 0xFFFF;
			if (M->ter->isTerrainInverted()) pixel = 0xFFFF - pixel;
			bufferH[i * M->ter->getTSize() + j] = 0xFF000000 | pixel;
			pixel = (float(pixel) / 0xFFFF) * 255;
			pixel = 0xFF000000 | pixel << 16 | pixel << 8 | pixel;
			//M->PW->theImage.setPixel(j, i, pixel) ;
			buffer_[i * M->ter->getTSize() + j] = pixel;
		}
	}
	*/
}

QImage Loader::applyEffectToImage(QImage src, QGraphicsEffect *effect, int extent)
{
	if (src.isNull()) return QImage();   //No need to do anything else!
	if (!effect) return src;             //No need to do anything else!
	QGraphicsScene scene;
	QGraphicsPixmapItem item;
	item.setPixmap(QPixmap::fromImage(src));
	item.setGraphicsEffect(effect);
	scene.addItem(&item);
	QImage res(src.size() + QSize(extent * 2, extent * 2), QImage::Format_RGB32);
	res.fill(Qt::transparent);
	QPainter ptr(&res);
	scene.render(&ptr, QRectF(), QRectF(-extent, -extent, src.width() + extent * 2, src.height() + extent * 2));
	return res;
}

QImage Loader::blur(QImage img){
	QGraphicsBlurEffect *blur = new QGraphicsBlurEffect;
	blur->setBlurHints(QGraphicsBlurEffect::QualityHint);
	blur->setBlurRadius(1.1);
	return applyEffectToImage(img, blur);
}
