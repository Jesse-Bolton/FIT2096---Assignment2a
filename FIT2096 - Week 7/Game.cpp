/*	FIT2096 - Example Code
*	Game.cpp
*	Created by Elliott Wilson & Mike Yeates - 2016 - Monash University
*/

#include "Game.h"
#include "TexturedShader.h"
#include "StaticObject.h"
#include "ThirdPersonCamera.h"
#include "MathsHelper.h"
#include "DirectXTK/CommonStates.h"
#include <sstream>

Game::Game()
{
	m_renderer = NULL;
	m_currentCam = NULL;
	m_input = NULL;
	m_meshManager = NULL;
	m_textureManager = NULL;
	m_diffuseTexturedShader = NULL;
	m_unlitVertexColouredShader = NULL;
	m_spriteBatch = NULL;
	m_arialFont12 = NULL;
	m_arialFont18 = NULL;
	m_collisionManager = NULL;
	
	m_bloodTransparent = 0.0f;
	m_ammoGenerationCooldown = 0.0f;
	m_fireRateCooldown = 0.0f;
	m_randomSeed = MathsHelper::RandomRange(1, 10);
}

Game::~Game() {}

bool Game::Initialise(Direct3D* renderer, InputController* input)
{
	m_renderer = renderer;	
	m_input = input;
	m_meshManager = new MeshManager();
	m_textureManager = new TextureManager();

	if (!InitShaders())
		return false;

	if (!LoadMeshes())
		return false;

	if (!LoadTextures())
		return false;

	m_currentCam = new FirstPersonCharacter(m_input, Vector3(0.0f, 1.5f, 0.0f));

	LoadFonts();
	
	InitUI();

	InitGameWorld();
	RefreshUI();

	// TODO Instantiate the Collision Manager here after completing it
	m_collisionManager = new CollisionManager(&m_rubys,&m_healthKits,&m_bullets,&m_enemies,&m_ammoBlocks,m_currentCam);


	return true;
}

bool Game::InitShaders()
{
	m_unlitVertexColouredShader = new Shader();
	if (!m_unlitVertexColouredShader->Initialise(m_renderer->GetDevice(), L"Assets/Shaders/VertexShader.vs", L"Assets/Shaders/UnlitVertexColourPixelShader.ps"))
		return false;

	m_unlitTexturedShader = new TexturedShader();
	if (!m_unlitTexturedShader->Initialise(m_renderer->GetDevice(), L"Assets/Shaders/VertexShader.vs", L"Assets/Shaders/UnlitTexturedPixelShader.ps"))
		return false;

	m_diffuseTexturedShader = new TexturedShader();
	if (!m_diffuseTexturedShader->Initialise(m_renderer->GetDevice(), L"Assets/Shaders/VertexShader.vs", L"Assets/Shaders/DiffuseTexturedPixelShader.ps"))
		return false;

	m_diffuseTexturedFogShader = new TexturedShader();
	if (!m_diffuseTexturedFogShader->Initialise(m_renderer->GetDevice(), L"Assets/Shaders/VertexShader.vs", L"Assets/Shaders/DiffuseTexturedFogPixelShader.ps"))
		return false;

	return true;
}

bool Game::LoadMeshes()
{
	if (!m_meshManager->Load(m_renderer, "Assets/Meshes/ground.obj"))
		return false;

	if (!m_meshManager->Load(m_renderer, "Assets/Meshes/bullet.obj"))
		return false;

	if (!m_meshManager->Load(m_renderer, "Assets/Meshes/ruby.obj"))
		return false;

	if (!m_meshManager->Load(m_renderer, "Assets/Meshes/enemy.obj"))
		return false;

	if (!m_meshManager->Load(m_renderer, "Assets/Meshes/ammoBlock.obj"))
		return false;

	return true;
}

bool Game::LoadTextures()
{
	if (!m_textureManager->Load(m_renderer, "Assets/Textures/ground.png"))
		return false;
	
	if (!m_textureManager->Load(m_renderer, "Assets/Textures/bullet.png"))
		return false;

	if (!m_textureManager->Load(m_renderer, "Assets/Textures/gradient_red.png"))
		return false;

	if (!m_textureManager->Load(m_renderer, "Assets/Textures/gradient_redOrange.png"))
		return false;

	if (!m_textureManager->Load(m_renderer, "Assets/Textures/crosshair.png")) //Reference: crosshair downloaded from: Icon made by Freepik from www.flaticon.com
		return false;

	if (!m_textureManager->Load(m_renderer, "Assets/Textures/sprite_healthBar.png"))
		return false;

	if (!m_textureManager->Load(m_renderer, "Assets/Textures/sprite_hurtOverlay.png"))
		return false;

	if (!m_textureManager->Load(m_renderer, "Assets/Textures/ammobox.png"))
		return false;

	if (!m_textureManager->Load(m_renderer, "Assets/Textures/healthkit.png"))
		return false;

	if (!m_textureManager->Load(m_renderer, "Assets/Textures/gameover.png"))
		return false;

	if (!m_textureManager->Load(m_renderer, "Assets/Textures/victory.png"))
		return false;

	if (!m_textureManager->Load(m_renderer, "Assets/Textures/treasure.png"))
		return false;

	return true;
}

