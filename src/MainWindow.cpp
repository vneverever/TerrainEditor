#include "MainWindow.h"
#include "Manager.h"
#include <QFileDialog>
#include <QMessageBox>
#include <qdebug.h>
#include <qgraphicsitem.h>

using namespace std;

MainWindow::MainWindow() :
QMainWindow()
{
	m_Ui.setupUi(this);

	m_Ui.PainterWidget->setManager(&M);
	m_Ui.RenderWidget->setManager(&M);
	M = Manager();
	M.init(m_Ui.RenderWidget, m_Ui.PainterWidget);
	//connect all slots:
	connect(&m_Timer, SIGNAL(timeout()),this, SLOT(on_Timer()));
	
	connect(m_Ui.Screenshot, SIGNAL(clicked()), SLOT(on_Screenshot()));
	connect(m_Ui.Light, SIGNAL(clicked(bool)), SLOT(toggleLight(bool)));
	connect(m_Ui.moveLightCircle, SIGNAL(clicked(bool)), SLOT(toggleLightMove(bool)));
	connect(m_Ui.specularMap, SIGNAL(clicked(bool)), SLOT(toggleSpecularMap(bool)));
	connect(m_Ui.Textures, SIGNAL(clicked(bool)), SLOT(toggleTextures(bool)));
	connect(m_Ui.NormalMap, SIGNAL(clicked(bool)), SLOT(toggleNormalMap(bool)));
	connect(m_Ui.Shadows, SIGNAL(clicked(bool)), SLOT(toggleShadows(bool)));
	connect(m_Ui.Water, SIGNAL(clicked(bool)), SLOT(toggleWater(bool)));
	connect(m_Ui.waterLevel, SIGNAL(sliderMoved(int)), SLOT(waterLevel(int)));
	connect(m_Ui.LightX, SIGNAL(valueChanged(double)), SLOT(lightX(double)));
	connect(m_Ui.LightY, SIGNAL(valueChanged(double)), SLOT(lightY(double)));
	connect(m_Ui.LightZ, SIGNAL(valueChanged(double)), SLOT(lightZ(double)));
	connect(m_Ui.textureScale, SIGNAL(valueChanged(double)), SLOT(textureScale(double)));
	connect(m_Ui.ChangeTexture, SIGNAL(clicked()), SLOT(changeTexture()));


	connect(m_Ui.fixedHeight, SIGNAL(valueChanged(int)), SLOT(drawHeight(int)));
	connect(m_Ui.fixedDraw, SIGNAL(clicked(bool)), SLOT(fixedDrawMode(bool)));
	connect(m_Ui.drawRadius, SIGNAL(valueChanged(int)), SLOT(drawWidth(int)));
	connect(m_Ui.averageHeights, SIGNAL(clicked()), SLOT(averageHeights()));
	connect(m_Ui.clearMap, SIGNAL(clicked()), SLOT(clearHMap()));


	m_Timer.start(60.f/1000);
	QGraphicsScene* scene[10];
	for (int i = 0; i < 10; i++)
		scene[i] = new QGraphicsScene();
	
	QGraphicsPixmapItem *item[10];
	item[0] = new QGraphicsPixmapItem(QPixmap::fromImage(QImage("../Framework/texture/snow2_d.jpg").scaled(100,100)));				//0
	item[1] = new QGraphicsPixmapItem(QPixmap::fromImage(QImage("../Framework/texture/snow1_d.jpg").scaled(100, 100)));				//1
	item[2] = new QGraphicsPixmapItem(QPixmap::fromImage(QImage("../Framework/texture/mntn_dark_d.jpg").scaled(100, 100)));			//2
	item[3] = new QGraphicsPixmapItem(QPixmap::fromImage(QImage("../Framework/texture/mntn_black_d.jpg").scaled(100, 100)));		//3
	item[4] = new QGraphicsPixmapItem(QPixmap::fromImage(QImage("../Framework/texture/mntn_green_d.jpg").scaled(100, 100)));		//4
	item[5] = new QGraphicsPixmapItem(QPixmap::fromImage(QImage("../Framework/texture/moss_plants_d.jpg").scaled(100, 100)));		//5
	item[6] = new QGraphicsPixmapItem(QPixmap::fromImage(QImage("../Framework/texture/grass_ground_d.jpg").scaled(100, 100)));		//6
	item[7] = new QGraphicsPixmapItem(QPixmap::fromImage(QImage("../Framework/texture/ground_ferns_d.jpg").scaled(100, 100)));		//7
	item[8] = new QGraphicsPixmapItem(QPixmap::fromImage(QImage("../Framework/texture/savanna_dry_d.jpg").scaled(100, 100)));		//8
	item[9] = new QGraphicsPixmapItem(QPixmap::fromImage(QImage("../Framework/texture/ground_crackedo_d.jpg").scaled(100, 100)));	//9
	
	for (int i = 0; i < 10; i++){
		scene[i]->addItem(item[i]);
	}
	
	m_Ui.graphicsView->setScene(scene[0]);
	m_Ui.graphicsView_2->setScene(scene[1]);
	m_Ui.graphicsView_3->setScene(scene[2]);
	m_Ui.graphicsView_4->setScene(scene[3]);
	m_Ui.graphicsView_5->setScene(scene[4]);
	m_Ui.graphicsView_6->setScene(scene[5]);
	m_Ui.graphicsView_7->setScene(scene[6]);
	m_Ui.graphicsView_8->setScene(scene[7]);
	m_Ui.graphicsView_9->setScene(scene[8]);
	m_Ui.graphicsView_10->setScene(scene[9]);

	m_Ui.graphicsView->show();
	m_Ui.graphicsView_2->show();
	m_Ui.graphicsView_3->show();
	m_Ui.graphicsView_4->show();
	m_Ui.graphicsView_5->show();
	m_Ui.graphicsView_6->show();
	m_Ui.graphicsView_7->show();
	m_Ui.graphicsView_8->show();
	m_Ui.graphicsView_9->show();
	m_Ui.graphicsView_10->show();
}

void MainWindow::on_Screenshot()
{
	QImage Image = m_Ui.RenderWidget->grabFramebuffer();// ->grabFrameBuffer(true);
	QString File=QFileDialog::getSaveFileName(this, "Save Image", "", "*.png;;*.jpg;;*.tiff;;*.bmp;;*.xpm;;*.xbm");
	if (!File.endsWith(".png", Qt::CaseInsensitive) ||
		!File.endsWith(".jpg", Qt::CaseInsensitive) ||
		!File.endsWith(".tiff", Qt::CaseInsensitive) ||
		!File.endsWith(".bmp", Qt::CaseInsensitive) ||
		!File.endsWith(".xpm", Qt::CaseInsensitive) ||
		!File.endsWith(".xbm", Qt::CaseInsensitive))
	{
		File = File + ".png";
	}
	if(!Image.save(File))
	{
		QMessageBox::critical(this, "Saving error", "Image could not be saved");
	}
}

void MainWindow::on_Timer(){
	M.RW->update();
}
