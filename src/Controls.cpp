#include "Controls.h"
#include "Manager.h"
void Controls::Init(Manager *manager){
	M = manager;



	speed = 5.3f;
	mouseSpeed = 0.009f;


	center = QVector3D(M->ter->getTSize() / 20.f, 10.f, -M->ter->getTSize() / 20.f);
	zoom = 30.f;
	hAngle = 0;
	vAngle = 0;

	FoV = 75.f;
	nearZ = 0.3f;
	farZ = 5000.f;

	computeCam();

	up = QVector3D(0.f, 1.f, 0.f);
	
	updateView();

	Projection.setToIdentity();
	Projection.perspective(FoV, 6.f / 8.f, nearZ, farZ);
	//Projection.ortho(-800, 800, -800,800, nearZ, farZ);
	
}

void Controls::picking(QPoint p){
	
}

void Controls::computeCam(){
	float x, y, z;

	if (center.y() < -30) center.setY(-30);
	if (center.y() > 30) center.setY(30);


	x = center.x() + zoom*sin(hAngle)*cos(vAngle);
	z = center.z() + zoom*cos(hAngle)*cos(vAngle);
	y = center.y() + zoom*sin(vAngle);
	

	cam.setX(x);
	cam.setY(y);
	cam.setZ(z);

	up.setX(-sin(vAngle)*sin(hAngle));
	up.setZ(-sin(vAngle)*cos(hAngle));
	up.setY(cos(vAngle));
	
	up.normalize();
}

void Controls::computeView(QPoint mouse){

	hAngle += mouse.x() * mouseSpeed;
	vAngle += mouse.y() * mouseSpeed;

	if (hAngle > 2*M_PI) hAngle = 0;
	if (vAngle >= M_PI_2 - 0.2f) vAngle = M_PI_2 - 0.2f;

	if (hAngle < 0) hAngle = 2*M_PI;
	if (vAngle < 0) vAngle = 0;

	updateView();

}
void Controls::updateViewFirstPerson(){
	center = cam;
	computeCam();

	View.setToIdentity();
	View.lookAt(cam, center, up);
}

void Controls::computeViewFirstPerson(QPoint mouse){

	hAngle += mouse.x() * mouseSpeed;
	vAngle += mouse.y() * mouseSpeed;

	if (hAngle > 2 * M_PI) hAngle = 0;
	if (vAngle >= M_PI_2 - 0.2f) vAngle = M_PI_2 - 0.2f;

	if (hAngle < 0) hAngle = 2 * M_PI;
	if (vAngle < 0) vAngle = 0;

	updateViewFirstPerson();

}


void Controls::computeView(){
	QVector3D direction;

	bool changed = false;

	if (keys[Qt::Key_Up]){
		center.setY(center.y() + speed);
		changed = true;
	}
	else if (keys[Qt::Key_Down]){
		center.setY(center.y() - speed);
		changed = true;
	}
	if (keys[Qt::Key_Left]){
		direction = QVector3D::crossProduct(up, center - cam);
		direction.normalize();
		center += direction*speed;
		changed = true;
	}
	else if (keys[Qt::Key_Right]){
		direction = QVector3D::crossProduct(up, center-cam);
		direction.normalize();
		center -= direction*speed;
		changed = true;
	}

	if (keys[Qt::Key_E]){
		setZoom(getZoom() - speed);
		changed = true;
	}
	else if (keys[Qt::Key_R]){
		setZoom(getZoom() + speed);
		changed = true;
	}

	if (keys[Qt::Key_Q]){
		M->ter->setScaleH(M->ter->getScaleH() + .3f);
		
	}
	else if (keys[Qt::Key_W]){
		M->ter->setScaleH(M->ter->getScaleH() - .3f);
	}
	if (keys[Qt::Key_X]){
		M->RW->change = 1;
	}
	else if (keys[Qt::Key_S]){
		M->RW->change = 0;
	}
	else if (keys[Qt::Key_C]){
		M->RW->change = 2;
	}

	if (keys[Qt::Key_4]){
		M->RW->texId = 3;
	}
	if (keys[Qt::Key_1]){
		M->RW->texId = 0;
	}
	if (keys[Qt::Key_2]){
		M->RW->texId = 1;
	}
	if (keys[Qt::Key_3]){
		M->RW->texId = 2;
	}
	if (keys[Qt::Key_U]){
		M->RW->shine += 0.1f;
	}
	if (keys[Qt::Key_J]){
		M->RW->shine -= 0.1f;;
	}if (keys[Qt::Key_P]){
		M->clearMap();
	}
	if (changed) updateView();
}

void Controls::singleEvent(int k){
	if (k == Qt::Key_V){
		M->ter->avgHeights();
	}
	if (k == Qt::Key_A){
		M->ter->invert();
	}
	if (k == Qt::Key_5){
		M->RW->texId --;
	}
	if (k == Qt::Key_6){
		M->RW->texId++;
	}
	if (k == Qt::Key_L){
		M->ter->toggle(0);
	}
	if (k == Qt::Key_B){
		M->ter->toggle(1);
	}
	if (k == Qt::Key_N){
		M->ter->toggle(2);
	}
	if (k == Qt::Key_M){
		M->ter->toggle(3);
	}
}
void Controls::updateView(){
	computeCam();

	View.setToIdentity();
	View.lookAt(cam, center, up);
}