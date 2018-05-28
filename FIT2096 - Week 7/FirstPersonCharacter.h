/*	FIT2096 - Example Code
*	FirstPersonCharacter.h
*	Created by Mike Yeates - 2016 - Monash University
*	A flying camera can move up and down, look around freely with
*	the mouse, and always moves parallel to the ground
*/

#ifndef FIRST_PERSON_CHARACTER_H
#define FIRST_PERSON_CHARACTER_H

#include "Camera.h"
#include "InputController.h"
#include "Collisions.h"
#include <time.h>

class FirstPersonCharacter : public Camera
{
private:
	InputController* m_input;
	CBoundingBox m_boundingBox;

	Vector3 m_position;
	Vector3 m_lastFramePosition;

	float m_moveSpeed;
	float m_rotationSpeed;
	float m_heightChangeSpeed;

	float m_heading;
	float m_pitch;

	bool m_shooting;
	bool m_isMoving;
	bool m_beenShot;

	int m_health;
	int m_bullets;
	int m_kills;
	int m_rubysFetch;
	int m_dmg;

public:
	FirstPersonCharacter(InputController* input, Vector3 startPos);

	void Update(float timestep);

	// Accessor
	int GetHealth() { return m_health; };		// Return player health
	int GetAmmo() { return m_bullets; };		// Return player ammo
	float GetHeading() { return m_heading; };	// Return player heading
	float GetPitch() { return m_pitch; };		// Return player pitch
	bool isShooting() { return m_shooting; };	// Return whether player want to shoot
	bool PlayerisMoving() { return m_isMoving; };	// Return whether player is moving
	int GetRubyNumber() { return m_rubysFetch; };	// Return number of rubys player has
	int GetKillsNumber() { return m_kills; }		// Return number of kills
	CBoundingBox GetBounds() { return m_boundingBox; };	// Return the bounding area
	bool PlayerBeenShot() { return m_beenShot; };	// Return whether player is been shot
	int GetDamage() { 
		BulletDamage();
		return m_dmg; }				// Return damage ammount

	// Mutator
	void LossHealth() {
		srand(time(NULL));
		m_health -= rand() % 3 + 1;

	};		// Player loss one health
	void LossAmmo() { m_bullets -= 1; };		// Player used one bullet
	void GainAmmo() { m_bullets += 10; };		// Player gain 10 bullets
	void SetPlayerBeenShot(bool value) { m_beenShot = value; };	// Set the value of m_beenShot
	void RubyFetched() { m_rubysFetch++; }	// Add one ruby to player
	void RestoreHealth() { m_health += 1; } // Restore one health for player
	void KilledOne() { m_kills += 1; }		// kill one enemy
	void DieNow() { m_health = 0; } // ops got caught
	void BulletDamage() {          // random dmg
		srand(time(NULL));
		m_dmg = rand() % 10 + 5;
	}
};

#endif