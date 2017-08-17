#include <FishEngine/Gizmos.hpp>
#include <FishEngine/Shader.hpp>
#include <FishEngine/Camera.hpp>
#include <FishEngine/Mesh.hpp>
#include <FishEngine/Pipeline.hpp>
#include <FishEngine/Texture.hpp>
#include <FishEngine/Transform.hpp>

using namespace FishEngine;

Color       Gizmos::s_color         = Color::green;
Matrix4x4   Gizmos::s_matrix        = Matrix4x4::identity;
SimpleMeshPtr Gizmos::s_circleMesh    = nullptr;
SimpleMeshPtr Gizmos::s_boxMesh       = nullptr;
SimpleMeshPtr Gizmos::s_light         = nullptr;


static TexturePtr cameraGizmoTexture;
static TexturePtr lightGizmoTexture;

//std::shared_ptr<SimpleMesh> Gizmos::s_wiredSphereMesh = nullptr;

constexpr int circleVertexCount = 64;


/************************************************************************/
/* helper functions                                                     */
/************************************************************************/

float getScaleForConstantSizeGeometry(const Vector3& center, float targetScale)
{
	// http://gamedev.stackexchange.com/questions/24968/constant-size-geometries
	auto camera = Camera::main();
	float camera_object_distance = Vector3::Distance(camera->transform()->position(), center);
	float worldSize = 2.0f * Mathf::Tan(Mathf::Radians(camera->fieldOfView()) * 0.5f) * camera_object_distance;
	return targetScale * worldSize;
}

void Gizmos::Init()
{
#if 0
	TextureImporter importer;
	cameraGizmoTexture = importer.FromFile(root_dir + "textures/Gizmos/camera.png");
	lightGizmoTexture  = importer.FromFile(root_dir + "textures/Gizmos/light.png");
#endif
	// circle
	constexpr float radStep = 2.0f * Mathf::PI / circleVertexCount;
	float circle_vertex[circleVertexCount*3];
	for (int i = 0; i < circleVertexCount; ++i) {
		const int j = i * 3;
		const float rad = radStep * i - Mathf::PI/2;
		circle_vertex[j] = std::cosf(rad);
		circle_vertex[j+1] = 0.f;
		circle_vertex[j+2] = std::sinf(rad);
	}
	s_circleMesh = std::make_shared<SimpleMesh>(circle_vertex, circleVertexCount, GL_LINE_LOOP);
	
	float box_vertex[] = {
		 0.5f,  0.5f,  0.5f,  0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f,  0.5f,  0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f, -0.5f,  0.5f,  0.5f,
		
		-0.5f,  0.5f, -0.5f, -0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f, -0.5f, -0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,  0.5f,  0.5f, -0.5f,
		
		-0.5f, -0.5f,  0.5f, -0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f, -0.5f,  0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,  0.5f, -0.5f,  0.5f,
		
		 0.5f, -0.5f, -0.5f,  0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f, -0.5f,  0.5f,  0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f,
	};
	s_boxMesh = std::make_shared<SimpleMesh>(box_vertex, 24, GL_LINES);

	float lightGizmoVertex[circleVertexCount * 3 + 8 * 2 * 3];
	memcpy(lightGizmoVertex, circle_vertex, circleVertexCount * 3 * sizeof(float));
	constexpr int   numLines    = 8;
	constexpr float step        = Mathf::PI * 2.0f / numLines;
	for (int i = 0; i < numLines; ++i)
	{
		int j = circleVertexCount * 3 + i * 2 * 3;
		const float rad = step * i;
		float s = std::sinf(rad);
		float c = std::cosf(rad);
		lightGizmoVertex[j]     = c;
		lightGizmoVertex[j + 1] = 0;
		lightGizmoVertex[j + 2] = s;
		lightGizmoVertex[j + 3] = c;
		lightGizmoVertex[j + 4] = 8;
		lightGizmoVertex[j + 5] = s;
	}
	s_light = std::make_shared<SimpleMesh>(lightGizmoVertex, circleVertexCount + numLines * 2, GL_LINES);
	
	// wired sphere
	//constexpr float radStep = 2.0f * Mathf::PI / circleVertexCount;
//    float wiredSphereVertex[circleVertexCount*3*3];
//    for (int i = 0; i < circleVertexCount; ++i) {
//        const int j1 = i * 3;
//        const int j2 = j1 + circleVertexCount*3;
//        const int j3 = j2 + circleVertexCount*3;
//        const float rad = radStep * i;
//        wiredSphereVertex[j3+1] = wiredSphereVertex[j2] =  wiredSphereVertex[j1] = std::cosf(rad);
//        wiredSphereVertex[j3] = wiredSphereVertex[j2+2] = wiredSphereVertex[j1+1] = 0.f;
//        wiredSphereVertex[j3+2] = wiredSphereVertex[j2+1] = wiredSphereVertex[j1+2] = std::sinf(rad);
//    }
//    
//    s_wiredSphereMesh = std::make_shared<SimpleMesh>(wiredSphereVertex, circleVertexCount*3, GL_LINE_STRIP);
}


