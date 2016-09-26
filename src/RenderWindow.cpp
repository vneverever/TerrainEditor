#include "RenderWindow.h"
#include "Manager.h"
#include <QMouseEvent>
#include <QOpenGLTexture>
#include <QDebug>
#include <QApplication>
#include <qopenglframebufferobject.h>


RenderWindow::RenderWindow(QWidget* Parent) :
	QOpenGLWidget(Parent)
{
	//Enable mouse tracking:
	this->setMouseTracking(true);



	QSurfaceFormat format;
	format.setVersion(4, 3);
	format.setSamples(2);
	format.setProfile(QSurfaceFormat::CoreProfile);
	format.setDepthBufferSize(16);
	//format.
	format.setSwapBehavior(QSurfaceFormat::DoubleBuffer);
	//format.setSwapInterval(1);

	setFormat(format);


	//mContext = new QOpenGLContext();
	//mContext->create();



	this->setFocus();

}

void RenderWindow::initializeGL()
{
	mContext = context();
	f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_4_3_Core>();
	f->initializeOpenGLFunctions();
	//f->glClear(GL_ALL_ATTRIB_BITS);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	f->glClearColor(0.6f, 0.6f, 0.6f, 0.5f);
	f->glEnable(GL_DEPTH_TEST);
	//f->glDepthMask(GL_TRUE);
	f->glDepthFunc(GL_LESS); // Change this to whatever kind of depth testing you want
	//f->glDepthRange(0.0f, 1.0f);
	f->glEnable(GL_FRAMEBUFFER_SRGB);
	//f->glEnable(GL_DEPTH_COMPONENT32F);
	//f->glEnable(GL_STENCIL_TEST);

	//f->glDepthFunc(GL_LESS);
	f->glEnable(GL_BLEND);
	f->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	f->glEnable(GL_CULL_FACE);
	f->glEnable(GL_POLYGON_OFFSET_FILL);
	f->glPolygonOffset(0.1f, 0.1f);
	f->glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	f->glEnable(GL_MULTISAMPLE);
	f->glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);


	vao[0] = new QOpenGLVertexArrayObject(this);
	vao[0]->create();
	vao[0]->bind();

	program = new QOpenGLShaderProgram();

	vertexShad = new QOpenGLShader(QOpenGLShader::Vertex);
	fragmentShad = new QOpenGLShader(QOpenGLShader::Fragment);


	if (vertexShad->compileSourceFile(":vs2")) qDebug() << "vs = true";
	else if (vertexShad->compileSourceFile(":vs2")) qDebug() << "vs = true";
	else {
		qDebug() << "vs = false";
		while (1);
	}
	if (fragmentShad->compileSourceFile(":fs2")) qDebug() << "fs = true";
	else if (vertexShad->compileSourceFile(":fs2")) qDebug() << "fs = true";
	else {
		qDebug() << "fs = false";
		while (1);
	}

	program->addShader(vertexShad);
	program->addShader(fragmentShad);

	program->link();
	program->bind();



	f->glGenBuffers(1, &_dataBlock);
	f->glBindBuffer(GL_UNIFORM_BUFFER, _dataBlock);
	f->glBufferData(GL_UNIFORM_BUFFER, 260, NULL, GL_DYNAMIC_DRAW); // allocate 150 bytes of memory
	f->glBindBufferBase(GL_UNIFORM_BUFFER, 4, _dataBlock);
	f->glBindBuffer(GL_UNIFORM_BUFFER, 0);

	_MVP = program->uniformLocation("MVP");
	program->uniformLocation("LightInfo");
	program->uniformLocation("ViewInverse");
	program->uniformLocation("heightScale");
	program->uniformLocation("invertHMap");

	//------------LIGHT--------------
	float ambient = 0.8f;
	float diffuse = 3.0f;
	float specular = 3.0f;

	lightPos = QVector3D(250, 250, -250);
	GLuint pos = program->uniformLocation("Light.position");
	GLuint Ld = program->uniformLocation("Light.Ld");
	GLuint Ls = program->uniformLocation("Light.Ls");
	GLuint constantAttenuation = program->uniformLocation("Light.constantAttenuation");
	GLuint linearAttenuation = program->uniformLocation("Light.linearAttenuation");
	GLuint quadraticAttenuation = program->uniformLocation("Light.quadraticAttenuation");
	GLuint spotCutoff = program->uniformLocation("Light.spotCutoff");
	GLuint spotExponent = program->uniformLocation("Light.spotExponent");
	GLuint spotDirection = program->uniformLocation("Light.spotDirection");
	GLuint sceneAmbient = program->uniformLocation("sceneAmbient");



	program->setUniformValue(pos, lightPos);
	program->setUniformValue(Ld, QVector4D(diffuse, diffuse, diffuse, 1.0f));
	program->setUniformValue(Ls, QVector4D(specular, specular, specular, 1.0f));

	program->setUniformValue(constantAttenuation, 1.0f);
	program->setUniformValue(linearAttenuation, 0.0f);
	program->setUniformValue(quadraticAttenuation, 0.0f);

	program->setUniformValue(spotCutoff, 100.f);
	program->setUniformValue(spotExponent, 0.f);
	program->setUniformValue(spotDirection, QVector3D(0.f, 0.f, 0.f));

	program->setUniformValue(sceneAmbient, QVector4D(ambient, ambient, ambient, 1.0));
	//------------LIGHT--------------


	//------------MATERIAL--------------
	GLuint shininess = program->uniformLocation("Material.Shininess");
	GLuint Ka = program->uniformLocation("Material.Ka");
	GLuint Kd = program->uniformLocation("Material.Kd");
	GLuint Ks = program->uniformLocation("Material.Ks");

	program->setUniformValue(shininess, 15.f);
	program->setUniformValue(Ka, QVector4D(1.f, 1.f, 1.f, 1.f));
	program->setUniformValue(Kd, QVector4D(0.9f, 0.9f, 0.9f, 1.0f));
	program->setUniformValue(Ks, QVector4D(0.9f, 0.9f, 0.9f, 1.0f));
	//------------MATERIAL--------------
	f->glPolygonMode(GL_FRONT, GL_FILL);

	//HeightMap
	M->setHMap("../Framework/heightmap/first.png");

	M->tex->loadSkybox("Test");

	float LODTexture = 0;
	float LODspecular = 0;
	float LODnormal = 0;

	//Textures

	M->tex->LoadTexture("../Framework/texture/snow2_d.jpg", LODTexture);		//1
	M->tex->LoadTexture("../Framework/texture/mntn_black_d.jpg", LODTexture);		//0
	M->tex->LoadTexture("../Framework/texture/mntn_dark_d.jpg", LODTexture);	//2
	M->tex->LoadTexture("../Framework/texture/mntn_black_d.jpg", LODTexture);	//3
	M->tex->LoadTexture("../Framework/texture/mntn_green_d.jpg", LODTexture);	//4
	M->tex->LoadTexture("../Framework/texture/moss_plants_d.jpg", LODTexture);	//5
	M->tex->LoadTexture("../Framework/texture/grass_ground_d.jpg", LODTexture);	//6
	M->tex->LoadTexture("../Framework/texture/ground_ferns_d.jpg", LODTexture);	//7
	M->tex->LoadTexture("../Framework/texture/savanna_dry_d.jpg", LODTexture);	//8
	M->tex->LoadTexture("../Framework/texture/desert_sand2_d.jpg", LODTexture);	//9
																				//M->tex->LoadTexture("../Framework/texture/desert_sand_big_d.jpg");
																				//M->tex->LoadTexture("../Framework/texture/desert_cracks_d.jpg");

																				/*
																				M->tex->LoadTexture("../Framework/texture/chess", LODTexture);		//0
																				M->tex->LoadTexture("../Framework/texture/chess", LODTexture);		//1
																				M->tex->LoadTexture("../Framework/texture/chess", LODTexture);		//2
																				M->tex->LoadTexture("../Framework/texture/chess", LODTexture);		//3
																				M->tex->LoadTexture("../Framework/texture/chess", LODTexture);		//4
																				M->tex->LoadTexture("../Framework/texture/chess", LODTexture);		//5
																				M->tex->LoadTexture("../Framework/texture/chess", LODTexture);		//6
																				M->tex->LoadTexture("../Framework/texture/chess", LODTexture);		//7
																				M->tex->LoadTexture("../Framework/texture/chess", LODTexture);		//8
																				M->tex->LoadTexture("../Framework/texture/chess", LODTexture);		//9
																				*/

																				//ShadowTextures
	M->tex->LoadTexture("../Framework/texture/snow2_s.jpg", LODspecular);
	M->tex->LoadTexture("../Framework/texture/mntn_black_s.jpg", LODspecular);
	M->tex->LoadTexture("../Framework/texture/mntn_dark_s.jpg", LODspecular);
	M->tex->LoadTexture("../Framework/texture/mntn_black_s.jpg", LODspecular);
	M->tex->LoadTexture("../Framework/texture/mntn_green_s.jpg", LODspecular);
	M->tex->LoadTexture("../Framework/texture/moss_plants_s.jpg", LODspecular);
	M->tex->LoadTexture("../Framework/texture/grass_ground_s.jpg", LODspecular);
	M->tex->LoadTexture("../Framework/texture/ground_ferns_s.jpg", LODspecular);
	M->tex->LoadTexture("../Framework/texture/savanna_dry_s.jpg", LODspecular);
	M->tex->LoadTexture("../Framework/texture/desert_sand2_s.jpg", LODspecular);
	//M->tex->LoadTexture("../Framework/texture/desert_sand_big_s.jpg");
	//M->tex->LoadTexture("../Framework/texture/desert_cracks_s.jpg");

	//NormalTextures
	M->tex->LoadTexture("../Framework/texture/snow2_n.jpg", LODnormal);
	M->tex->LoadTexture("../Framework/texture/mntn_black_n.jpg", LODnormal);
	M->tex->LoadTexture("../Framework/texture/mntn_dark_n.jpg", LODnormal);
	M->tex->LoadTexture("../Framework/texture/mntn_black_n.jpg", LODnormal);
	M->tex->LoadTexture("../Framework/texture/mntn_green_n.jpg", LODnormal);
	M->tex->LoadTexture("../Framework/texture/moss_plants_n.jpg", LODnormal);
	M->tex->LoadTexture("../Framework/texture/grass_ground_n.jpg", LODnormal);
	M->tex->LoadTexture("../Framework/texture/ground_ferns_n.jpg", LODnormal);
	M->tex->LoadTexture("../Framework/texture/savanna_dry_n.jpg", LODnormal);
	M->tex->LoadTexture("../Framework/texture/desert_sand2_n.jpg", LODnormal);
	//M->tex->LoadTexture("../Framework/texture/desert_sand_big_n.jpg");
	//M->tex->LoadTexture("../Framework/texture/desert_cracks_n.jpg");


	_sky = program->uniformLocation("skyUnit");

	_hMap = program->uniformLocation("hMap");

	_tex[0] = program->uniformLocation("texUnit[0]");
	_tex[1] = program->uniformLocation("texUnit[1]");
	_tex[2] = program->uniformLocation("texUnit[2]");
	_tex[3] = program->uniformLocation("texUnit[3]");
	_tex[4] = program->uniformLocation("texUnit[4]");
	_tex[5] = program->uniformLocation("texUnit[5]");
	_tex[6] = program->uniformLocation("texUnit[6]");
	_tex[7] = program->uniformLocation("texUnit[7]");
	_tex[8] = program->uniformLocation("texUnit[8]");
	_tex[9] = program->uniformLocation("texUnit[9]");

	_shadow[0] = program->uniformLocation("shadowUnit[0]");
	_shadow[1] = program->uniformLocation("shadowUnit[1]");
	_shadow[2] = program->uniformLocation("shadowUnit[2]");
	_shadow[3] = program->uniformLocation("shadowUnit[3]");
	_shadow[4] = program->uniformLocation("shadowUnit[4]");
	_shadow[5] = program->uniformLocation("shadowUnit[5]");
	_shadow[6] = program->uniformLocation("shadowUnit[6]");
	_shadow[7] = program->uniformLocation("shadowUnit[7]");
	_shadow[8] = program->uniformLocation("shadowUnit[8]");
	_shadow[9] = program->uniformLocation("shadowUnit[9]");

	_normal[0] = program->uniformLocation("normalUnit[0]");
	_normal[1] = program->uniformLocation("normalUnit[1]");
	_normal[2] = program->uniformLocation("normalUnit[2]");
	_normal[3] = program->uniformLocation("normalUnit[3]");
	_normal[4] = program->uniformLocation("normalUnit[4]");
	_normal[5] = program->uniformLocation("normalUnit[5]");
	_normal[6] = program->uniformLocation("normalUnit[6]");
	_normal[7] = program->uniformLocation("normalUnit[7]");
	_normal[8] = program->uniformLocation("normalUnit[8]");
	_normal[9] = program->uniformLocation("normalUnit[9]");


	M->tex->cubemap->bind(30);

	for (int i = 0; i < 10; i++) {
		program->setUniformValue(_tex[i], i);
	}
	for (int i = 10; i < 20; i++) {
		program->setUniformValue(_shadow[i - 10], i);
	}
	for (int i = 20; i < 30; i++) {
		program->setUniformValue(_normal[i - 20], i);
	}
	program->setUniformValue(_hMap, 31);

	for (int i = 0; i < 30; i++) {
		M->tex->textureMap[i]->bind(i);
	}
	program->setUniformValue(_sky, 30);
	M->tex->cubemap->bind(30);

	initTextureValues();
	program->setUniformValue("LightFlag", true);
	program->setUniformValue("SpecularFlag", false);
	program->setUniformValue("NormalMapFlag", false);
	program->setUniformValue("ShadowFlag", false);
	program->setUniformValue("TextureFlag", true);
	program->setUniformValue("TextureScale", 0.5f);

	vao[0]->release();

	groundVAO();
	skyVAO();
	waterVAO();

	vao[4] = new QOpenGLVertexArrayObject(this);
	vao[4]->create();
	vao[4]->bind();

	rayProgram = new QOpenGLShaderProgram();

	rayVertexShader = new QOpenGLShader(QOpenGLShader::Vertex);
	rayFragmentShader = new QOpenGLShader(QOpenGLShader::Fragment);
	rayGeometryShader = new QOpenGLShader(QOpenGLShader::Geometry);

	if (rayVertexShader->compileSourceFile("../Framework/shader/ray.v")) qDebug() << "rayVs = true";
	else {
		qDebug() << "rayVs = false";
		while (1);
	}
	if (rayGeometryShader->compileSourceFile("../Framework/shader/ray.g")) qDebug() << "rayGs = true";
	else {
		qDebug() << "rayGs = false";
		while (1);
	}
	if (rayFragmentShader->compileSourceFile("../Framework/shader/ray.f")) qDebug() << "rayFs = true";
	else {
		qDebug() << "rayFs = false";
		while (1);
	}

	rayProgram->addShader(rayVertexShader);
	rayProgram->addShader(rayGeometryShader);
	rayProgram->addShader(rayFragmentShader);

	rayProgram->link();
	rayProgram->bind();

}

