#include "CommonUtils.h"

#include "ObjectMesh.h"
#include "ObjectMeshDragable.h"
#include "SphereCollisionShape.h"
#include "CuboidCollisionShape.h"
#include "CommonMeshes.h"

Vector4 CommonUtils::GenColour(float scalar, float alpha)
{
	Vector4 c;
	c.w = alpha;

	float t;
	c.x = abs(modf(scalar + 1.0f, &t) * 6.0f - 3.0f) - 1.0f;
	c.y = abs(modf(scalar + 2.0f / 3.0f, &t) * 6.0f - 3.0f) - 1.0f;
	c.z = abs(modf(scalar + 1.0f / 3.0f, &t) * 6.0f - 3.0f) - 1.0f;

	c.x = min(max(c.x, 0.0f), 1.0f);
	c.y = min(max(c.y, 0.0f), 1.0f);
	c.z = min(max(c.z, 0.0f), 1.0f);

	return c;
}

Object* CommonUtils::BuildSphereObject(
	const std::string& name,
	const Vector3& pos,
	float radius,
	bool physics_enabled,
	float inverse_mass,
	bool collidable,
	bool dragable,
	const Vector4& color)
{
	ObjectMesh* pSphere = dragable
		? new ObjectMeshDragable(name)
		: new ObjectMesh(name);

	pSphere->SetMesh(CommonMeshes::Sphere(), false);
	pSphere->SetTexture(CommonMeshes::SphereTex(), false);
	pSphere->SetLocalTransform(Matrix4::Scale(Vector3(radius, radius, radius)));
	pSphere->SetColour(color);
	pSphere->SetBoundingRadius(radius);

	if (!physics_enabled)
	{
		//If no physics object is present, just set the local transform (modelMatrix) directly
		pSphere->SetLocalTransform(Matrix4::Translation(pos) * pSphere->GetLocalTransform());
	}
	else
	{
		//Otherwise create a physics object, and set it's position etc
		pSphere->CreatePhysicsNode();

		pSphere->Physics()->SetPosition(pos);
		pSphere->Physics()->SetInverseMass(inverse_mass);
		
		if (!collidable)
		{
			//Even without a collision shape, the inertia matrix for rotation has to be derived from the objects shape
			pSphere->Physics()->SetInverseInertia(SphereCollisionShape(radius).BuildInverseInertia(inverse_mass));
		}
		else
		{
			CollisionShape* pColshape = new SphereCollisionShape(radius);
			pSphere->Physics()->SetCollisionShape(pColshape);
			pSphere->Physics()->SetInverseInertia(pColshape->BuildInverseInertia(inverse_mass));
		}			
	}

	return pSphere;
}

Object* CommonUtils::BuildCuboidObject(
	const std::string& name,
	const Vector3& pos,
	const Vector3& halfdims,
	bool physics_enabled,
	float inverse_mass,
	bool collidable,
	bool dragable,
	const Vector4& color
	)
{
	ObjectMesh* pCuboid = dragable
		? new ObjectMeshDragable(name)
		: new ObjectMesh(name);

	pCuboid->SetMesh(CommonMeshes::Cube(), false);
	pCuboid->SetTexture(CommonMeshes::BoxTex(), false);
	pCuboid->SetLocalTransform(Matrix4::Scale(halfdims));
	pCuboid->SetColour(color);
	pCuboid->SetBoundingRadius(halfdims.Length());

	if (!physics_enabled)
	{
		//If no physics object is present, just set the local transform (modelMatrix) directly
		pCuboid->SetLocalTransform(Matrix4::Translation(pos) * pCuboid->GetLocalTransform());
	}
	else
	{
		//Otherwise create a physics object, and set it's position etc
		pCuboid->CreatePhysicsNode();
		pCuboid->Physics()->SetPosition(pos);
		pCuboid->Physics()->SetInverseMass(inverse_mass);

		if (!collidable)
		{
			//Even without a collision shape, the inertia matrix for rotation has to be derived from the objects shape
			pCuboid->Physics()->SetInverseInertia(CuboidCollisionShape(halfdims).BuildInverseInertia(inverse_mass));
		}
		else
		{
			CollisionShape* pColshape = new CuboidCollisionShape(halfdims);
			pCuboid->Physics()->SetCollisionShape(pColshape);
			pCuboid->Physics()->SetInverseInertia(pColshape->BuildInverseInertia(inverse_mass));
		}
	}

	return pCuboid;
}

