#pragma once
#include <vector>
#include <mutex>

#include "VegetationNode.h"
#include "CylinderSegment.h"
#include "Leaf.h"

class Vegetation : public Transform
{
public:
	Vegetation(Species species);
	virtual ~Vegetation();

public:
	void Start(DX::DeviceResources* deviceResources, Vegetation_Ecosystem::RendererResources* rendererResources);
	void Update(float time);
	void Render(Vegetation_Ecosystem::ModelViewProjectionConstantBuffer constantBufferData, bool renderLeaves = true);

	bool IsComplete(); 
	float GetProgress();
	void UpdateAllFeatures(std::vector<VegetationFeature*>* allFeatures);
	void SetAllFeatures(std::vector<VegetationFeature*> allFeatures);
	void UpdateLight(float time);

	float m_age = 0.0f;

private:
	void JobQueueThread();
	void BuildModel(VegetationNode* node, CylinderSegment* previous);

	VegetationNode* m_vegetationNode;

	float m_growth = 0.0f;
	Species m_species;

	DX::DeviceResources* m_deviceResources;
	Vegetation_Ecosystem::RendererResources* m_rendererResources;

	bool m_keepThreads = true;
	std::atomic<bool> m_updateThreads;
	std::atomic<int> m_threadCount;
	std::atomic<int> m_threadIterator;
	std::atomic<int> m_threadsComplete;
	std::vector<std::thread> m_doThreads;
	std::vector<VegetationFeature*> m_allFeatures;
	std::vector<VegetationFeature*> m_allSelfFeatures;

	std::vector<CylinderSegment*> m_core;
	std::vector<Leaf*> m_leaves;
};
