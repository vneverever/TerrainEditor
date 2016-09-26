#include "PainterWindow.h"
#include "RenderWindow.h"
#include <qrect.h>
#include <QtConcurrent\qtconcurrentrun.h>
#include <qthread.h>

PainterWindow::PainterWindow(QWidget* Parent) :
	QWidget(Parent),
	color(Qt::white),
	lastPos(0,0)
{	
	
	this->setMouseTracking(true);
	drawWidth = 30;
	gradient[0] = new QRadialGradient(0, 0, drawWidth);
	gradient[1] = new QRadialGradient(0, 0, drawWidth);
	gradient[2] = new QRadialGradient(0, 0, drawWidth);
	gradient[3] = new QRadialGradient(0, 0, drawWidth);
	
	int color = 0;
	
	int w = 0xFFFF;
	gradient[0]->setColorAt(0.0, 0x0000FFFF);
	
	gradient[0]->setColorAt(1.0, 0.0);
	
	gradient[1]->setColorAt(0.0, 0.0);
	gradient[1]->setColorAt(0.1, 0x00000200);
	gradient[1]->setColorAt(0.2, 0x00000800);
	gradient[1]->setColorAt(0.3, 0x00001100);
	gradient[1]->setColorAt(0.4, 0x00002200);
	gradient[1]->setColorAt(0.5, 0x00004400);
	gradient[1]->setColorAt(0.6, 0x00008800);
	gradient[1]->setColorAt(0.7, 0x0000CC00);
	gradient[1]->setColorAt(1.0, 0x0000FFFF);

	gradient[2]->setColorAt(0, 0x0000000FF);
	gradient[2]->setColorAt(1, 0.0);

	gradient[3]->setColorAt(0, 0x0000000FF);
	gradient[3]->setColorAt(1, 0.0);

	//gradient[0]->setCoordinateMode(QGradient::CoordinateMode::LogicalMode);
	//gradient[1]->setCoordinateMode(QGradient::CoordinateMode::LogicalMode);
	//gradient[2]->setCoordinateMode(QGradient::CoordinateMode::LogicalMode);
	//gradient[3]->setCoordinateMode(QGradient::CoordinateMode::LogicalMode);

	cgradient = new QConicalGradient(0, 0, drawWidth);
	cgradient->setColorAt(0.0, 0xFFFFFF);
	cgradient->setColorAt(1.0, 0.111111);

	lgradient[0] = new QLinearGradient();
	lgradient[0]->setColorAt(0.0, 0x000001);
	lgradient[0]->setColorAt(0.15, 0x000011);
	lgradient[0]->setColorAt(0.2, 0x00FF11);
	lgradient[0]->setColorAt(0.25, 0x00FF55);
	lgradient[0]->setColorAt(0.3, 0x00FFFF);
	lgradient[0]->setColorAt(0.5, 0x00FFFF);
	lgradient[0]->setColorAt(0.7, 0x00FFFF);
	lgradient[0]->setColorAt(0.75, 0x00FF55);
	lgradient[0]->setColorAt(0.8, 0x00FF11);
	lgradient[0]->setColorAt(0.85, 0x000011);
	lgradient[0]->setColorAt(1.0, 0x000001);
	lgradient[0]->setCoordinateMode(QGradient::CoordinateMode::LogicalMode);

	lgradient[1] = new QLinearGradient();
	lgradient[1]->setColorAt(0.0, 0x0000FFFF);
	lgradient[1]->setColorAt(0.15, 0x0000CC00);
	lgradient[1]->setColorAt(0.2,  0x00008800);
	lgradient[1]->setColorAt(0.25, 0x00004400);
	lgradient[1]->setColorAt(0.3,  0x00000001);
	lgradient[1]->setColorAt(0.5,  0x00000001);
	lgradient[1]->setColorAt(0.7,  0x00000001);
	lgradient[1]->setColorAt(0.75, 0x00004400);
	lgradient[1]->setColorAt(0.8,  0x00008800);
	lgradient[1]->setColorAt(0.85, 0x0000CC00);
	lgradient[1]->setColorAt(1.0,  0x0000FFFF);
	lgradient[1]->setCoordinateMode(QGradient::CoordinateMode::LogicalMode);

	lgradient[2] = new QLinearGradient();
	lgradient[2]->setColorAt(0.0,0x00000001);
	lgradient[2]->setColorAt(0.5, 0x000000FF);
	lgradient[2]->setColorAt(1, 0x00000001);
	lgradient[2]->setCoordinateMode(QGradient::CoordinateMode::LogicalMode);

	lgradient[3] = new QLinearGradient();
	lgradient[3]->setColorAt(0.0, 0x0000001F);
	lgradient[3]->setColorAt(0.5, 0x000000FF);
	lgradient[3]->setColorAt(1,	  0x00000001);
	lgradient[3]->setCoordinateMode(QGradient::CoordinateMode::LogicalMode);
	
	
	//gradientRaise2->setInterpolationMode(QGradient::ComponentInterpolation);

	t1 = QtConcurrent::run(this, &PainterWindow::mouseHold);
	for (int i = 0; i < 4; i++) gradImage[i] = new QImage(M->ter->getTSize(), M->ter->getTSize(),QImage::Format_ARGB32_Premultiplied);
}

