#include "TestScene.h"

#include <nclgl\Vector4.h>
#include <ncltech\PhysicsEngine.h>
#include <ncltech\DistanceConstraint.h>
#include <ncltech\SceneManager.h>
#include <ncltech\CommonUtils.h>
using namespace CommonUtils;

TestScene::TestScene(const std::string& friendly_name)
	: Scene(friendly_name)
	, m_AccumTime(0.0f)
	, m_pPlayer1(NULL)
	, m_pPlayer2(NULL)
{
}

TestScene::~TestScene()
{

}


void TestScene::OnInitializeScene()
{
	//Disable the physics engine (We will be starting this later!)
	PhysicsEngine::Instance()->SetPaused(true);

	//Set the camera position
	SceneManager::Instance()->GetCamera()->SetPosition(Vector3(15.0f, 10.0f, -15.0f));
	SceneManager::Instance()->GetCamera()->SetYaw(140.f);
	SceneManager::Instance()->GetCamera()->SetPitch(-20.f);

	PhysicsEngine::Instance()->SetDebugDrawFlags(DEBUGDRAW_FLAGS_CONSTRAINT);

	m_AccumTime = 0.0f;

	//Example usage of Log and LogE inside NCLDebug functionality
	NCLDebug::Log("This is a log entry");
	NCLERROR("THIS IS AN ERROR!");



	//<--- SCENE CREATION --->
		//Create Ground
	this->AddGameObject(BuildCuboidObject("Ground", Vector3(0.0f, -1.0f, 0.0f), Vector3(20.0f, 1.0f, 20.0f), false, 0.0f, true, false, Vector4(0.2f, 0.5f, 1.0f, 1.0f)));

	//Create Player (See OnUpdateScene)
	m_pPlayer1 = BuildCuboidObject(
		"Player1",					// Optional: Name
		Vector3(5.f, 0.5f, 0.0f),	// Position
		Vector3(0.5f, 0.5f, 1.0f),  // Half-Dimensions
		true,						// Physics Enabled?
		0.0f,						// Physical Mass (must have physics enabled)
		true,						// Physically Collidable (has collision shape)
		true,						// Dragable by user?
		Vector4(0.1f, 0.1f, 0.1f, 1.0f)); // Render colour
	this->AddGameObject(m_pPlayer1);

	m_pPlayer2 = BuildSphereObject(
		"Player2",					// Optional: Name
		Vector3(12.f, 0.5f, 1.0f),	// Position
		0.5f,	// Half-Dimensions
		true,				// Physics Enabled?
		0.1f,				// Physical Mass (must have physics enabled)
		true,				// Physically Collidable (has collision shape)
		true,				// Dragable by user?
		Vector4(0.8f, 0.f, 1.0f, 1.0f));// Render colour
	this->AddGameObject(m_pPlayer2);

	auto create_cube_tower = [&](const Vector3& offset, float cubewidth)
	{
		const Vector3 halfdims = Vector3(cubewidth, cubewidth, cubewidth) * 0.5f;
		for (int x = 0; x < 2; ++x)
		{
			for (int y = 0; y < 6; ++y)
			{
				uint idx = x * 5 + y;
				Vector4 colour = GenColour(idx / 10.f, 0.5f);
				Vector3 pos = offset + Vector3(x * cubewidth, 1e-3f + y * cubewidth, cubewidth * (idx % 2 == 0) ? 0.5f : -0.5f);

				Object* cube = BuildCuboidObject(
					"",						// Optional: Name
					pos,					// Position
					halfdims,				// Half-Dimensions
					false,					// Physics Enabled?
					0.f,					// Physical Mass (must have physics enabled)
					false,					// Physically Collidable (has collision shape)
					true,					// Dragable by user?
					colour);				// Render colour
				this->AddGameObject(cube);
			}
		}
	};

	auto create_ball_cube = [&](const Vector3& offset, const Vector3& scale, float ballsize)
	{
		const int dims = 10;
		const Vector4 col = Vector4(1.0f, 0.5f, 0.2f, 1.0f);

		for (int x = 0; x < dims; ++x)
		{
			for (int y = 0; y < dims; ++y)
			{
				for (int z = 0; z < dims; ++z)
				{
					Vector3 pos = offset + Vector3(scale.x *x, scale.y * y, scale.z * z);
					Object* sphere = BuildSphereObject(
						"",					// Optional: Name
						pos,				// Position
						ballsize,			// Half-Dimensions
						false,				// Physics Enabled?
						0.f,				// Physical Mass (must have physics enabled)
						false,				// Physically Collidable (has collision shape)
						false,				// Dragable by user?
						col);// Render colour
					this->AddGameObject(sphere);
				}
			}
		}
	};

	//create ball
	auto create_ball = [&](const Vector3& offset, float ballsize)
	{
		const int dims = 10;
		const Vector4 col = Vector4(1.0f, 0.2f, 0.2f, 1.0f);

		Vector3 pos = offset;
		Object* sphere1 = BuildSphereObject(
			"",					// Optional: Name
			pos,				// Position
			ballsize,			// Half-Dimensions
			true,				// Physics Enabled?
			0.0f,				// Physical Mass (must have physics enabled)
			false,				// Physically Collidable (has collision shape)
			true,				// Dragable by user?
			col);// Render colour
		this->AddGameObject(sphere1);
	};

	auto create_cube = [&](const Vector3& offset, float cubewidth)
	{
		const Vector3 halfdims = Vector3(cubewidth, cubewidth, cubewidth) * 0.5f;
		Vector3 pos = offset + Vector3(cubewidth, 1e-3f + cubewidth, 0.5f);
		Object* cube1 = BuildCuboidObject(
		"cube1",					// Optional: Name
		pos,	// Position
		halfdims,				    // Half-Dimensions
		true,						// Physics Enabled?
		0.0f,						// Physical Mass (must have physics enabled)
		true,						// Physically Collidable (has collision shape)
		true,						// Dragable by user?
		Vector4(0.1f, 0.1f, 0.1f, 1.0f)); // Render colour
	this->AddGameObject(cube1);
	};
	//Create Cube Towers
	create_cube_tower(Vector3(3.0f, 0.5f, 3.0f), 1.0f);
	create_cube_tower(Vector3(-3.0f, 0.5f, -3.0f), 1.0f);

	//Create Test Ball Pit
	create_ball_cube(Vector3(-8.0f, 0.5f, 12.0f), Vector3(0.5f, 0.5f, 0.5f), 0.1f);
	create_ball_cube(Vector3(8.0f, 0.5f, 12.0f), Vector3(0.3f, 0.3f, 0.3f), 0.1f);
	create_ball_cube(Vector3(-8.0f, 0.5f, -12.0f), Vector3(0.2f, 0.2f, 0.2f), 0.1f);
	create_ball_cube(Vector3(8.0f, 0.5f, -12.0f), Vector3(0.5f, 0.5f, 0.5f), 0.1f);

	//Create Ball
	create_ball(Vector3(15.0f, 1.0f, -2.0f), 1.0f);

	//Create cube
	create_cube(Vector3(3.5f, 0.6f, 3.5f), 2.0f);

///////////////////////create two objects and their consraint
	{
		Object* sphere3 = BuildSphereObject(
		"ball1",					// Optional: Name
		Vector3(-4.f, 7.f, -5.0f),				// Position
		0.5f,			// Half-Dimensions
		true,				// Physics Enabled?
		0.0f,				// Physical Mass (must have physics enabled)
		false,				// Physically Collidable (has collision shape)
		true,				// Dragable by user?
		CommonUtils::GenColour(0.45f, 0.5f));// Render colour

		Object* sphere4 = BuildSphereObject(
		"ball2",					// Optional: Name
		Vector3(-7.f, 7.f, -5.0f),				// Position
		0.5f,			// Half-Dimensions
		true,				// Physics Enabled?
		0.0f,				// Physical Mass (must have physics enabled)
		false,				// Physically Collidable (has collision shape)
		true,				// Dragable by user?
		CommonUtils::GenColour(0.5f, 1.0f));// Render colour

		
		this->AddGameObject(sphere3);
		this->AddGameObject(sphere4);

		//Add distance constraint between the two objects
		DistanceConstraint* constraint = new DistanceConstraint(
			sphere3->Physics(),					//Physics Object A
			sphere4->Physics(),					//Physics Object B
			sphere3->Physics()->GetPosition(),	//Attachment Position on Object A	-> Currently the centre
			sphere4->Physics()->GetPosition());	//Attachment Position on Object B	-> Currently the centre  
		PhysicsEngine::Instance()->AddConstraint(constraint);
	}

///////////////////////create two objects and their consraint
	//{
	//	Object* sphere5 = BuildSphereObject(
	//		"ball3",					// Optional: Name
	//		Vector3(-6.f, 2.f, -5.0f),				// Position
	//		0.5f,			// Half-Dimensions
	//		true,				// Physics Enabled?
	//		0.0f,				// Physical Mass (must have physics enabled)
	//		false,				// Physically Collidable (has collision shape)
	//		true,				// Dragable by user?
	//		CommonUtils::GenColour(0.2f, 0.8f));// Render colour


	//	this->AddGameObject(sphere5);

	//	//Add distance constraint between the two objects
	//	DistanceConstraint* constraint = new DistanceConstraint(
	//		sphere5->Physics(),					//Physics Object A
	//		//sphere5->Physics(),					//Physics Object B
	//		sphere5->Physics()->GetPosition(),	//Attachment Position on Object A	-> Currently the centre
	//		//sphere4->Physics()->GetPosition());	//Attachment Position on Object B	-> Currently the centre  
	//	PhysicsEngine::Instance()->AddConstraint(constraint);
	//}

}

