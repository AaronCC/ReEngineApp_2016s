#include "MyMesh.h"
//  MyMesh
void MyMesh::Init(void)
{
	m_bBinded = false;
	m_uVertexCount = 0;
	m_uMaterialIndex = -1;
	
	m_VAO = 0;
	m_VBO = 0;
	m_VertexBuffer = 0;
	m_ColorBuffer = 0;
		
	m_nShader = 0;
	
	m_pMatMngr = MaterialManagerSingleton::GetInstance();
	m_pShaderMngr = ShaderManagerSingleton::GetInstance();
	m_nShader = m_pShaderMngr->GetShaderID("BasicColor");
}
void MyMesh::Swap(MyMesh& other)
{
	std::swap(m_bBinded, other.m_bBinded);
	std::swap(m_uVertexCount, other.m_uVertexCount);
	std::swap(m_uMaterialIndex, other.m_uMaterialIndex);
	
	std::swap(m_VAO, other.m_VAO);
	std::swap(m_VertexBuffer, other.m_VertexBuffer);
	std::swap(m_ColorBuffer, other.m_ColorBuffer);
	std::swap(m_nShader, other.m_nShader);

	std::swap(m_lVertex, other.m_lVertex);
	std::swap(m_lVertexPos, other.m_lVertexPos);
	std::swap(m_lVertexCol, other.m_lVertexCol);

	std::swap(m_pMatMngr, other.m_pMatMngr);
	std::swap(m_pShaderMngr, other.m_pShaderMngr);
}
void MyMesh::Release(void)
{
	m_pMatMngr = nullptr;
	m_pShaderMngr = nullptr;

	DisconnectOpenGL3X();

	if (m_ColorBuffer > 0)
		glDeleteBuffers(1, &m_ColorBuffer);

	if (m_VertexBuffer > 0)
		glDeleteBuffers(1, &m_VertexBuffer);

	if (m_VBO > 0)
		glDeleteBuffers(1, &m_VBO);

	if (m_VAO > 0)
		glDeleteVertexArrays(1, &m_VAO);

	m_lVertex.clear();
	m_lVertexPos.clear();
	m_lVertexCol.clear();
}
//The big 3
MyMesh::MyMesh() { Init(); }
MyMesh::MyMesh(MyMesh const& other)
{
	m_bBinded = other.m_bBinded;
	m_uVertexCount = other.m_uVertexCount;
	m_uMaterialIndex = other.m_uMaterialIndex;
	

	m_VAO = other.m_VAO;
	m_VertexBuffer = other.m_VertexBuffer;
	m_ColorBuffer = other.m_ColorBuffer;

	m_nShader = other.m_nShader;

	m_lVertex = other.m_lVertex;
	m_lVertexPos = other.m_lVertexPos;
	m_lVertexCol = other.m_lVertexCol;

	m_pMatMngr = other.m_pMatMngr;
	m_pShaderMngr = other.m_pShaderMngr;
}
MyMesh& MyMesh::operator=(MyMesh const& other)
{
	if (this != &other)
	{
		Release();
		Init();
		MyMesh temp(other);
		Swap(temp);
	}
	return *this;
}
MyMesh::~MyMesh() { Release(); };

//Accessors
bool MyMesh::GetBinded(void) { return m_bBinded; }
int MyMesh::GetVertexCount(void) { return m_uVertexCount; }
void MyMesh::AddVertexPosition(vector3 input) { m_lVertexPos.push_back(input); m_uVertexCount++; }
void MyMesh::AddVertexColor(vector3 input) { m_lVertexCol.push_back(input); }
std::vector<vector3> MyMesh::GetVertexList(void) { return m_lVertexPos; }
vector3 MyMesh::GetFirstColor(void) { return m_lVertexCol[0]; }

//Methods
void MyMesh::CompleteTriangleInfo(bool a_bAverageNormals)
{
	//Complete Colors
	int nColors = static_cast<int>(m_lVertexCol.size());
	for (uint i = nColors; i < m_uVertexCount; i++)
		m_lVertexCol.push_back(vector3(1, 1, 1));
}
void MyMesh::DisconnectOpenGL3X(void)
{
	m_VAO = 0;
	m_VertexBuffer = 0;
	m_ColorBuffer = 0;
}
void MyMesh::CompleteMesh(void)
{
	uint nColorTotal = m_lVertexCol.size();
	for (uint nColor = nColorTotal; nColor < m_uVertexCount; nColor++)
		m_lVertexCol.push_back(vector3(1.0f, 0.0f, 1.0f));
}
void MyMesh::CompileOpenGL3X(void)
{
	if (m_bBinded)
		return;

	if (m_uVertexCount == 0)
		return;

	CompleteMesh();

	m_lVertex.clear();

	for (uint i = 0; i < m_uVertexCount; i++)
	{
		//Position
		m_lVertex.push_back(m_lVertexPos[i]);
		//Color
		m_lVertex.push_back(m_lVertexCol[i]);
	}
	glGenVertexArrays(1, &m_VAO);//Generate vertex array object
	glGenBuffers(1, &m_VBO);//Generate Vertex Buffered Object

	glBindVertexArray(m_VAO);//Bind the VAO
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);//Bind the VBO
	glBufferData(GL_ARRAY_BUFFER, m_uVertexCount * 2 * sizeof(vector3), &m_lVertex[0], GL_STATIC_DRAW);//Generate space for the VBO

																									   // Position attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(vector3), (GLvoid*)0);

	// Color attribute
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(vector3), (GLvoid*)(1 * sizeof(vector3)));

	if (m_uMaterialIndex < 0)
	{
		if (m_pMatMngr->GetMaterialCount() < 1)
		{
			MaterialClass temp("Default");
			temp.SetDiffuse(vector3(1.0f, 0.0f, 1.0f));
			m_uMaterialIndex = m_pMatMngr->AddMaterial(temp);
		}
		else
			m_uMaterialIndex = 0;
	}

	m_bBinded = true;

	glBindVertexArray(0); // Unbind VAO

	return;
}

void MyMesh::Render(matrix4 a_mProjection, matrix4 a_mView, matrix4 a_mWorld)
{

	if (!m_bBinded)
		return;

	GLuint nShader = m_pShaderMngr->GetShaderID("BasicColor");
	// Use the buffer and shader
	glUseProgram(nShader);

	glBindVertexArray(m_VAO);

	// Get the GPU variables by their name and hook them to CPU variables
	GLuint MVP = glGetUniformLocation(nShader, "MVP");
	GLuint WireColor = glGetUniformLocation(nShader, "WireColor");

	//Final Projection of the Camera
	glUniformMatrix4fv(MVP, 1, GL_FALSE, glm::value_ptr(a_mProjection * a_mView * a_mWorld));

	//Render

	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glUniform3f(WireColor, -1, -1, -1);
	glDrawArrays(GL_TRIANGLES, 0, m_uVertexCount);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glUniform3f(WireColor, 1 - m_lVertexCol[0].r, 1 - m_lVertexCol[0].g, 1 - m_lVertexCol[0].b);
	glDrawArrays(GL_TRIANGLES, 0, m_uVertexCount);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glBindVertexArray(0);
}