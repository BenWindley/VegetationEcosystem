#include "pch.h"

#include "VegetationNode.h"
#include "VegetationBud.h"

#include <random>

 
VegetationNode::~VegetationNode()
{
	while (m_vegetationFeatures.size() > 0)
	{
		delete m_vegetationFeatures[m_vegetationFeatures.size() - 1];
		m_vegetationFeatures.pop_back();
	}
}

// Used for first node
void VegetationNode::Start(Transform* parent, Species species, DX::DeviceResources* deviceResources, Vegetation_Ecosystem::RendererResources* rendererResources)
{
	m_parent = parent;
	m_species = species;

	m_deviceResources = deviceResources;
	m_rendererResources = rendererResources;

	m_branchWidth = 1.0f;

	m_cube = new Cube();
	m_cube->m_parent = this;
	m_cube->Init(m_deviceResources, m_rendererResources);

	m_vegetationFeatures.push_back(new VegetationBud(this, true));
	m_vegetationFeatures.back()->SetLocalRotation(DirectX::XMQuaternionRotationRollPitchYaw( 0, 0, 0 ));
	m_vegetationFeatures.back()->Start(m_deviceResources, m_rendererResources);
	m_vegetationFeatures.back()->m_branchWidth = m_branchWidth;
}

// Used for sub-nodes
void VegetationNode::Start(VegetationNode* parent, Species species, DX::DeviceResources* deviceResources, Vegetation_Ecosystem::RendererResources* rendererResources)
{
	m_parentNode = parent;
	m_parent = parent;
	m_species = species;

	m_deviceResources = deviceResources;
	m_rendererResources = rendererResources;

	m_cube = new Cube();
	m_cube->m_parent = this;
	m_cube->Init(m_deviceResources, m_rendererResources);

	if (parent)
		m_depth = parent->m_depth + 1;
	else
		m_depth = 0;

	static std::random_device r;
	static std::uniform_real_distribution<float> range(0, 1);
	static std::uniform_real_distribution<float> smallTilt(DirectX::XMConvertToRadians(-SPREAD_RANDOM), DirectX::XMConvertToRadians(SPREAD_RANDOM));

	if (range(r) > m_species.m_syllepticChance)
	{
		bool LHS = m_depth % 2;
		bool primary = range(r) > 0.5f;

		m_vegetationFeatures.push_back(new VegetationBud(this, false));
		m_vegetationFeatures.back()->Start(m_deviceResources, m_rendererResources);
		m_vegetationFeatures.back()->m_branchWidth = m_branchWidth * (primary ? WIDTH_MAIN : WIDTH_BRANCH);

		if (LHS)
			m_vegetationFeatures.back()->SetLocalRotation(DirectX::XMQuaternionRotationRollPitchYawFromVector({ 0, 0, DirectX::XMConvertToRadians(primary ? -SPREAD_MAIN : -SPREAD_BRANCH), 0 }));
		else
			m_vegetationFeatures.back()->SetLocalRotation(DirectX::XMQuaternionRotationRollPitchYawFromVector({ DirectX::XMConvertToRadians(primary ? -SPREAD_MAIN : -SPREAD_BRANCH), 0, 0 }));

		m_vegetationFeatures.push_back(new VegetationBud(this, true));
		m_vegetationFeatures.back()->Start(m_deviceResources, m_rendererResources);
		m_vegetationFeatures.back()->m_branchWidth = m_branchWidth * (!primary ? WIDTH_MAIN : WIDTH_BRANCH);

		if (LHS)
			m_vegetationFeatures.back()->SetLocalRotation(DirectX::XMQuaternionRotationRollPitchYawFromVector({ 0, 0, DirectX::XMConvertToRadians(primary ? SPREAD_BRANCH : SPREAD_MAIN), 0 }));
		else
			m_vegetationFeatures.back()->SetLocalRotation(DirectX::XMQuaternionRotationRollPitchYawFromVector({ DirectX::XMConvertToRadians(primary ? SPREAD_BRANCH : SPREAD_MAIN), 0, 0, 0 }));
	}
	else
	{
		m_vegetationFeatures.push_back(new VegetationBud(this, true));
		m_vegetationFeatures.back()->Start(m_deviceResources, m_rendererResources);
		m_vegetationFeatures.back()->SetLocalRotation(DirectX::XMQuaternionRotationRollPitchYawFromVector({ smallTilt(r), 0, smallTilt(r) }));
		m_vegetationFeatures.back()->m_branchWidth = m_branchWidth;
	}
}