void RenderWindow::resizeGL(int Width, int Height)
{
	static int oldHeight = Height;
	f->glViewport(0, 0, Width, Height);
	QMatrix4x4 pMat;
	pMat.setToIdentity();
	float ratio = float(Width / Height);
	pMat.perspective(M->con->getFov()*Height / oldHeight, (float)Width / Height, M->con->getNearZ(), M->con->getFarZ());
	//pMat.ortho(-Width/16, Width / 16,-Height / 16,Height / 16, M->con->getNearZ(), M->con->getFarZ());

	M->con->setProjectionMatrix(pMat);
	/*
	m_projection.setToIdentity();
	m_projection.perspective(60.0f, (float)w / h, .3f, 1000);
	*/
	oldHeight = Height;


}

void RenderWindow::initBuffer() {
	vao[0]->bind();
	program->bind();

	printf("TSIZE: %d\n", M->ter->getTSize());
	tVertexBuffer = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
	tVertexBuffer.create();
	tVertexBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
	tVertexBuffer.bind();
	tVertexBuffer.allocate(&M->ter->vertices, (M->ter->getTSize() * M->ter->getTSize() * 3 + 4) * sizeof(QVector2D));


	tMapBuffer = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
	tMapBuffer.create();
	tMapBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
	tMapBuffer.bind();
	tMapBuffer.allocate(M->ter->hMapIndex, M->ter->getTSize() * M->ter->getTSize() * sizeof(QVector2D));

	tIndexBuffer = QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
	tIndexBuffer.create();
	tIndexBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
	tIndexBuffer.bind();
	tIndexBuffer.allocate(M->ter->index, (M->ter->getTSize() * 6 * M->ter->getTSize() + 6) * sizeof(GLuint));


	tVertexBuffer.bind();
	program->enableAttributeArray(0);
	program->setAttributeBuffer(0, GL_FLOAT, 0, 2);

	tMapBuffer.bind();
	program->enableAttributeArray(3);
	program->setAttributeBuffer(3, GL_FLOAT, 0, 2);
	vao[0]->release();

	vao[1]->bind();
	program2->bind();


	tVertexBuffer2 = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
	tVertexBuffer2.create();
	tVertexBuffer2.setUsagePattern(QOpenGLBuffer::StaticDraw);
	tVertexBuffer2.bind();
	tVertexBuffer2.allocate(M->ter->verticesGround, (M->ter->getTSize() - 1) * 4 * 2 * sizeof(QVector2D));

	tIndexBuffer2 = QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
	tIndexBuffer2.create();
	tIndexBuffer2.setUsagePattern(QOpenGLBuffer::StaticDraw);
	tIndexBuffer2.bind();
	tIndexBuffer2.allocate(M->ter->indexGround, ((M->ter->getTSize() - 1) * 4 * 6 + 6) * sizeof(GLuint));

	tVertexBuffer2.bind();
	program2->enableAttributeArray(0);
	program2->setAttributeBuffer(0, GL_FLOAT, 0, 2);
	vao[1]->release();

	vao[3]->bind();
	program4->bind();

	waterVerts[0] = QVector4D(0, waterLevel, 0, 1);
	waterVerts[1] = QVector4D(102.3f, waterLevel, 0, 1);
	waterVerts[2] = QVector4D(102.3f, waterLevel, -102.3f, 1);

	waterVerts[3] = QVector4D(102.3f, waterLevel, -102.3f, 1);
	waterVerts[4] = QVector4D(0, waterLevel, -102.3f, 1);
	waterVerts[5] = QVector4D(0, waterLevel, 0, 1);

	waterVertexBuffer = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
	waterVertexBuffer.create();
	waterVertexBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
	waterVertexBuffer.bind();
	waterVertexBuffer.allocate(waterVerts, 6 * sizeof(QVector4D));

	program4->enableAttributeArray(0);
	program4->setAttributeBuffer(0, GL_FLOAT, 0, 4);
	vao[3]->release();


	vao[4]->bind();
	rayProgram->bind();

	rayStart = QVector4D(0, 20.f, 0, 1);

	rayBuffer = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
	rayBuffer.create();
	rayBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
	rayBuffer.bind();
	rayBuffer.allocate(&rayStart, sizeof(QVector4D));

	rayProgram->enableAttributeArray(0);
	rayProgram->setAttributeBuffer(0, GL_FLOAT, 0, 4);
	vao[4]->release();

}

