#include "pch.h"
#include <random>
#include <fstream>
#include <iostream>


void App::UpdateCamera(float dt)
{
	const float rotSpeed = 2.0f;                 // radians par seconde
	const float step = rotSpeed * dt;
	const float maxPitch = XM_PIDIV2 - 0.05f;    // évite le retournement aux pôles

	// Entrées
	if (cpuInput.IsLeft())  m_camYaw += step;
	if (cpuInput.IsRight()) m_camYaw -= step;
	if (cpuInput.IsUp())    m_camPitch += step;
	if (cpuInput.IsDown())  m_camPitch -= step;

	m_camPitch = std::clamp(m_camPitch, -maxPitch, maxPitch);

	// Point central : le héros (un seul point pour l'orbite ET le LookAt)
	XMFLOAT3 target = {
		m_pPlateform->transform.pos.x,
		m_pPlateform->transform.pos.y + 1.0f,
		m_pPlateform->transform.pos.z
	};

	// Position de la caméra en coordonnées sphériques
	const float cp = cosf(m_camPitch);
	XMFLOAT3 camPos = {
		target.x + m_camDistance * cp * sinf(m_camYaw),
		target.y + m_camDistance * sinf(m_camPitch),
		target.z + m_camDistance * cp * cosf(m_camYaw)
	};

	// Application à la caméra
	auto& camTransform = cpuEngine.GetCamera()->transform;
	camTransform.pos = camPos;   // ou camTransform.SetPosition(camPos.x, camPos.y, camPos.z)
	camTransform.LookAt(target.x, target.y, target.z);
}


App::App()
{
	s_pApp = this;
	CPU_CALLBACK_START(OnStart);
	CPU_CALLBACK_UPDATE(OnUpdate);
	CPU_CALLBACK_EXIT(OnExit);
	CPU_CALLBACK_RENDER(OnRender);
}

App::~App()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void App::ClearVector()
{
	for (auto it = m_pLoot.begin(); it != m_pLoot.end(); )
	{
		it = m_pLoot.erase(it);
	};
}

float App::GenerateRandomNumber(float min, float max)
{
	min *= 1000;
	max *= 1000;
	int resultresolution = rand() % ((int)max - (int)min + 1) + (int)min;
	float result = resultresolution / 1000.0f;
	return result;
}

bool App::Collision(cpu_entity* S1, cpu_entity* S2)
{
	XMFLOAT3 S1pos = S1->transform.pos;
	XMFLOAT3 S2pos = S2->transform.pos;

	int d2 = (S1pos.x - S2pos.x) * (S1pos.x - S2pos.x) + (S1pos.y - S2pos.y) * (S1pos.y - S2pos.y) + (S1pos.z - S2pos.z) * (S1pos.z - S2pos.z);
	if (d2 > (S1->pMesh->radius / 1.75 + S2->pMesh->radius / 1.75) * (S1->pMesh->radius / 1.75 + S2->pMesh->radius / 1.75))
		return false;
	else
		return true;
}

void App::SpawnLoot()
{
	XMFLOAT3 rotation = { m_pPlateform->transform.pos.x, m_pPlateform->transform.pos.y + 15.0f, m_pPlateform->transform.pos.z };

	cpu_entity* pLoot = cpuEngine.CreateEntity();

	pLoot = cpuEngine.CreateEntity();
	pLoot->pMesh = &m_meshLoot;
	pLoot->pMaterial = &m_materialLoot;
	pLoot->transform.SetYPR(0, XM_PI * 0.5, 0);
	m_angleLastLoot = GenerateRandomNumber(-XM_PI, XM_PI);
	pLoot->transform.OrbitAroundAxis(rotation, CPU_VEC3_UP, 4, m_angleLastLoot);

	m_pLoot.push_back(pLoot);
}


