#include "CommonMeshes.h"
#include <nclgl\OBJMesh.h>

Mesh* CommonMeshes::m_pPlane	= NULL;
Mesh* CommonMeshes::m_pCube		= NULL;
Mesh* CommonMeshes::m_pSphere	= NULL;
Mesh* CommonMeshes::m_pPlanet   = NULL;
Mesh* CommonMeshes::m_pTarget	= NULL;
Mesh* CommonMeshes::m_pCan		= NULL;
Mesh* CommonMeshes::m_pTardis   = NULL;

GLuint CommonMeshes::m_CheckerboardTex = 0;
GLuint CommonMeshes::m_PlanetTex = 0;
GLuint CommonMeshes::m_SphereTex = 0;
GLuint CommonMeshes::m_BoxTex = 0;
GLuint CommonMeshes::m_TargetTex = 0;
GLuint CommonMeshes::m_CanTex = 0;
GLuint CommonMeshes::m_TardisTex = 0;

void CommonMeshes::InitializeMeshes()
{
	if (m_pPlane == NULL)
	{
		
		m_CheckerboardTex = SOIL_load_OGL_texture(TEXTUREDIR"checkerboard.tga", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
		//06/12/2016
		//Yupei
		//Get texture from http://www.solarsystemscope.com/textures/
		m_PlanetTex = SOIL_load_OGL_texture(TEXTUREDIR"moon.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
		//Get texture from http://www.robinwood.com/Catalog/FreeStuff/Textures/TexturePages/BallMaps.html
		m_SphereTex = SOIL_load_OGL_texture(TEXTUREDIR"TennisBallColorMap.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
		//Get texture from https://www.pinterest.com/pin/407646203750268821/
		m_BoxTex = SOIL_load_OGL_texture(TEXTUREDIR"container.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
		//Get texture from http://www.graphicsfuel.com/2011/02/3d-target-dart-psd-icons/
		m_TargetTex = SOIL_load_OGL_texture(TEXTUREDIR"target-green.png",SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
		////Get texture from http://www.psdgraphics.com/textures/dirty-metal-surface-texture/
		m_CanTex = SOIL_load_OGL_texture(TEXTUREDIR"can.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
		////Get texture from 
		m_TardisTex = SOIL_load_OGL_texture(TEXTUREDIR"TARDIS.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);

		glBindTexture(GL_TEXTURE_2D, m_CheckerboardTex);
		glBindTexture(GL_TEXTURE_2D, m_SphereTex);
		glBindTexture(GL_TEXTURE_2D, m_BoxTex);
		glBindTexture(GL_TEXTURE_2D, m_PlanetTex);
		glBindTexture(GL_TEXTURE_2D, m_TargetTex);
		glBindTexture(GL_TEXTURE_2D, m_CanTex);
		glBindTexture(GL_TEXTURE_2D, m_TardisTex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST); //No linear interpolation to get crisp checkerboard no matter the scalling
		glBindTexture(GL_TEXTURE_2D, 0);
		

		m_pPlane = Mesh::GenerateQuadTexCoordCol(Vector2(1.f, 1.f), Vector2(0.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f));
		m_pCube = new OBJMesh(MESHDIR"cube.obj");
		m_pTarget = new OBJMesh(MESHDIR"cube.obj");
		m_pSphere = new OBJMesh(MESHDIR"sphere.obj");
		m_pPlanet = new OBJMesh(MESHDIR"sphere.obj");
		m_pCan = new OBJMesh(MESHDIR"coca-cola.obj");
		m_pTardis = new OBJMesh(MESHDIR"cube.obj");


		m_pPlane->SetTexture(m_CheckerboardTex);
		m_pCube->SetTexture(m_BoxTex);
		m_pTarget->SetTexture(m_TargetTex);
		m_pSphere->SetTexture(m_SphereTex);
		m_pPlanet->SetTexture(m_PlanetTex);
		m_pCan->SetTexture(m_CanTex);
		m_pTardis->SetTexture(m_TardisTex);
	}
}

void CommonMeshes::ReleaseMeshes()
{
	if (m_pPlane != NULL)
	{
		glDeleteTextures(1, &m_CheckerboardTex);
		glDeleteTextures(1, &m_PlanetTex);
		glDeleteTextures(1, &m_SphereTex);
		glDeleteTextures(1, &m_BoxTex);
		glDeleteTextures(1, &m_TargetTex);
		glDeleteTextures(1, &m_CanTex);
		glDeleteTextures(1, &m_TardisTex);
		delete m_pPlane;
		delete m_pCube;
		delete m_pSphere;
		delete m_pPlanet;
		delete m_pTarget;
		delete m_pCan;
		delete m_pTardis;
	}

	m_pPlane = NULL;
}
