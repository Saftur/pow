//------------------------------------------------------------------------------
//
// File Name:	Mesh.h
// Author(s):	Doug Schilling (dschilling)
// Project:		MyGame
// Course:		CS230S17
//
// Copyright © 2017 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "AEEngine.h"
#include "Mesh.h"

//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

struct AEGfxVertexList;

//------------------------------------------------------------------------------
// Public Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Create a mesh using the Alpha Engine.
// Params:
//	 xHalfSize = The X half-size of the mesh.
//	 yHalfSize = The Y half-size of the mesh.
//   uSize = The U size of the mesh, relative to texture coordinates (0.0 .. 1.0).
//   vSize = The V size of the mesh, relative to texture coordinates (0.0 .. 1.0).
//	 name = A name for the mesh.  Currently used only for error messaging.
// Returns:
//	 If the memory allocation was successful,
//	   then return a pointer to the allocated memory,
//	   else return NULL.
struct AEGfxVertexList * MeshCreateQuad(float xHalfSize, float yHalfSize, float uSize, float vSize, const char * name)
{
	UNREFERENCED_PARAMETER(name);

	AEGfxVertexList* verts;

	// Start adding vertices
	AEGfxMeshStart();

	// Add some triangles
	AEGfxTriAdd(-xHalfSize, yHalfSize, 0, 0.0f, 0.0f,
		xHalfSize, yHalfSize, 0, uSize, 0.0f,
		-xHalfSize, -yHalfSize, 0, 0.0f, vSize);
	AEGfxTriAdd(xHalfSize, -yHalfSize, 0, uSize, vSize,
		xHalfSize, yHalfSize, 0, uSize, 0.0f,
		-xHalfSize, -yHalfSize, 0, 0.0f, vSize);

	// Stop adding vertices
	verts = AEGfxMeshEnd();

	if (verts == NULL)
		return NULL;
	else
		return verts;
}

//------------------------------------------------------------------------------


