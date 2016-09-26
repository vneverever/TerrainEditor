#pragma once
#include <QOpenGLWidget>
#include <qopengltimerquery.h>
#include <qopenglfunctions_4_3_core.h>

#define _USE_MATH_DEFINES
#include "qevent.h"
#include <math.h>
#include <vector>
#include <QMap>
#include <qmatrix.h>
#include <qopenglshaderprogram.h>
#include <qopenglvertexarrayobject.h>
#include <qopenglbuffer.h>
#include "Manager.h"
#include <qvector.h>
#include <qelapsedtimer.h>
class Manager;

class RenderWindow : public QOpenGLWidget, protected QOpenGLFunctions_4_3_Core
{
	Q_OBJECT //for Qt signals/slots
	//QOpenGLContext *context;
public:
	RenderWindow(QWidget* Parent);
	void initializeGL();// is called after OpenGL was initialized for this widget
	void resizeGL(int Width, int Height);// is called whenever the window-size changes
	void paintGL();// is called when the scene should be rendered

	void setManager(Manager *manager);
	void mouseMoveEvent(QMouseEvent* event);// is called whenever the mouse is moved above this widget
	void mousePressEvent(QMouseEvent* event);

	void wheelEvent(QWheelEvent* event);// is called whenever the mousewheel is used above this widget
	void keyPressEvent(QKeyEvent* event);
	void keyReleaseEvent(QKeyEvent* event);

	//This methods get called from the gui
	void setLight(bool enabled){ program->bind();  program->setUniformValue("LightFlag", enabled); };
	void setLightMove(bool enable){ flagLightMove = enable; };
	void setTexture(bool enabled){ program->bind();  program->setUniformValue("TextureFlag", enabled); };
	void setSpecularMap(bool enabled){ program->bind();  program->setUniformValue("SpecularFlag", enabled); };
	void setNormalMap(bool enabled){ program->bind();  program->setUniformValue("NormalMapFlag", enabled); };
	void setShadows(bool enabled){ program->bind();  program->setUniformValue("ShadowFlag", enabled); };
	void setWater(bool enable){ waterFlag = enable; };
	void setWaterLevel(int level){ waterLevel = level/50.f; };
	void setLightX(double d) { lightPos.setX(d); };
	void setLightY(double d){ lightPos.setY(d); };
	void setLightZ(double d){ lightPos.setZ(d); };
	void setFirstPerson(bool enable){ firstPerson = enable; };
	void setTextureScale(float scale){ program->bind();  program->setUniformValue("TextureScale", scale); };
	void setTextureLevel(int level, QVector3D texVector){ program->bind(); program->setUniformValue(_texLevel[level], texVector); };



	void timerUpdate();
	
	void initTextureValues();

	QVector3D texturesAtLevel[10];

	int change = 0;
	int texId = 2;
	float shine = 25.f;
	GLuint gbo = 0;
	GLuint _tex[12];
	GLuint _shadow[12];
	GLuint _normal[12];
	GLuint _texLevel[10];
	GLuint _sky;
	GLuint _dataBlock;
	bool rdy = false;

private:
	Manager *M;
	QVector4D skyVerts[6], waterVerts[6];
	QVector4D rayStart, rayStartMouse, rayHit;
	QVector4D cam;
	QPoint rayPoint;
	float waterLevel;
	QVector3D lightPos;


	QOpenGLFramebufferObject *fbo;
	QOpenGLShaderProgram *program, *program2, *program3, *program4, *rayProgram;
	QOpenGLShader *vertexShad, *skyVertexShader;
	QOpenGLShader *fragmentShad, *skyFragmentShader;
	QOpenGLShader *vertexShaderGround, *fragmentShaderGround;
	QOpenGLShader *waterVertexShader, *waterFragmentShader;
	QOpenGLShader *rayVertexShader, *rayFragmentShader, *rayGeometryShader;
	QOpenGLContext *mContext;
	QOpenGLFunctions_4_3_Core *f;

	QOpenGLVertexArrayObject *vao[5];

	QOpenGLBuffer tVertexBuffer, tVertexBuffer2, tVertexBuffer3, waterVertexBuffer, rayBuffer;
	QOpenGLBuffer tIndexBuffer, tIndexBuffer2;
	QOpenGLBuffer tMapBuffer;
	//QOpenGLFramebufferObject *fbo;
	
	
	QMatrix4x4 MVP;
	QMatrix4x4 ViewMatrix;
	QMatrix4x4 ProjectionMatrix;
	QElapsedTimer t;

	GLuint _MVP,_hMap;

	void drawCoordinateSystem();
	void groundVAO();
	void skyVAO();
	void waterVAO();
	void rayVAO();
	void initBuffer();

	// Gui Control
	bool uiChange = false;
	bool flagLight = true;
	bool flagSpecular = false; 
	bool flagNormal = false; 
	bool flagShadow = false;
	bool flagTexture = true;
	bool waterFlag = false;
	bool firstPerson = false;
	bool flagLightMove = false;


	bool hMapChange = true;
	bool ctrlPressed = false;

	// Mouse
	QPoint m_LastMousePos;// needed to calculate the mouse delta since the last movement

};