void RenderWindow::paintGL()
{
	if (!rdy) return;
	makeCurrent();
	f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_4_3_Core>();
	f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (hMapChange) {
		initBuffer();
		hMapChange = false;
		M->tex->setHeightMap(M->PW->hImage);
		//M->mPixels = new GLuint[M->ter->getTSize() * M->ter->getTSize()];
		printf("Change\n");
	}
	else {
		M->con->computeView();
	}

	
	//t.start();

	GLfloat flo = M->ter->getHeightScale();
	GLint invert = M->ter->isTerrainInverted() == true ? 1 : 0;
	MVP = M->con->getProjectionMatrix() * M->con->getViewMatrix();
	ProjectionMatrix = M->con->getProjectionMatrix();
	ViewMatrix = M->con->getViewMatrix();
	f->glBindBuffer(GL_UNIFORM_BUFFER, _dataBlock);
	f->glBufferSubData(GL_UNIFORM_BUFFER, 0, 64, &MVP);
	f->glBufferSubData(GL_UNIFORM_BUFFER, 64, 4, &flo);
	f->glBufferSubData(GL_UNIFORM_BUFFER, 68, 4, &invert);
	f->glBufferSubData(GL_UNIFORM_BUFFER, 80, 16, &M->con->getCamPosition());
	f->glBufferSubData(GL_UNIFORM_BUFFER, 96, 16, &lightPos);
	f->glBufferSubData(GL_UNIFORM_BUFFER, 112, 64, &ProjectionMatrix);
	f->glBufferSubData(GL_UNIFORM_BUFFER, 176, 64, &ViewMatrix);

	//printf("After Uniform Buffer: %i\n", t.elapsed());

	f->glDepthMask(GL_FALSE);
	vao[2]->bind();
	program3->bind();
	f->glDrawArrays(GL_TRIANGLES, 0, 6);
	program3->release();
	vao[2]->release();
	f->glDepthMask(GL_TRUE);


	vao[1]->bind();
	program2->bind();
	//printf("After Sky: %i\n", t.elapsed());
	f->glDrawElements(GL_TRIANGLES, (M->ter->getTSize() - 1) * 4 * 6 + 6, GL_UNSIGNED_INT, 0);
	program2->release();
	vao[1]->release();

	//printf("After Ground: %i\n", t.elapsed());

	vao[0]->bind();
	program->bind();


	if (M->refreshHmap) {
		M->tex->hMap->bind(31);
		M->tex->hMap->setData(0, 0, QOpenGLTexture::PixelFormat::RGBA, QOpenGLTexture::PixelType::UInt8, M->PW->hImage.bits());
		M->refreshHmap = false;
		//glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, M->mPixels);
		M->PW->update();
	}


	static float angle = 0.0f;
	static float distance = 10.0f;
	static float d = 0.25f;
	if (flagLightMove) {
		angle += 0.05f;
		distance += d;
		if (distance > 70 || distance < -70.0) d = -d;
		if (angle > M_PI * 2)angle = 0.0f;

		lightPos = QVector3D(lightPos.x() + d *sin(angle), lightPos.y(), lightPos.z() + d *cos(angle));
	}

	f->glDrawElements(GL_TRIANGLES, (M->ter->getTSize() - 1) * (M->ter->getTSize() - 1) * 6, GL_UNSIGNED_INT, 0);

	program->release();
	vao[0]->release();


	//printf("After Terrain: %i\n", t.elapsed());
	//f->glClearDepthf(1.f);

	if (waterFlag) {
		vao[3]->bind();
		program4->bind();
		program4->setUniformValue("waterLevel", waterLevel);
		static float timer = 0.4f;
		static float interval = 0.01f;
		static float timer2 = 0.4f;
		static float interval2 = 0.05f;


		timer = timer + interval;
		timer2 = timer2 + interval2;
		if (timer > 0.5 || timer < 0.3) interval = -interval;
		if (timer2 > M_PI * 2) timer2 = 0;
		program4->setUniformValue("timer", timer);
		program4->setUniformValue("timer2", timer2);

		f->glDrawArrays(GL_TRIANGLES, 0, 6);

		program4->release();
		vao[3]->release();
	}
	//printf("After Water: %i\n", t.elapsed());
	vao[4]->bind();
	rayProgram->bind();
	//rayProgram->setUniformValue("start", rayStartMouse);
	rayProgram->setUniformValue("hit", rayHit);
	f->glDrawArrays(GL_POINTS, 0, 1);
	vao[4]->release();
	

}

