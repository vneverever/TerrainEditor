#pragma once
#include <qopenglfunctions_3_3_core.h>
#include <qimage.h>
#include <qopengltexture.h>
#include "Manager.h"


class TextureUtil : QObject{
public:
	void init(Manager *manager);
	unsigned int LoadTexture(QString FileName, float LODbias)const;
	void generate2DTextureArray();
	void setHeightMap(QImage img);
	void loadSkybox(QString FileName);

	mutable QMap<int, QOpenGLTexture*> textureMap;
	QOpenGLTexture *textureArray;
	QOpenGLTexture *cubemap;
	QOpenGLTexture *hMap;
	GLuint _hMap;
private:
	Manager *M;
};