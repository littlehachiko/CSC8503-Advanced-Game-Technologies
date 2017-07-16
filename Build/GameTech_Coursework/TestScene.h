#pragma once

#include <nclgl\Mesh.h>
#include <ncltech\Scene.h>

class TestScene : public Scene
{
public:
	TestScene(const std::string& friendly_name);
	virtual ~TestScene();

	void GenerateSphere();

	virtual void OnInitializeScene()	 override;
	virtual void OnCleanupScene()		 override;
	virtual void OnUpdateScene(float dt) override;
	
protected:
	float m_AccumTime;
	Object* m_pPlayer1;
	Object* m_pPlayer2;
	Object* sphere1;
	Object* sphere2;
	Object* cube1;
};