Object * CommonUtils::BuildPlanetObject(
	const std::string& name,
	const Vector3& pos,
	float radius,
	bool physics_enabled,
	float inverse_mass,			//requires physics_enabled = true
	bool collidable,				//requires physics_enabled = true
	bool dragable,
	const Vector4& color)
{
	ObjectMesh* pPlanet = dragable
		? new ObjectMeshDragable(name)
		: new ObjectMesh(name);

	pPlanet->SetMesh(CommonMeshes::Planet(), false);
	pPlanet->SetTexture(CommonMeshes::PlanetTex(), false);
	pPlanet->SetLocalTransform(Matrix4::Scale(Vector3(radius, radius, radius)));
	pPlanet->SetColour(color);
	pPlanet->SetBoundingRadius(radius);

	if (!physics_enabled)
	{
		//If no physics object is present, just set the local transform (modelMatrix) directly
		pPlanet->SetLocalTransform(Matrix4::Translation(pos) * pPlanet->GetLocalTransform());
	}
	else
	{
		//Otherwise create a physics object, and set it's position etc
		pPlanet->CreatePhysicsNode();

		pPlanet->Physics()->SetPosition(pos);
		pPlanet->Physics()->SetInverseMass(inverse_mass);

		if (!collidable)
		{
			//Even without a collision shape, the inertia matrix for rotation has to be derived from the objects shape
			pPlanet->Physics()->SetInverseInertia(SphereCollisionShape(radius).BuildInverseInertia(inverse_mass));
		}
		else
		{
			CollisionShape* pColshape = new SphereCollisionShape(radius);
			pPlanet->Physics()->SetCollisionShape(pColshape);
			pPlanet->Physics()->SetInverseInertia(pColshape->BuildInverseInertia(inverse_mass));
		}
	}
	return pPlanet;
}

Object* CommonUtils::BuildBoardObject(
	const std::string& name,
	const Vector3& pos,
	const Vector3& halfdims,
	bool physics_enabled,
	float inverse_mass,
	bool collidable,
	bool dragable,
	const Vector4& color
	)
{
	ObjectMesh* pCuboid = dragable
		? new ObjectMeshDragable(name)
		: new ObjectMesh(name);

	pCuboid->SetMesh(CommonMeshes::Target(), false);
	pCuboid->SetTexture(CommonMeshes::TargetTex(), false);
	pCuboid->SetLocalTransform(Matrix4::Scale(halfdims));
	pCuboid->SetColour(color);
	pCuboid->SetBoundingRadius(halfdims.Length());

	if (!physics_enabled)
	{
		//If no physics object is present, just set the local transform (modelMatrix) directly
		pCuboid->SetLocalTransform(Matrix4::Translation(pos) * pCuboid->GetLocalTransform());
	}
	else
	{
		//Otherwise create a physics object, and set it's position etc
		pCuboid->CreatePhysicsNode();
		pCuboid->Physics()->SetPosition(pos);
		pCuboid->Physics()->SetInverseMass(inverse_mass);

		if (!collidable)
		{
			//Even without a collision shape, the inertia matrix for rotation has to be derived from the objects shape
			pCuboid->Physics()->SetInverseInertia(CuboidCollisionShape(halfdims).BuildInverseInertia(inverse_mass));
		}
		else
		{
			CollisionShape* pColshape = new CuboidCollisionShape(halfdims);
			pCuboid->Physics()->SetCollisionShape(pColshape);
			pCuboid->Physics()->SetInverseInertia(pColshape->BuildInverseInertia(inverse_mass));
		}
	}

	return pCuboid;
}

Object* CommonUtils::BuildCanObject(
	const std::string& name,
	const Vector3& pos,
	const Vector3& halfdims,
	bool physics_enabled,
	float inverse_mass,
	bool collidable,
	bool dragable,
	const Vector4& color
	)
{
	ObjectMesh* pCan = dragable
		? new ObjectMeshDragable(name)
		: new ObjectMesh(name);

	pCan->SetMesh(CommonMeshes::Can(), false);
	pCan->SetTexture(CommonMeshes::CanTex(), false);
	pCan->SetLocalTransform(Matrix4::Scale(halfdims));
	pCan->SetColour(color);
	pCan->SetBoundingRadius(halfdims.Length());

	if (!physics_enabled)
	{
		//If no physics object is present, just set the local transform (modelMatrix) directly
		pCan->SetLocalTransform(Matrix4::Translation(pos) * pCan->GetLocalTransform());
	}
	else
	{
		//Otherwise create a physics object, and set it's position etc
		pCan->CreatePhysicsNode();
		pCan->Physics()->SetPosition(pos);
		pCan->Physics()->SetInverseMass(inverse_mass);

		if (!collidable)
		{
			//Even without a collision shape, the inertia matrix for rotation has to be derived from the objects shape
			pCan->Physics()->SetInverseInertia(CuboidCollisionShape(halfdims*1.5f).BuildInverseInertia(inverse_mass));
		}
		else
		{
			CollisionShape* pColshape = new CuboidCollisionShape(halfdims*1.5f);
			pCan->Physics()->SetCollisionShape(pColshape);
			pCan->Physics()->SetInverseInertia(pColshape->BuildInverseInertia(inverse_mass));
		}
	}

	return pCan;
}

