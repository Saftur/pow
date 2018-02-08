#pragma once

enum LM_STATE { IDLE, LOADING };

class LevelManager
{
public:
	void Init();
	void Update(float dt);
	void Shutdown();
	static void Load(const char* fileName);
	static LevelManager& GetInstance();
private:
	void loadObject();

	static LM_STATE stateCurr, stateNext;
	unsigned int id = 0;
};
