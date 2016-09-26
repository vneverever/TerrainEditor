#pragma once

#include <qwidget.h>
#include <qpainter.h>
#include <qevent.h>
#include <qimage.h>
#include <qlist.h>
#include <qelapsedtimer.h>
#include "Manager.h"
#include <qopenglframebufferobject.h>
#include <qopenglpaintdevice.h>
#include <qmutex.h>
#include <stack>
#include <QtConcurrent\qtconcurrentrun.h>

class PainterWindow : public QWidget
{

	Q_OBJECT //for Qt signals/slots
public:

	explicit PainterWindow(QWidget* Parent);



	void setImage(QImage &image);
	QImage getImage(){ return theImage; };
	void setBrushWidth(int width);
	void setGradImage();
	void fixedDraw(bool enable){
		fixedDrawMode = enable;
	};
	void setDrawHeight(int d);
	void setManager(Manager *manager){ M = manager; };
	void changeSize();
	void blur();
	void clearHMap();

	QSize sizeHint() const Q_DECL_OVERRIDE;
	void inline compareImage(short x, short y);
	
	void inline compareImageIncrement(short x, short y);

	void inline testMethod(QPoint pCur, QPointF lastPos);
	
	void clearTempImage();
	QImage theImage;
	QImage hImage;
	QImage *gradImage[4];
	bool clear = true;
	bool raise = true;

	int drawWidth = 70;
	

	void inline mouseMoveEvent(QMouseEvent* event);
	void inline mousePressEvent(QMouseEvent* event);
	QPoint lastPos;
protected:
	QImage tempImage; 
	QOpenGLFramebufferObject *pBuffer;
	void inline paintEvent(QPaintEvent* event);
	
	void mouseReleaseEvent(QMouseEvent* event);
	void underMouse();
	void inline mouseHold();

private:
	bool fixedDrawMode = true;
	bool drawing = false;
	bool needUpdate = true;
	QPoint terStart, terEnd;
	QColor color;
	uint* buffer_;
	bool init = true;
	bool Lock = false;
	bool moving = false;
	bool invert = false;
	bool down = false;

	QFuture<void> t1;

	Manager *M;
	//BrushInterface *brushInterface;
	QOpenGLPaintDevice *device;
	QPainter *p;
	QPainter painter;
	QString brush;
	
	QRect target = QRect(0, 0, 512, 512);
	bool inverse = false;
	bool notDrawn = true;
	int depht = 0;

	QElapsedTimer *timer;
	QPoint areaStart;
	QPoint areaEnd;
	std::stack<std::pair<short,short>> pointStack;
	
	QPainterPath* rectPath;
	QElapsedTimer eTimer;
	QMutex mtx;
	QMutex mtx2;
	QMutex mtx3;

	QPainterPath pendingPath;
	QRadialGradient* gradient[4];
	QLinearGradient* lgradient[4];
	short currentGradient = 0;
	QConicalGradient *cgradient;
};