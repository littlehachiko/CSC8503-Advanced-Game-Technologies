#include "PhysicsEngine.h"
#include "Object.h"
#include "CollisionDetectionSAT.h"
#include "NCLDebug.h"
#include <nclgl\Window.h>
#include <omp.h>

void PhysicsEngine::SetDefaults()
{
	m_DebugDrawFlags = NULL;
	m_IsPaused = false;
	m_UpdateTimestep = 1.0f / 60.f;
	m_UpdateAccum = 0.0f;
	m_Gravity = Vector3(0.0f, -9.81f, 0.0f);
	m_DampingFactor = 0.999f;
	//Yupei
	m_IsAtmosphere = false;
	m_score = 0.0f;
	visableStateFlag = true;
	
}

PhysicsEngine::PhysicsEngine()
{
	SetDefaults();
}

PhysicsEngine::~PhysicsEngine()
{
	RemoveAllPhysicsObjects();
}

void PhysicsEngine::AddPhysicsObject(PhysicsObject* obj)
{
	m_PhysicsObjects.push_back(obj);
}

void PhysicsEngine::RemovePhysicsObject(PhysicsObject* obj)
{
	//Lookup the object in question
	auto found_loc = std::find(m_PhysicsObjects.begin(), m_PhysicsObjects.end(), obj);

	//If found, remove it from the list
	if (found_loc != m_PhysicsObjects.end())
	{
		m_PhysicsObjects.erase(found_loc);
	}
}

void PhysicsEngine::RemoveAllPhysicsObjects()
{
	//Delete and remove all constraints/collision manifolds
	for (Constraint* c : m_vpConstraints)
	{
		delete c;
	}
	m_vpConstraints.clear();

	for (Manifold* m : m_vpManifolds)
	{
		delete m;
	}
	m_vpManifolds.clear();


	//Delete and remove all physics objects
	// - we also need to inform the (possible) associated game-object
	//   that the physics object no longer exists
	for (PhysicsObject* obj : m_PhysicsObjects)
	{
		if (obj->m_pParent != NULL) obj->m_pParent->m_pPhysicsObject = NULL;
			delete obj;
	}
	m_PhysicsObjects.clear();
}


void PhysicsEngine::Update(float deltaTime)
{
	const int max_updates_per_frame = 5;

	if (!m_IsPaused)
	{
		m_UpdateAccum += deltaTime;
		for (int i = 0; (m_UpdateAccum >= m_UpdateTimestep) && i < max_updates_per_frame; ++i)
		{
			m_UpdateAccum -= m_UpdateTimestep;
			if (!m_IsPaused) UpdatePhysics(); //Additional check here incase physics was paused mid-update and the contents of the physics need to be displayed
		}

		if (m_UpdateAccum >= m_UpdateTimestep)
		{
			NCLERROR("Physics too slow to run in real time!");
			//Drop Time in the hope that it can continue to run in real-time
			m_UpdateAccum = 0.0f;
		}
	}

}


void PhysicsEngine::UpdatePhysics()
{
	for (Manifold* m : m_vpManifolds)
	{
		delete m;
	}
	m_vpManifolds.clear();

	//Check for collisions
	BroadPhaseCollisions();
	NarrowPhaseCollisions();

	//Solve collision constraints
	SolveConstraints();

	//Update movement
	for (PhysicsObject* obj : m_PhysicsObjects)
	{
		UpdatePhysicsObject(obj);
	}
}


void PhysicsEngine::SolveConstraints()
{
	//Optional step to allow constraints to 
	// precompute values based off current velocities 
	// before they are updated in the main loop below.
	
	for (Manifold* m : m_vpManifolds) 
	{ 
		m->PreSolverStep(m_UpdateTimestep); 
	} 
	for (Constraint* c : m_vpConstraints) 
	{
		c->PreSolverStep(m_UpdateTimestep); 
	} 

	// Solve all Constraints and Collision Manifolds
	for (size_t i = 0; i < SOLVER_ITERATIONS; ++i)
	{
		for (Manifold * m : m_vpManifolds)
		{
			m->ApplyImpulse(/*factor*/);
		}

		for (Constraint * c : m_vpConstraints)
		{
			c->ApplyImpulse();
		}
	}

}

