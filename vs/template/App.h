#pragma once

class App
{
public:
	App();
	virtual ~App();

	static App& GetInstance() { return *s_pApp; }

	void ClearVector();

	float GenerateRandomNumber(float min, float max);

	bool Collision(cpu_entity* S1, cpu_entity* S2);

	void SpawnLoot();

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

	cpu_font m_font;
	cpu_font m_fontB;
	cpu_font m_fontL;
	cpu_mesh m_meshHero;
	cpu_material m_materialHero;
	cpu_entity* m_pHero;

	cpu_mesh m_meshLoot;
	cpu_material m_materialLoot;
	std::vector<cpu_entity*> m_pLoot;
	float m_SpeedLoot;

	float m_timerLoot;
	float m_timerLootMax;
	float m_angleHero;
	float m_angleLastLoot;

	bool m_loose;
	bool m_pause;
	int m_hp;
	int m_score;
	std::string toto;
};
