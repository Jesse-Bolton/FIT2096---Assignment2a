#include "AmmoBlock.h"



AmmoBlock::AmmoBlock(Mesh* mesh, Shader* shader, Texture* texture, Vector3 position):
	StaticObject(mesh, shader, texture, position)
{
	SetUniformScale(5.0f);
	m_boundingBox = CBoundingBox(m_position + mesh->GetMin(), m_position + mesh->GetMax());
	m_destory = false;
	
}


AmmoBlock::~AmmoBlock()
{
}