void RenderWindow::mouseMoveEvent(QMouseEvent* event)
{
	if (event->x() < 0 || event->y() < 0 || event->x() > width() || event->y() > height()) return;
	else if (event->buttons() & Qt::LeftButton && !ctrlPressed)
	{
		keyboardGrabber();
		QPoint MouseDelta = event->pos() - m_LastMousePos;

		M->con->computeView(MouseDelta);
	}
	else if ((event->buttons() & Qt::LeftButton || event->buttons() & Qt::RightButton) && ctrlPressed) {
		//M->con->picking(event->pos());
		//float nearPlaneLocation[3];

		QVector3D cam = M->con->getCamPosition();
		if (cam.y() < 0) return;

		float x = (2.0f * event->x()) / width() - 1.0f;
		float y = 1.0f - (2.0f * event->y()) / height();
		float z = 1.0f;
		QVector3D ray_nds = QVector3D(x, y, z);
		QVector4D ray_clip = QVector4D(QVector2D(ray_nds.x(), ray_nds.y()), -1.0, 1.0);
		QVector4D ray_eye = M->con->getProjectionMatrix().inverted() * ray_clip;
		ray_eye = QVector4D(ray_eye.x(), ray_eye.y(), -1.0, 0.0);
		QVector3D ray_wor = QVector3D(M->con->getViewMatrix().inverted() * ray_eye);
		// don't forget to normalise the vector at some point
		ray_wor.normalize();
		//printf("RAY: %f, %f, %f\n", ray_wor.x(), ray_wor.y(), ray_wor.z());
		rayStartMouse = ray_wor;

		boolean hit = false;

		if (ray_wor.y() >= 0) return;

		if (cam.y() > M->ter->getHeightScale()) {
			cam = cam + ray_wor*(cam.y() - M->ter->getHeightScale());
		}

		ray_wor *= 0.1f;

		float hScale = M->ter->getHeightScale();
		ray_wor.setY((ray_wor.y()/hScale)*0xFFFF);
		cam.setY((cam.y() / hScale)*0xFFFF);
		//int width = M->PW->drawWidth / 2;
		while (!hit) {
			cam += ray_wor;
			//printf("HEIGHT: %f\n", (M->tra->getValue(cam.x(), cam.z()) / float(0xFFFF)) * hScale);
			//printf("CAM: %f, %f, %f\n", cam.x(),cam.y(),cam.z());
			if (cam.y() <= M->tra->getValue(cam.x() * 10, cam.z() * 10)) {
				//printf("CAM: %f,%f,%f\n", cam.x(), cam.y(), -cam.z());
				//if (cam.y() < -1.f) return;
				rayHit = QVector4D(cam.x(), (cam.y()/0xFFFF)*hScale, cam.z(),1.0);
				cam *= 5;
				//M->PW->mouseMoveEvent(new QMouseEvent(event->type(),QPointF(cam.x(),-cam.z()),event->button(),event->buttons(),event->modifiers()));
				M->refreshHmap = true;
				hit = true;
			}
		}
		//M->PW->lastPos = event->pos();
	}
	else if (ctrlPressed && true) {
		//rayHit = 
	}

	m_LastMousePos = event->pos();
	
}

