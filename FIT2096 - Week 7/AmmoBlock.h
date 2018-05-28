#ifndef AMMO_BLOCK_H
#define AMMO_BLOCK_H
#include "StaticObject.h"
#include "Collisions.h"

class AmmoBlock:public StaticObject
{
private:
	CBoundingBox m_boundingBox;
	bool m_destory;
public:
	AmmoBlock(Mesh* mesh, Shader* shader, Texture* texture, Vector3 Position);
	// Accessor
	bool GetDestoryable() { return m_destory; }
	CBoundingBox GetBounds() { return m_boundingBox; }

	// Mutator
	void SetDestoryable(bool value) { m_destory = value; }

	~AmmoBlock();
};
#endif