void test(){
	printf("HELLO");
}
void PainterWindow::setDrawHeight(int d){
	
	/*
	int color = d;
	printf("COLOR:%x\n", color);
	for (int i = 10; i < 100; i++){
		color = (i / 100.f) * (i / 100.f) * 0xFF;
		color = color << 16 | color << 8 | color;
		color = 0xFF - color;
		gradient->setColorAt(i / 100.f, color);
	}
	*/
	float f = d / 255.f;
	gradient[0] = new QRadialGradient(0, 0, drawWidth);
	gradient[1] = new QRadialGradient(0, 0, drawWidth);
	int color = d << 16 | d << 8 | d;

	int start = 0xFFFF * f;

	gradient[1]->setColorAt(0.0, 0x00FFFF - color);
	gradient[1]->setColorAt(0.2, 0x00FFFF - color);
	gradient[1]->setColorAt(0.4, 0x00FFFF - color);
	gradient[1]->setColorAt(0.6, 0x00FFFF - color);
	gradient[1]->setColorAt(0.8, 0x00FFFF - color);
	gradient[1]->setColorAt(1.0, 0x00FFFF);

	gradient[0]->setColorAt(0.0, color);
	gradient[0]->setColorAt(0.2, color);
	gradient[0]->setColorAt(1.0, 0.0);
	
	
	//if (!inverse) p->setBrush(*gradient[0]);
	//else p->setBrush(*gradient[1]);

	clear = true;

	
}

void PainterWindow::paintEvent(QPaintEvent *e){
	if (!mtx3.tryLock()) return;
	painter.begin(this);
	//painter.setCompositionMode(QPainter::CompositionMode::CompositionMode_Source);

	painter.drawImage(target, hImage.convertToFormat(QImage::Format::Format_RGB32, Qt::ImageConversionFlag::ColorOnly));

	painter.end();
	needUpdate = false;
	mtx3.unlock();
	//printf("EndPaint: %d\n", timer.elapsed());
	
}

void PainterWindow::changeSize(){

}

void PainterWindow::blur(){
	hImage = M->loa->blur(hImage);
	M->updateHeightsToImage();
}

void PainterWindow::clearTempImage(){

}


void PainterWindow::setImage(QImage &image)
{
	hImage = image.copy();
	
	buffer_ = (uint*)hImage.bits();
	for (int i = 0; i < M->ter->getTSize() * M->ter->getTSize(); i++){
		buffer_[i] = 0xFF000000 | buffer_[i] & 0xFFFF;
	}
	
	setGradImage();
	

	areaStart.setX(0);
	areaStart.setY(0);
	areaEnd.setX(M->ter->getTSize());
	areaEnd.setY(M->ter->getTSize());
	
}
void PainterWindow::underMouse(){
	printf("hi");
}


