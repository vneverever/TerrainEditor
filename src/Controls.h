#pragma once
#include <qmatrix.h>
#include <qmatrix4x4.h>
#include "qvector.h"
#include "qpoint.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include "Manager.h"

const class Controls : QObject
{
public:
	void Init(Manager *m);
	QMatrix4x4 getProjectionMatrix(){ return Projection; };
	void setProjectionMatrix(QMatrix4x4 m){ Projection = m; };
	QMatrix4x4 getViewMatrix(){ return View; };
	void computeMovement();
	void computeView();
	void computeView(QPoint mouse);
	void computeViewFirstPerson(QPoint mouse);
	void updateView();
	void updateViewFirstPerson();
	void setZoom(float z){ if (z < 0.5f)z = 0.5f;  zoom = z; updateView(); }
	float getZoom(){ return zoom; updateView(); }
	float getFov(){ return FoV; };
	float getNearZ(){ return nearZ; };
	float getFarZ(){ return farZ; };
	void enableKey(int k){ keys[Qt::Key(k)] = true; }
	void disableKey(int k){ keys[Qt::Key(k)] = false; }
	QVector3D getCamPosition(){ return cam; };
	QVector3D getCenterPosition() { return center; };

	void picking(QPoint p);

	void singleEvent(int k);

	float getSpeed(){ return speed; }
	bool terrainChange = true;

private :
	double hAngle, vAngle, FoV, speed, mouseSpeed, zoom, nearZ, farZ;
	Manager *M;
	void computeCam();
	QMap<Qt::Key,bool> keys;
	QVector3D cam, up, center;
	QMatrix4x4 Projection;
	QMatrix4x4 View;
};