void App::OnStart()
{
	srand(time(NULL));

	std::ifstream fichier("C:/Users/afoucaud/Documents/GitHub/Pipeline-3D-Exo/Score.txt");
	while(std::getline(fichier,toto))
	{
	}

	fichier.close();

	m_pause = false;
	m_loose = false;

	m_hp = 300000000;

	m_score = 0;

	m_SpeedLoot = 3;
	m_timerLootMax = 0.75f;
	m_timerLoot = m_timerLootMax;

	m_angleHero = 0;

	//Ressource
	m_font.Create(cpuDevice.GetHeight() <= 512 ? 14 : 28);
	m_fontB.Create(111);
	m_fontL.Create(15);
	m_meshPlateform.CreateAirPlane();
	m_meshHero.CreateCube(0.00005f);
	m_meshLoot.CreateSphere(0.5f, 5, 50);


	//Shader
	m_materialPlateform.color = cpu::ToColor(85,0,255);
	m_materialHero.color = cpu::ToColor(255, 0, 255);
	m_materialLoot.color = cpu::ToColor(255, 255, 0);


	//3D
	m_pPlateform = cpuEngine.CreateEntity(); 
	m_pPlateform->pMesh = &m_meshPlateform;
	m_pPlateform->pMaterial = &m_materialPlateform;
	m_pPlateform->transform.pos.x = 0.0f;
	m_pPlateform->transform.pos.y = -0.5f;
	m_pPlateform->transform.pos.z = 0.0f;

	m_pHero = cpuEngine.CreateEntity();
	m_pHero->pMesh = &m_meshHero;
	m_pHero->pMaterial = &m_materialHero;
	m_pHero->transform.pos.x = 0.0f;
	m_pHero->transform.pos.y = 0.5f;
	m_pHero->transform.pos.z = -4.0f;


	// Camera
	cpuEngine.GetCamera()->transform.pos.x = 0.0f;
	cpuEngine.GetCamera()->transform.pos.y = 10.0f;
	cpuEngine.GetCamera()->transform.pos.z = -15.0f;
	cpuEngine.GetCamera()->transform.LookAt(0.0f, 0.5f, 0.0f);
}



void App::OnUpdate()
{
	if (cpuInput.IsBackPressed() && m_loose == false)
	{
		m_pause = !m_pause;
	}


	if (m_pause || m_loose)
	{
		if (cpuInput.IsSpacePressed())
			cpuEngine.Quit();

		if (cpuInput.IsRestartPressed())
		{
			cpuEngine.ClearManagers();
			ClearVector();
			OnStart();
		}
		return;
	}

	float dt = cpuTime.delta;
	float time = cpuTime.total;







	//new start

	UpdateCamera(dt);


	//new end





	XMFLOAT3 rotationHero = { m_pPlateform->transform.pos.x, m_pPlateform->transform.pos.y + 1.0f, m_pPlateform->transform.pos.z };
	XMFLOAT3 rotationCam = { m_pPlateform->transform.pos.x, m_pPlateform->transform.pos.y + 10.0f, m_pPlateform->transform.pos.z };
	/*if (cpuInput.IsLeft())
	{
		m_angleHero += dt;
		m_pHero->transform.OrbitAroundAxis(rotationHero, CPU_VEC3_UP, 4, 3 * m_angleHero);
		m_pHero->transform.LookAt(rotationHero.x, rotationHero.y, rotationHero.z);
		cpuEngine.GetCamera()->transform.OrbitAroundAxis(rotationCam, CPU_VEC3_UP, 15, 3 * m_angleHero);
		cpuEngine.GetCamera()->transform.LookAt(rotationHero.x, rotationHero.y, rotationHero.z);
	}
	if (cpuInput.IsRight())
	{
		m_angleHero -= dt;
		m_pHero->transform.OrbitAroundAxis(rotationHero, CPU_VEC3_UP, 4, 3 * m_angleHero);
		m_pHero->transform.LookAt(rotationHero.x, rotationHero.y, rotationHero.z);
		cpuEngine.GetCamera()->transform.OrbitAroundAxis(rotationCam, CPU_VEC3_UP, 15, 3 * m_angleHero);
		cpuEngine.GetCamera()->transform.LookAt(rotationHero.x, rotationHero.y, rotationHero.z);
	}*/



	//Camera
	

	// Spawn Loot
	m_timerLoot -= dt;
	m_timerLootMax *= 0.99999;
	if (m_timerLoot <= 0)
	{
		cpuApp.SpawnLoot();
		m_timerLoot = m_timerLootMax;
	}

	// Action Loot
	for (auto it = m_pLoot.begin(); it != m_pLoot.end(); ++it)
	{
		cpu_entity* pLoot = *it;
		pLoot->transform.Move(dt * m_SpeedLoot);
		if (pLoot->transform.pos.y < pLoot->pMesh->radius / 2)
		{
			cpuEngine.Release(pLoot);
			m_hp--;
			continue;
		}
		if (Collision(pLoot, m_pHero))
		{
			cpuEngine.Release(pLoot);
			m_score++;
		}
	}


	// Purge Loot
	for (auto it = m_pLoot.begin(); it != m_pLoot.end(); )
	{
		if ((*it)->dead)
			it = m_pLoot.erase(it);
		else
			++it;
	};

	// Loose
	if (m_hp <= 0)
	{
		m_loose = true;
		cpuEngine.GetCamera()->transform.LookAt(0, 1000.f, 0);
	}

	if (m_score > atoi(toto.c_str()))
	{
		std::ofstream fichierOF("C:/Users/afoucaud/Documents/GitHub/Pipeline-3D-Exo/Score.txt");
		fichierOF << m_score;
		

		fichierOF.close();

		std::ifstream fichierIF("C:/Users/afoucaud/Documents/GitHub/Pipeline-3D-Exo/Score.txt");
		while (std::getline(fichierIF, toto))
		{
		}
		fichierIF.close();
	}
}

