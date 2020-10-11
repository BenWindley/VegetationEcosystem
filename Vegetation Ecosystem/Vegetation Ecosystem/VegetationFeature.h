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
	virtual ~VegetationFeature() = default;

public:
	virtual void Start(DX::DeviceResources* deviceResources, Vegetation_Ecosystem::RendererResources* rendererResources);
	virtual void Update(float growth);
	virtual void Render(Vegetation_Ecosystem::ModelViewProjectionConstantBuffer constantBufferData);

	bool GetFate();
	bool GetRemove();
	virtual float GetGrowthFactor();
	virtual void UpdateLighting(std::vector<VegetationFeature*>* allFeatures);
	virtual float GetLightAbsorbtion();

protected:
	DX::DeviceResources* m_deviceResources;
	Vegetation_Ecosystem::RendererResources* m_rendererResources;

	//Cube* m_cube;

	bool m_fate = false;
	bool m_remove = false;

	float m_light = 0.0f;

	int id;
};
