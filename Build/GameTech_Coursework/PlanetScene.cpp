#include "PlanetScene.h"

#include <nclgl\OBJMesh.h>
#include <nclgl\Vector4.h>
#include <ncltech\ObjectMesh.h>
#include <ncltech\PhysicsEngine.h>
#include <ncltech\DistanceConstraint.h>
#include <ncltech\SceneManager.h>
#include <ncltech\CommonUtils.h>
#include <ncltech\CommonMeshes.h>
using namespace CommonUtils;

const Vector4 status_colour = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
float rotation = 0.0f;

PlanetScene::PlanetScene(const std::string& friendly_name)
	: Scene(friendly_name)
	, m_AccumTime(0.0f)
	, m_pServerConnection(NULL)
	, tardis(NULL)
{

}

PlanetScene::~PlanetScene()
{

}


void PlanetScene::OnInitializeScene()
{
	//13/12/2016
	//Initialize Client Network
	if (m_Network.Initialize(0))
	{
		NCLDebug::Log("Network: Initialized!");

		//Attempt to connect to the server on localhost:1234
		m_pServerConnection = m_Network.ConnectPeer(127, 0, 0, 1, 1234);
		NCLDebug::Log("Network: Attempting to connect to server.");
	}

	//Disable the physics engine (We will be starting this later!)
	PhysicsEngine::Instance()->SetPaused(true);
	//13/12/2016
	//Yupei
	PhysicsEngine::Instance()->SetAtmosphere(false);

	//Set the camera position
	SceneManager::Instance()->GetCamera()->SetPosition(Vector3(15.0f, 10.0f, -15.0f));
	SceneManager::Instance()->GetCamera()->SetYaw(140.f);
	SceneManager::Instance()->GetCamera()->SetPitch(-20.f);

	PhysicsEngine::Instance()->SetDebugDrawFlags(DEBUGDRAW_FLAGS_CONSTRAINT);

	m_AccumTime = 0.0f;
	m_visable = true;
	Vector4 col = Vector4(1.0f, 1.0f, 1.0f, 1.0f);

	//<--- SCENE CREATION --->
	//06/12/2016
	//Yupei
	//Create a planet
	{
		Object* planet1 = BuildPlanetObject(
			"planet1",					// Optional: Name
			Vector3(-40.f, -18.f, 50.f),	// Position
			26.0f,			// Half-Dimensions
			true,				// Physics Enabled?
			0.0f,				// Physical Mass (must have physics enabled)
			true,				// Physically Collidable (has collision shape)
			false,				// Dragable by user?
			col);// Render colour

		this->AddGameObject(planet1);
		Object* obj1 = this->FindGameObject("planet1");
		obj1->Physics()->SetRestState(false);
	}

	//Create a cube 
	{
		Object* cube = CommonUtils::BuildCuboidObject(
			"cube",						// Optional: Name
			Vector3(-10.0f, 0.5f, 50.f),	// Position
			Vector3(1.6f, 1.6f, 1.6f),		// Half-Dimensions
			true,					// Physics Enabled?
			0.0f,					// Physical Mass (must have physics enabled)
			true,					// Physically Collidable (has collision shape)
			false,					// Dragable by user?
			col);				// Render colour
		this->AddGameObject(cube);
	}

	//Create a sphere
	{
		Object* ball = CommonUtils::BuildPlanetObject(
			"ball",						// Optional: Name
			Vector3(0.0f, 0.5f, 50.f),	// Position
			1.6f,			// Half-Dimensions
			true,				// Physics Enabled?
			0.0f,				// Physical Mass (must have physics enabled)
			true,				// Physically Collidable (has collision shape)
			false,				// Dragable by user?
			col);// Render colour
		this->AddGameObject(ball);
	}

	//Create a can
	{
		Object*  can = CommonUtils::BuildCanObject(
			"can",						// Optional: Name
			Vector3(10.0f, 0.5f, 50.0f),	// Position
			Vector3(0.7f, 0.8f, 0.7f),		// Half-Dimensions
			true,					// Physics Enabled?
			0.0f,					// Physical Mass (must have physics enabled)
			true,					// Physically Collidable (has collision shape)
			false,					// Dragable by user?
			col);				// Render colour
		this->AddGameObject(can);
	}

	//Create Target
	{
		Object* target = CommonUtils::BuildBoardObject(
			"Target",
			Vector3(-40.0f, 13.1f, 50.0f),
			Vector3(5.0f, 5.0f, 0.5f),
			true,
			0.0f,
			true,
			false,
			col);
		target->Physics()->SetTargetBoardState(true);
		this->AddGameObject(target);
	}

	//Create a ball wall
	auto create_ball_cube = [&](const Vector3& offset, const Vector3& scale, float ballsize)
	{
		const int dims = 5;

		for (int x = 0; x < dims; ++x)
		{
			for (int y = 0; y < dims; ++y)
			{

				Vector3 pos = offset + Vector3(scale.x *x, scale.y * y, 1.0f);
				Object* sphere = BuildSphereObject(
					"",					// Optional: Name
					pos,				// Position
					ballsize,			// Half-Dimensions
					true,				// Physics Enabled?
					1.f,				// Physical Mass (must have physics enabled)
					true,				// Physically Collidable (has collision shape)
					false,				// Dragable by user?
					col);// Render colour
				this->AddGameObject(sphere);
			}
		}
	};
	create_ball_cube(Vector3(-60.0f, 2.5f, 20.0f), Vector3(0.8f, 0.8f, 0.8f), 0.3f);

	//Create a cube wall
	auto create_cube_wall = [&](const Vector3& offset, float cubewidth)
	{
		const Vector3 halfdims = Vector3(cubewidth, cubewidth, cubewidth) * 0.3f;
		for (int x = 0; x < 5; ++x)
		{
			for (int y = 0; y < 5; ++y)
			{
				Vector3 pos = offset + Vector3(x * cubewidth, y * cubewidth, cubewidth);

				Object* cube = BuildCuboidObject(
					"",						// Optional: Name
					pos,					// Position
					halfdims,				// Half-Dimensions
					true,					// Physics Enabled?
					1.f,					// Physical Mass (must have physics enabled)
					true,					// Physically Collidable (has collision shape)
					false,					// Dragable by user?
					col);				// Render colour
				this->AddGameObject(cube);
			}
		}
	};
	create_cube_wall(Vector3(-50.0f, 3.5f, 20.0f), 0.6f);

	//Create a can wall
	auto create_can_wall = [&](const Vector3& offset, float cubewidth)
	{
		const Vector3 halfdims = Vector3(cubewidth, cubewidth + 0.1f, cubewidth) * 0.3f;
		for (int x = 0; x < 5; ++x)
		{
			for (int y = 0; y < 5; ++y)
			{

				Vector3 pos = offset + Vector3(x * cubewidth, 1e-3f + y * cubewidth, cubewidth);

				Object* cans = BuildCanObject(
					"",						// Optional: Name
					pos,					// Position
					halfdims,				// Half-Dimensions
					true,					// Physics Enabled?
					1.f,					// Physical Mass (must have physics enabled)
					true,					// Physically Collidable (has collision shape)
					false,					// Dragable by user?
					col);				// Render colour
				this->AddGameObject(cans);
			}
		}
	};
	create_can_wall(Vector3(-45.0f, 3.5f, 20.0f), 0.6f);
	
	//Create Atmosphere
	{
	Object* atmosphere = BuildPlanetObject(
		"atmosphere",					// Optional: Name
		Vector3(-40.f, -18.f, 50.f),	// Position
		28.0f,			// Half-Dimensions
		true,				// Physics Enabled?
		0.0f,				// Physical Mass (must have physics enabled)
		false,				// Physically Collidable (has collision shape)
		false,				// Dragable by user?
		Vector4(1.0f, 1.0f, 1.0f, 0.16f));// Render colour
		this->AddGameObject(atmosphere);
	}

	//Create Tardis
	{
		tardis = CommonUtils::BuildTardisObject(
			"Tardis",
			Vector3(0.0f, 1.0f, 0.0f),
			Vector3(1.6f, 2.5f, 1.6f),
			true,									//Physics Enabled here Purely to make setting position easier via Physics()->SetPosition()
			0.0f,
			true,
			false,
			Vector4(1.0f, 1.0f, 1.0f, 1.0f));
		this->AddGameObject(tardis);
	}

	const Vector3 ss_pos = Vector3(-40.f, -18.f, 50.f);
	//Create a box which has horizontal quads and vertical quads
	{
		Object* plane1 = CommonUtils::BuildPlaneObject(
			"Plane1",
			ss_pos + Vector3(5.0f, 0.f, -45.0f),
			Vector3(5.f, 5.0f, 0.0f),
			true,
			0.0f,
			true,
			false,
			Vector4(1.f, 1.0f, 1.0f, 1.0f));
		this->AddGameObject(plane1);
		Object* Plane1 = this->FindGameObject("Plane1");
		Plane1->Physics()->SetOrientation(Quaternion::EulerAnglesToQuaternion(0.0f, 0.0f, 0.0f));

		Object* plane2 = CommonUtils::BuildPlaneObject(
			"Plane2",
			ss_pos + Vector3(5.0f, 0.f, -55.0f),
			Vector3(5.f, 5.0f, 0.0f),
			true,
			0.0f,
			true,
			false,
			Vector4(1.f, 1.0f, 1.0f, 1.0f));


		this->AddGameObject(plane2);
		Object* Plant2 = this->FindGameObject("Plane2");
		Plant2->Physics()->SetOrientation(Quaternion::EulerAnglesToQuaternion(-180.0f, .0f, 0.0f));

		Object* plane3 = CommonUtils::BuildPlaneObject(
			"Plane3",
			ss_pos + Vector3(5.0f, 5.f, -50.0f),
			Vector3(5.f, 5.0f, 0.0f),
			true,
			0.0f,
			true,
			false,
			Vector4(1.f, 1.0f, 1.0f, 1.0f));
		this->AddGameObject(plane3);
		Object* Plant3 = this->FindGameObject("Plane3");
		Plant3->Physics()->SetOrientation(Quaternion::EulerAnglesToQuaternion(270.0f, .0f, 0.0f));

		Object* plane4 = CommonUtils::BuildPlaneObject(
			"Plane4",
			ss_pos + Vector3(5.0f, -5.f, -50.0f),
			Vector3(5.f, 5.0f, 0.0f),
			true,
			0.0f,
			true,
			false,
			Vector4(1.f, 1.0f, 1.0f, 1.0f));


		this->AddGameObject(plane4);
		Object* Plant4 = this->FindGameObject("Plane4");
		Plant4->Physics()->SetOrientation(Quaternion::EulerAnglesToQuaternion(-270.0f, .0f, 0.0f));

		Object* plane5 = CommonUtils::BuildPlaneObject(
			"Plane5",
			ss_pos + Vector3(10.0f, 0.f, -50.0f),
			Vector3(5.f, 5.0f, 0.0f),
			true,
			0.0f,
			true,
			false,
			Vector4(1.f, 1.0f, 1.0f, 1.0f));


		this->AddGameObject(plane5);
		Object* Plane5 = this->FindGameObject("Plane5");
		Plane5->Physics()->SetOrientation(Quaternion::EulerAnglesToQuaternion(0.0f, 90.0f, 0.0f));

		Object* plane6 = CommonUtils::BuildPlaneObject(
			"Plane6",
			ss_pos + Vector3(0.0f, 0.f, -50.0f),
			Vector3(5.f, 5.0f, 0.0f),
			true,
			0.0f,
			true,
			false,
			Vector4(1.f, 1.0f, 1.0f, 1.0f));


		this->AddGameObject(plane6);
		Object* Plant6 = this->FindGameObject("Plane6");
		Plant6->Physics()->SetOrientation(Quaternion::EulerAnglesToQuaternion(0.0f, -90.0f, 0.0f));

	}

}