void PainterWindow::mousePressEvent(QMouseEvent *e){
	if (e->button() == Qt::LeftButton || e->button() == Qt::RightButton) {
		drawing = true;
		
		if (!inverse && e->buttons() & Qt::RightButton){
			if (fixedDrawMode){
				currentGradient = 1;
				//p->setBrush(*gradient[1]);
				//p->setCompositionMode(QPainter::CompositionMode_Darken);
			}
			else{
				currentGradient = 3;
				//p->setBrush(*gradient[3]);
				//p->setCompositionMode(QPainter::CompositionMode_Lighten);
			}
			clear = true;
			inverse = true;
			raise = false;
		}
		if (inverse && e->buttons() & Qt::LeftButton){
			if (fixedDrawMode){
				currentGradient = 0;
				//p->setBrush(*gradient[0]);
				//p->setCompositionMode(QPainter::CompositionMode_Lighten);
			}
			else{
				currentGradient = 2;
				//p->setBrush(*gradient[2]);
				//p->setCompositionMode(QPainter::CompositionMode_Lighten);
			}
			clear = true;
			inverse = false;
			raise = true;
		}
		//if (clear)clearTempImage();

		
		int x = e->pos().x()*(M->ter->getTSize()/512.f)-drawWidth;
		int y = e->pos().y()*(M->ter->getTSize() / 512.f)-drawWidth;
		lastPos = QPoint(x, y);
		//areaStart.setX(x - drawWidth-5);
		//areaStart.setY(y - drawWidth-5);
		//areaEnd.setX(x + drawWidth+5);
		//areaEnd.setY(y + drawWidth+5);

		//terStart.setX(areaStart.x());
		//terStart.setY(areaStart.y());
		//terEnd.setX(areaEnd.x());
		//terEnd.setY(areaEnd.y());
		//while (!mtx.tryLock());
		//for (int i = 0; i < M->ter->getTSize(); i+=10){
		//	for (int j = 0; j < M->ter->getTSize(); j+=10){
		//			pointStack.push(std::make_pair(i, j));
		//	}
		//}
		//mtx.unlock();
		pointStack.push(std::make_pair(x,y));
		t1.resume();
		

		//p->setBrushOrigin(x, y);
		//p->drawEllipse(x - drawWidth, y - drawWidth, drawWidth * 2, drawWidth * 2);
		

		e->accept();

		
		
		return;
	}
}


void PainterWindow::mouseHold(){

	while (true){
		while (!drawing)Sleep(10);
		//printf("Awake");
		//if (!drawing)break;
		/*
		if (!fixedDrawMode)compareImageIncrement(areaStart, areaEnd);
		else if (notDrawn){
			compareImage(QPoint(lastPos.x() - drawWidth, lastPos.y() - drawWidth));
			//mtx3.lock();

		}
		*/
		if (!fixedDrawMode){
			short arr[2];
			while (!pointStack.empty()){
				mtx.lock();
				arr[0] = pointStack.top().first;
				arr[1] = pointStack.top().second;
				compareImageIncrement(pointStack.top().first, pointStack.top().second);
				pointStack.pop();
				mtx.unlock();
				//Sleep(uint(1));
				}
				//compareImageIncrement(arr[0], arr[1]);
				//Sleep(uint(0.0));
		}
		else{
			while (!pointStack.empty()){
				mtx.lock();
				//printf("SIZE: %d", pointStack.size());
				//std::pair<short, short> temp = pointStack.top();
 				compareImage(pointStack.top().first, pointStack.top().second);
				pointStack.pop();
				mtx.unlock();
				//Sleep(uint(1));
			}
		}
		
		//if (needUpdate)update();
		//printf("HEELO");
		
		//if (!moving)break;
		//mtx.unlock();
		//}
		//Sleep(uint(1));
		

	}
}

void PainterWindow::mouseReleaseEvent(QMouseEvent* e){
		
	if (!(e->buttons() & Qt::LeftButton || e->buttons() & Qt::RightButton)){
		drawing = false;
		clearTempImage();
		if (pointStack.empty()) t1.setPaused(true);
	}
}