//Tutorial 2
void PhysicsEngine::UpdatePhysicsObject(PhysicsObject* obj)
{
	//06/12/2016
	//Yupei 
	//Implement gravity
	Vector3 m_gravity = obj->GetPosition() - Vector3(-40.f, -18.f, 50.f);
	m_gravity.Normalise();
	m_Gravity = m_gravity*(-8.f);

// Apply gravity
// Technically gravity here is calculated by formula :
// ( m_Gravity / invMass * invMass * dt )
// So even though the divide and multiply cancel out , we still need
// to handle the possibility of divide by zero .
	if (obj->m_InvMass > 0.0f)
	{
		obj->m_LinearVelocity += m_Gravity * m_UpdateTimestep;
	}

	//Yupei
	if (obj->restFlag == true)
	{
		obj->m_LinearVelocity = Vector3(0.0f,0.0f,0.0f);
		obj->m_AngularVelocity = Vector3(0.0f, 0.0f, 0.0f);
	}
	//Yupei
	Vector4 originCol = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	Vector4 whenCol = Vector4(1.0f, 0.0f, 0.5f, 1.0f);

	if (!GetVisableState())
	{
		obj->GetAssociatedObject()->SetColour(Vector4(1.0f, 1.0f, 1.0f, 0.0f));
	}
	else
	{
		if (obj->GetAssociatedObject()->GetName() == "atmosphere")
		{
			obj->GetAssociatedObject()->SetColour(Vector4(1.0f, 1.0f, 1.0f, 0.16f));
		}
		else
		{
			obj->GetAssociatedObject()->SetColour(originCol);
		}
	}

	SetDampingFactor(0.999f);
	if (IsAtmosphered())
	{
		float disToPlanet = (obj->GetPosition() - Vector3(-40.f, -18.f, 50.f)).LengthSquared();


		if (disToPlanet <= 28.0f * 28.0f)
		{
			obj->SetInAtmosphereState(true);
		}
		else
		{
			obj->SetInAtmosphereState(false);
		}

		if (obj->IsInAtmosphere())
		{
			SetDampingFactor(0.9f);
			if (obj->IsProjectile())
			{
				obj->GetAssociatedObject()->SetColour(whenCol);
			}
		}
		else
		{
			obj->GetAssociatedObject()->SetColour(originCol);
		}

	}

	
	Vector3 tempVelocity = obj->m_LinearVelocity;

	// Semi - Implicit Euler Intergration
	// - See " Update Position " below
	obj -> m_LinearVelocity += obj -> m_Force * obj -> m_InvMass
 	 * m_UpdateTimestep;
	obj->m_LinearVelocity = obj->m_LinearVelocity * m_DampingFactor;

	// Apply Velocity Damping
	// - This removes a tiny bit of energy from the simulation each
	// update to stop slight calculation errors accumulating and adding
	// force from nowhere .
	// - In it 's present form this can be seen as a rough approximation
	// of air resistance , albeit wrongly making the assumption that all
	// objects have the same surface area .
	obj -> m_LinearVelocity = obj -> m_LinearVelocity * m_DampingFactor ;

	// Update Position
	// - Euler integration , works on the assumption that linearvelocity
	// does not change over time (or changes so slightly it doesnt make
	// a difference ).
	// - In this scenario , gravity / will / be increasing velocity over
	// time . The inaccuracy of not taking into account of these changes
	// over time can be visibly seen in the Phy2_Integration Test
	// Scene .. and thus how better integration schemes lead to better
	// approximations by taking into account of curvature .
	 //obj -> m_Position += obj -> m_LinearVelocity * m_UpdateTimestep ;

///////////RK2
	obj->m_Position += (obj->m_LinearVelocity + tempVelocity) *m_UpdateTimestep *0.5;

	// Angular Rotation
	// - These are the exact same calculations as the three lines above
	// except for rotations rather than positions .
	// - Mass -> Torque
	// - Velocity -> Rotational Velocity
	// - Position -> Orientation
 	obj -> m_AngularVelocity += obj -> m_InvInertia
 	* obj -> m_Torque * m_UpdateTimestep ;


	// Apply Velocity Damping
	obj -> m_AngularVelocity = obj -> m_AngularVelocity * m_DampingFactor ;


	// Update Orientation
	// - This is slightly different calculation due to the weirdness of
	// quaternions .
	// - If you are interested in it 's derivation , there is lots of
	// stuff online about it. Though in essence , i t s the fastest
	// approximation for updating orientation , and the smaller
	// timestep / velocity is the more accurate it is - so fits nicely
	// with Euler above . ;)
	obj -> m_Orientation = obj -> m_Orientation + obj -> m_Orientation
	* ( obj -> m_AngularVelocity * m_UpdateTimestep * 0.5f);
	obj -> m_Orientation . Normalise ();


	// Finally invalidate the world - transform matrix .
	// - The next time it is requested ; it will be rebuilt from scratch
	// with the new position / orientation we set above .
	obj -> m_wsTransformInvalidated = true ;

	
}


