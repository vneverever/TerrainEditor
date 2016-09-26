#pragma once

#include <QMainWindow.h>
#include <QTimer>
#include "ui_MainWindow.h"
#include "Manager.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT
public:
	MainWindow();
	Ui::MainWindow m_Ui;
	Manager M;
public Q_SLOTS:
	
	void on_Screenshot();
	void on_Timer();

	void toggleLight(bool enable){ M.RW->setLight(enable);};
	void toggleLightMove(bool enable){ M.RW->setLightMove(enable); };
	void toggleTextures(bool enable){ M.RW->setTexture(enable); };
	void toggleSpecularMap(bool enable){ M.RW->setSpecularMap(enable); };
	void toggleNormalMap(bool enable){ M.RW->setNormalMap(enable); };
	void toggleShadows(bool enable){ M.RW->setShadows(enable); };
	void toggleWater(bool enable){ M.RW->setWater(enable); };
	void waterLevel(int level){ M.RW->setWaterLevel(level); };

	void textureLevel(int level){
		
	}

	void lightX(double d){ M.RW->setLightX(d); };
	void lightY(double d){ M.RW->setLightY(d); };
	void lightZ(double d){ M.RW->setLightZ(d); };
	void toggleFirstPerson(bool enable){ M.RW->setFirstPerson(enable); };
	void textureScale(double d){ M.RW->setTextureScale(d); };
	
	void changeTexture(){
		int level = m_Ui.spinBox->value();

		QVector3D t;
		t = QVector3D(0, 0, 0);
		t.setX(m_Ui.TextureXZ->value());
		t.setY(m_Ui.TextureXY->value());
		t.setZ(m_Ui.TextureYZ->value());

		M.RW->setTextureLevel(level, t);
	};

	void fixedDrawMode(bool enable){ M.PW->fixedDraw(enable); };
	void drawHeight(int d){ M.PW->setDrawHeight(d); };
	void drawWidth(int width){ M.PW->setBrushWidth(width); };
	void averageHeights(){ M.ter->avgHeights(); };
	void clearHMap(){ M.clearMap(); };
	
private:

	QTimer m_Timer;
};