void Game::LoadFonts()
{
	// There's a few different size fonts in there, you know
	m_arialFont12 = new SpriteFont(m_renderer->GetDevice(), L"Assets/Fonts/Arial-12pt.spritefont");
	m_arialFont18 = new SpriteFont(m_renderer->GetDevice(), L"Assets/Fonts/Arial-18pt.spritefont");
}

void Game::InitUI()
{
	m_spriteBatch = new SpriteBatch(m_renderer->GetDeviceContext());
	m_crossHair = m_textureManager->GetTexture("Assets/Textures/crosshair.png");
	m_healthBar = m_textureManager->GetTexture("Assets/Textures/sprite_healthBar.png");
	m_beenHit = m_textureManager->GetTexture("Assets/Textures/sprite_hurtOverlay.png");
	m_endSceneLost = m_textureManager->GetTexture("Assets/Textures/gameover.png"); // ref: https://www.moddb.com/games/slime-box/images/game-over-11
	m_endSceneWin = m_textureManager->GetTexture("Assets/Textures/victory.png"); // ref: https://www.roblox.com/library/51655943/z0mg-hax-YOU-WIN
	// Also init any buttons here
}

void Game::RefreshUI()
{
	// Ensure text in UI matches latest scores etc (call this after data changes)
	m_rubyNum = m_currentCam->GetRubyNumber();
	rubys = std::to_wstring(m_rubyNum);
	m_ammoNum = m_currentCam->GetAmmo();
	ammos = std::to_wstring(m_ammoNum);
	m_enemyKilledNum = m_currentCam->GetKillsNumber();
	kills = std::to_wstring(m_enemyKilledNum);

}

void Game::InitGameWorld()
{
	InitRuby();
	InitEnemy();
	InitAmmoBlocks();
	InitHealthKit();

	// Static scenery objects
	m_gameObjects.push_back(new StaticObject(m_meshManager->GetMesh("Assets/Meshes/ground.obj"),
		m_diffuseTexturedFogShader,
		m_textureManager->GetTexture("Assets/Textures/ground.png")));

	//Init UI variables after init all game objects
	m_rubyNum = m_currentCam->GetRubyNumber();
	rubys = std::to_wstring(m_rubyNum);
	m_ammoNum = m_currentCam->GetAmmo();
	ammos = std::to_wstring(m_ammoNum);
	m_enemyKilledNum = m_currentCam->GetKillsNumber();
	kills = std::to_wstring(m_enemyKilledNum);
}

void Game::InitAmmoBlocks() 
{
	for (int i = 0; i < 2; i++)
	{
		Vector3 position = Vector3(MathsHelper::RandomRange(-100.0f, 100.0f), 0.0f, MathsHelper::RandomRange(-100.0f, 100.0f));

		m_ammoBlocks.push_back
		(
			new AmmoBlock(
				m_meshManager->GetMesh("Assets/Meshes/ammoBlock.obj"),
				m_diffuseTexturedFogShader,
				m_textureManager->GetTexture("Assets/Textures/ammobox.png"), // needs textre
				position)
		);
	}
}

void Game::InitRuby()
{
	// Spawn some rubys in random positions
	for (int i = 0; i < 10; i++)
	{
		Vector3 position = Vector3(MathsHelper::RandomRange(-100.0f, 100.0f), 0.0f, MathsHelper::RandomRange(-100.0f, 100.0f));

		Ruby* ruby = new Ruby(m_meshManager->GetMesh("Assets/Meshes/ruby.obj"),
			m_diffuseTexturedFogShader,
			m_textureManager->GetTexture("Assets/Textures/treasure.png"), // ref https://www.textures.com/download/downloadImage?photoId=112251&size=S&pix=1024
			position);
		
		m_rubys.push_back(ruby);
		//m_gameObjects.push_back(ruby);
	}
}