void App::OnExit()
{
	// YOUR CODE HERE
}

void App::OnRender(int pass)
{
	switch (pass)
	{
	case CPU_PASS_PARTICLE_BEGIN:
	{
		// Blur particles
		//cpuEngine.SetRT(m_rts[0]);
		//cpuEngine.ClearColor();
		break;
	}
	case CPU_PASS_PARTICLE_END:
	{
		// Blur particles
		//cpuEngine.Blur(10);
		//cpuEngine.SetMainRT();
		//cpuEngine.AlphaBlend(m_rts[0]);
		break;
	}
	case CPU_PASS_UI_END:
	{
		// Debug
		cpu_stats& stats = *cpuEngine.GetStats();
		std::string score = "score : " + CPU_STR(m_score);
		std::string hp = "hp : " + CPU_STR(m_hp);
		std::string topscore = "Best Score : " + toto;

		XMFLOAT3 tint = { 1.0f, 1.0f, 0.8f };

		if (m_loose)
		{
			cpuDevice.DrawText(&m_fontB, "LOOSE", (int)(cpuDevice.GetWidth() * 0.5f), 100, CPU_TEXT_CENTER, &tint);
			cpuDevice.DrawText(&m_fontB, score.c_str(), (int)(cpuDevice.GetWidth() * 0.5f), 275, CPU_TEXT_CENTER, &tint);
			cpuDevice.DrawText(&m_fontL, "Press 'SPACE' Quit", 100, (int)(cpuDevice.GetHeight() * 0.9), CPU_TEXT_CENTER, &tint);
			cpuDevice.DrawText(&m_fontL, "Press 'R' Restart", (int)(cpuDevice.GetWidth() - 100), (int)(cpuDevice.GetHeight() * 0.9), CPU_TEXT_CENTER, &tint);
			return;
		}

		if (m_pause)
		{
			cpuDevice.DrawText(&m_font, "PAUSE", (int)(cpuDevice.GetWidth() - 60), 10, CPU_TEXT_CENTER, &tint);
			cpuDevice.DrawText(&m_fontL, "Press 'SPACE' Quit", 100, (int)(cpuDevice.GetHeight() * 0.9), CPU_TEXT_CENTER, &tint);
			cpuDevice.DrawText(&m_fontL, "Press 'R' Restart", (int)(cpuDevice.GetWidth() - 100), (int)(cpuDevice.GetHeight() * 0.9), CPU_TEXT_CENTER, &tint);
		}

		cpuDevice.DrawText(&m_font, topscore.c_str(), (int)(cpuDevice.GetWidth() * 0.75f), 10, CPU_TEXT_CENTER, &tint);
		cpuDevice.DrawText(&m_font, score.c_str(), (int)(cpuDevice.GetWidth() * 0.5f), 10, CPU_TEXT_CENTER, &tint);
		cpuDevice.DrawText(&m_font, hp.c_str(), 50, 10, CPU_TEXT_CENTER, &tint);
		break;
	}
	}
}

void App::MyPixelShader(cpu_ps_io& io)
{
	// YOUR CODE HERE
	io.color = io.p.color;
}