void RenderWindow::mousePressEvent(QMouseEvent* event) {
	if (event->x() < 0 || event->y() < 0 || event->x() > width() || event->y() > height()) return;
	else if ((event->buttons() & Qt::LeftButton || event->buttons() & Qt::RightButton) && ctrlPressed) {
		//M->con->picking(event->pos());
		//float nearPlaneLocation[3];
		QVector3D cam = M->con->getCamPosition();
		if (cam.y() < 0) return;

		float x = (2.0f * event->x()) / width() - 1.0f;
		float y = 1.0f - (2.0f * event->y()) / height();
		float z = 1.0f;
		QVector3D ray_nds = QVector3D(x, y, z);
		QVector4D ray_clip = QVector4D(QVector2D(ray_nds.x(), ray_nds.y()), -1.0, 1.0);
		QVector4D ray_eye = M->con->getProjectionMatrix().inverted() * ray_clip;
		ray_eye = QVector4D(ray_eye.x(), ray_eye.y(), -1.0, 0.0);
		QVector3D ray_wor = QVector3D(M->con->getViewMatrix().inverted() * ray_eye);
		// don't forget to normalise the vector at some point
		ray_wor.normalize();
		//printf("RAY: %f, %f, %f\n", ray_wor.x(), ray_wor.y(), ray_wor.z());
		rayStartMouse = ray_wor;
		if (ray_wor.y() >= 0) return;

		boolean hit = false;
		if (cam.y() > M->ter->getHeightScale()) {
			cam = cam + ray_wor*(cam.y() - M->ter->getHeightScale());
		}
		//cam.setX(cam.x()*10);
		//cam.setZ(cam.z()*10);

		ray_wor *= 0.1f;
		float hScale = M->ter->getHeightScale();
		while (!hit) {
			cam += ray_wor;
			//printf("HEIGHT: %f\n", (M->tra->getValue(cam.x(), cam.z()) / float(0xFFFF)) * hScale);
			//printf("CAM: %f, %f, %f\n", cam.x(),cam.y(),cam.z());
			if (cam.y() <= (M->tra->getValue(cam.x()*10, cam.z()*10) / float(0xFFFF)) * hScale) {
				//printf("CAM: %f,%f,%f\n", cam.x(), cam.y(), -cam.z());
				if (cam.y() < -1.f) return;
				cam *= 5;
				//M->PW->mousePressEvent(new QMouseEvent(event->type(), QPointF(cam.x(), -cam.z()), event->button(), event->buttons(), event->modifiers()));
				M->refreshHmap = true;
				hit = true;
			}
		}
		//M->PW->lastPos = event->pos();
	}
}

