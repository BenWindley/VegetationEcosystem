#pragma once
#include <vector>
#include <memory>

#include "VegetationParameters.h"
#include "VegetationFeature.h"
#include "Cube.h"
#include "CylinderSegment.h"

class VegetationNode : public Transform
{
public:
	VegetationNode() = default;
	virtual ~VegetationNode();

public:
	virtual void Start(Transform* parent, Species species, DX::DeviceResources* deviceResources, Vegetation_Ecosystem::RendererResources* rendererResources);
	virtual void Start(VegetationNode* parent, Species species, DX::DeviceResources* deviceResources, Vegetation_Ecosystem::RendererResources* rendererResources);
	virtual void Update(float growth);
	virtual void Render(Vegetation_Ecosystem::ModelViewProjectionConstantBuffer constantBufferData);

	void CreateNewNode(VegetationFeature growthBud);

	void SetBranchWidth(float branchWidth);

	bool GetRemove();
	float GetBranchWidth();
	float GetGrowthFactor();
	void GetFeatures(std::vector<VegetationFeature*>* allFeatures);

	int m_depth = 0;
	CylinderSegment* m_cylinder;

private:
	VegetationNode* m_parentNode;
	std::vector<VegetationFeature*>	m_vegetationFeatures;
	std::vector<VegetationNode*> m_childNodes;

	DX::DeviceResources* m_deviceResources;
	Vegetation_Ecosystem::RendererResources* m_rendererResources;

	Cube* m_cube;
	Species m_species;

	bool m_remove = false;
	float m_branchWidth;
};
