#pragma once

class App
{
public:
	App();
	virtual ~App();

	static App& GetInstance() { return *s_pApp; }

	void OnStart();
	void OnUpdate();
	void OnExit();
	void OnRender(int pass);

	static void MyPixelShader(cpu_ps_io& io);

private:
	inline static App* s_pApp = nullptr;
	cpu_mesh m_meshPlateform;
	cpu_material m_materialPlateform;
	cpu_entity* m_pPlateform;

	cpu_mesh m_meshHero;
	cpu_material m_materialHero;
	cpu_entity* m_pHero;

	/*cpu_mesh m_meshL;
	cpu_material m_materialHero;
	cpu_entity* m_pHero;*/

	float angle;
};
