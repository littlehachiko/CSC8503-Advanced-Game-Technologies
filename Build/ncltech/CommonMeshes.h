/******************************************************************************
Class: CommonMeshes
Implements:
Author: Pieran Marris <p.marris@newcastle.ac.uk>
Description: 
A quick and dirty library of common meshes, to save loading the same common meshes over and over again.

These are loaded when the scene is first initialised and released when it is deleted, so will 
be globally availible for the entirity of the program. 

If they are being used within an ObjectMesh instance, remember to set the 'deleteOnCleanup' flag to 
false in order to prevent them being deleted when the ObjectMesh instance is deleted.


		(\_/)
		( '_')
	 /""""""""""""\=========     -----D
	/"""""""""""""""""""""""\
....\_@____@____@____@____@_/

*//////////////////////////////////////////////////////////////////////////////

#pragma once
#include <nclgl\Mesh.h>

class Scene;

class CommonMeshes
{
	friend class SceneRenderer; //Initializes/Destroys the given meshes within it's own lifecycle

public:

	//Quad
	static Mesh* Plane()			{ return m_pPlane; }

	//Cube
	static Mesh* Cube()				{ return m_pCube; }

	//Sphere
	static Mesh* Sphere()			{ return m_pSphere; }

	//Planet
	static Mesh* Planet()			{ return m_pPlanet; }

	//Target
	static Mesh* Target()			{ return m_pTarget; }

	//Can
	static Mesh* Can()				{ return m_pCan; }

	//Tardis
	static Mesh* Tardis()				{ return m_pTardis; }

	//PhysicsEngine Checkerboard - Hidden here for reasons of laziness
	static GLuint CheckerboardTex() { return m_CheckerboardTex; }
	//06/12/2016
	//Yupei
	//Planet Texture
	static GLuint PlanetTex()		{ return m_PlanetTex; }
	
	//Sphere Texture - Actually tennis balls
	static GLuint SphereTex()	    { return m_SphereTex; }
	
	//Cube Texture - wooden boxes
	static GLuint BoxTex()			{ return m_BoxTex; }
	
	//Targets Board Texture
	static GLuint TargetTex()		{ return m_TargetTex; }
	
	//Darts Board Texture
	static GLuint CanTex()			{ return m_CanTex; }

	//Tardis Texture
	static GLuint TardisTex()	    { return m_TardisTex; }

	
protected:
	//Called by SceneRenderer
	static void InitializeMeshes();
	static void ReleaseMeshes();

	static Mesh* m_pCube;
	static Mesh* m_pSphere;
	static Mesh* m_pPlane;
	static Mesh* m_pPlanet;
	static Mesh* m_pTarget;
	static Mesh* m_pCan;
	static Mesh* m_pTardis;

	static GLuint m_CheckerboardTex;
	static GLuint m_PlanetTex;
	static GLuint m_SphereTex;
	static GLuint m_BoxTex;
	static GLuint m_TargetTex;
	static GLuint m_CanTex;
	static GLuint m_TardisTex;

	GLuint	m_whiteTexture;
};