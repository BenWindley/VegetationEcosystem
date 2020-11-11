#include "pch.h"
#include "VegetationFeature.h"
#include "VegetationNode.h"

#include <cmath>
#include <random>

using namespace DirectX;

VegetationFeature::VegetationFeature(Transform* parent)
{
	m_parent = parent;

	static int totalIDs = 0;
	m_id = totalIDs++;
}

VegetationFeature::~VegetationFeature()
{
	delete m_cube;
}

void VegetationFeature::Start(DX::DeviceResources* deviceResources, Vegetation_Ecosystem::RendererResources* rendererResources)
{
	m_deviceResources = deviceResources;
	m_rendererResources = rendererResources;
}

void VegetationFeature::Update(float growth, float time)
{

}

void VegetationFeature::Render(Vegetation_Ecosystem::ModelViewProjectionConstantBuffer constantBufferData)
{
	//m_cube->Render(constantBufferData);
}

bool VegetationFeature::GetFate()
{
	return m_fate;
}

bool VegetationFeature::IsDormant()
{
	return m_dormant;
}

float VegetationFeature::GetTropismFactor()
{
	return TROPISM_FACTOR;
}

XMVECTOR VegetationFeature::GetSpatialTropism()
{
	return XMVectorScale(m_spatialTropism, SPATIALTROPISM_FACTOR);
}

XMVECTOR VegetationFeature::GetTropismDirectionQuaternion()
{
	// Phototropism
	auto photoTropism = GetLookatRotation(m_photoTropismDirection);
	auto graviTropism = GetLookatRotation({ 0,1,0 });
	auto spatialTropism = GetLookatRotation(m_spatialTropism);

	return XMQuaternionSlerp(
		XMQuaternionSlerp(photoTropism, graviTropism, GRAVITROPISM_FACTOR / std::fmaxf(GRAVITROPISM_FACTOR + PHOTOTROPISM_FACTOR, 0.01f)), 
		spatialTropism, 
		SPATIALTROPISM_FACTOR / std::fmaxf(GRAVITROPISM_FACTOR + PHOTOTROPISM_FACTOR + SPATIALTROPISM_FACTOR, 0.01f)
	);
}

float VegetationFeature::GetBiomass()
{
	return m_branchWidth * 20.0f;
}

XMVECTOR VegetationFeature::GetLookatRotation(DirectX::XMVECTOR directionVector)
{
	directionVector = XMVector3Normalize({ directionVector.m128_f32[0], directionVector.m128_f32[2], directionVector.m128_f32[1] });

	XMVECTOR sampleScale;
	XMVECTOR samplePosition;
	XMVECTOR sampleRotation;

	static const XMVECTOR pos = { 0.0f, 0.0f, 0.0f };
	static const XMVECTOR at = XMVector3Normalize(directionVector);
	static const XMVECTOR up = { 0.0f, 1.0f, 0.0f };

	auto lookAtMatrix = XMMatrixLookAtLH(pos, at, up);

	XMMatrixDecompose(&sampleScale, &sampleRotation, &samplePosition, lookAtMatrix);

	return sampleRotation;
}

float VegetationFeature::GetGrowthFactor()
{
	return m_light;
}

void VegetationFeature::UpdateTropisms(std::vector<VegetationFeature*>* allFeatures)
{
	if (m_dormant) return;

	float light = 1.0f;

	XMVECTOR selfScale;
	XMVECTOR selfPosition;
	XMVECTOR selfRotation;

	XMMatrixDecompose(&selfScale, &selfRotation, &selfPosition, XMMatrixTranspose(GetTransposeMatrix()));
	selfScale *= m_branchWidth;

	XMVECTOR sampleScale;
	XMVECTOR samplePosition;
	XMVECTOR sampleRotation;

	XMVECTOR totalPhototropism = { 0, 0, 0 };

	static std::random_device r;
	static std::uniform_real_distribution<float> lightRange( -1, 1);

	for (int x = 0; x < LIGHTRAYS; ++x)
	{
		float rayLight = 1.0f;
		XMVECTOR rayDir = DirectX::XMVector3Normalize({ lightRange(r), 1, lightRange(r) });

		for (auto& vFeature : *allFeatures)
		{
			if (light == 0.0f)
				break;
			if (vFeature->m_id == m_id)
				continue;

			XMMatrixDecompose(&sampleScale, &sampleRotation, &samplePosition, XMMatrixTranspose(vFeature->GetTransposeMatrix()));

			sampleScale *= vFeature->m_branchWidth;

			if (samplePosition.m128_f32[1] < selfPosition.m128_f32[1]) 
				continue;
			if (XMVector3LengthEst(XMVectorSubtract(selfPosition, samplePosition)).m128_f32[0] < (sampleScale.m128_f32[0] + selfScale.m128_f32[0]))
				continue;

			auto oc = XMVectorSubtract(selfPosition, samplePosition);
			float a = XMVector3Dot(rayDir, rayDir).m128_f32[0];
			float b = 2.0 * XMVector3Dot(oc, rayDir).m128_f32[0];
			float c = XMVector3Dot(oc, oc).m128_f32[0] - pow(sampleScale.m128_f32[0], 2.0f);
			float discriminant = b * b - 2 * a * c;

			if (discriminant >= 0.0)
			{
				float numerator = -b - sqrt(discriminant);

				if (numerator > 0.0)
				{
					rayLight *= 1 - vFeature->GetLightAbsorbtion();
				}
				else
				{
					numerator = -b + sqrt(discriminant);
					if (numerator > 0.0)
					{
						rayLight *= 1 - vFeature->GetLightAbsorbtion();
					}
				}
			}
		}

		light += rayLight;
		totalPhototropism = XMVectorAdd(totalPhototropism, XMVectorScale(rayDir, rayLight));
	}

	m_photoTropismDirection = XMVector3Normalize(totalPhototropism);

	m_light = (light / LIGHTRAYS) * 0.5f + m_light * 0.5f;

	m_spatialTropism = XMVectorZero();

	for (auto& vFeature : *allFeatures)
	{
		XMMatrixDecompose(&sampleScale, &sampleRotation, &samplePosition, XMMatrixTranspose(vFeature->GetTransposeMatrix()));

		XMVECTOR v = XMVectorSubtract(selfPosition, samplePosition);
		v = XMVectorMultiply(v, { 1,1,1,0 });
		float dist = XMVectorGetX(XMVector3LengthEst(v));
		float factor = pow(2, -dist);

		m_spatialTropism = XMVectorAdd(m_spatialTropism, XMVectorScale(XMVector3Normalize(v), factor));
	}
}

float VegetationFeature::GetLightAbsorbtion()
{
	return 0.3f * m_branchWidth;
}
