#pragma once
#include "RE\ReEng.h"

class MyBoundingSphereClass
{
public:
	float m_fRadius = 0.0f;
	vector3 m_v3Center = vector3(0.0f);

	MyBoundingSphereClass(std::vector<vector3> vertexList);
};