//Create tennis balls
void PlanetScene::GenerateSphere()
{
	Object* sphere2 = BuildSphereObject(
		"balls",					// Optional: Name
		Vector3(SceneManager::Instance()->GetCamera()->GetPosition()),  // Position
		0.6f,				// Half-Dimensions
		true,				// Physics Enabled?
		2.6f,				// Physical Mass (must have physics enabled)
		true,				// Physically Collidable (has collision shape)
		true,				// Dragable by user?
		Vector4(1.0f, 1.0f, 1.0f, 1.0f));// Render colour

	Matrix3 view_rotation = Matrix3::Inverse(Matrix3(SceneManager::Instance()->GetCamera()->BuildViewMatrix()));
	Vector3 forward = view_rotation * Vector3(0, 0, -1);
	sphere2->Physics()->SetRestState(false);
	sphere2->Physics()->SetLinearVelocity(forward * 66.88f);
	sphere2->Physics()->SetProjectileState(true);
	this->AddGameObject(sphere2);
}

void PlanetScene::OnCleanupScene()
{
	//Just delete all created game objects 
	//  - this is the default command on any Scene instance so we don't really need to override this function here.
	Scene::OnCleanupScene();
	tardis = NULL;

	//Network
	//Send one final packet telling the server we are disconnecting
	// - We are not waiting to resend this, so if it fails to arrive
	//   the server will have to wait until we time out naturally
	if (m_pServerConnection) {
		enet_peer_disconnect_now(m_pServerConnection, 0);

		//Release network and all associated data/peer connections
		m_Network.Release();
		m_pServerConnection = NULL;
	}
}


