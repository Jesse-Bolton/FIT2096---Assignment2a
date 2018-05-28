#include "Enemy.h"
#include "MathsHelper.h"
#include <time.h>
#define PI 3.1415926

Enemy::Enemy(Mesh* mesh, Shader* shader, Texture* texture, Vector3 position, Strategy s, FirstPersonCharacter* player):
	GameObject(mesh, shader, texture, position)
{
	m_boundingBox = CBoundingBox(//
		m_position + m_mesh->GetMin(), 
		m_position + m_mesh->GetMax()
	);
	m_targetPosition = position;//
	m_strategy = s;
	m_player = player;//
	m_heading = 0.0f;//
	m_rotateSpeed = 5.0f;//
	m_isTurning = true;//
	m_reachDestination = true;
	m_movespeed = 3.0f;
	m_cooldown = 0.0f;
	srand(time(NULL));
	m_HP = rand() % 20 + 5;


}

void Enemy::Update(float timestep) 
{
	switch (m_strategy) 
	{
	case Strategy::chase:
		ChasePlayer();
		break;
	case Strategy::flee:
		FleeAway();
		break;
	case Strategy::patrol:
		RandomPatrol();
		break;
	case Strategy::headoff:
		HeadOff();
		break;
	case Strategy::fear:
		Fear();
	default:
		break;
	}

	m_boundingBox.SetMin(m_position + m_mesh->GetMin());
	m_boundingBox.SetMax(m_position + m_mesh->GetMax());

	// If player is not in sight, enemy facing its target location, otherwise facing the player.
	if (!PlayerInSight())
		Turning(timestep,m_targetPosition);
	else
		Turning(timestep,  m_player->GetPositionLockedY());

	// if the enemy not reach the location and its not facing backwards to its destination, then starting move
	if (!PositionInRange() && !m_isTurning) 
	{
		m_reachDestination = false;
		Vector3 _direction = m_targetPosition - m_position;
		_direction.Normalize();
		m_position += _direction * timestep * m_movespeed;
	}
	else if (PositionInRange()) 
	{
		m_reachDestination = true;
	}

	// If game is not over and enemy is facing the player then shoot
	if (PlayerInSight() && !m_isTurning&&m_player->GetHealth() > 0 && m_player->GetRubyNumber() < 10)
	{

		if (m_cooldown <= 0)
		{
			m_isShooting = true;
			// the weapon cooldown for flee class is 0.8 sec and others are 1.3 sec.
			if (m_strategy == Strategy::flee)
				m_cooldown = 0.8f;
			else
				m_cooldown = 1.3f;
		}
		else
			// enemy's weapon cooldown is affected by the timefactor
			m_cooldown -= timestep;
	}
}

bool Enemy::PositionInRange() 
{
	// Whether enemy reached its destination
	if (Vector3::Distance(m_position, m_targetPosition) <= 10)
		return true;
	return false;

}

void Enemy::ChasePlayer() 
{
	// Enemy chasing player
	m_targetPosition = m_player->GetPositionLockedY();
}

void Enemy::FleeAway()
{
	// Enemy starting flee away from the player
	if (Vector3::Distance(m_position, m_player->GetPosition()) < 15)
		m_targetPosition = m_player->GetPositionLockedY() * -1;
	else
		m_targetPosition = m_position;
}

void Enemy::RandomPatrol() 
{	
	// if enemy reached its destination, reset the destination
	if(m_reachDestination)
		m_targetPosition = Vector3(rand() % 100 + 1, 0, rand() % 100 + 1);
}

void Enemy::Fear()
{
	m_targetPosition = m_player->GetPositionLockedY() * -1.5;
}
void Enemy::HeadOff()
{
	m_targetPosition = m_player->GetPositionLockedY() + Vector3(1.0f, 0, 1.0f);
}



void Enemy::Turning(float timestep, Vector3 direction)
{
	// Calculate the angle between enemy's world based vector and destination's world based vector
	Matrix heading = Matrix::CreateRotationY(m_rotY);
	Vector3 worldForward = Vector3(0, 0, 1);
	Vector3 localForward = Vector3::TransformNormal(worldForward, heading);
	Vector3 localTarget = direction - m_position;
	float angle = acos(localForward.Dot(localTarget) /(localForward.Length() * localTarget.Length()));
	
	// Turning clockwise(maybe) a little bit and recalculate the angle between enemy and destination, 
	//if the angle is bigger than last time, then turning(anit-clockwise). otherwise keep turning
	Matrix heading_1 = Matrix::CreateRotationY(m_rotY+0.01);
	Vector3 localForward_1 = Vector3::TransformNormal(worldForward, heading_1);
	float angle_1 = acos(localForward_1.Dot(localTarget) / (localForward_1.Length() * localTarget.Length()));

	if (angle_1 > angle)
	{
		if (angle > 0.02)	// When the angle is smaller than 0.02 radians, stop turning.
		{
			m_rotY -= angle * timestep * m_rotateSpeed;
			// if the angle between enemy and destiantion is smaller than 0.7 radians, then consider it already facing in the right direction.
			m_isTurning = angle > 0.7 ? true : false;
		}
		else 
		{
			m_isTurning = false;
		}
	}
	else if(angle_1 < angle) // When the angle is smaller than 0.02 radians, stop turning.
	{
		if (angle > 0.02)
		{
			m_rotY += angle * timestep * m_rotateSpeed;
			// if the angle between enemy and destiantion is smaller than 0.7 radians, then consider it already facing in the right direction.
			m_isTurning = angle > 0.7 ? true : false;
		}
		else 
		{
			m_isTurning = false;
		}
		
	}
}

Vector3 Enemy::GetLocalForward() 
{
	Matrix heading = Matrix::CreateRotationY(m_rotY);
	Vector3 worldForward = Vector3(0, 0, 1);
	Vector3 localForward = Vector3::TransformNormal(worldForward, heading);
	return localForward;
}

bool Enemy::PlayerInSight() 
{
	if (Vector3::Distance(m_position, m_player->GetPositionLockedY()) <= 40)
		return true;
	return false;
}

void Enemy::Respawn() 
{
	m_HP = 2;
	m_position = Vector3(MathsHelper::RandomRange(-100.0f, 100.0f), 0.0f, MathsHelper::RandomRange(-100.0f, 100.0f));
}

Enemy::~Enemy()
{

}