void RenderWindow::wheelEvent(QWheelEvent* event)
{

	if (event->angleDelta().y() > 0) M->con->setZoom(M->con->getZoom() + M->con->getSpeed());
	else if (event->angleDelta().y() < 0) M->con->setZoom(M->con->getZoom() - M->con->getSpeed());;


	update();

}

void RenderWindow::timerUpdate()
{

	//if (!m_Timer) return; //dont update when timer is off
}

void RenderWindow::setManager(Manager *manager) {
	M = manager;
};

void RenderWindow::keyPressEvent(QKeyEvent* event) {
	if (event->key() == Qt::Key::Key_Control) {
		ctrlPressed = true;
	}
	M->con->enableKey(event->key());
	M->con->singleEvent(event->key());
	event->accept();
	update();
}

void RenderWindow::keyReleaseEvent(QKeyEvent* event) {
	if (event->key() == Qt::Key::Key_Control) {
		ctrlPressed = false;
	}
	M->con->disableKey(event->key());
	update();
}

void RenderWindow::groundVAO() {

	vao[1] = new QOpenGLVertexArrayObject(this);
	vao[1]->create();
	vao[1]->bind();

	vertexShaderGround = new QOpenGLShader(QOpenGLShader::Vertex);
	fragmentShaderGround = new QOpenGLShader(QOpenGLShader::Fragment);

	if (vertexShaderGround->compileSourceFile(":vsGround")) qDebug() << "vs = true";
	else {
		qDebug() << "vs = false";
		while (1);
	}
	if (fragmentShaderGround->compileSourceFile(":fsGround")) qDebug() << "fs = true";
	else {
		qDebug() << "fs = false";
		while (1);
	}


	program2 = new QOpenGLShaderProgram();

	program2->addShader(vertexShaderGround);
	program2->addShader(fragmentShaderGround);

	program2->link();
	program2->bind();


	program2->setUniformValue("ground", 9);
	program2->setUniformValue("hMap", 31);

	vao[1]->release();


}

