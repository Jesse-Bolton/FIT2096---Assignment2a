#ifndef BULLET_H
#define BULLET_H

#include "GameObject.h"

class Bullet: public GameObject
{
private:
	float m_heading;
	float m_pitch;
	Vector3 m_localForward;
	float m_movespeed;
	CBoundingBox m_boundingBox;
	float m_travelDistance;
	float m_existTime;
	bool m_destory;

public:
	Bullet() :GameObject() {};
	Bullet(Mesh* mesh, Shader* shader, Texture* texture, Vector3 position, float heading, float pitch, Vector3 direction);

	void Die();		//	unUsed

	void Update(float timestep);

	// Accessor
	CBoundingBox GetBounds() { return m_boundingBox; }
	bool Destoryable() { return m_destory; }

	// Mutator
	void DestoryBullet() { m_destory = true; }
};

#endif