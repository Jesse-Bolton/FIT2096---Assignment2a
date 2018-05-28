#include "Bullet.h"
#include "MathsHelper.h"

Bullet::Bullet(Mesh* mesh, Shader* shader, Texture* texture, Vector3 position, float heading, float pitch, Vector3 direction) :
	GameObject(mesh, shader, texture, position)
{
	m_rotY = heading;
	m_rotX = pitch;
	m_localForward = Vector3::TransformNormal(Vector3(0, 0, 1), Matrix::CreateRotationX(m_rotX) * Matrix::CreateRotationY(m_rotY));
	m_movespeed = 500.0f;
	m_position += m_localForward;
	m_existTime = 0.0f;
	m_destory = false;

	m_boundingBox = CBoundingBox(
		m_position + m_mesh->GetMin(),
		m_position + m_mesh->GetMax()
	);
}

void Bullet::Update(float timestep)
{
	m_position += m_localForward * m_movespeed * timestep;

	m_boundingBox.SetMin(m_position + m_mesh->GetMin());
	m_boundingBox.SetMax(m_position + m_mesh->GetMax());

	m_existTime += timestep;
}

void Bullet::Die() 
{
	if (m_existTime >= 4.0f)
		m_destory = true;
	else
		m_destory = false;
}