void Game::InitHealthKit()
{
	//limited supply
	for (int i = 0; i < 10; i++)
	{
		Vector3 position = Vector3(MathsHelper::RandomRange(-100.0f, 100.0f), 0.0f, MathsHelper::RandomRange(-100.0f, 100.0f));

		HealthKit* healthKit = new HealthKit(m_meshManager->GetMesh("Assets/Meshes/ammoBlock.obj"),
			m_diffuseTexturedFogShader,
			m_textureManager->GetTexture("Assets/Textures/healthkit.png"),
			position);

		m_healthKits.push_back(healthKit);
	}
	
}

void Game::InitEnemy() 
{
	// Spawn five enemy in random location
	for (int i = 0; i < 5; i++)
	{
		Vector3 position = Vector3(MathsHelper::RandomRange(-100.0f, 100.0f), 0.0f, MathsHelper::RandomRange(-100.0f, 100.0f));

		Enemy* enemy = new Enemy(
			m_meshManager->GetMesh("Assets/Meshes/enemy.obj"),
			m_diffuseTexturedFogShader,
			m_textureManager->GetTexture("Assets/Textures/gradient_redOrange.png"),
			position,
			Strategy::default,
			m_currentCam
		);

		m_enemies.push_back(enemy);
		m_gameObjects.push_back(enemy);
	}

	// assign tasks
	m_enemies[0]->SetStrategy(Strategy::chase);
	m_enemies[1]->SetStrategy(Strategy::patrol);
	m_enemies[2]->SetStrategy(Strategy::fear);
	m_enemies[3]->SetStrategy(Strategy::flee);
	m_enemies[4]->SetStrategy(Strategy::headoff);
}

void Game::CreateBullet() 
{
	// Create a bullet from player
	m_bullets.push_back(new Bullet(
		m_meshManager->GetMesh("Assets/Meshes/bullet.obj"),
		m_diffuseTexturedShader,
		m_textureManager->GetTexture("Assets/Textures/bullet.png"),
		m_currentCam->GetLookAtTarget(),
		m_currentCam->GetHeading(),
		m_currentCam->GetPitch(),
		m_currentCam->GetDirection()
	));

	// Player loss one ammo
	m_currentCam->LossAmmo();
}

void Game::ReCreateAmmoBlocks() 
{
	// Recreate the ammoBlocks
	Vector3 position = Vector3(MathsHelper::RandomRange(-100.0f, 100.0f), 0.0f, MathsHelper::RandomRange(-100.0f, 100.0f));

	m_ammoBlocks.push_back(new AmmoBlock(
		m_meshManager->GetMesh("Assets/Meshes/ammoBlock.obj"),
		m_diffuseTexturedFogShader,
		m_textureManager->GetTexture("Assets/Textures/ammobox.png"),
		position));
}

void Game::EnemyShooting(Enemy* enemy) 
{
	// Create bullet from enemy
	m_bullets.push_back(
		new Bullet(
			m_meshManager->GetMesh("Assets/Meshes/bullet.obj"),
			m_diffuseTexturedShader,
			m_textureManager->GetTexture("Assets/Textures/bullet.png"),
			Vector3(enemy->GetPosition().x, 1.5f, enemy->GetPosition().z),
			enemy->GetYRotation(),
			enemy->GetXRotation(),
			enemy->GetLocalForward()
		)
	);
}

void Game::DestoryBullet(Bullet* bullet) 
{
	delete bullet;
}