void Gizmos::DrawLine(const Vector3& from, const Vector3& to)
{
	auto shader = Shader::FindBuiltin("SolidColor-Internal");
	static DynamicMesh lineMesh;
	float vertices[6];
	vertices[0] = from.x;
	vertices[1] = from.y;
	vertices[2] = from.z;
	vertices[3] = to.x;
	vertices[4] = to.y;
	vertices[5] = to.z;
	//glLineWidth(50);  // TODO, no effect
	auto const & view = Camera::main()->worldToCameraMatrix();
	auto const & proj = Camera::main()->projectionMatrix();
	shader->Use();
	shader->BindUniformMat4("MATRIX_MVP", proj*view*s_matrix);
	shader->BindUniformVec4("_Color", s_color);
	lineMesh.Render(vertices, 2, GL_LINES);
}


void Gizmos::
DrawIcon(
	const Vector3&        center,
	const std::string&    name,
	bool                  allowScaling)
{
#if 0
	auto shader = Shader::builtinShader("Alpha-Internal");
	static auto quad = Mesh::builtinMesh(PrimitiveType::ScreenAlignedQuad);
	shader->Use();
	//auto cameraPos = Camera::main()->transform()->position();
	auto view = Camera::main()->worldToCameraMatrix();
	auto proj = Camera::main()->projectionMatrix();
	Matrix4x4 m;
	float scale = 0.5f;
	if (!allowScaling)
		scale = getScaleForConstantSizeGeometry(center, 0.05f);
	m.SetTRS(center, view.ToRotation().inverse(), Vector3::one*scale);

	shader->BindUniformMat4("MATRIX_MVP", proj*view*m);
	std::map<std::string, TexturePtr> textures;
	if (name == "Camera")
		textures["_MainTex"] = cameraGizmoTexture;
	else
		textures["_MainTex"] = lightGizmoTexture;
	shader->BindTextures(textures);
	shader->PreRender();
	quad->Render();
	shader->PostRender();
#endif
}


void Gizmos::
DrawWireSphere(
	const Vector3&      center,
	const float         radius)
{
	Matrix4x4 m;
	auto const & view = Camera::main()->worldToCameraMatrix();
	auto const & proj = Camera::main()->projectionMatrix();
	auto shader = Shader::FindBuiltin("SolidColor-Internal");
	shader->Use();
	shader->BindUniformVec4("_Color", s_color);
	
	float euler_angles[] =
	{
		0, 0, 90,
		0, 0, 0,
		90, 0, 0,
	};
	
	for (int i = 0; i < 3; ++i)
	{
		float* e = euler_angles + i*3;
		m.SetTRS(center, Quaternion::Euler(Vector3(e)), Vector3::one * radius);
		shader->BindUniformMat4("MATRIX_MVP", proj*view*s_matrix*m);
		s_circleMesh->Render();
	}
}