void RenderWindow::waterVAO() {
	vao[3] = new QOpenGLVertexArrayObject(this);
	vao[3]->create();
	vao[3]->bind();

	waterLevel = 2;
	waterVertexShader = new QOpenGLShader(QOpenGLShader::Vertex);
	waterFragmentShader = new QOpenGLShader(QOpenGLShader::Fragment);

	if (waterVertexShader->compileSourceFile(":vsWater")) qDebug() << "WaterVS = true";
	else {
		qDebug() << "vsWater = false";
		while (1);
	}
	if (waterFragmentShader->compileSourceFile(":fsWater")) qDebug() << "WaterFS = true";
	else {
		qDebug() << "fsWater = false";
		while (1);
	}


	program4 = new QOpenGLShaderProgram();

	program4->addShader(waterVertexShader);
	program4->addShader(waterFragmentShader);

	program4->link();
	program4->bind();


	//program4->setUniformValue("water", 20);
	program4->setUniformValue("hMap", 31);
	program4->setUniformValue("sky", 30);
	vao[3]->release();

}

void RenderWindow::skyVAO() {
	vao[2] = new QOpenGLVertexArrayObject(this);
	vao[2]->create();
	vao[2]->bind();

	skyVertexShader = new QOpenGLShader(QOpenGLShader::Vertex);
	skyFragmentShader = new QOpenGLShader(QOpenGLShader::Fragment);

	if (skyVertexShader->compileSourceFile(":vsSky")) qDebug() << "SkyVS = true";
	else {
		qDebug() << "vsSky = false";
		while (1);
	}
	if (skyFragmentShader->compileSourceFile(":fsSky")) qDebug() << "SkyFS = true";
	else {
		qDebug() << "fsSky = false";
		while (1);
	}


	program3 = new QOpenGLShaderProgram();

	program3->addShader(skyVertexShader);
	program3->addShader(skyFragmentShader);

	program3->link();
	program3->bind();

	skyVerts[0] = QVector4D(-1, -1, 0.0, 1);
	skyVerts[1] = QVector4D(1, -1, 0.0, 1);
	skyVerts[2] = QVector4D(1, 1, 0.0, 1);

	skyVerts[3] = QVector4D(-1, -1, 0.0, 1);
	skyVerts[4] = QVector4D(1, 1, 0.0, 1);
	skyVerts[5] = QVector4D(-1, 1, 0.0, 1);

	tVertexBuffer3 = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
	tVertexBuffer3.create();
	tVertexBuffer3.setUsagePattern(QOpenGLBuffer::StaticDraw);
	tVertexBuffer3.bind();
	tVertexBuffer3.allocate(skyVerts, 6 * sizeof(QVector4D));

	tVertexBuffer3.bind();
	program3->enableAttributeArray(0);
	program3->setAttributeBuffer(0, GL_FLOAT, 0, 4);


	program3->setUniformValue("skyUnit", 30);

	vao[2]->release();
}




