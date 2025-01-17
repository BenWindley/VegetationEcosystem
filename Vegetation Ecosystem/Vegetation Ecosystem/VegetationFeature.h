#pragma once
#include <memory>

#include "Content/ShaderStructures.h"
#include "..\Common\DeviceResources.h"

#include "Transform.h"
#include "Cube.h"
#include "VegetationParameters.h"

class VegetationFeature : public Transform
{
public:
	VegetationFeature() = default;
	VegetationFeature(Transform* parent);
	virtual ~VegetationFeature();

public:
	virtual void Start(DX::DeviceResources* deviceResources, Vegetation_Ecosystem::RendererResources* rendererResources, Species species);
	virtual void Update(float growth, float time);
	virtual void Render(Vegetation_Ecosystem::ModelViewProjectionConstantBuffer constantBufferData);

	virtual float GetGrowthFactor();
	virtual float GetLightAbsorbtion();
	virtual void UpdateTropisms(std::vector<VegetationFeature*>* allFeatures);

	bool GetFate();
	bool IsDormant();
	float GetTropismFactor();
	DirectX::XMVECTOR GetTropismDirectionQuaternion();
	float GetBiomass();

	float m_branchWidth = 1.0f;
	bool m_sylleptic = false;
	bool m_terminal = true;

protected:
	DirectX::XMVECTOR GetLookatRotation(DirectX::XMVECTOR directionVector);

	DX::DeviceResources* m_deviceResources;
	Vegetation_Ecosystem::RendererResources* m_rendererResources;

	Cube* m_cube;

	Species m_species;

	int m_id;
	bool m_fate = false;
	bool m_dormant = false;

	float m_light = 1.0f;

	DirectX::XMVECTOR m_photoTropismDirection = { 0, 1, 0 };
};