void PainterWindow::testMethod(QPoint pCur,QPointF lastPos){
	pCur.setX(pCur.x() * (M->ter->getTSize() / 512));
	pCur.setY(pCur.y() * (M->ter->getTSize() / 512));


	//t.start();
	//printf("START: %i\n", t.elapsed());
	//areaStart.setX(lastPos.x() - drawWidth - 5);
	//areaStart.setY(lastPos.y() - drawWidth - 5);
	//areaEnd.setX(lastPos.x() + drawWidth + 5);
	//areaEnd.setY(lastPos.y() + drawWidth + 5);

	//QVector2D* normal;
	//normal = new QVector2D(pCur.x() - lastPos.x(), pCur.y() - lastPos.y());

	//p->setBrushOrigin(lastPos);
	//p->setBrush(*gradient[currentGradient]);
	//p->drawEllipse(lastPos.x() - drawWidth, lastPos.y() - drawWidth, drawWidth * 2, drawWidth * 2);
	
	/*if (qAbs(normal->length()) > drawWidth*0.5){
		normal = new QVector2D(normal->y(), -normal->x());
		normal->normalize();

		//printf("INSIDE\n");
		QPointF *points = new QPointF[4];
		points[0] = (QVector2D(lastPos) - *normal*drawWidth).toPointF();
		points[1] = (QVector2D(lastPos) + *normal*drawWidth).toPointF();
		points[2] = (QVector2D(pCur) + (*normal*drawWidth)).toPointF();
		points[3] = (QVector2D(pCur) - *normal*drawWidth).toPointF();

		rectPath = new QPainterPath();
		rectPath->moveTo(points[0]);
		rectPath->lineTo(points[1]);
		rectPath->lineTo(points[2]);
		rectPath->lineTo(points[3]);
		rectPath->closeSubpath();


		p->setBrushOrigin(0.5, 0.5);
		lgradient[currentGradient]->setStart(points[0]);
		lgradient[currentGradient]->setFinalStop(points[1]);
		p->setBrush(*lgradient[currentGradient]);

		p->drawPath(*rectPath);

	}*/
	
	//delete (normal);

	//p->setBrushOrigin(pCur);
	//p->setBrush(*gradient[currentGradient]);
	//p->drawEllipse(pCur.x() - drawWidth, pCur.y() - drawWidth, drawWidth * 2, drawWidth * 2);

	//printf("After Draw: %i\n", t.elapsed());


	//areaStart.setX(std::max(std::min(pCur.x() - drawWidth - 5, areaStart.x()), 0));
	//areaStart.setY(std::max(std::min(pCur.y() - drawWidth - 5, areaStart.y()), 0));
	//areaEnd.setX(std::min(std::max(pCur.x() + drawWidth + 5, areaEnd.x()), M->ter->getTSize()));
	//areaEnd.setY(std::max(std::min(pCur.y() + drawWidth + 5, areaEnd.y()), M->ter->getTSize()));

	
	if (fixedDrawMode){

		//printf("Before update: %i\n", t.elapsed());
		//QFuture<void> t1 = QtConcurrent::run(this, &PainterWindow::compareImage);
		//compareImage(QPoint(pCur.x() - drawWidth, pCur.y() - drawWidth));
		//printf("After1 Update: %i\n", t.elapsed());
		//clearTempImage();
		//M->updateArea();
	}
	else{

		//QFuture<void> t1 = QtConcurrent::run(M->updateArea, hImage, terStart, terEnd);
		//QFuture<void> t1 = QtConcurrent::run(this, &PainterWindow::compareImageIncrement);
		//while (Lock);
		//compareImageIncrement(areaStart, areaEnd);
		//QFuture<void> t1 = QtConcurrent::run(this, &PainterWindow::mouseHold);
		//M->updateArea();
		//update();
		//mtx3.lock();
		//mtx.lock();
		//clearTempImage();
		//M->ter->avgHeights(terStart, terEnd);
	}
}

void PainterWindow::mouseMoveEvent(QMouseEvent *e)
{
	if (((e->buttons() & Qt::LeftButton) || (e->buttons() & Qt::RightButton))){
		QElapsedTimer t;
		t.start();
		moving = true;

		down = true;
		int scale = (M->ter->getTSize() / 512);

		//printf("EPOS: %d, %d \n", e->pos().x(), e->pos().y());
		if (e->pos().x() < -drawWidth || e->pos().x() > 512 + drawWidth || e->pos().y() < -drawWidth || e->pos().y() >512 + drawWidth){
			drawing = false;
			lastPos.setX((e->pos().x()*scale) - drawWidth);
			lastPos.setY((e->pos().y()*scale) - drawWidth);
			return;
		}
		drawing = true;
		
		
		
		//if(clear)clearTempImage();
		
		//QPoint pCur; 
		//pCur.setX(e->pos().x());
		//pCur.setY(e->pos().y());
		//if(!Lock)QFuture<void> t1 = QtConcurrent::run(this, &PainterWindow::testMethod,pCur,lastPos);
		//testMethod(QPoint(e->pos().x(), e->pos().y()), lastPos);
		float tempx = (e->pos().x()*scale - drawWidth - lastPos.x());
		float tempy = (e->pos().y()*scale - drawWidth - lastPos.y());
		float length = sqrt(pow(tempx, 2) + pow(tempy, 2));
		tempx = (tempx/length)*4;
		tempy = (tempy/length)*4;
		//printf("TEMPX: %f\n", tempx);
		//printf("TEMPY: %f\n", tempy);
		mtx.lock();
		for (int i = 0; i <= length/4; i++){
			pointStack.push(std::make_pair(short(lastPos.x() + tempx*i), short(lastPos.y() + tempy*i)));
			//printf("I: %f, Point: %f, %d\n", i, (lastPos.x() + tempx*i), (lastPos.y() + tempy*i));
		}
		mtx.unlock();

		lastPos.setX((e->pos().x()*scale)-drawWidth);
		lastPos.setY((e->pos().y()*scale)-drawWidth);
		
		//if (fixedDrawMode)pointStack.push(std::make_pair(lastPos.x()*scale - drawWidth, lastPos.y()*scale - drawWidth));;

		//update();
		e->accept();
		//printf("End: %i\n", t.elapsed());
		moving = false;
	}
}

