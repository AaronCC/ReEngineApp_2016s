#include "MyPrimitive.h"
MyPrimitive::MyPrimitive() { }
MyPrimitive::MyPrimitive(const MyPrimitive& other) { }
MyPrimitive& MyPrimitive::operator=(const MyPrimitive& other) { return *this; }
MyPrimitive::~MyPrimitive(void) { super::Release(); }
void MyPrimitive::CompileObject(vector3 a_v3Color)
{
	m_uVertexCount = static_cast<int> (m_lVertexPos.size());
	for (uint i = 0; i < m_uVertexCount; i++)
	{
		AddVertexColor(a_v3Color);
	}
	
	CompleteTriangleInfo(true);
	CompileOpenGL3X();

}
//C--D
//|  |
//A--B
//This will make the triang A->B->C and then the triang C->B->D
void MyPrimitive::AddQuad(vector3 a_vBottomLeft, vector3 a_vBottomRight, vector3 a_vTopLeft, vector3 a_vTopRight)
{
	AddVertexPosition(a_vBottomLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopLeft);

	AddVertexPosition(a_vTopLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopRight);
}
void MyPrimitive::GeneratePlane(float a_fSize, vector3 a_v3Color)
{
	if (a_fSize < 0.01f)
		a_fSize = 0.01f;

	Release();
	Init();

	float fValue = 0.5f * a_fSize;

	vector3 pointA(-fValue, -fValue, 0.0f); //0
	vector3 pointB(fValue, -fValue, 0.0f); //1
	vector3 pointC(fValue, fValue, 0.0f); //2
	vector3 pointD(-fValue, fValue, 0.0f); //3

	vector3 pointE(fValue, -fValue, -0.001f); //1
	vector3 pointF(-fValue, -fValue, -0.001f); //0
	vector3 pointG(fValue, fValue, -0.001f); //2
	vector3 pointH(-fValue, fValue, -0.001f); //3

											  //F
	AddQuad(pointA, pointB, pointD, pointC);
	//Double sided
	AddQuad(pointE, pointF, pointG, pointH);

	CompileObject(a_v3Color);
}
void MyPrimitive::GenerateCube(float a_fSize, vector3 a_v3Color)
{
	if (a_fSize < 0.01f)
		a_fSize = 0.01f;

	Release();
	Init();

	float fValue = 0.5f * a_fSize;
	//3--2
	//|  |
	//0--1
	vector3 point0(-fValue, -fValue, fValue); //0
	vector3 point1(fValue, -fValue, fValue); //1
	vector3 point2(fValue, fValue, fValue); //2
	vector3 point3(-fValue, fValue, fValue); //3

	vector3 point4(-fValue, -fValue, -fValue); //4
	vector3 point5(fValue, -fValue, -fValue); //5
	vector3 point6(fValue, fValue, -fValue); //6
	vector3 point7(-fValue, fValue, -fValue); //7

											  //F
	AddQuad(point0, point1, point3, point2);

	//B
	AddQuad(point5, point4, point6, point7);

	//L
	AddQuad(point4, point0, point7, point3);

	//R
	AddQuad(point1, point5, point2, point6);

	//U
	AddQuad(point3, point2, point7, point6);

	//D
	AddQuad(point4, point5, point0, point1);

	CompileObject(a_v3Color);
}
void MyPrimitive::GenerateCone
(float a_fRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	//Calculating the bottom vertices
	std::vector<vector3 > vertex;
	vertex.push_back(vector3(0.0f, -a_fHeight / 2.0f, 0.0f));

	GLfloat theta = 0;
	for (int i = 0; i < a_nSubdivisions; i++)
	{
		theta += static_cast<GLfloat>(2 * PI / a_nSubdivisions);
		vector3 temp = vector3(static_cast<GLfloat>(cos(theta)) * a_fRadius,
			-a_fHeight / 2.0f,
			-static_cast<GLfloat>(sin(theta)) * a_fRadius);
		vertex.push_back(temp);
	}

	vertex.push_back(vector3(0.0f, a_fHeight / 2.0f, 0.0f));

	//Bottom Faces
	for (int i = 1; i < a_nSubdivisions; i++)
	{
		AddVertexPosition(vertex[0]);
		AddVertexPosition(vertex[i + 1]);
		AddVertexPosition(vertex[i]);
	}
	AddVertexPosition(vertex[0]);
	AddVertexPosition(vertex[1]);
	AddVertexPosition(vertex[a_nSubdivisions]);

	//Side Faces
	for (int i = 1; i < a_nSubdivisions; i++)
	{
		AddVertexPosition(vertex[i]);
		AddVertexPosition(vertex[i + 1]);
		AddVertexPosition(vertex[a_nSubdivisions + 1]);
	}

	AddVertexPosition(vertex[a_nSubdivisions]);
	AddVertexPosition(vertex[1]);
	AddVertexPosition(vertex[a_nSubdivisions + 1]);


	// Adding information about color and UVs to all faces
	int nFaces = (a_nSubdivisions * 3 + 3) * 2;
	for (int i = 1; i < nFaces; i++)
	{
		AddVertexColor(a_v3Color);
	}

	CompileObject(a_v3Color);
}
void MyPrimitive::GenerateCylinder(float a_fRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	//Calculating the bottom vertices
	std::vector<vector3 > vertexB;
	vertexB.push_back(vector3(0.0f, -a_fHeight / 2.0f, 0.0f));
	GLfloat theta = 0;
	for (int i = 0; i < a_nSubdivisions; i++)
	{
		theta += static_cast<GLfloat>(2 * PI / a_nSubdivisions);
		vector3 temp = vector3(static_cast<GLfloat>(cos(theta)) * a_fRadius,
			-a_fHeight / 2.0f,
			-static_cast<GLfloat>(sin(theta)) * a_fRadius);
		vertexB.push_back(temp);
	}

	//calculating the top vertices
	std::vector<vector3 > vertexU;
	vertexU.push_back(vector3(0.0f, a_fHeight / 2.0f, 0.0f));
	theta = 0;
	vector3 vHeight = vector3(0.0f, a_fHeight, 0.0f);
	for (int i = 0; i < a_nSubdivisions; i++)
	{
		vertexU.push_back(vertexB[i + 1] + vHeight);
	}

	//Bottom Faces
	for (int i = 1; i < a_nSubdivisions; i++)
	{
		AddVertexPosition(vertexB[0]);
		AddVertexPosition(vertexB[i + 1]);
		AddVertexPosition(vertexB[i]);
	}
	AddVertexPosition(vertexB[0]);
	AddVertexPosition(vertexB[1]);
	AddVertexPosition(vertexB[a_nSubdivisions]);

	//Top Faces
	for (int i = 1; i < a_nSubdivisions; i++)
	{
		AddVertexPosition(vertexU[0]);
		AddVertexPosition(vertexU[i]);
		AddVertexPosition(vertexU[i + 1]);
	}
	AddVertexPosition(vertexU[0]);
	AddVertexPosition(vertexU[a_nSubdivisions]);
	AddVertexPosition(vertexU[1]);

	//Side Faces
	for (int i = 1; i < a_nSubdivisions; i++)
	{
		AddVertexPosition(vertexB[i]);
		AddVertexPosition(vertexB[i + 1]);
		AddVertexPosition(vertexU[i]);

		AddVertexPosition(vertexU[i]);
		AddVertexPosition(vertexB[i + 1]);
		AddVertexPosition(vertexU[i + 1]);
	}
	AddVertexPosition(vertexB[a_nSubdivisions]);
	AddVertexPosition(vertexB[1]);
	AddVertexPosition(vertexU[a_nSubdivisions]);

	AddVertexPosition(vertexU[a_nSubdivisions]);
	AddVertexPosition(vertexB[1]);
	AddVertexPosition(vertexU[1]);

	// Adding information about color and UVs to all faces
	int nFaces = (a_nSubdivisions * 6 + 6) * 3;
	for (int i = 1; i < nFaces; i++)
	{
		AddVertexColor(a_v3Color);
	}
	CompileObject(a_v3Color);
}
void MyPrimitive::GenerateTube(float a_fOuterRadius, float a_fInnerRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	//Calculating the top vertices
	std::vector<vector3 > vertexTopOuter;
	GLfloat theta = 0;
	for (int i = 0; i < a_nSubdivisions; i++)
	{
		theta += static_cast<GLfloat>(2 * PI / a_nSubdivisions);
		vector3 temp = vector3(static_cast<GLfloat>(cos(theta)) * a_fOuterRadius,
			a_fHeight / 2.0f,
			-static_cast<GLfloat>(sin(theta)) * a_fOuterRadius);
		vertexTopOuter.push_back(temp);
	}

	std::vector<vector3 > vertexTopInner;
	theta = 0;
	for (int i = 0; i < a_nSubdivisions; i++)
	{
		theta += static_cast<GLfloat>(2 * PI / a_nSubdivisions);
		vector3 temp = vector3(static_cast<GLfloat>(cos(theta)) * a_fInnerRadius,
			a_fHeight / 2.0f,
			-static_cast<GLfloat>(sin(theta)) * a_fInnerRadius);
		vertexTopInner.push_back(temp);
	}

	//Bottom Outer
	for (int i = 0; i < a_nSubdivisions - 1; i++)
	{
		AddVertexPosition(vertexTopInner[i]);
		AddVertexPosition(vertexTopOuter[i]);
		AddVertexPosition(vertexTopOuter[i + 1]);
	}
	AddVertexPosition(vertexTopInner[a_nSubdivisions - 1]);
	AddVertexPosition(vertexTopOuter[a_nSubdivisions - 1]);
	AddVertexPosition(vertexTopOuter[0]);

	//Bottom Inner
	for (int i = 0; i < a_nSubdivisions - 1; i++)
	{
		AddVertexPosition(vertexTopOuter[i + 1]);
		AddVertexPosition(vertexTopInner[i + 1]);
		AddVertexPosition(vertexTopInner[i]);
	}
	AddVertexPosition(vertexTopOuter[0]);
	AddVertexPosition(vertexTopInner[0]);
	AddVertexPosition(vertexTopInner[a_nSubdivisions - 1]);

	//Calculating the bottom vertices
	std::vector<vector3 > vertexBottomOuter;
	theta = 0;
	for (int i = 0; i < a_nSubdivisions; i++)
	{
		theta += static_cast<GLfloat>(2 * PI / a_nSubdivisions);
		vector3 temp = vector3(static_cast<GLfloat>(cos(theta)) * a_fOuterRadius,
			-a_fHeight / 2.0f,
			-static_cast<GLfloat>(sin(theta)) * a_fOuterRadius);
		vertexBottomOuter.push_back(temp);
	}

	std::vector<vector3 > vertexBottomInner;
	theta = 0;
	for (int i = 0; i < a_nSubdivisions; i++)
	{
		theta += static_cast<GLfloat>(2 * PI / a_nSubdivisions);
		vector3 temp = vector3(static_cast<GLfloat>(cos(theta)) * a_fInnerRadius,
			-a_fHeight / 2.0f,
			-static_cast<GLfloat>(sin(theta)) * a_fInnerRadius);
		vertexBottomInner.push_back(temp);
	}

	//Bottom Outer
	for (int i = 0; i < a_nSubdivisions - 1; i++)
	{
		AddVertexPosition(vertexBottomOuter[i + 1]);
		AddVertexPosition(vertexBottomOuter[i]);
		AddVertexPosition(vertexBottomInner[i]);
	}
	AddVertexPosition(vertexBottomOuter[0]);
	AddVertexPosition(vertexBottomOuter[a_nSubdivisions - 1]);
	AddVertexPosition(vertexBottomInner[a_nSubdivisions - 1]);


	//Bottom Inner
	for (int i = 0; i < a_nSubdivisions - 1; i++)
	{
		AddVertexPosition(vertexBottomInner[i]);
		AddVertexPosition(vertexBottomInner[i + 1]);
		AddVertexPosition(vertexBottomOuter[i + 1]);
	}
	AddVertexPosition(vertexBottomInner[a_nSubdivisions - 1]);
	AddVertexPosition(vertexBottomInner[0]);
	AddVertexPosition(vertexBottomOuter[0]);


	// Adding information about color and UVs to all faces
	int nFaces = ((a_nSubdivisions * 24) + 24);
	for (int i = 1; i < nFaces; i++)
	{
		AddVertexColor(a_v3Color);
	}
	//Vertical Inner
	for (int i = 0; i < a_nSubdivisions - 1; i++)
	{
		AddVertexPosition(vertexTopInner[i]);
		AddVertexPosition(vertexBottomInner[i + 1]);
		AddVertexPosition(vertexBottomInner[i]);
	}
	AddVertexPosition(vertexTopInner[a_nSubdivisions - 1]);
	AddVertexPosition(vertexBottomInner[0]);
	AddVertexPosition(vertexBottomInner[a_nSubdivisions - 1]);

	for (int i = 0; i < a_nSubdivisions - 1; i++)
	{
		AddVertexPosition(vertexTopInner[i + 1]);
		AddVertexPosition(vertexBottomInner[i + 1]);
		AddVertexPosition(vertexTopInner[i]);
	}
	AddVertexPosition(vertexTopInner[0]);
	AddVertexPosition(vertexBottomInner[0]);
	AddVertexPosition(vertexTopInner[a_nSubdivisions - 1]);


	nFaces = ((a_nSubdivisions * 6) + 6);
	for (int i = 1; i < nFaces; i++)
	{
		AddVertexColor(a_v3Color);
	}

	//Vertical Outer
	for (int i = 0; i < a_nSubdivisions - 1; i++)
	{
		AddVertexPosition(vertexBottomOuter[i]);
		AddVertexPosition(vertexBottomOuter[i + 1]);
		AddVertexPosition(vertexTopOuter[i]);
	}
	AddVertexPosition(vertexBottomOuter[a_nSubdivisions - 1]);
	AddVertexPosition(vertexBottomOuter[0]);
	AddVertexPosition(vertexTopOuter[a_nSubdivisions - 1]);

	for (int i = 0; i < a_nSubdivisions - 1; i++)
	{
		AddVertexPosition(vertexTopOuter[i + 1]);
		AddVertexPosition(vertexTopOuter[i]);
		AddVertexPosition(vertexBottomOuter[i + 1]);
	}
	AddVertexPosition(vertexTopOuter[0]);
	AddVertexPosition(vertexTopOuter[a_nSubdivisions - 1]);
	AddVertexPosition(vertexBottomOuter[0]);


	nFaces = ((a_nSubdivisions * 6) + 6);
	for (int i = 1; i < nFaces; i++)
	{
		AddVertexColor(a_v3Color);
	}

	CompileObject(a_v3Color);
}
void MyPrimitive::GenerateTorus(float a_fOuterRadius, float a_fInnerRadius, int a_nSubdivisionsA, int a_nSubdivisionsB, vector3 a_v3Color)
{
	if (a_fOuterRadius <= a_fInnerRadius + 0.1f)
		return;

	if (a_nSubdivisionsA < 3)
		a_nSubdivisionsA = 3;
	if (a_nSubdivisionsA > 25)
		a_nSubdivisionsA = 25;

	if (a_nSubdivisionsB < 3)
		a_nSubdivisionsB = 3;
	if (a_nSubdivisionsB > 25)
		a_nSubdivisionsB = 25;

	float fRadius = (glm::distance(vector3(a_fOuterRadius, 0.0f, 0.0f), vector3(a_fInnerRadius, 0.0f, 0.0f))) / 2.0f;
	vector3 v3MiddlePoint = (vector3(a_fOuterRadius, 0.0f, 0.0f) + vector3(a_fInnerRadius, 0.0f, 0.0f)) / 2.0f;

	Release();
	Init();

	//Calculating the top vertices
	std::vector<std::vector<vector3>> lCircle;
	GLfloat theta = 0;
	//lCircle.push_back( vector3(0.0f) + v3MiddlePoint );
	std::vector<vector3> lLevel;
	for (int i = 0; i < a_nSubdivisionsA; i++)
	{
		vector3 temp = vector3(static_cast<GLfloat>(cos(theta)) * fRadius,
			static_cast<GLfloat>(sin(theta)) * fRadius,
			0.0f);
		temp += v3MiddlePoint;
		lLevel.push_back(temp);
		theta += static_cast<GLfloat>(2 * PI / a_nSubdivisionsA);
	}
	lCircle.push_back(lLevel);
	GLfloat theta2 = 0;
	for (int nLevel = 1; nLevel < a_nSubdivisionsB; nLevel++)
	{
		lLevel.clear();
		theta2 += static_cast<GLfloat>(2 * PI / a_nSubdivisionsB);
		matrix4 m4Rotation = glm::rotate(matrix4(1.0f), glm::degrees(theta2), vector3(0.0f, 1.0f, 0.0f));
		for (int i = 0; i < a_nSubdivisionsA; i++)
		{
			lLevel.push_back(static_cast<vector3>((m4Rotation * vector4(lCircle[0][i], 1.0f))));
		}
		lCircle.push_back(lLevel);
	}

	for (int nSide = 0; nSide < a_nSubdivisionsA - 1; nSide++)
	{
		for (int nLevel = 0; nLevel < a_nSubdivisionsB - 1; nLevel++)
		{
			AddQuad(lCircle[nLevel][nSide], lCircle[nLevel + 1][nSide], lCircle[nLevel][nSide + 1], lCircle[nLevel + 1][nSide + 1]);
		}
		AddQuad(lCircle[a_nSubdivisionsB - 1][nSide], lCircle[0][nSide], lCircle[a_nSubdivisionsB - 1][nSide + 1], lCircle[0][nSide + 1]);
	}

	for (int nLevel = 0; nLevel < a_nSubdivisionsB - 1; nLevel++)
	{
		AddQuad(lCircle[nLevel][a_nSubdivisionsA - 1], lCircle[nLevel + 1][a_nSubdivisionsA - 1], lCircle[nLevel][0], lCircle[nLevel + 1][0]);
	}
	AddQuad(lCircle[a_nSubdivisionsB - 1][a_nSubdivisionsA - 1], lCircle[0][a_nSubdivisionsA - 1], lCircle[a_nSubdivisionsB - 1][0], lCircle[0][0]);

	CompileObject(a_v3Color);
}
void MyPrimitive::GenerateSphere(float a_fRadius, int a_nSubdivisions, vector3 a_v3Color)
{
	//Sets minimum and maximum of subdivisions
	if (a_nSubdivisions < 1)
	{
		GenerateCube(a_fRadius * 2, a_v3Color);
		return;
	}
	if (a_nSubdivisions > 6)
		a_nSubdivisions = 6;

	Release();
	Init();

	float fValue = 1.0f;
	vector3 pointA(-fValue, -fValue, fValue); //0
	vector3 pointB(fValue, -fValue, fValue); //1
	vector3 pointC(-fValue, fValue, fValue); //2

											 //left to right List of vector3
	std::vector<vector3> vectorAB;
	vectorAB.push_back(pointA);
	for (int i = 0; i < a_nSubdivisions; i++)
	{
		vector3 temp(pointB - pointA);
		temp /= a_nSubdivisions + 1;
		temp *= (i + 1);
		vectorAB.push_back(temp + pointA);
	}
	vectorAB.push_back(pointB);

	//height increments
	float fHeight = pointC.y - pointA.y;
	fHeight /= a_nSubdivisions + 1;

	//List of Lists
	std::vector<std::vector<vector3>> list;
	list.push_back(vectorAB);
	for (int j = 0; j < a_nSubdivisions + 1; j++)
	{
		std::vector<vector3> temp = list[0];
		float increment = fHeight * (j + 1);
		for (int i = 0; i < a_nSubdivisions + 2; i++)
		{
			temp[i].y += increment;
		}
		list.push_back(temp);
	}

	//Creating the patch of quads
	for (int j = 0; j < a_nSubdivisions + 1; j++)
	{
		for (int i = 0; i < a_nSubdivisions + 1; i++)
		{
			AddQuad(list[j][i], list[j][i + 1], list[j + 1][i], list[j + 1][i + 1]);
		}
	}

	//normalizing the vectors to make them round
	for (uint i = 0; i < m_uVertexCount; i++)
	{
		m_lVertexPos[i] = glm::normalize(m_lVertexPos[i]);
		m_lVertexPos[i] *= a_fRadius;
	}

	//RightSideFace
	int nVert = m_uVertexCount;
	std::vector<vector3> right;
	for (int i = 0; i < nVert; i++)
	{
		matrix4 rotation;
		rotation = glm::rotate(matrix4(1.0f), 90.0f, vector3(0.0f, 1.0f, 0.0f));
		right.push_back(static_cast <vector3>(rotation * glm::vec4(m_lVertexPos[i], 1.0f)));
	}


	for (int i = 0; i < nVert; i++)
	{
		AddVertexPosition(right[i]);
	}

	//LeftSideFace
	std::vector<vector3> left;
	for (int i = 0; i < nVert; i++)
	{
		matrix4 rotation;
		rotation = glm::rotate(matrix4(1.0f), -90.0f, vector3(0.0f, 1.0f, 0.0f));
		left.push_back(static_cast <vector3>(rotation * glm::vec4(m_lVertexPos[i], 1.0f)));
	}

	for (int i = 0; i < nVert; i++)
	{
		AddVertexPosition(left[i]);
	}

	//BackSideFace
	std::vector<vector3> back;
	for (int i = 0; i < nVert; i++)
	{
		matrix4 rotation;
		rotation = glm::rotate(matrix4(1.0f), 180.0f, vector3(0.0f, 1.0f, 0.0f));
		back.push_back(static_cast <vector3>(rotation * glm::vec4(m_lVertexPos[i], 1.0f)));
	}

	for (int i = 0; i < nVert; i++)
	{
		AddVertexPosition(back[i]);
	}

	//TopSideFace
	std::vector<vector3> top;
	for (int i = 0; i < nVert; i++)
	{
		matrix4 rotation;
		rotation = glm::rotate(matrix4(1.0f), -90.0f, vector3(1.0f, 0.0f, 0.0f));
		top.push_back(static_cast <vector3>(rotation * glm::vec4(m_lVertexPos[i], 1.0f)));
	}

	for (int i = 0; i < nVert; i++)
	{
		AddVertexPosition(top[i]);
	}

	//BottomSideFace
	std::vector<vector3> bottom;
	for (int i = 0; i < nVert; i++)
	{
		matrix4 rotation;
		rotation = glm::rotate(matrix4(1.0f), 90.0f, vector3(1.0f, 0.0f, 0.0f));
		bottom.push_back(static_cast <vector3>(rotation * glm::vec4(m_lVertexPos[i], 1.0f)));
	}

	for (int i = 0; i < nVert; i++)
	{
		AddVertexPosition(bottom[i]);
	}

	CompileObject(a_v3Color);
}