void TestScene::OnCleanupScene()
{
	//Just delete all created game objects 
	//  - this is the default command on any Scene instance so we don't really need to override this function here.
	Scene::OnCleanupScene(); 
}

void TestScene::GenerateSphere()
{
	Object* sphere2 = BuildSphereObject(
		"",					// Optional: Name
		Vector3(SceneManager::Instance()->GetCamera()->GetPosition()),  // Position
		0.16f,				// Half-Dimensions
		true,				// Physics Enabled?
		0.0f,				// Physical Mass (must have physics enabled)
		true,				// Physically Collidable (has collision shape)
		true,				// Dragable by user?
		Vector4(1.0f, 0.7f, 0.76f, 1.0f));// Render colour

	Matrix3 view_rotation = Matrix3::Inverse(Matrix3(SceneManager::Instance()->GetCamera()->BuildViewMatrix()));
	Vector3 forward = view_rotation * Vector3(0, 0, -1);
	sphere2->Physics()->SetLinearVelocity(forward * 10.f);
	this->AddGameObject(sphere2);
}

void TestScene::OnUpdateScene(float dt)
{
	m_AccumTime += dt;


	// You can add status entries to the top left from anywhere in the program
	NCLDebug::AddStatusEntry(Vector4(1.0f, 0.2f, 0.2f, 1.0f), "Welcome to the Game Tech Framework!");
	NCLDebug::AddStatusEntry(Vector4(1.0f, 0.4f, 0.4f, 1.0f), "   \x01 You can move the black car with the arrow keys");

	// You can print text using 'printf' formatting
	bool donkeys = false;
//////////// bool drawConstraints ///////////
	bool drawConstraints = PhysicsEngine::Instance()->GetDebugDrawFlags() & DEBUGDRAW_FLAGS_CONSTRAINT;
/////////////////////////////////////////////////////////////////////////////
	NCLDebug::AddStatusEntry(Vector4(1.0f, 0.4f, 0.4f, 1.0f), "   \x01 The %s in this scene are dragable", donkeys ? "donkeys" : "cubes");
////////////NCLDebug::AddStatusEntry(Vector4(1.0f, 0.5f, 0.5f, 1.0f)/////////////////////////////////////////////////////////////////
	NCLDebug::AddStatusEntry(Vector4(1.0f, 0.5f, 0.5f, 1.0f), "     Draw Constraints : %s (Press C to toggle)", drawConstraints ? "Enabled" : "Disabled");

	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_C))
	{
		drawConstraints = !drawConstraints;
	}

	PhysicsEngine::Instance()->SetDebugDrawFlags(drawConstraints ? DEBUGDRAW_FLAGS_CONSTRAINT : NULL);