void PainterWindow::setBrushWidth(int width)
{
	drawWidth = width;
	gradient[0]->setRadius(width);
	gradient[1]->setRadius(width);
	gradient[2]->setRadius(width);
	gradient[3]->setRadius(width);

	setGradImage();
}

void PainterWindow::setGradImage(){
	QPainter *p;
	p = new QPainter();
	gradImage[0]->fill(0xFF000000);
	gradImage[1]->fill(0xFF00FFFF);
	gradImage[2]->fill(0xFF000000);
	gradImage[3]->fill(0xFF000000);
	for (int i = 0; i < 4; i++){
		
		
		p->begin(gradImage[i]);
		p->setPen(Qt::NoPen);
		p->setBrush(*gradient[i]);

		p->setRenderHint(QPainter::RenderHint::HighQualityAntialiasing);
		//p->setCompositionMode(QPainter::CompositionMode::RasterOp_SourceOrDestination);

		p->setBrushOrigin(drawWidth, drawWidth);
		//p->setBrush(*gradient[i]);
		p->drawEllipse(0, 0, drawWidth * 2, drawWidth * 2);
		p->end();
		
	}
	delete(p);
}

QSize PainterWindow::sizeHint() const
{
	return theImage.size();
}

void PainterWindow::compareImage(short x, short y){
	//QElapsedTimer t;
	//t.start();
	//printf("VALS: %d, %d\n", x, y);
	int loopVariables[4];
	int tSize = M->ter->getTSize();
	if (x < 0) loopVariables[0] = -(x);
	else loopVariables[0] = 0;

	if (x + drawWidth * 2> tSize) loopVariables[1] = -(x - tSize);
	else loopVariables[1] = drawWidth*2;

	if (y < 0) loopVariables[2] = -(y);
	else loopVariables[2] = 0;

	if (y + drawWidth * 2 > tSize) loopVariables[3] = -(y - tSize);
	else loopVariables[3] = drawWidth*2;

	//if (areaStart.x() < 0) areaStart.setX(0);
	//if (areaEnd.y() > M->ter->getTSize()) areaEnd.setY(M->ter->getTSize());
	//if (areaEnd.x() > M->ter->getTSize()) areaEnd.setX(M->ter->getTSize());
	//printf("Start: %i\n", t.elapsed());
	//printf("Point: %d, %d\n", x, y);
	//printf("Point: %d, %d, %d, %d\n", loopVariables[0], loopVariables[1], loopVariables[2], loopVariables[3]);
	//int start = 
	uint* buffer2_;
	buffer2_ = (uint*)gradImage[currentGradient]->bits();
	//std::copy()
	//memcpy(&buffer2_[0], tempImage.bits(), tempImage.width()*tempImage.height()*sizeof(uint));
	if (!inverse && fixedDrawMode){
		for (int i = loopVariables[2]; i < loopVariables[3]; i++){
			uint row = (y + i) * tSize;
			uint row2 = i * tSize;
			for (int j = loopVariables[0]; j < loopVariables[1]; j++){
				uint col = j + x;
				//printf("row: %d\n", row);
				//printf("VALUE2:0x%X\n", buffer2_[i * M->ter->getTSize() + j]);
				if (buffer2_[row2 + j]> buffer_[row + col]){
					//printf("VALUE:0x%08X\n", buffer2_[i * M->ter->getTSize() + j]);
					buffer_[row + col] = buffer2_[row2 + j];

				}
			}
		}
	}
	else{
		for (int i = loopVariables[2]; i < loopVariables[3]; i++){
			uint row = (y + i) * tSize;
			uint row2 = i * tSize;
			for (int j = loopVariables[0]; j < loopVariables[1]; j++){
				uint col = j + x;
				//printf("row: %d\n", row);
				//printf("VALUE2:0x%X\n", buffer2_[i * M->ter->getTSize() + j]);
				if (buffer2_[row2+j] < buffer_[row+col]){
					//printf("VALUE:0x%08X\n", buffer2_[i * M->ter->getTSize() + j]);
					buffer_[row+col] = buffer2_[row2+j];

				}
			}
		}
	}
	needUpdate = true;
	M->updateArea();
	//delete(buffer2_);
	//printf("EndDraw: %i\n", t.elapsed());
}

