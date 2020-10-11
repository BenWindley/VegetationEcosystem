#include "pch.h"
#include "VegetationFeature.h"
#include "VegetationNode.h"

VegetationFeature::VegetationFeature(Transform* parent)
{
	m_parent = parent;

	static int totalIDs = 0;
	id = totalIDs++;
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

float VegetationFeature::GetGrowthFactor()
{
	return m_light;
}

void VegetationFeature::UpdateLighting(std::vector<VegetationFeature*>* allFeatures)
{
	float light = 1.0f;

	DirectX::XMVECTOR selfScale;
	DirectX::XMVECTOR selfPosition;
	DirectX::XMVECTOR selfRotation;
	DirectX::XMMatrixDecompose(&selfScale, &selfRotation, &selfPosition, DirectX::XMMatrixTranspose(GetTransposeMatrix()));
	selfPosition = DirectX::XMVector3Rotate(selfPosition, selfRotation);

	DirectX::XMVECTOR sampleScale;
	DirectX::XMVECTOR samplePosition;
	DirectX::XMVECTOR sampleRotation;

	for (auto& vFeature : *allFeatures)
	{
		if (vFeature->id == id)
			continue;
		if (light == 0.0f)
			break;

		DirectX::XMMatrixDecompose(&sampleScale, &sampleRotation, &samplePosition, DirectX::XMMatrixTranspose(vFeature->GetTransposeMatrix()));
		samplePosition = DirectX::XMVector3Rotate(samplePosition, sampleRotation);

		auto oc = DirectX::XMVectorSubtract(selfPosition, samplePosition);
		float a = DirectX::XMVectorGetX(DirectX::XMVector3Dot({ 0, 1, 0 }, { 0, 1, 0 }));
		float b = 2.0 * DirectX::XMVectorGetX(DirectX::XMVector3Dot(oc, { 0, 1, 0 }));
		float c = DirectX::XMVectorGetX(DirectX::XMVector3Dot(oc, oc)) - 4 * DirectX::XMVectorGetX(sampleScale) * DirectX::XMVectorGetX(sampleScale);
		float discriminant = b * b - 4 * a * c;

		if (discriminant >= 0.0)
		{
			float numerator = -b - sqrt(discriminant);

			if (numerator > 0.0)
			{
				light *= vFeature->GetLightAbsorbtion();
				continue;
			}

			numerator = -b + sqrt(discriminant);
			if (numerator > 0.0)
			{
				light *= vFeature->GetLightAbsorbtion();
			}
		}
	}

	m_light = light;
}

float VegetationFeature::GetLightAbsorbtion()
{
	return 0.5f;
}