Object* CommonUtils::BuildBallObject(
	const std::string& name,
	const Vector3& pos,
	float radius,
	bool physics_enabled,
	float inverse_mass,
	bool collidable,
	bool dragable,
	const Vector4& color)
{
	ObjectMesh* pSphere = dragable
		? new ObjectMeshDragable(name)
		: new ObjectMesh(name);

	pSphere->SetMesh(CommonMeshes::Sphere(), false);
	pSphere->SetTexture(CommonMeshes::CheckerboardTex(), false);
	pSphere->SetLocalTransform(Matrix4::Scale(Vector3(radius, radius, radius)));
	pSphere->SetColour(color);
	pSphere->SetBoundingRadius(radius);

	if (!physics_enabled)
	{
		//If no physics object is present, just set the local transform (modelMatrix) directly
		pSphere->SetLocalTransform(Matrix4::Translation(pos) * pSphere->GetLocalTransform());
	}
	else
	{
		//Otherwise create a physics object, and set it's position etc
		pSphere->CreatePhysicsNode();

		pSphere->Physics()->SetPosition(pos);
		pSphere->Physics()->SetInverseMass(inverse_mass);

		if (!collidable)
		{
			//Even without a collision shape, the inertia matrix for rotation has to be derived from the objects shape
			pSphere->Physics()->SetInverseInertia(SphereCollisionShape(radius).BuildInverseInertia(inverse_mass));
		}
		else
		{
			CollisionShape* pColshape = new SphereCollisionShape(radius);
			pSphere->Physics()->SetCollisionShape(pColshape);
			pSphere->Physics()->SetInverseInertia(pColshape->BuildInverseInertia(inverse_mass));
		}
	}

	return pSphere;
}

Object* CommonUtils::BuildPlaneObject(
	const std::string& name,
	const Vector3& pos,
	const Vector3& halfdims,
	bool physics_enabled,
	float inverse_mass,
	bool collidable,
	bool dragable,
	const Vector4& color
	)
{
	ObjectMesh* pPlane = dragable
		? new ObjectMeshDragable(name)
		: new ObjectMesh(name);

	pPlane->SetMesh(CommonMeshes::Plane(), false);
	pPlane->SetTexture(CommonMeshes::BoxTex(), false);
	//pPlane->SetLocalTransform(Matrix4::Scale(halfdims));
	
	pPlane->SetLocalTransform(Matrix4::Scale(Vector3(halfdims.x * 2, halfdims.y * 2, halfdims.z * 2)));
	pPlane->SetLocalTransform(Matrix4::Translation(Vector3(-1.0f*halfdims.x, -1.0f*halfdims.y, halfdims.z))*pPlane->GetLocalTransform());
	
	pPlane->SetColour(color);
	pPlane->SetBoundingRadius(halfdims.Length());

	if (!physics_enabled)
	{
		//If no physics object is present, just set the local transform (modelMatrix) directly
		pPlane->SetLocalTransform(Matrix4::Translation(pos) * pPlane->GetLocalTransform());
	}
	else
	{
		//Otherwise create a physics object, and set it's position etc
		pPlane->CreatePhysicsNode();
		pPlane->Physics()->SetPosition(pos);
		pPlane->Physics()->SetInverseMass(inverse_mass);

		if (!collidable)
		{
			//Even without a collision shape, the inertia matrix for rotation has to be derived from the objects shape
			pPlane->Physics()->SetInverseInertia(CuboidCollisionShape(halfdims).BuildInverseInertia(inverse_mass));
		}
		else
		{
			CollisionShape* pColshape = new CuboidCollisionShape(halfdims);
			pPlane->Physics()->SetCollisionShape(pColshape);
			pPlane->Physics()->SetInverseInertia(pColshape->BuildInverseInertia(inverse_mass));
		}
	}

	return pPlane;
}


Object* CommonUtils::BuildTardisObject(
	const std::string& name,
	const Vector3& pos,
	const Vector3& halfdims,
	bool physics_enabled,
	float inverse_mass,
	bool collidable,
	bool dragable,
	const Vector4& color
	)
{
	ObjectMesh* pTardis = dragable
		? new ObjectMeshDragable(name)
		: new ObjectMesh(name);

	pTardis->SetMesh(CommonMeshes::Tardis(), false);
	pTardis->SetTexture(CommonMeshes::TardisTex(), false);
	pTardis->SetLocalTransform(Matrix4::Scale(halfdims));
	pTardis->SetColour(color);
	pTardis->SetBoundingRadius(halfdims.Length());

	if (!physics_enabled)
	{
		//If no physics object is present, just set the local transform (modelMatrix) directly
		pTardis->SetLocalTransform(Matrix4::Translation(pos) * pTardis->GetLocalTransform());
	}
	else
	{
		//Otherwise create a physics object, and set it's position etc
		pTardis->CreatePhysicsNode();
		pTardis->Physics()->SetPosition(pos);
		pTardis->Physics()->SetInverseMass(inverse_mass);

		if (!collidable)
		{
			//Even without a collision shape, the inertia matrix for rotation has to be derived from the objects shape
			pTardis->Physics()->SetInverseInertia(CuboidCollisionShape(halfdims).BuildInverseInertia(inverse_mass));
		}
		else
		{
			CollisionShape* pColshape = new CuboidCollisionShape(halfdims);
			pTardis->Physics()->SetCollisionShape(pColshape);
			pTardis->Physics()->SetInverseInertia(pColshape->BuildInverseInertia(inverse_mass));
		}
	}

	return pTardis;
}
