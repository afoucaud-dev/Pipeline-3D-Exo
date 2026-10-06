#include "pch.h"

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

void App::OnStart()
{
	angle = 0;

	//Ressource
	m_meshPlateform.CreateCylinder(0.5f, 5, 50);

	m_meshHero.CreateCube(0.5f);


	//Shader
	m_materialPlateform.color = cpu::ToColor(85,0,255);

	m_materialHero.color = cpu::ToColor(255, 0, 255);


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

	DirectX::XMFLOAT3 rotation = { m_pPlateform->transform.pos.x, m_pPlateform->transform.pos.y + 1.0f, m_pPlateform->transform.pos.z };
	if (cpuInput.IsLeft())
	{
		angle += dt;
		m_pHero->transform.OrbitAroundAxis(rotation, CPU_VEC3_UP, 4, 25 * angle);
		m_pHero->transform.LookAt(rotation.x, rotation.y, rotation.z);
	}
	if (cpuInput.IsRight())
	{
		angle -= dt;
		m_pHero->transform.OrbitAroundAxis(rotation, CPU_VEC3_UP, 4, 25 * angle);
		m_pHero->transform.LookAt(rotation.x, rotation.y, rotation.z);
	}
	//Camera
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
