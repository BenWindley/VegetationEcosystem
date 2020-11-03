#pragma once
#include "VegetationFeature.h"

class VegetationBud : public VegetationFeature
{
public:
	VegetationBud(Transform* parent, bool terminalBud);
	~VegetationBud() = default;

public:
	void Start(DX::DeviceResources* deviceResources, Vegetation_Ecosystem::RendererResources* rendererResources) override;
	void Update(float growth, float time) override;
	void Render(Vegetation_Ecosystem::ModelViewProjectionConstantBuffer constantBufferData) override;

private:
	bool m_terminalBud;
	float m_storedGrowth;
	float m_age;
};
