#ifndef HEALTHKIT_H
#define HEALTHKIT_H
#include "GameObject.h"
#include "Collisions.h"

class HealthKit : public GameObject
{
private:
	CBoundingBox m_boundingBox;
	float m_rotateSpeed;

	bool m_destory;

	void Spin(float timestep);
public:
	HealthKit(Mesh* mesh, Shader* shader, Texture* texture, Vector3 position);
	void Update(float timestep);

	~HealthKit();

	//Accessor
	CBoundingBox GetBounds() { return m_boundingBox; }
	bool GetDestoryable() { return m_destory; }

	//Mutator
	void SetDestory(bool value) { m_destory = value; }
};

#endif

