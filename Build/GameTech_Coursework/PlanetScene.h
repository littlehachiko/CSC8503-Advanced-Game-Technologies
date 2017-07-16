#pragma once

#include <nclgl\Mesh.h>
#include <ncltech\Scene.h>
#include <ncltech\CommonUtils.h>
#include <ncltech\NetworkBase.h>

class PlanetScene : public Scene
{
public:
	PlanetScene(const std::string& friendly_name);

	virtual ~PlanetScene();

	virtual void OnInitializeScene()	 override;
	virtual void OnCleanupScene()		 override;
	virtual void OnUpdateScene(float dt) override;

	void GenerateSphere();

	//13/12/2016
	//Network
	//Yupei
	void ProcessNetworkEvent(const ENetEvent& evnt);
	void SendScoreToServer();
	float m_AccumTime;
	bool  m_Rotating;
	int   m_score;
	bool  m_visable;
	//const Vector3 sc_pos = Vector3(-40.f, -18.f, 50.f);
	Object* tardis;
	//Network
	//Yupei
	NetworkBase m_Network;
	ENetPeer*	m_pServerConnection;
};