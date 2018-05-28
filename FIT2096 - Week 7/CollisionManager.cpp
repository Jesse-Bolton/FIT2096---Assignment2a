#include "CollisionManager.h"
#include <vector>
#include <iostream>

CollisionManager::CollisionManager(std::vector<Ruby*>* rubys, std::vector<HealthKit*>* healthKits, std::vector<Bullet*>* bullets, std::vector<Enemy*>* enemies, std::vector<AmmoBlock*>* ammoBlocks, FirstPersonCharacter* player)
{
	m_rubys = rubys;
	m_healthKits = healthKits;
	m_bullets = bullets;
	m_enemies = enemies;
	m_player = player;
	m_ammoBlocks = ammoBlocks;

	// Clear our arrays to 0 (NULL)
	memset(m_currentCollisions, 0, sizeof(m_currentCollisions));
	memset(m_previousCollisions, 0, sizeof(m_previousCollisions));

	m_nextCurrentCollisionSlot = 0;
}

void CollisionManager::CheckCollisions()
{
	// Check kart to item box collisions
	//KartToItemBox();
	BulletsToEnemy();
	BulletsToPlayer();
	PlayerToRuby();
	PlayerToAmmo();
	PlayerToKit();
	EnemyToPlayer();

	// Move all current collisions into previous
	memcpy(m_previousCollisions, m_currentCollisions, sizeof(m_currentCollisions));

	// Clear out current collisions
	memset(m_currentCollisions, 0, sizeof(m_currentCollisions));
	
	// Now current collisions is empty, we'll start adding from the start again
	m_nextCurrentCollisionSlot = 0;

}

bool CollisionManager::ArrayContainsCollision(GameObject* arrayToSearch[], GameObject* first, GameObject* second)
{
	// See if these two GameObjects appear one after the other in specified collisions array
	// Stop one before length so we don't overrun as we'll be checking two elements per iteration
	for (int i = 0; i < MAX_ALLOWED_COLLISIONS - 1; i += 2)
	{
		if ((arrayToSearch[i] == first && arrayToSearch[i + 1] == second) ||
			arrayToSearch[i] == second && arrayToSearch[i + 1] == first)
		{
			// Found them!
			return true;
		}
	}

	// These objects were not colliding last frame
	return false;
}

void CollisionManager::AddCollision(GameObject* first, GameObject* second)
{	
	// Add the two colliding objects to the current collisions array
	// We keep track of the next free slot so no searching is required
	m_currentCollisions[m_nextCurrentCollisionSlot] = first;
	m_currentCollisions[m_nextCurrentCollisionSlot + 1] = second;
	
	m_nextCurrentCollisionSlot += 2;
}

void CollisionManager::BulletsToPlayer() 
{
	// Test every bullet's bounding area against player's bounding area, if collide, destory the bullet and player loss one health
	CBoundingBox playerBounds = m_player->GetBounds();
	for (unsigned int i = 0; i < m_bullets->size(); i++)
	{
		Bullet* bullet = (*m_bullets)[i];

		CBoundingBox bulletBounds = bullet->GetBounds();

		bool isColliding = CheckCollision(playerBounds, bulletBounds);
		
		if (isColliding) 
		{
			bullet->DestoryBullet();
			m_player->LossHealth(); 
			m_player->SetPlayerBeenShot(true);	// Trigger hurt overlay
		}
	}
}

void CollisionManager::EnemyToPlayer()
{
	// Test every enemie's bounding area with player's bounding area, if collision happens, kill player and end game
	CBoundingBox playerBounds = m_player->GetBounds();
	for (unsigned i = 0; i < m_enemies->size(); i++)
	{
		Enemy* enemy = (*m_enemies)[i];
		CBoundingBox enemyBounds = enemy->GetBounds();

		bool isColliding = CheckCollision(playerBounds, enemyBounds);

		if (isColliding)
		{
			m_player->DieNow();
		}
	}

}

void CollisionManager::BulletsToEnemy()
{
	// Test every enemies' bounding area with every bullets' bounding area, if collide destory the bullet and enemy loss 1 health
	for (unsigned i = 0; i < m_enemies->size(); i++)
	{
		for (unsigned j = 0; j < m_bullets->size(); j++)
		{
			Bullet* bullet = (*m_bullets)[j];
			Enemy* enemy = (*m_enemies)[i];

			CBoundingBox bulletBounds = bullet->GetBounds();
			CBoundingBox enemyBounds = enemy->GetBounds();

			bool isColliding = CheckCollision(bulletBounds, enemyBounds);

			bool wasColliding = ArrayContainsCollision(m_previousCollisions, bullet, enemy);

			if (isColliding) 
			{
				// If enemy dead, call respawn function(teleport to the new location and restore health)
				bullet->DestoryBullet();
				enemy->LossHp();
				if (enemy->GetHp() <= 0) 
				{
					m_player->KilledOne();
					enemy->Respawn();
				}
			}
		}
	}
}

void CollisionManager::PlayerToRuby() 
{
	// Test every rubys' bounding area with player's bounding area, if collide destory the ruby and add one ruby to player
	CBoundingBox playerBounds = m_player->GetBounds();
	for (unsigned int i = 0; i < m_rubys->size(); i++)
	{
		Ruby* ruby = (*m_rubys)[i];
		CBoundingBox rubyBounds = ruby->GetBounds();

		bool isColliding = CheckCollision(playerBounds, rubyBounds);

		if (isColliding) 
		{
			m_player->RubyFetched();
			ruby->SetDestory(true);
		}
	}
}

void CollisionManager::PlayerToKit()
{
	CBoundingBox playerBounds = m_player->GetBounds();
	for (unsigned int i = 0; i < m_healthKits->size(); i++)
	{
		HealthKit* healthKit = (*m_healthKits)[i];
		CBoundingBox kitBounds = healthKit->GetBounds();

		bool isColliding = CheckCollision(playerBounds, kitBounds);

		if (isColliding)
		{
			m_player->RestoreHealth();
			healthKit->SetDestory(true);
		}
	}

}

void CollisionManager::PlayerToAmmo() 
{
	//Check every ammoBox against the player, if collides player restore 10 bullets.
	CBoundingBox playerBounds = m_player->GetBounds();
	for (unsigned int i = 0; i < m_ammoBlocks->size(); i++)
	{
		AmmoBlock* ammoBlock = (*m_ammoBlocks)[i];
		CBoundingBox ammoBounds = ammoBlock->GetBounds();

		bool isColliding = CheckCollision(playerBounds, ammoBounds);

		if (isColliding) 
		{
			m_player->GainAmmo();
			ammoBlock->SetDestoryable(true);
		}
	}
}

CollisionManager::~CollisionManager() 
{
	m_bullets = NULL;
	m_rubys = NULL;
	m_enemies = NULL;
	m_ammoBlocks = NULL;
	m_player = NULL;
}