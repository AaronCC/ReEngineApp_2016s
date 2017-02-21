/*--------------------------------------------------------------------------------------------------
Programmer: Alberto Bobadilla (labigm@rit.edu)
Date: 2016/02
--------------------------------------------------------------------------------------------------*/
#ifndef __MYPRIMITIVE_H_
#define __MYPRIMITIVE_H_

#include "MyMesh.h"

class MyPrimitive : public MyMesh
{
	typedef MyMesh super;

public:
	/*
	MyPrimitive
	USAGE: Constructor
	ARGUMENTS: ---
	OUTPUT: class object
	*/
	MyPrimitive();
	/* Copy Constructor */
	/*
	USAGE:
	ARGUMENTS:
	OUTPUT:
	*/
	MyPrimitive(const MyPrimitive& other);
	/* Copy Assignment Operator */
	/*
	USAGE:
	ARGUMENTS:
	OUTPUT:
	*/
	virtual MyPrimitive& operator=(const MyPrimitive& other);

	/* Destructor */
	/*
	USAGE:
	ARGUMENTS: ---
	OUTPUT: ---
	*/
	~MyPrimitive(void);

	/* Generates a cube shape */
	/*
	USAGE:
	ARGUMENTS:
	OUTPUT: ---
	*/
	void GenerateCube(float a_fSize, vector3 a_v3Color);
	/* Generates a cone shape */
	/*
	USAGE:
	ARGUMENTS:
	OUTPUT: ---
	*/
	void GenerateCone(float a_fRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color);
	/* Generates a cylinder shape */
	/*
	USAGE:
	ARGUMENTS:
	OUTPUT: ---
	*/
	void GenerateCylinder(float a_fRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color);
	/* Generates a tube shape */
	/*
	USAGE:
	ARGUMENTS:
	OUTPUT: ---
	*/
	void GenerateTube(float a_fOuterRadius, float a_fInnerRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color);
	/* Generates a torus shape */
	/*
	USAGE:
	ARGUMENTS:
	OUTPUT: ---
	*/
	void GenerateTorus(float a_fOuterRadius, float a_fInnerRadius, int a_nSubdivisionHeigh, int a_nSubdivisionAxis, vector3 a_v3Color);
	/* Generates a Sphere shape */
	/*
	USAGE:
	ARGUMENTS:
	OUTPUT: ---
	*/
	void GenerateSphere(float a_fRadius, int a_nSubdivisions, vector3 a_v3Color);
	/* Generates a plane shape */
	/*
	USAGE:
	ARGUMENTS:
	OUTPUT: ---
	*/
	void GeneratePlane(float a_fSize, vector3 a_v3Color);

private:
	/* Compiles the object with an specific color and name */
	/*
	USAGE:
	ARGUMENTS:
	OUTPUT: ---
	*/
	void CompileObject(vector3 a_v3Color);
	/* Adds a quad to the list points in the buffer to be compiled*/
	/*
	USAGE:
	ARGUMENTS:
	OUTPUT: ---
	*/
	void AddQuad(vector3 a_vBottomLeft, vector3 a_vBottomRight, vector3 a_vTopLeft, vector3 a_vTopRight);
};

#endif //__MYPRIMITIVE_H_