void RenderWindow::initTextureValues() {
	texturesAtLevel[0] = QVector3D(8, 9, 8);
	texturesAtLevel[1] = QVector3D(8, 6, 8);
	texturesAtLevel[2] = QVector3D(6, 7, 6);
	texturesAtLevel[3] = QVector3D(4, 5, 4);
	texturesAtLevel[4] = QVector3D(2, 4, 2);
	texturesAtLevel[5] = QVector3D(2, 3, 2);
	texturesAtLevel[6] = QVector3D(2, 1, 3);
	texturesAtLevel[7] = QVector3D(3, 1, 3);
	texturesAtLevel[8] = QVector3D(1, 0, 3);
	texturesAtLevel[9] = QVector3D(1, 0, 1);

	_texLevel[0] = program->uniformLocation("texLevel[0]");
	_texLevel[1] = program->uniformLocation("texLevel[1]");
	_texLevel[2] = program->uniformLocation("texLevel[2]");
	_texLevel[3] = program->uniformLocation("texLevel[3]");
	_texLevel[4] = program->uniformLocation("texLevel[4]");
	_texLevel[5] = program->uniformLocation("texLevel[5]");
	_texLevel[6] = program->uniformLocation("texLevel[6]");
	_texLevel[7] = program->uniformLocation("texLevel[7]");
	_texLevel[8] = program->uniformLocation("texLevel[8]");
	_texLevel[9] = program->uniformLocation("texLevel[9]");

	program->bind();
	for (int i = 0; i < 10; i++) {
		program->setUniformValue(_texLevel[i], texturesAtLevel[i]);
	}
}