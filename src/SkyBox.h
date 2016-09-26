#pragma once

//#include "Global.h"
#include <glm/glm.hpp>

class RenderWindow;//for texture Loading

class SkyBox
{
public:
	void Init(const RenderWindow& RW);//Loads all Textures
	void Render(glm::vec3 CameraPosition);//Renders the Skybox
private:
	//Our Textures
	unsigned int m_Front, m_Back, m_Left, m_Right, m_Top, m_Bottom;
};