void Gizmos::
DrawHalfWireSphere(
	const Vector3&      center,
	const float         radius,
	const Matrix4x4&    modelMatrix)
{
	auto shader = Shader::FindBuiltin("SolidColor-Internal");
	Matrix4x4 m;
	auto const & v = Camera::main()->worldToCameraMatrix();
	auto const & p = Camera::main()->projectionMatrix();

	float euler_angles[] = {
		0, 0, 90,
		0, 0, 0,
		0, 90, 90,
	};
	
	shader->Use();
	shader->BindUniformVec4("_Color", s_color);
	for (int i = 0; i < 3; ++i)
	{
		float* e = euler_angles + i*3;
		m.SetTRS(center, Quaternion::Euler(Vector3(e)), Vector3::one * radius);
		shader->BindUniformMat4("MATRIX_MVP", p * v * m * modelMatrix);
		glBindVertexArray(s_circleMesh->m_VAO);
		GLsizei count = static_cast<GLsizei>(s_circleMesh->m_positionBuffer.size()/3);
		if (i == 1) {
			glDrawArrays(GL_LINE_LOOP, 0, count);
		} else {
			count = count/2+1;
			glDrawArrays(GL_LINE_STRIP, 0, count);
		}
		
		glBindVertexArray(0);
	}

}


void Gizmos::
DrawWireCube(
	const Vector3& center,
	const Vector3& size)
{
	auto shader = Shader::FindBuiltin("SolidColor-Internal");
	shader->Use();
	Matrix4x4 m;
	m.SetTRS(center, Quaternion::identity, size);
	ShaderUniforms uniforms;
	auto const & v = Camera::main()->worldToCameraMatrix();
	auto const & p = Camera::main()->projectionMatrix();
	shader->BindUniformVec4("_Color", s_color);
	shader->BindUniformMat4("MATRIX_MVP", p * v * s_matrix * m);
	s_boxMesh->Render();
}


void Gizmos::
DrawWireCapsule(
	const Vector3&      center,
	const float         radius,
	const float         height)
{
	Vector3 c1 = center+Vector3(0, height*0.5f-radius, 0);
	Vector3 c2 = center-Vector3(0, height*0.5f-radius, 0);
	DrawHalfWireSphere(c1, radius);
	DrawHalfWireSphere(c2, radius, Matrix4x4::Scale(-1, 1, 1));
	
	Vector3 offset(radius, 0, 0);
	DrawLine(c1+offset, c2+offset);
	offset.Set(-radius, 0, 0);
	DrawLine(c1+offset, c2+offset);
	offset.Set(0, 0, radius);
	DrawLine(c1+offset, c2+offset);
	offset.Set(0, 0, -radius);
	DrawLine(c1+offset, c2+offset);
}


void FishEngine::Gizmos::
DrawHalfCircle(
	const Vector3&  center,
	const float     radius,
	const Vector3&  dir1,
	const Vector3&  dir2)
{
	const auto& shader = Shader::FindBuiltin("SolidColor-Internal");
	shader->Use();
	auto const & v = Camera::main()->worldToCameraMatrix();
	auto const & p = Camera::main()->projectionMatrix();

	Vector3 y = Vector3::Normalize(dir1);
	Vector3 x = Vector3::Normalize(dir2);
	Vector3 z = Vector3::Normalize(Vector3::Cross(x, y));
	Matrix4x4 m;    // local to world
	m.rows[0] = Vector4{ x.x, y.x, z.x, 0};
	m.rows[1] = Vector4{ x.y, y.y, z.y, 0};
	m.rows[2] = Vector4{ x.z, y.z, z.z, 0};
	m.rows[3] = Vector4{ 0, 0, 0, 1 };
	m *= Matrix4x4::Scale(radius);
	m.rows[0][3] = center.x;
	m.rows[1][3] = center.y;
	m.rows[2][3] = center.z;
	//Matrix4x4 m;
	//m.SetTRS(center, Quaternion::FromToRotation(Vector3::up, dir1), Vector3(radius, radius, radius));
	shader->BindUniformMat4("MATRIX_MVP", p * v * m);
	shader->BindUniformVec4("_Color", s_color);
	glBindVertexArray(s_circleMesh->m_VAO);
	GLsizei count = static_cast<GLsizei>(s_circleMesh->m_positionBuffer.size() / 3);
	count = count / 2 + 1;
	glDrawArrays(GL_LINE_STRIP, 0, count);
	glBindVertexArray(0);
}


