#include "pch.h"
#include <random>

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


int App::GenerateRandomNumber(int min, int max)
{
	return rand() % (max - min + 1) + min;
}

void App::SpawnLoot()
{
	XMFLOAT3 rotation = { m_pPlateform->transform.pos.x, m_pPlateform->transform.pos.y + 15.0f, m_pPlateform->transform.pos.z };

	cpu_entity* pLoot = cpuEngine.CreateEntity();

	pLoot = cpuEngine.CreateEntity();
	pLoot->pMesh = &m_meshLoot;
	pLoot->pMaterial = &m_materialLoot;
	pLoot->transform.SetYPR(0, XM_PI * 0.5, 0);
	m_angleLastLoot = GenerateRandomNumber(0, 360);
	pLoot->transform.OrbitAroundAxis(rotation, CPU_VEC3_UP, 4, m_angleLastLoot);

	m_pLoot.push_back(pLoot);
}


void App::OnStart()
{
	srand(time(NULL));

	m_SpeedLoot = 5;
	m_timerLootMax = 0.5f;
	m_timerLoot = m_timerLootMax;

	m_angleHero = 0;

	//Ressource
	m_meshPlateform.CreateCylinder(0.5f, 5, 50);
	m_meshHero.CreateCube(0.5f);
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
	cpuEngine.GetCamera()->transform.pos.y = 10.0f;
	cpuEngine.GetCamera()->transform.pos.z = -15.0f;
	cpuEngine.GetCamera()->transform.LookAt(0.0f, 0.5f, 0.0f);
}

void App::OnUpdate()
{
	float dt = cpuTime.delta;
	float time = cpuTime.total;

	XMFLOAT3 rotationHero = { m_pPlateform->transform.pos.x, m_pPlateform->transform.pos.y + 1.0f, m_pPlateform->transform.pos.z };
	XMFLOAT3 rotationCam = { m_pPlateform->transform.pos.x, m_pPlateform->transform.pos.y + 10.0f, m_pPlateform->transform.pos.z };
	if (cpuInput.IsLeft())
	{
		m_angleHero += dt;
		m_pHero->transform.OrbitAroundAxis(rotationHero, CPU_VEC3_UP, 4, 5 * m_angleHero);
		m_pHero->transform.LookAt(rotationHero.x, rotationHero.y, rotationHero.z);
		cpuEngine.GetCamera()->transform.OrbitAroundAxis(rotationCam, CPU_VEC3_UP, 15, 5 * m_angleHero);
		cpuEngine.GetCamera()->transform.LookAt(rotationHero.x, rotationHero.y, rotationHero.z);
	}
	if (cpuInput.IsRight())
	{
		m_angleHero -= dt;
		m_pHero->transform.OrbitAroundAxis(rotationHero, CPU_VEC3_UP, 4, 5 * m_angleHero);
		m_pHero->transform.LookAt(rotationHero.x, rotationHero.y, rotationHero.z);
		cpuEngine.GetCamera()->transform.OrbitAroundAxis(rotationCam, CPU_VEC3_UP, 15, 5 * m_angleHero);
		cpuEngine.GetCamera()->transform.LookAt(rotationHero.x, rotationHero.y, rotationHero.z);
	}


	//Camera
	

	// Spawn Loot
	m_timerLoot -= dt;
	if (m_timerLoot <= 0)
	{
		cpuApp.SpawnLoot();
		m_timerLoot = m_timerLootMax;
	}

	// Move Loot
	for (auto it = m_pLoot.begin(); it != m_pLoot.end(); ++it)
	{
		cpu_entity* pLoot = *it;
		pLoot->transform.Move(dt * m_SpeedLoot);
		if (pLoot->transform.pos.y < pLoot->pMesh->radius / 2)
			cpuEngine.Release(pLoot);
	}


	// Purge Loot
	for (auto it = m_pLoot.begin(); it != m_pLoot.end(); )
	{
		if ((*it)->dead)
			it = m_pLoot.erase(it);
		else
			++it;
	}

	
	// Quit
	if (cpuInput.IsBackPressed())
		cpuEngine.Quit();
}

void App::OnExit()
{
	// YOUR CODE HERE
}

void App::OnRender(int pass)
{
	// YOUR CODE HERE
}

void App::MyPixelShader(cpu_ps_io& io)
{
	// YOUR CODE HERE
	io.color = io.p.color;
}
