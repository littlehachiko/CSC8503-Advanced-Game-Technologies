#pragma once

#include "Object.h"

namespace CommonUtils
{

	//Generates a unique colour based on scalar parameter in the range of 0-1.
	// - Works off HSV color format, in this case Saturation and Value are 100% and Hue is dependant on 'scalar'
	Vector4 GenColour(float scalar, float alpha);


	//Generates a default Sphere object with the parameters specified.
	Object* BuildSphereObject(
		const std::string& name,
		const Vector3& pos,
		float radius,
		bool physics_enabled = false,
		float inverse_mass = 0.0f,			//requires physics_enabled = true
		bool collidable = true,				//requires physics_enabled = true
		bool dragable = true,
		const Vector4& color = Vector4(1.0f, 1.0f, 1.0f, 1.0f));

	//Generates a default Cuboid object with the parameters specified
	Object* BuildCuboidObject(
		const std::string& name,
		const Vector3& pos,
		const Vector3& scale,
		bool physics_enabled = false,
		float inverse_mass = 0.0f,			//requires physics_enabled = true
		bool collidable = true,				//requires physics_enabled = true
		bool dragable = true,
		const Vector4& color = Vector4(1.0f, 1.0f, 1.0f, 1.0f)
		);

	//Generates a default Planet object with the parameters specified
	Object* BuildPlanetObject(
		const std::string& name,
		const Vector3& pos,
		float radius,
		bool physics_enabled = false,
		float inverse_mass = 0.0f,			//requires physics_enabled = true
		bool collidable = true,				//requires physics_enabled = true
		bool dragable = true,
		const Vector4& color = Vector4(1.0f, 1.0f, 1.0f, 1.0f));

	//Generates a default board
	Object* BuildBoardObject(
		const std::string& name,
		const Vector3& pos,
		const Vector3& scale,
		bool physics_enabled = false,
		float inverse_mass = 0.0f,			//requires physics_enabled = true
		bool collidable = true,				//requires physics_enabled = true
		bool dragable = true,
		const Vector4& color = Vector4(1.0f, 1.0f, 1.0f, 1.0f)
		);

	//Generates a default Can object with the parameters specified
	Object* BuildCanObject(
		const std::string& name,
		const Vector3& pos,
		const Vector3& scale,
		bool physics_enabled = false,
		float inverse_mass = 0.0f,			//requires physics_enabled = true
		bool collidable = true,				//requires physics_enabled = true
		bool dragable = true,
		const Vector4& color = Vector4(1.0f, 1.0f, 1.0f, 1.0f)
		);

	//Generates a default Sphere object with the parameters specified.
	Object* BuildBallObject(
		const std::string& name,
		const Vector3& pos,
		float radius,
		bool physics_enabled = false,
		float inverse_mass = 0.0f,			//requires physics_enabled = true
		bool collidable = true,				//requires physics_enabled = true
		bool dragable = true,
		const Vector4& color = Vector4(1.0f, 1.0f, 1.0f, 1.0f));

	//Generates a default Cuboid object with the parameters specified
	Object* BuildPlaneObject(
		const std::string& name,
		const Vector3& pos,
		const Vector3& scale,
		bool physics_enabled = false,
		float inverse_mass = 0.0f,			//requires physics_enabled = true
		bool collidable = true,				//requires physics_enabled = true
		bool dragable = true,
		const Vector4& color = Vector4(1.0f, 1.0f, 1.0f, 1.0f)
		);

	//Generates Tardis!!
	Object* BuildTardisObject(
		const std::string& name,
		const Vector3& pos,
		const Vector3& scale,
		bool physics_enabled = false,
		float inverse_mass = 0.0f,			//requires physics_enabled = true
		bool collidable = true,				//requires physics_enabled = true
		bool dragable = true,
		const Vector4& color = Vector4(1.0f, 1.0f, 1.0f, 1.0f)
		);

};