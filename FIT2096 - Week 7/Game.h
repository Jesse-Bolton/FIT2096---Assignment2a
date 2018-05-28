/*	FIT2096 - Example Code
*	Game.h
*	Created by Elliott Wilson & Mike Yeates - 2016 - Monash University
*	This class is the heart of our game and is also where our game logic will reside
*	It contains the overall Update and Render method for the whole game
*	You should eventually split some game logic into other objects which Game will manage
*	Watch the size of this class - it can grow out of control very fast!
*/

#ifndef GAME_H
#define GAME_H

#include "Direct3D.h"
#include "Camera.h"
#include "InputController.h"
#include "MeshManager.h"
#include "TextureManager.h"
#include "CollisionManager.h"
#include "GameObject.h"
#include "Ruby.h"
#include "FirstPersonCharacter.h"
#include "Bullet.h"
#include "Enemy.h"
#include "EnemyStrategyEnum.h"
#include "AmmoBlock.h"
#include "HealthKit.h"

#include "DirectXTK/SpriteBatch.h"
#include "DirectXTK/SpriteFont.h"

#include <vector>

class Game
{
private:
	FirstPersonCharacter* m_currentCam;		
	Direct3D* m_renderer;
	InputController* m_input;
	MeshManager* m_meshManager;
	TextureManager* m_textureManager;
	CollisionManager* m_collisionManager;

	// Sprites / Fonts
	SpriteBatch* m_spriteBatch;
	SpriteFont* m_arialFont12;
	SpriteFont* m_arialFont18;

	// Shaders
	Shader* m_unlitVertexColouredShader;
	Shader* m_unlitTexturedShader;
	Shader* m_diffuseTexturedShader;
	Shader* m_diffuseTexturedFogShader;
	
	// Often we like to talk to the player directly so we'll keep a pointer to it

	// This contains everything for easy calls to update and render
	std::vector<GameObject*> m_gameObjects;

	// We also need more specific collections for easier collision checks
	std::vector<Ruby*> m_rubys;
	std::vector<Bullet*> m_bullets;
	std::vector<Enemy*> m_enemies;
	std::vector<AmmoBlock*> m_ammoBlocks;
	std::vector<HealthKit*> m_healthKits;

	Texture* m_crossHair;
	Texture* m_healthBar;
	Texture* m_beenHit;
	Texture* m_endSceneLost;
	Texture* m_endSceneWin;

	
	float m_bloodTransparent;
	float m_ammoGenerationCooldown;
	float m_fireRateCooldown;

	int m_rubyNum;
	std::wstring rubys;
	int m_ammoNum;
	std::wstring ammos;
	int m_enemyKilledNum;
	std::wstring kills;
	

	int m_randomSeed;

	// Initialisation Helpers
	bool InitShaders();
	bool LoadMeshes();
	bool LoadTextures();
	void LoadFonts();
	void InitGameWorld();
	void InitRuby();
	void InitHealthKit();
	void InitEnemy();
	void InitAmmoBlocks();
	void ReCreateAmmoBlocks();
	void CreateBullet();
	void EnemyShooting(Enemy* enemy);
	void DestoryBullet(Bullet* bullet);

	void InitUI();
	void DrawUI();
	void RefreshUI();

public:
	Game();	
	~Game();

	bool Initialise(Direct3D* renderer, InputController* input); //The initialise method will load all of the content for the game (meshes, textures, etc.)

	void Update(float timestep);	//The overall Update method for the game. All gameplay logic will be done somewhere within this method
	void Render();					//The overall Render method for the game. Here all of the meshes that need to be drawn will be drawn

	void Shutdown(); //Cleanup everything we initialised
};

#endif