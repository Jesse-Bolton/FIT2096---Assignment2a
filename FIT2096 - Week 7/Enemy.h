#ifndef ENEMY_H
#define ENEMY_H
#include "GameObject.h"
#include "Collisions.h"
#include "EnemyStrategyEnum.h"
#include "FirstPersonCharacter.h"
#include "Ruby.h"
#include <vector>

class Enemy : public GameObject
{
private:
	CBoundingBox m_boundingBox;//
	float m_movespeed;
	Vector3 m_targetPosition;//
	Strategy m_strategy;
	FirstPersonCharacter* m_player;//

	float m_rotateSpeed;//
	float m_heading;//
	float m_cooldown;
	int m_HP;//
	int m_damage;

	bool m_isTurning;//
	bool m_reachDestination;
	bool m_isShooting;

	Vector3 m_localFacing;//

	std::vector<Ruby*>* m_rubys;

	void ChasePlayer();		// Enemy chase player
	void RandomPatrol();	// Enemy patrol to the random location
	void Fear();		// Enemy patrol to the random ruby
	void HeadOff();		// Enemy stops in front of player
	void FleeAway();		// Enemy flee away when player getting too close

	bool PlayerInSight();	// Determine whether the player is in shooting range
	bool PositionInRange();		// Determine whether the player is in range.

public:
	Enemy(Mesh* mesh, Shader* shader, Texture* texture, Vector3 position, Strategy s, FirstPersonCharacter* m_player);

	void Update(float timestep);//
	void Turning(float timestep, Vector3 direction);	// Enemy turns to the certain direction//

	void Respawn();	// Teleport to a new location and restore all health//

	//Accessor
	CBoundingBox GetBounds() { return m_boundingBox; }	// Return the bounding area//
	Vector3 GetLocalForward();							// Return the enemy's local forward vector
	bool IsShooting() { return m_isShooting; }			// Return whether the enemy is shooting
	int GetHp() { return m_HP; }						// Return enemy's health
	int GetDamage() { return m_damage; }				// Return enemy's damage

	//Mutator
	void SetTarget(Vector3 target) { m_targetPosition = target; };	// Set enemy's target location
	void SetStrategy(Strategy other) { m_strategy = other; };		// Set enemy's strategy
	void SetShootingState(bool other) { m_isShooting = other; }		// Set enemy is shooting or not
	void LossHp() { m_HP -= m_player->GetDamage(); }				// enemy lost random hp
	void EnemyDamage() {
		srand(time(NULL));
		m_damage = rand() % 10 + 5;
	}
	
	~Enemy();
};

#endif

