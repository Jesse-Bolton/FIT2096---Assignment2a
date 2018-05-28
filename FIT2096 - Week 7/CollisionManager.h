#ifndef COLLISION_MANAGER_H
#define COLLISION_MANAGER_H

#include <vector>
#include "Collisions.h"
#include "Bullet.h"
#include "Ruby.h"
#include "Enemy.h"
#include "FirstPersonCharacter.h"
#include "AmmoBlock.h"
#include "HealthKit.h"

#define MAX_ALLOWED_COLLISIONS 2048

class CollisionManager
{
private:
	std::vector<Bullet*>* m_bullets;
	std::vector<Ruby*>* m_rubys;
	std::vector<Enemy*>* m_enemies;
	std::vector<AmmoBlock*>* m_ammoBlocks;
	std::vector<HealthKit*>* m_healthKits;
	FirstPersonCharacter* m_player;

	GameObject* m_currentCollisions[MAX_ALLOWED_COLLISIONS];

	// We need to know what objects were colliding last frame so we can determine if a collision has just begun or ended
	GameObject* m_previousCollisions[MAX_ALLOWED_COLLISIONS];

	int m_nextCurrentCollisionSlot;

	// Check if we already know about two objects colliding
	bool ArrayContainsCollision(GameObject* arrayToSearch[], GameObject* first, GameObject* second);

	// Register that a collision has occurred
	void AddCollision(GameObject* first, GameObject* second);

	// Collision check helpers
	void BulletsToPlayer();
	void BulletsToEnemy();
	void PlayerToRuby();
	void PlayerToKit();
	void PlayerToAmmo();
	void EnemyToPlayer();


public:
	CollisionManager(std::vector<Ruby*>* rubys, std::vector<HealthKit*>* healthKits, std::vector<Bullet*>* bullets, std::vector<Enemy*>* enemies, std::vector<AmmoBlock*>* ammoBlocks, FirstPersonCharacter* player);
	void CheckCollisions();

	~CollisionManager();

};

#endif