void PainterWindow::compareImageIncrement(short x, short y){
	//timer->start();
	//Lock = true;
	//eTimer.start();
	//if (areaStart.y() < 0) areaStart.setY(0);
	//if (areaStart.x() < 0) areaStart.setX(0);
	//if (areaEnd.y() > M->ter->getTSize()) areaEnd.setY(M->ter->getTSize());
	//if (areaEnd.x() > M->ter->getTSize()) areaEnd.setX(M->ter->getTSize());
	//uint* buffer_;
	//buffer_ = (uint*)hImage.bits();

	int loopVariables[4];
	int tSize = M->ter->getTSize();
	if (x < 0) loopVariables[0] = -(x);
	else loopVariables[0] = 0;

	if (x + drawWidth * 2> tSize) loopVariables[1] = -(x - tSize);
	else loopVariables[1] = drawWidth * 2;

	if (y < 0) loopVariables[2] = -(y);
	else loopVariables[2] = 0;

	if (y + drawWidth * 2 > tSize) loopVariables[3] = -(y - tSize);
	else loopVariables[3] = drawWidth * 2;
	uint* buffer2_;
	
	buffer2_ = (uint*)gradImage[currentGradient]->bits();
	//uint* buffer2_ = new uint[M->ter->getTSize() * M->ter->getTSize()];
	//memcpy(&buffer2_[0], tempImage.bits(), tempImage.width()*tempImage.height()*sizeof(uint));
	
	if (raise){
		for (int i = loopVariables[2]; i < loopVariables[3]; i++){
			uint row = (y + i) * tSize;
			uint row2 = i * tSize;
			for (int j = loopVariables[0]; j < loopVariables[1]; j++){
				uint pos = j + x + row;
				//printf("row: %d\n", row);
				//printf("VALUE2:0x%X\n", buffer_[pos]);
				
				uint result = buffer_[pos] + buffer2_[row2 + j];
				
				if (result  > 0xFE00FFFF){
					//printf("VALUE:0x%08X\n", buffer2_[i * M->ter->getTSize() + j]);
					buffer_[pos] = 0xFF00FFFF;
					continue;
				}
				buffer_[pos] = 0xFF000000 | result;
			}
		}
	}
	else{
		for (int i = loopVariables[2]; i < loopVariables[3]; i++){
			uint row = (y + i) * tSize;
			uint row2 = i * tSize;
			for (int j = loopVariables[0]; j < loopVariables[1]; j++){
				uint pos = j + x + row;
					//printf("row: %d\n", row);
					//printf("VALUE2:0x%X\n", buffer2_[i * M->ter->getTSize() + j]);

					uint result = buffer_[pos] - buffer2_[row2 + j];
					if (result > 0xFFFF){
						//printf("VALUE:0x%08X\n", buffer2_[i * M->ter->getTSize() + j]);
						buffer_[pos] = 0xFF000000;
						continue;
					}
					buffer_[pos] = 0xFF000000 | result;
				}
		}
	}

	needUpdate = true;
	M->updateArea();
	//printf("ELAPSED:%d\n", eTimer.elapsed());
	//Lock = false;
	//delete(buffer2_);
	
}

void PainterWindow::clearHMap(){
	hImage.fill(0xFF000000);
}