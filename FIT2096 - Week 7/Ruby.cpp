#include "Ruby.h"



Ruby::Ruby(Mesh* mesh, Shader* shader, Texture* texture, Vector3 position):
	GameObject(mesh, shader, texture, position)
{
	m_rotateSpeed = 3.0f;
	SetUniformScale(2.0f);
	m_boundingBox = CBoundingBox(m_position + m_mesh->GetMin(), m_position + m_mesh->GetMax());
	m_destory = false;
}

void Ruby::Spin(float timestep) 
{
	m_rotY += m_rotateSpeed * timestep;
}

void Ruby::Update(float timestep)
{
	//Spin(timestep);
}

Ruby::~Ruby()
{
}
