#include "TextureUtil.h"
#include "Manager.h"

void TextureUtil::init(Manager *manager){
	M = manager;
	hMap = new QOpenGLTexture(QOpenGLTexture::Target2D);
	
}


unsigned int TextureUtil::LoadTexture(QString FileName, float LODbias) const
{
	static int id = 0;

	QImage img = QImage(FileName);// .mirrored();
	if (img.isNull()) printf("File:%d not loaded.\n",id);

	QOpenGLTexture *texture = new QOpenGLTexture(QOpenGLTexture::Target2D);
	if(id<=9) texture->setFormat(QOpenGLTexture::TextureFormat::SRGB8);
	//texture->setMipLevels(1000);
	texture->setData(img);
	texture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
	texture->setMagnificationFilter(QOpenGLTexture::LinearMipMapLinear);
	texture->setMaximumAnisotropy(1000.f);
	texture->setLevelOfDetailRange(-1000.f, 1000.f);
	texture->setLevelofDetailBias(LODbias);
	texture->generateMipMaps();
	
	textureMap.insert(id, texture);
	id++;

	return 0;
}

void TextureUtil::loadSkybox(QString FileName){

	QImage images[6];
	images[0].load("../Framework/skybox/Right2.png");
	images[1].load("../Framework/skybox/Up2.png");
	images[2].load("../Framework/skybox/Front2.png");
	images[3].load("../Framework/skybox/Left2.png");
	images[4].load("../Framework/skybox/Down2.png");
	images[5].load("../Framework/skybox/Back2.png");

	for (int i = 0; i < 6; i++){
		images[i] = images[i].scaled(1024, 1024,Qt::KeepAspectRatio).convertToFormat(QImage::Format::Format_RGB32);
	}
	//images[0].mirrored();
	//images[1].mirrored();
	//images[2].mirrored();
	//images[3].mirrored();
	//images[4].mirrored();
	//images[5].mirrored();


	cubemap = new QOpenGLTexture(QOpenGLTexture::TargetCubeMap);
	cubemap->setSize(1024, 1024);
	cubemap->setFormat(QOpenGLTexture::TextureFormat::SRGB8_Alpha8);
	cubemap->allocateStorage();
	//cubemap->setLevelofDetailBias(2.f);
	cubemap->setMinMagFilters(QOpenGLTexture::Nearest, QOpenGLTexture::Nearest);

	QOpenGLTexture::PixelType pixelType = QOpenGLTexture::PixelType::UInt8;
	QOpenGLTexture::PixelFormat pixelFormat = QOpenGLTexture::PixelFormat::BGRA;

	cubemap->setData(0, 0, QOpenGLTexture::CubeMapPositiveX, pixelFormat, pixelType, images[3].bits());
	cubemap->setData(0, 0, QOpenGLTexture::CubeMapPositiveY, pixelFormat, pixelType, images[1].bits());
	cubemap->setData(0, 0, QOpenGLTexture::CubeMapPositiveZ, pixelFormat, pixelType, images[2].bits());
	cubemap->setData(0, 0, QOpenGLTexture::CubeMapNegativeX, pixelFormat, pixelType, images[0].bits());
	cubemap->setData(0, 0, QOpenGLTexture::CubeMapNegativeY, pixelFormat, pixelType, images[4].bits());
	cubemap->setData(0, 0, QOpenGLTexture::CubeMapNegativeZ, pixelFormat, pixelType, images[5].bits());

	cubemap->create();
}

void TextureUtil::generate2DTextureArray(){
	return;
	this->textureArray = new QOpenGLTexture(QOpenGLTexture::Target2DArray);
	int s = 0;
	for (int i = 0; i < textureMap.values().size(); i++){
		s += sizeof(textureMap[i]);
	}
	textureArray->setFormat(QOpenGLTexture::TextureFormat::SRGB8);
	textureArray->setSize(s);
	for (int i = 0; i < textureMap.values().size(); i++){
		textureArray->setData(0, i, QOpenGLTexture::RGBA, QOpenGLTexture::UInt8, textureMap[i]);
	}
	textureArray->setMinMagFilters(QOpenGLTexture::Linear, QOpenGLTexture::Linear);
	textureArray->setWrapMode(QOpenGLTexture::ClampToEdge);
}

void TextureUtil::setHeightMap(QImage img){
	static boolean init = true;
	if (init){
		_hMap = hMap->textureId();
		QElapsedTimer t;
		t.start();
		printf("Start: %i\n", t.elapsed());
		hMap->destroy();
		printf("afterDestroy: %i\n", t.elapsed());
		

		hMap = new QOpenGLTexture(img);
		printf("afterSetData: %i\n", t.elapsed());
		printf("afterTexID: %i\n", t.elapsed());
		hMap->setMinMagFilters(QOpenGLTexture::Filter::Linear, QOpenGLTexture::Linear);
		printf("afterFilters: %i\n", t.elapsed());
		hMap->setWrapMode(QOpenGLTexture::WrapMode::ClampToEdge);
		printf("AfterWrapMode: %i\n", t.elapsed());
		hMap->setBorderColor(Qt::black);
		printf("AfterBorderCOlor: %i\n", t.elapsed());
		init = false;
	}
	return;
	//QGLPixelBuffer
}