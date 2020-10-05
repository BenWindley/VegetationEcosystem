#pragma once
#include <vector>
#include <memory>

#include "VegetationFeature.h"
#include "Cube.h"

class VegetationNode : public Transform
{
public:
	VegetationNode() = default;
	virtual ~VegetationNode();

public:
	virtual void Start(Transform* parent, DX::DeviceResources* deviceResources, Vegetation_Ecosystem::RendererResources* rendererResources);
	virtual void Start(VegetationNode* parent, DX::DeviceResources* deviceResources, Vegetation_Ecosystem::RendererResources* rendererResources);
	virtual void Update(float growth);
	virtual void Render(Vegetation_Ecosystem::ModelViewProjectionConstantBuffer constantBufferData);

	void CreateNewNode(VegetationFeature growthBud);

	bool GetRemove();

	int m_depth = 0;

private:
	VegetationNode* m_parentNode;
	std::vector<VegetationFeature*>	m_vegetationFeatures;
	std::vector<VegetationNode*> m_childNodes;

	DX::DeviceResources* m_deviceResources;
	Vegetation_Ecosystem::RendererResources* m_rendererResources;

	Cube* m_cube;

	bool m_remove = false;
};