void PlanetScene::OnUpdateScene(float dt)
{

	m_AccumTime += dt;


	// You can add status entries to the top left from anywhere in the program
	NCLDebug::AddStatusEntry(Vector4(1.0f, 0.2f, 0.2f, 1.0f), "Welcome to the Game Tech Planet Scene!");
	NCLDebug::AddStatusEntry(Vector4(1.0f, 0.2f, 0.4f, 1.0f), "Press L and see the high score board!");

	// Lets sun a little bit...
	Vector3 invLightDir = Matrix4::Rotation(15.f * dt, Vector3(0.0f, 1.0f, 0.0f)) * SceneManager::Instance()->GetInverseLightDirection();
	SceneManager::Instance()->SetInverseLightDirection(invLightDir);

	//Vector4 beforeCol = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	//Vector4 whenCol = CommonUtils::GenColour(0.1f, 1.0f);

	Object* obj1 = this->FindGameObject("planet1");
	if (obj1 != NULL)
	{
		obj1->Physics()->SetRestState(false);
		obj1->Physics()->SetAngularVelocity(Vector3(0.0f, 0.18f, 0.0f));
	}
	//
	if (!PhysicsEngine::Instance()->GetVisableState())
	{
		obj1->SetColour(Vector4(1.0f, 1.0f, 1.0f, 0.0f));
	}

	Object* obj2 = this->FindGameObject("Target");
	if (obj2 != NULL)
	{
		obj2->Physics()->SetRestState(false);
		obj2->Physics()->SetTargetBoardState(true);
		obj2->Physics()->SetAngularVelocity(Vector3(0.0f, 0.18f, 0.0f));
	}

	NCLDebug::AddStatusEntry(Vector4(1.0f, 0.5f, 0.2f, 1.0f), "Score: %5.2f" , PhysicsEngine::Instance()->GetScore());

	obj2->Physics()->SetOnCollisionCallback([obj1](PhysicsObject* self, PhysicsObject* collidingObject)
	{
		float dis = (self->GetPosition() - collidingObject->GetPosition()).LengthSquared();

		if (collidingObject->IsProjectile() && collidingObject->IsFirstCol())
		{
			float tempScore =  1/dis * 1000 + PhysicsEngine::Instance()->GetScore();
			PhysicsEngine::Instance()->SetScore(tempScore);
		}
		return true;
	});
		
	{
		rotation += 0.5f * PI * dt;
		Vector3 pos = Vector3(cos(rotation) * 2.0f, 1.5f, sin(rotation) * 2.0f);
		tardis->Physics()->SetPosition(pos);
		if (tardis->Physics()->IsCollide()) 
		{
			PhysicsEngine::Instance()->SetScore(PhysicsEngine::Instance()->GetScore()+200.0f);
		}

		
	}
	//Launch balls into the scene
	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_J))
	{
		GenerateSphere();
	}

	//Newwork
	//Update Network
	auto callback = std::bind(
		&PlanetScene::ProcessNetworkEvent,	// Function to call
		this,								// Associated class instance
		std::placeholders::_1);				// Where to place the first parameter
	m_Network.ServiceNetwork(dt, callback);



	//Add Debug Information to screen
	uint8_t ip1 = m_pServerConnection->address.host & 0xFF;
	uint8_t ip2 = (m_pServerConnection->address.host >> 8) & 0xFF;
	uint8_t ip3 = (m_pServerConnection->address.host >> 16) & 0xFF;
	uint8_t ip4 = (m_pServerConnection->address.host >> 24) & 0xFF;

	NCLDebug::DrawTextWs(tardis->Physics()->GetPosition() + Vector3(0.f, 0.5f, 0.f), 14.f, TEXTALIGN_CENTRE, Vector4(),
		"Peer: %u.%u.%u.%u:%u", ip1, ip2, ip3, ip4, m_pServerConnection->address.port);

	Vector4 status_color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	NCLDebug::AddStatusEntry(status_color, "Network Traffic");
	NCLDebug::AddStatusEntry(status_color, "    Incoming: %5.2fKbps", m_Network.m_IncomingKb);
	NCLDebug::AddStatusEntry(status_color, "    Outgoing: %5.2fKbps", m_Network.m_OutgoingKb);

	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_L))
	{
		SendScoreToServer();
	}

}


