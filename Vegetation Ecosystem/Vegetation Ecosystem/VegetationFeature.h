#pragma once
#include <memory>

#include "Content/ShaderStructures.h"
#include "..\Common\DeviceResources.h"

#include "Transform.h"
#include "Cube.h"

class VegetationFeature : public Transform
{
public:
	VegetationFeature() = default;
	VegetationFeature(Transform* parent);
	virtual ~VegetationFeature();

public:
	virtual void Start(DX::DeviceResources* deviceResources, Vegetation_Ecosystem::RendererResources* rendererResources);
	virtual void Update(float growth);
	virtual void Render(Vegetation_Ecosystem::ModelViewProjectionConstantBuffer constantBufferData);

	virtual float GetGrowthFactor();
	virtual float GetLightAbsorbtion();
	virtual void UpdateTropisms(std::vector<VegetationFeature*>* allFeatures);

	bool GetFate();
	bool GetRemove();
	float GetTropismFactor();
	DirectX::XMVECTOR GetSpatialTropism();
	DirectX::XMVECTOR GetTropismDirectionQuaternion();

	float m_branchWidth = 1.0f;

protected:
	DirectX::XMVECTOR GetLookatRotation(DirectX::XMVECTOR directionVector);

	DX::DeviceResources* m_deviceResources;
	Vegetation_Ecosystem::RendererResources* m_rendererResources;

	Cube* m_cube;

	int m_id;
	bool m_fate = false;
	bool m_remove = false;
	float m_light = 0.0f;

	DirectX::XMVECTOR m_photoTropismDirection = { 0, 1, 0 };
	DirectX::XMVECTOR m_spatialTropism = { 0, 0, 0 };
};
