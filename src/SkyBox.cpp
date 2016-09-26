#include "SkyBox.h"
//#include "gl_3.3.h"
#include "RenderWindow.h"

void SkyBox::Init(const RenderWindow& RW)
{
	/*in this files, z and y are flipped
	m_Front=RW.LoadTexture(QString(":posz.jpg"));
	
	m_Back=RW.LoadTexture(":negz.jpg");
	
	m_Left=RW.LoadTexture(":posx.jpg");//here pos and neg is flipped
	
	m_Right=RW.LoadTexture(":negx.jpg");
	
	m_Bottom=RW.LoadTexture(":negy.jpg");
	
	m_Top=RW.LoadTexture(":posy.jpg");
	*/
}

void SkyBox::Render(glm::vec3 CameraPosition)
{
	
	//int textures[6] = { m_Front, m_Left, m_Back, m_Right, m_Top, m_Bottom, };

}