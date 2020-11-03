#pragma once
#include <vector>
#include <memory>

#include "VegetationParameters.h"
#include "VegetationFeature.h"
#include "Cube.h"

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
	void GetAllFeatures(std::vector<VegetationFeature*>* allFeatures);
	VegetationNode* GetParentNode();
	std::vector<VegetationNode*> GetChildren();
	std::vector<VegetationFeature*> GetFeatures();

	int m_depth = 0;

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