void Game::Update(float timestep)
{
	m_input->BeginUpdate();

		if (m_input->GetKeyDown(VK_ESCAPE))
		{
			PostQuitMessage(0);
		}

		

		// GameObjects Update
		for (unsigned int i = 0; i < m_gameObjects.size(); i++)
		{
			m_gameObjects[i]->Update(timestep);
		}

		for (unsigned int i = 0; i < m_rubys.size(); i++)
		{
			m_rubys[i]->Update(timestep);
		}

		for (unsigned int i = 0; i < m_healthKits.size(); i++)
		{
			m_healthKits[i]->Update(timestep);
		}

		for (unsigned int i = 0; i < m_enemies.size(); i++)
		{
			if (m_enemies[i]->IsShooting())
			{
				EnemyShooting(m_enemies[i]);
				m_enemies[i]->SetShootingState(false);
			}
		}

		for (unsigned int i = 0; i < m_bullets.size(); i++)
		{
			m_bullets[i]->Update(timestep);
		}

		// GameObjects Update end

		// Player weapon Cooldown count
		if (m_currentCam->isShooting() && m_currentCam->GetAmmo() > 0)
		{
			if (m_fireRateCooldown <= 0.0f)
			{
				CreateBullet();
				m_fireRateCooldown = 0.4f;
			}
		}
		if (m_fireRateCooldown > 0.0f)
			// time factor's does not affect the player weapon cooldown, to achieve a more tactical way for player.
			m_fireRateCooldown -= timestep;

		m_currentCam->Update(timestep);	 // player update and camera update

		// TODO Call CheckCollisions so our collision manager has a chance to detect collisions and inform the objects involved
		m_collisionManager->CheckCollisions();

		// Destory items when its no longer in use.
		for (unsigned int i = 0; i < m_bullets.size(); i++)
		{
			if (m_bullets[i]->Destoryable())
			{
				DestoryBullet(m_bullets[i]);
				m_bullets.erase(m_bullets.begin() + i);
			}
		}
		for (unsigned int i = 0; i < m_rubys.size(); i++)
		{
			if (m_rubys[i]->GetDestoryable())
			{
				delete m_rubys[i];
				m_rubys.erase(m_rubys.begin() + i);
			}
		}

		for (unsigned int i = 0; i < m_healthKits.size(); i++)
		{
			if (m_healthKits[i]->GetDestoryable())
			{
				delete m_healthKits[i];
				m_healthKits.erase(m_healthKits.begin() + i);
			}
		}

		for (unsigned int i = 0; i < m_ammoBlocks.size(); i++)
		{
			if (m_ammoBlocks[i]->GetDestoryable())
			{
				delete m_ammoBlocks[i];
				m_ammoBlocks.erase(m_ammoBlocks.begin() + i);
			}
		}

		// If ammoBlocks is less than 2, after 1 to 10 seconds, respawn it at random location
		if (m_ammoBlocks.size() < 2)
		{
			if (m_ammoGenerationCooldown == 0.0f)
				m_randomSeed = MathsHelper::RandomRange(1, 10);

			// timefactor affects the cooldown
			m_ammoGenerationCooldown += timestep;

			if (m_ammoGenerationCooldown >= m_randomSeed)
			{
				ReCreateAmmoBlocks();
				m_ammoGenerationCooldown = 0.0f;
			}
		}

		// If UI variable changes, refresh UI
		if (m_rubyNum != m_currentCam->GetRubyNumber() || m_ammoNum != m_currentCam->GetAmmo() || m_enemyKilledNum != m_currentCam->GetKillsNumber())
		{
			RefreshUI();
		}

	m_input->EndUpdate();

}

void Game::Render()
{
	m_renderer->BeginScene(0.8f, 1.0f, 0.9f, 1.0f);

	for (unsigned int i = 0; i < m_gameObjects.size(); i++)
	{
		m_gameObjects[i]->Render(m_renderer, m_currentCam);
	}
	for (unsigned int i = 0; i < m_bullets.size(); i++) 
	{
		m_bullets[i]->Render(m_renderer, m_currentCam);
	}
	for (unsigned int i = 0; i < m_rubys.size(); i++)
	{
		m_rubys[i]->Render(m_renderer, m_currentCam);
	}
	for (unsigned int i = 0; i < m_healthKits.size(); i++)
	{
		m_healthKits[i]->Render(m_renderer, m_currentCam);
	}

	for (unsigned int i = 0; i < m_ammoBlocks.size(); i++)
	{
		m_ammoBlocks[i]->Render(m_renderer, m_currentCam);
	}

	DrawUI();

	m_renderer->EndScene();		
}