////////////////////////////////////////////////
	// Lets sun a little bit...
	Vector3 invLightDir = Matrix4::Rotation(15.f * dt, Vector3(0.0f, 1.0f, 0.0f)) * SceneManager::Instance()->GetInverseLightDirection();
	SceneManager::Instance()->SetInverseLightDirection(invLightDir);

	
	//Or move our car around the scene..
	{
		const float mv_speed = 10.f * dt;			//Motion: Meters per second
		const float rot_speed = 90.f * dt;			//Rotation: Degrees per second

		bool boosted = false;

		PhysicsObject* pobj = m_pPlayer1->Physics();
		//PhysicsObject* pobj2 = sphere->Physics();

		if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_J))
		{
			GenerateSphere();
		}

		if (Window::GetKeyboard()->KeyHeld(KEYBOARD_2))
		{
			pobj = m_pPlayer2->Physics();
		}

		if (Window::GetKeyboard()->KeyDown(KEYBOARD_UP))
		{
			pobj->SetPosition(pobj->GetPosition() +
				pobj->GetOrientation().ToMatrix3() * Vector3(0.0f, 0.0f, -mv_speed));
			boosted = true;
		}

		if (Window::GetKeyboard()->KeyDown(KEYBOARD_DOWN))
		{
			pobj->SetPosition(pobj->GetPosition() +
				pobj->GetOrientation().ToMatrix3()* Vector3(0.0f, 0.0f, mv_speed / 2.f));
		}

		if (Window::GetKeyboard()->KeyDown(KEYBOARD_LEFT))
		{
			pobj->SetOrientation(pobj->GetOrientation() *
				Quaternion::AxisAngleToQuaterion(Vector3(0.0f, 1.0f, 0.0f), rot_speed));
		}

		if (Window::GetKeyboard()->KeyDown(KEYBOARD_RIGHT))
		{
			pobj->SetOrientation(pobj->GetOrientation() *
				Quaternion::AxisAngleToQuaterion(Vector3(0.0f, 1.0f, 0.0f), -rot_speed));
		}

	

		// Also (and importantly), as the projMatrix/viewMatrix is all abstracted away
		//  we can now use debug draw tools to render geometry in world-space from anywhere
		//  in the program. Very useful for debugging!
		if (boosted)
		{
			//Draw the rocket booster on the car using NCLDebug
			Vector3 backward_dir = pobj->GetOrientation().ToMatrix3() * Vector3(0, 0, 1);
			NCLDebug::DrawPoint(pobj->GetPosition() + backward_dir, 0.3f, Vector4(1.f, 0.7f, 0.0f, 1.0f));
			NCLDebug::DrawPoint(pobj->GetPosition() + backward_dir * 1.333f, 0.26f, Vector4(0.9f, 0.5f, 0.0f, 1.0f));
			NCLDebug::DrawPoint(pobj->GetPosition() + backward_dir * 1.666f, 0.23f, Vector4(0.8f, 0.3f, 0.0f, 1.0f));
			NCLDebug::DrawPoint(pobj->GetPosition() + backward_dir * 2.f, 0.2f, Vector4(0.7f, 0.2f, 0.0f, 1.0f));
		}
	}
}

