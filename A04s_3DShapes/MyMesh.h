/*----------------------------------------------
Programmer: Alberto Bobadilla (labigm@rit.edu)
Date: 2016/02
Notes: This is a simplified version of MyMesh
inside of ReEng.dll, its stripped out of a lot
of functionality in order to focuss on important
concepts of OpenGL, please use a MyMesh object
for better functionality.
----------------------------------------------*/
#ifndef __MYMESH_H_
#define __MYMESH_H_

#include "RE\ReEng.h"

class MyMesh
{
protected:
	bool m_bBinded = false; //Binded flag

	uint m_uMaterialIndex = 0; //Material index of this mesh
	uint m_uVertexCount = 0; //Number of vertices in this Mesh

	GLuint m_VAO = 0;			//OpenGL Vertex Array Object
	GLuint m_VBO = 0;			//OpenGL Vertex Array Object
	GLuint m_VertexBuffer = 0;	//OpenGL Buffer (Will hold the vertex buffer pointer)
	GLuint m_ColorBuffer = 0;	//OpenGL Buffer (Will hold the color buffer pointer)
	
	GLuint m_nShader = 0;	//Index of the shader

	ShaderManagerSingleton* m_pShaderMngr = nullptr;	//Shader Manager
	MaterialManagerSingleton* m_pMatMngr = nullptr;		//Material Manager

	std::vector<vector3> m_lVertex; //Composed vertex array
	std::vector<vector3> m_lVertexPos;	//List of Vertices
	std::vector<vector3> m_lVertexCol;	//List of Colors

public:
	/*
	USAGE: Constructor
	ARGUMENTS: ---
	OUTPUT: class object
	*/
	MyMesh(void);
	/*
	USAGE: Copy Constructor
	ARGUMENTS: class object (to copy)
	OUTPUT: class object
	*/
	MyMesh(MyMesh const& other);
	/*
	USAGE: Copy Assignment Operator
	ARGUMENTS: class object (to copy)
	OUTPUT: class object
	*/
	MyMesh& operator=(MyMesh const& other);
	/*
	USAGE: Destructor
	ARGUMENTS: ---
	OUTPUT: ---
	*/
	~MyMesh(void);
	/*
	USAGE: Swaps the contents of the object with another object's content
	ARGUMENTS: class object  (to swap with)
	OUTPUT: ---
	*/
	void Swap(MyMesh& other);
	/*
	USAGE: Compiles the Mesh for OpenGL 3.X use
	ARGUMENTS: ---
	OUTPUT: ---
	*/
	void CompileOpenGL3X(void);
	/*
	USAGE: Asks the Mesh if it has been binded
	ARGUMENTS: ---
	OUTPUT: Whether its being binded or not
	*/
	bool GetBinded(void);
	/*
	USAGE: Returns the total number of vertices in this Mesh
	ARGUMENTS: ---
	OUTPUT: Number of vertices
	*/
	int GetVertexCount(void);
	/*
	USAGE: Adds a new point to the vector of vertices
	ARGUMENTS:
	- vector3 a_v3Input -> vector input
	OUTPUT: ---
	*/
	void AddVertexPosition(vector3 a_v3Input);
	/*
	USAGE: Adds a new color to the vector of vertices
	ARGUMENTS:
	- vector3 a_v3Input -> vector input
	OUTPUT: ---
	*/
	void AddVertexColor(vector3 a_v3Input);
	/*
	USAGE: Asks the shape for the list of vertices
	ARGUMENTS: ---
	OUTPUT: list of vertices of this mesh
	*/
	std::vector<vector3> GetVertexList(void);
	/*
	USAGE: Returns the first color used for the shape
	ARGUMENTS: ---
	OUTPUT: first color vertex
	*/
	vector3 GetFirstColor(void);

	/*
	USAGE: renders the object in wireframe mode
	ARGUMENTS:
	matrix4 a_mViewProjection -> View * Projection matrix
	matrix4 a_mToWorld -> to world matrix
	vector3 a_v3Color -> Color to render
	OUTPUT: ---
	*/
	virtual void Render(matrix4 a_mProjection, matrix4 a_mView, matrix4 a_mWorld);

protected:
	/*
	USAGE: Initialize the object's fields
	ARGUMENTS: ---
	OUTPUT: ---
	*/
	void Init(void);
	/*
	USAGE: Releases the object from memory
	ARGUMENTS: ---
	OUTPUT: ---
	*/
	void Release(void);
	/*
	USAGE: Completes the information missing to create the mesh
	ARGUMENTS: ---
	OUTPUT: ---
	*/
	void CompleteMesh(void);
	/*
	USAGE: Disconnects OpenGL3.x from the Mesh without releasing the buffers
	ARGUMENTS: ---
	OUTPUT: ---
	*/
	void DisconnectOpenGL3X(void);

public:
	/*
	USAGE: Completes the triangle information
	ARGUMENTS:
	- bool a_bAverageNormals = false -> soften the edges of the model
	OUTPUT: ---
	*/
	void CompleteTriangleInfo(bool a_bAverageNormals = false);

};

#endif //__MYMESH_H_