void FishEngine::Gizmos::
DrawCircle(
	const Vector3&  center,
	const float     radius,
	const Vector3&  direction)
{
	const auto& shader = Shader::FindBuiltin("SolidColor-Internal");
	shader->Use();
	auto const & v = Camera::main()->worldToCameraMatrix();
	auto const & p = Camera::main()->projectionMatrix();
	Matrix4x4 m;
	m.SetTRS(center, Quaternion::FromToRotation(Vector3::up, direction), Vector3(radius, radius, radius));
	shader->BindUniformMat4("MATRIX_MVP", p * v * m);
	shader->BindUniformVec4("_Color", s_color);
	s_circleMesh->Render();
}


void FishEngine::Gizmos::
DrawLight(
	const Vector3& center,
	const Vector3& direction)
{
	const auto& shader = Shader::FindBuiltin("SolidColor-Internal");
	shader->Use();
	float scale = getScaleForConstantSizeGeometry(center, 0.02f);
	Matrix4x4 m;
	m.SetTRS(center, Quaternion::FromToRotation(Vector3::up, direction), Vector3::one*scale);
	auto const & view = Camera::main()->worldToCameraMatrix();
	auto const & proj = Camera::main()->projectionMatrix();
	shader->Use();
	shader->BindUniformMat4("MATRIX_MVP", proj*view*m);
	shader->BindUniformVec4("_Color", s_color);
	s_light->Render();
}


void Gizmos::
DrawFrustum(
	const Vector3&  center,
	const float     fov,
	const float     maxRange,
	const float     minRange,
	const float     aspect)
{
	static DynamicMesh frustumMesh;
	constexpr int numLines = 4 * 3;
	float vertices[numLines * 2 * 3];
	float rad = Mathf::Radians(fov) * 0.5f;
	float tan2 = Mathf::Tan(rad);
	//float tan2 = Mathf::Tan(fov * 0.5f);
	float h1 = tan2 * minRange;
	float w1 = h1 * aspect;
	float h2 = tan2 * maxRange;
	float w2 = h2 * aspect;

	Vector3 v[8];
	v[0].Set(-w1, h1, minRange);
	v[1].Set(w1, h1, minRange);
	v[2].Set(w1, -h1, minRange);
	v[3].Set(-w1, -h1, minRange);
	v[4].Set(-w2, h2, maxRange);
	v[5].Set(w2, h2, maxRange);
	v[6].Set(w2, -h2, maxRange);
	v[7].Set(-w2, -h2, maxRange);
	for (int i = 0; i < 8; ++i)
	{
		v[i] = s_matrix.MultiplyPoint(v[i]);
	}

	const int indices[numLines * 2] = {
		0, 1, 1, 2, 2, 3, 3, 0,
		4, 5, 5, 6, 6, 7, 7, 4,
		0, 4, 1, 5, 2, 6, 3, 7,
	};

	float *p = vertices;
	for (int i = 0; i < numLines * 2; ++i)
	{
		auto v3 = v[indices[i]];
		*p = v3.x; p++;
		*p = v3.y; p++;
		*p = v3.z; p++;
	}

	auto const & view = Camera::main()->worldToCameraMatrix();
	auto const & proj = Camera::main()->projectionMatrix();
	auto shader = Shader::FindBuiltin("SolidColor-Internal");
	shader->Use();
	shader->BindUniformMat4("MATRIX_MVP", proj*view);
	shader->BindUniformVec4("_Color", s_color);
	frustumMesh.Render(vertices, numLines * 2, GL_LINES);
}

void Gizmos::DrawFrustum(
	const Frustum&      frustum)
{
	// TODO: remove Vector3::zero
	DrawFrustum(Vector3::zero, frustum.fov, frustum.maxRange, frustum.minRange, frustum.aspect);
}

