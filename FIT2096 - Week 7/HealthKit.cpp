#include "HealthKit.h"



HealthKit::HealthKit(Mesh* mesh, Shader* shader, Texture* texture, Vector3 position) :
	GameObject(mesh, shader, texture, position)
{
	m_rotateSpeed = 3.0f;
	SetUniformScale(2.0f);
	m_boundingBox = CBoundingBox(m_position + m_mesh->GetMin(), m_position + m_mesh->GetMax());
	m_destory = false;
}

void HealthKit::Spin(float timestep)
{
	m_rotY += m_rotateSpeed * timestep;
}

void HealthKit::Update(float timestep)
{
	//Spin(timestep);
}

HealthKit::~HealthKit()
{
}
