#include "pch.h"
#include "VegetationFeature.h"
#include "VegetationNode.h"

using namespace DirectX;

VegetationFeature::VegetationFeature(Transform* parent)
{
	m_parent = parent;

	static int totalIDs = 0;
	m_id = totalIDs++;
}

void VegetationFeature::Start(DX::DeviceResources* deviceResources, Vegetation_Ecosystem::RendererResources* rendererResources)
{
	m_deviceResources = deviceResources;
	m_rendererResources = rendererResources;

	//m_cube = new Cube();
	//m_cube->Init(m_deviceResources, m_rendererResources);
	//m_cube->m_parent = this;
}

void VegetationFeature::Update(float growth)
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

bool VegetationFeature::GetRemove()
{
	return m_remove;
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
	auto photoTropism = XMVectorScale(m_photoTropismDirection, PHOTOTROPISM_FACTOR);
	auto graviTropism = XMVectorScale(m_photoTropismDirection, GRAVITROPISM_FACTOR);
	auto spatialTropism = XMVectorScale(m_photoTropismDirection, SPATIALTROPISM_FACTOR);

	auto sum = XMVectorScale(XMVectorAdd(XMVectorAdd(photoTropism, graviTropism), spatialTropism), 1 / (PHOTOTROPISM_FACTOR + GRAVITROPISM_FACTOR + SPATIALTROPISM_FACTOR));

	return GetLookatRotation(sum);
}

DirectX::XMVECTOR VegetationFeature::GetLookatRotation(DirectX::XMVECTOR directionVector)
{
	XMVECTOR sampleScale;
	XMVECTOR samplePosition;
	XMVECTOR sampleRotation;

	static const XMVECTOR pos = { 0.0f, 0.0f, 0.0f, 0.0f };
	static const XMVECTOR at = directionVector;
	static const XMVECTOR up = { 0.0f, 1.0f, 0.0f, 0.0f };

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
	float light = 1.0f;

	XMVECTOR selfScale;
	XMVECTOR selfPosition;
	XMVECTOR selfRotation;

	XMMatrixDecompose(&selfScale, &selfRotation, &selfPosition, XMMatrixTranspose(GetTransposeMatrix()));
	selfPosition = XMVector3Rotate(selfPosition, selfRotation);

	XMVECTOR sampleScale;
	XMVECTOR samplePosition;
	XMVECTOR sampleRotation;

	XMVECTOR totalPhototropism = { 0, 0, 0 };

	for (int x = -1; x <= 1; ++x)
	{
		for (int z = -1; z <= 1; ++z)
		{
			float rayLight = 1.0f;
			XMVECTOR rayDir = XMVector3Normalize({ (float)x, 1.0f, (float)z });

			for (auto& vFeature : *allFeatures)
			{
				XMMatrixDecompose(&sampleScale, &sampleRotation, &samplePosition, XMMatrixTranspose(vFeature->GetTransposeMatrix()));
				samplePosition = XMVector3Rotate(samplePosition, sampleRotation);
				
				if (vFeature->m_id == m_id)
					continue;
				if (light == 0.0f)
					break;
				if (XMVector3Equal(selfPosition, samplePosition))
					break;

				auto oc = XMVectorSubtract(selfPosition, samplePosition);
				float a = XMVectorGetX(XMVector3Dot(rayDir, rayDir));
				float b = 2.0 * XMVectorGetX(XMVector3Dot(oc, rayDir));
				float c = XMVectorGetX(XMVector3Dot(oc, oc)) - 2 * XMVectorGetX(sampleScale) * XMVectorGetX(sampleScale);
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
			totalPhototropism = XMVectorAdd(totalPhototropism, XMVectorScale(rayDir, rayLight / 9.0f));
		}
	}

	m_photoTropismDirection = totalPhototropism;

	m_light = light / 9;

	m_spatialTropism = { 0, 0, 0 };

	for (auto& vFeature : *allFeatures)
	{
		XMMatrixDecompose(&sampleScale, &sampleRotation, &samplePosition, XMMatrixTranspose(vFeature->GetTransposeMatrix()));

		XMVECTOR v = XMVectorSubtract(selfPosition, samplePosition);
		v = XMVectorMultiply(v, { 1,1,1,0 });
		float dist = XMVectorGetX(XMVector3Length(v));
		float factor = pow(SPATIAL_FALLOFF, -dist);

		m_spatialTropism = XMVectorAdd(m_spatialTropism, XMVectorScale(XMVector3Normalize(v), factor));
	}
}

float VegetationFeature::GetLightAbsorbtion()
{
	return 0.7f;
}
