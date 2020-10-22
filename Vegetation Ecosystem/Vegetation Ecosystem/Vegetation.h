#pragma once
#include <vector>
#include <mutex>

#include "VegetationNode.h"

class Vegetation : public Transform
{
public:
	Vegetation(Species species);
	virtual ~Vegetation();

public:
	void Start(DX::DeviceResources* deviceResources, Vegetation_Ecosystem::RendererResources* rendererResources);
	void Update(float time);
	void Render(Vegetation_Ecosystem::ModelViewProjectionConstantBuffer constantBufferData);

	bool IsComplete(); 

private:
	void DoThread();

	VegetationNode* m_vegetationNode;

	float m_age = 0.0f;
	float m_growth = 0.0f;
	Species m_species;

	std::vector<std::thread> m_doThreads;
	std::vector<VegetationFeature*> m_updateFeatures;
	std::vector<VegetationFeature*> m_allFeatures;
	std::mutex m_mutex;
	bool m_keepThreads = true;
	std::atomic<bool> m_updateThreads;
	std::atomic<int> m_threadCount;
	std::atomic<int> m_threadIterator;
	std::atomic<int> m_threadsComplete;
};