void VegetationNode::Update(float growth)
{
	if(m_depth > MAX_DEPTH)
		return;

	float totalGrowthFactor = GetGrowthFactor();

	// Append Pass
	for (auto& v : m_vegetationFeatures)
	{
		v->Update(growth * v->GetGrowthFactor() / totalGrowthFactor);
	}
	
	for (auto& v : m_childNodes)
	{
		v->Update(growth * v->GetGrowthFactor() / totalGrowthFactor);
	}

	// Shed Pass
	for (size_t i = m_vegetationFeatures.size(); i--; )
	{
		if (m_vegetationFeatures[i]->GetRemove())
		{
			if(m_vegetationFeatures[i]->GetFate())
				CreateNewNode(*m_vegetationFeatures[i]);

			delete m_vegetationFeatures[i];
			m_vegetationFeatures.erase(m_vegetationFeatures.begin() + i);
		}
	}

	for (size_t i = m_childNodes.size(); i--; )
	{
		if (m_childNodes[i]->GetRemove())
		{
			delete m_childNodes[i];
			m_childNodes.erase(m_childNodes.begin() + i);
		}
	}
}

void VegetationNode::Render(Vegetation_Ecosystem::ModelViewProjectionConstantBuffer constantBufferData)
{
	for (auto& vFeature : m_vegetationFeatures)
	{
		vFeature->Render(constantBufferData);
	}

	for (auto& vNode : m_childNodes)
	{
		vNode->Render(constantBufferData);
	}

	m_cube->Render(constantBufferData);
}

void VegetationNode::CreateNewNode(VegetationFeature growthBud)
{
	VegetationNode* node = new VegetationNode();

	node->SetBranchWidth(growthBud.m_branchWidth);
	node->Start(this, m_species, m_deviceResources, m_rendererResources);

	node->SetLocalPosition(DirectX::XMVectorAdd(growthBud.GetLocalPosition(), { 0, 7.0f * pow(8.0f, growthBud.m_branchWidth - 1), 0 }));

	auto rotationQuaternion = growthBud.GetTropismDirectionQuaternion();
	auto budRotationQuaternion = growthBud.GetRotation();

	auto adjustedQuaternion = DirectX::XMQuaternionSlerp(budRotationQuaternion, rotationQuaternion, growthBud.GetTropismFactor());

	node->SetRotation(adjustedQuaternion);

	m_childNodes.push_back(node);
}

void VegetationNode::SetBranchWidth(float branchWidth)
{
	{ m_branchWidth = branchWidth; }
}

bool VegetationNode::GetRemove()
{
	return m_remove;
}

float VegetationNode::GetBranchWidth()
{
	return m_branchWidth;
}

float VegetationNode::GetGrowthFactor()
{
	float totalGrowthFactor = 0.0f;

	for (auto& vFeature : m_vegetationFeatures)
	{
		totalGrowthFactor += vFeature->GetGrowthFactor();
	}

	for (auto& vNode : m_childNodes)
	{
		totalGrowthFactor += vNode->GetGrowthFactor();
	}

	return totalGrowthFactor * pow(m_branchWidth, 4);
}

void VegetationNode::GetAllFeatures(std::vector<VegetationFeature*>* allFeatures)
{
	for (auto& vFeature : m_vegetationFeatures)
	{
		allFeatures->push_back(vFeature);
	}

	for (auto& vNode : m_childNodes)
	{
		vNode->GetAllFeatures(allFeatures);
	}
}

VegetationNode* VegetationNode::GetParentNode()
{
	return m_parentNode;
}

std::vector<VegetationNode*> VegetationNode::GetChildren()
{
	return m_childNodes;
}

std::vector<VegetationFeature*> VegetationNode::GetFeatures()
{
	return m_vegetationFeatures;
}