void Game::DrawUI()
{
	// Sprites don't use a shader 
	m_renderer->SetCurrentShader(NULL);

	CommonStates states(m_renderer->GetDevice());
	m_spriteBatch->Begin(SpriteSortMode_Deferred, states.NonPremultiplied());

	// Do UI drawing between the Begin and End calls

	// Let's draw some text over our game

	m_arialFont18->DrawString(m_spriteBatch, L"Gold: ", Vector2(20, 90), Color(0.0f, 0.0f, 0.0f), 0, Vector2(0, 0));
	m_arialFont18->DrawString(m_spriteBatch, L"ESC to quit", Vector2(20, 20), Color(0.0f, 0.0f, 0.0f), 0, Vector2(0, 0));
	m_arialFont18->DrawString(m_spriteBatch, rubys.c_str(), Vector2(100, 90), Color(0.0f, 0.0f, 0.0f), 0, Vector2(0, 0));
	m_arialFont18->DrawString(m_spriteBatch, L"Ammo: ", Vector2(20, 120), Color(0.0f, 0.0f, 0.0f), 0, Vector2(0, 0));
	m_arialFont18->DrawString(m_spriteBatch, ammos.c_str(), Vector2(100, 120), Color(0.0f, 0.0f, 0.0f), 0, Vector2(0, 0));
	m_arialFont18->DrawString(m_spriteBatch, L"Kill: ", Vector2(20, 150), Color(0.0f, 0.0f, 0.0f), 0, Vector2(0, 0));
	m_arialFont18->DrawString(m_spriteBatch, kills.c_str(), Vector2(100, 150), Color(0.0f, 0.0f, 0.0f), 0, Vector2(0, 0));





	m_spriteBatch->Draw(m_crossHair->GetShaderResourceView(), Vector2(640 - 16,360 - 16), Color(1.0f, 1.0f, 1.0f));
	for (unsigned i = 0; i < m_currentCam->GetHealth(); i++)
	{
		m_spriteBatch->Draw(m_healthBar->GetShaderResourceView(), Vector2(20 + i*(32+1), 50), Color(1.0f, 0.0f, 0.0f));	}

	
	if (m_currentCam->GetHealth() <= 0)  // lose scene
	{
		m_spriteBatch->Draw(m_endSceneLost->GetShaderResourceView(), Vector2(300, 200), Color(1.0f, 1.0f, 1.0f));
	}

	// Winning scene
	if (m_currentCam->GetKillsNumber() >= 5 || m_currentCam->GetRubyNumber() >= 40) 
	{
		m_spriteBatch->Draw(m_endSceneWin->GetShaderResourceView(), Vector2(400, 100), Color(1.0f, 1.0f, 1.0f));
	}

	// If player been shot at, change the transparent value of hurt overlay to 1.0f
	if (m_currentCam->PlayerBeenShot()) 
	{
		m_bloodTransparent = 1.0f;
		m_currentCam->SetPlayerBeenShot(false);
	}
	else
	{
		// transparent value decreased at a certain speed, does not affect by timestep or timefactor
		if (m_bloodTransparent > 0.0f)
			m_bloodTransparent -= 0.02;
	}

	m_spriteBatch->Draw(m_beenHit->GetShaderResourceView(), Vector2(0, 0), Color(1.0f, 1.0f, 1.0f, m_bloodTransparent));
	
	m_spriteBatch->End();
}

void Game::Shutdown()
{
	for (unsigned int i = 0; i < m_gameObjects.size(); i++)
	{
		delete m_gameObjects[i];
	}
	m_gameObjects.clear();
	m_enemies.clear();

	for (unsigned int i = 0; i < m_ammoBlocks.size(); i++)
	{
		delete m_ammoBlocks[i];
	}
	m_ammoBlocks.clear();

	for (unsigned int i = 0; i < m_bullets.size(); i++)
	{
		delete m_bullets[i];
	}
	m_bullets.clear();


	for (unsigned int i = 0; i < m_rubys.size(); i++)
	{
		delete m_rubys[i];
	}
	m_rubys.clear();

	for (unsigned int i = 0; i < m_healthKits.size(); i++)
	{
		delete m_healthKits[i];
	}
	m_healthKits.clear();

	if (m_currentCam)
	{
		delete m_currentCam;
		m_currentCam = NULL;
	}

	if (m_collisionManager) 
	{
		delete m_collisionManager;
		m_collisionManager = NULL;
	}

	if (m_unlitVertexColouredShader)
	{
		m_unlitVertexColouredShader->Release();
		delete m_unlitVertexColouredShader;
		m_unlitVertexColouredShader = NULL;
	}

	if (m_unlitTexturedShader)
	{
		m_unlitTexturedShader->Release();
		delete m_unlitTexturedShader;
		m_unlitTexturedShader = NULL;
	}

	if(m_diffuseTexturedShader)
	{
		m_diffuseTexturedShader->Release();
		delete m_diffuseTexturedShader;
		m_diffuseTexturedShader = NULL;
	}

	if (m_diffuseTexturedFogShader)
	{
		m_diffuseTexturedFogShader->Release();
		delete m_diffuseTexturedShader;
		m_diffuseTexturedShader = NULL;
	}

	if (m_meshManager)
	{
		m_meshManager->Release();
		delete m_meshManager;
		m_meshManager = NULL;
	}

	if (m_textureManager)
	{
		m_textureManager->Release();
		delete m_textureManager;
		m_textureManager = NULL;
	}

	if (m_spriteBatch)
	{
		delete m_spriteBatch;
		m_spriteBatch = NULL;
	}

	if (m_arialFont12)
	{
		delete m_arialFont12;
		m_arialFont12 = NULL;
	}

	if (m_arialFont18)
	{
		delete m_arialFont18;
		m_arialFont18 = NULL;
	}
}