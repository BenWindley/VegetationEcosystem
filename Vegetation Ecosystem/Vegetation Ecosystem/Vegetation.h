#pragma once
#include <vector>

#include "VegetationNode.h"
#include "VegetationParameters.h"

class Vegetation : public Transform
{
public:
	Vegetation(Species species);
	virtual ~Vegetation();

public:
	void Start(DX::DeviceResources* deviceResources, Vegetation_Ecosystem::RendererResources* rendererResources);
	void Update(float time);
	void Render(Vegetation_Ecosystem::ModelViewProjectionConstantBuffer constantBufferData);

private:
	VegetationNode* m_vegetationNode;

	float m_age = 0.0f;
	float m_growth = 0.0f;
	Species m_species;
};
