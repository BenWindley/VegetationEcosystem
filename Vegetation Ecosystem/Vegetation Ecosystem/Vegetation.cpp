#include "pch.h"
#include "Vegetation.h"

#include <thread>
#include <random>

Vegetation::Vegetation(Species species)
{
	m_species = species;
}

Vegetation::~Vegetation()
{
	m_keepThreads = false;

	while (m_threadCount > 0)
	{
		Sleep(0.1);
	}

	delete m_vegetationNode;
}

void Vegetation::Start(DX::DeviceResources* deviceResources, Vegetation_Ecosystem::RendererResources* rendererResources)
{
	m_deviceResources = deviceResources;
	m_rendererResources = rendererResources;

	m_vegetationNode = new VegetationNode();
	m_vegetationNode->Start(this, m_species, deviceResources, rendererResources);

	for (int i = 0; i < MAX_THREADS; ++i)
	{
		m_doThreads.push_back(std::thread(&Vegetation::DoThread, this));
		m_doThreads.back().detach();
		m_threadCount++;
	}
}

void Vegetation::Update(float time)
{
	if (!this) return;

	time *= 5;

	if (m_age > MAX_AGE) 
	{
		for (auto& c : m_core)
		{
			c->SetRotation(GetRotation());
		}
		for (auto& c : m_leaves)
		{
			c->SetRotation(GetRotation());
		}

		if (m_keepThreads)
		{
			// Build plant structure

			BuildModel(m_vegetationNode, nullptr);
			m_keepThreads = false;
		}

		return;
	}

	// Update features list

	m_vegetationNode->GetAllFeatures(&m_allFeatures);
	m_threadIterator = 0;
	m_threadsComplete = 0;

	m_updateThreads = true;

	float compTime = 0.0f;

	while (m_updateThreads)
	{
		compTime += 0.001f;
		Sleep(1);
	} 

	m_allFeatures.clear();
	m_age += time + compTime;

	// Update growth rate
	m_growth += time * m_species.m_growthRate * m_vegetationNode->GetGrowthFactor() * 0.9f;

	// Pass growth down list of nodes
	m_vegetationNode->Update(m_growth);

	// This can be changed if some growth is stored to prevent sudden changes of growth
	m_growth *= 0.1f;
}

void Vegetation::Render(Vegetation_Ecosystem::ModelViewProjectionConstantBuffer constantBufferData)
{
	if (!this) return;

	if(!IsComplete())
		m_vegetationNode->Render(constantBufferData);
	else
	{
		for (auto& c : m_core)
		{
			c->Render(constantBufferData);
		}
		for (auto& c : m_leaves)
		{
			c->Render(constantBufferData);
		}
	}
}

bool Vegetation::IsComplete()
{
	return m_age > MAX_AGE;
}

void Vegetation::DoThread()
{
	VegetationFeature* currentFeature;
	int i;

	while (m_keepThreads)
	{
		if (m_updateThreads)
		{
			i = m_threadIterator++;

			if (i >= m_allFeatures.size())
			{
				while (m_threadsComplete < m_allFeatures.size())
				{
					Sleep(0.01);
				}

				m_updateThreads = false;

				continue;
			}
			else
			{
				currentFeature = m_allFeatures[i];
				currentFeature->UpdateTropisms(&m_allFeatures);
				m_threadsComplete++;
			}
		}
		else
		{
			Sleep(0.05f);
		}
	}

	m_threadCount--;
}

void Vegetation::BuildModel(VegetationNode* node, CylinderSegment* previous)
{
	std::random_device r;
	std::random_device _r;
	std::uniform_real_distribution<float> rotation(0, DirectX::XM_2PI);
	std::uniform_real_distribution<float> offset(-LEAF_DISPLACEMENT * node->GetBranchWidth(), LEAF_DISPLACEMENT * node->GetBranchWidth());
	Leaf* l;

	for (auto& f : node->GetFeatures())
	{
		for (int i = 0; i < LEAF_QUANTITY * node->GetBranchWidth(); ++i)
		{
			l = new Leaf();

			l->Init(
				m_deviceResources,
				m_rendererResources,
				node,
				DirectX::XMQuaternionRotationRollPitchYaw(rotation(r), rotation(r), rotation(r)),
				{ offset(r), offset(r), offset(r) }
			);

			m_leaves.push_back(l);
		}
	}
	for (auto& v : node->GetChildren())
	{
		CylinderSegment* c = new CylinderSegment();

		c->Init(
			m_deviceResources,
			m_rendererResources, 
			node->GetBranchWidth(), 
			previous, 
			node, 
			v->GetChildren().size() == 0
		);

		m_core.push_back(c);

		BuildModel(v, c);
	}
}