void PhysicsEngine::BroadPhaseCollisions()
{
	m_BroadphaseCollisionPairs.clear();

	for (auto* obj : m_PhysicsObjects)
	{
		obj->colFlag = false;
	}

	PhysicsObject *m_pObj1, *m_pObj2;
	//	The broadphase needs to build a list of all potentially colliding objects in the world,
	//	which then get accurately assesed in narrowphase. If this is too coarse then the system slows down with
	//	the complexity of narrowphase collision checking, if this is too fine then collisions may be missed.


	//	Brute force approach.
	//  - For every object A, assume it could collide with every other object.. 
	//    even if they are on the opposite sides of the world.
	if (m_PhysicsObjects.size() > 0)
	{
		for (size_t i = 0; i < m_PhysicsObjects.size() - 1; ++i)
		{
			for (size_t j = i + 1; j < m_PhysicsObjects.size(); ++j)
			{
				m_pObj1 = m_PhysicsObjects[i];
				m_pObj2 = m_PhysicsObjects[j];

				//Check they both atleast have collision shapes
				if (m_pObj1->GetCollisionShape() != NULL
					&& m_pObj2->GetCollisionShape() != NULL)
				{
					CollisionPair cp;
					cp.pObjectA = m_pObj1;
					cp.pObjectB = m_pObj2;
					m_BroadphaseCollisionPairs.push_back(cp);
				}
				
			}
		}
	}
}


void PhysicsEngine::NarrowPhaseCollisions()
{
	if (m_BroadphaseCollisionPairs.size() > 0)
	{
		//Collision data to pass between detection and manifold generation stages.
		CollisionData colData;				

		//Collision Detection Algorithm to use
		CollisionDetectionSAT colDetect;	

		// Iterate over all possible collision pairs and perform accurate collision detection
		for (size_t i = 0; i < m_BroadphaseCollisionPairs.size(); ++i)
		{
			CollisionPair& cp = m_BroadphaseCollisionPairs[i];

			CollisionShape *shapeA = cp.pObjectA->GetCollisionShape();
			CollisionShape *shapeB = cp.pObjectB->GetCollisionShape();

			

			colDetect.BeginNewPair(
				cp.pObjectA,
				cp.pObjectB,
				cp.pObjectA->GetCollisionShape(),
				cp.pObjectB->GetCollisionShape()
				);

			//--TUTORIAL 4 CODE--
			// Detects if the objects are colliding - Seperating Axis Theorem
			if (colDetect.AreColliding(&colData))
			{
				//Draw collision data to the window if requested
				// - Have to do this here as colData is only temporary. 
				if (m_DebugDrawFlags & DEBUGDRAW_FLAGS_COLLISIONNORMALS)
				{
					NCLDebug::DrawPointNDT(colData._pointOnPlane, 0.1f, Vector4(0.5f, 0.5f, 1.0f, 1.0f));
					NCLDebug::DrawThickLineNDT(colData._pointOnPlane, colData._pointOnPlane - colData._normal * colData._penetration, 0.05f, Vector4(0.0f, 0.0f, 1.0f, 1.0f));
				}
				
				

				//Check to see if any of the objects have collision callbacks that dont want the objects to physically collide
				bool okA = cp.pObjectA->FireOnCollisionEvent(cp.pObjectA, cp.pObjectB);
				bool okB = cp.pObjectB->FireOnCollisionEvent(cp.pObjectB, cp.pObjectA);

				if (okA && okB)
				{
					cp.pObjectA->colFlag = true;
					cp.pObjectB->colFlag = true;

					if (!cp.pObjectA->GetRestState()|| !cp.pObjectB->GetRestState() )
					{
						cp.pObjectA->restFlag = false;
						cp.pObjectB->restFlag = false;
					}

				

					//-- TUTORIAL 5 CODE --
					// Build full collision manifold that will also handle the collision response between the two objects in the solver stage
					Manifold* manifold = new Manifold();
					manifold->Initiate(cp.pObjectA, cp.pObjectB);

					// Construct contact points that form the perimeter of the collision manifold
					colDetect.GenContactPoints(manifold);

					// Add to list of manifolds that need solving
					m_vpManifolds.push_back(manifold);
				}
				
			}
			
		}
		
	}
}


void PhysicsEngine::DebugRender()
{
	// Draw all collision manifolds
	if (m_DebugDrawFlags & DEBUGDRAW_FLAGS_MANIFOLD)
	{
		for (Manifold* m : m_vpManifolds)
		{
			m->DebugDraw();
		}
	}

	// Draw all constraints
	if (m_DebugDrawFlags & DEBUGDRAW_FLAGS_CONSTRAINT)
	{
		for (Constraint* c : m_vpConstraints)
		{
			c->DebugDraw();
		}
	}

	// Draw all associated collision shapes
	if (m_DebugDrawFlags & DEBUGDRAW_FLAGS_COLLISIONVOLUMES)
	{
		for (PhysicsObject* obj : m_PhysicsObjects)
		{
			if (obj->GetCollisionShape() != NULL)
			{
				obj->GetCollisionShape()->DebugDraw(obj);
			}
		}
	}
	
}

void PhysicsEngine :: UpdatePhysicsObjects ()
{
	for ( PhysicsObject* obj : m_PhysicsObjects )
	{
		UpdatePhysicsObject (obj );
	}
}