void PlanetScene::ProcessNetworkEvent(const ENetEvent& evnt)
{
	switch (evnt.type)
	{
		//New connection request or an existing peer accepted our connection request
	case ENET_EVENT_TYPE_CONNECT:
	{
		if (evnt.peer == m_pServerConnection)
		{
			NCLDebug::Log("Network: Successfully connected to server!");
		}
	}
	break;


	//Server has sent us a new packet
	case ENET_EVENT_TYPE_RECEIVE:
	{

		int* all_ints = reinterpret_cast<int *>(evnt.packet->data);
		int num_ints = evnt.packet->dataLength / sizeof(int);

		if (num_ints)
		{
			printf("----------High Score----------\n");
			for (int i = 0; i < num_ints; i++)
			{
				printf(" %d\t%d\n", (i + 1), all_ints[i]);
			}

			printf("\n");
		}
	}
	break;


	//Server has disconnected
	case ENET_EVENT_TYPE_DISCONNECT:
	{
		NCLDebug::Log("Network: Server has disconnected!");
	}
	break;
	}
}

void PlanetScene::SendScoreToServer()
{
	if (PhysicsEngine::Instance()->GetScore() > 0)
	{
		// Send score data to server
		int score = (int)PhysicsEngine::Instance()->GetScore();

		ostringstream text_score;
		text_score << score;
		ENetPacket* packet = enet_packet_create(
			text_score.str().c_str(),
			strlen(text_score.str().c_str()) + 1,
			ENET_PACKET_FLAG_RELIABLE
			);
		enet_peer_send(m_pServerConnection, 0, packet);
	}
}