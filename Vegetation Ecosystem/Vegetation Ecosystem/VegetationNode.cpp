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

	m_branchWidth = species.m_initialBranchWidth;

	m_cube = new Cube();
	m_cube->m_parent = this;
	m_cube->Init(m_deviceResources, m_rendererResources);

	m_vegetationFeatures.push_back(new VegetationBud(this, true));
	m_vegetationFeatures.back()->SetLocalRotation(DirectX::XMQuaternionRotationRollPitchYaw( 0, 0, 0 ));
	m_vegetationFeatures.back()->Start(m_deviceResources, m_rendererResources, m_species);
	m_vegetationFeatures.back()->m_branchWidth = m_branchWidth;
}

// Used for sub-nodes
void VegetationNode::Start(VegetationNode* parent, Species species, DX::DeviceResources* deviceResources, Vegetation_Ecosystem::RendererResources* rendererResources)
{
	m_treeID = parent->m_treeID;
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
	static std::uniform_real_distribution<float> smallTilt(DirectX::XMConvertToRadians(-m_species.m_randomness), DirectX::XMConvertToRadians(m_species.m_randomness));

	float prolepticChance = m_terminal ? m_species.m_prolepticChance : m_species.m_branchProlepticChance;

	float chance = 1 - powf(prolepticChance + 1, -m_parentNode->m_previousGrowth);
	bool LHS = m_depth % 2;

	if (range(r) < chance)
	{
		bool primary = range(r) > 0.5f;

		m_vegetationFeatures.push_back(new VegetationBud(this, false));
		m_vegetationFeatures.back()->Start(m_deviceResources, m_rendererResources, m_species);
		m_vegetationFeatures.back()->m_branchWidth = m_branchWidth * (primary ? m_species.m_widthMain: m_species.m_widthBranch);
		m_vegetationFeatures.back()->m_terminal = m_terminal;
		m_vegetationFeatures.back()->m_sylleptic = true;

		if (LHS)
			m_vegetationFeatures.back()->SetLocalRotation(DirectX::XMQuaternionRotationRollPitchYawFromVector({ 0, 0, DirectX::XMConvertToRadians(primary ? - m_species.m_spreadMain : - m_species.m_spreadBranch), 0 }));
		else
			m_vegetationFeatures.back()->SetLocalRotation(DirectX::XMQuaternionRotationRollPitchYawFromVector({ DirectX::XMConvertToRadians(primary ? -m_species.m_spreadMain : -m_species.m_spreadBranch), 0, 0 }));

		m_vegetationFeatures.push_back(new VegetationBud(this, true));
		m_vegetationFeatures.back()->Start(m_deviceResources, m_rendererResources, m_species);
		m_vegetationFeatures.back()->m_branchWidth = m_branchWidth * (!primary ? m_species.m_widthMain : m_species.m_widthBranch);
		m_vegetationFeatures.back()->m_terminal = m_terminal;
		m_vegetationFeatures.back()->m_sylleptic = true;

		if (LHS)
			m_vegetationFeatures.back()->SetLocalRotation(DirectX::XMQuaternionRotationRollPitchYawFromVector({ 0, 0, DirectX::XMConvertToRadians(primary ? m_species.m_spreadBranch : m_species.m_spreadMain), 0 }));
		else
			m_vegetationFeatures.back()->SetLocalRotation(DirectX::XMQuaternionRotationRollPitchYawFromVector({ DirectX::XMConvertToRadians(primary ? m_species.m_spreadBranch : m_species.m_spreadMain), 0, 0, 0 }));
	}
	else
	{
		m_vegetationFeatures.push_back(new VegetationBud(this, true));
		m_vegetationFeatures.back()->Start(m_deviceResources, m_rendererResources, m_species);
		m_vegetationFeatures.back()->SetLocalRotation(DirectX::XMQuaternionRotationRollPitchYawFromVector({ smallTilt(r), 0, smallTilt(r) }));
		m_vegetationFeatures.back()->m_branchWidth = m_branchWidth * m_species.m_widthStraight;
		m_vegetationFeatures.back()->m_terminal = m_terminal;
		m_vegetationFeatures.back()->m_sylleptic = false;

		if (m_terminal)
		{
			m_vegetationFeatures.push_back(new VegetationBud(this, true));
			m_vegetationFeatures.back()->Start(m_deviceResources, m_rendererResources, m_species);
			m_vegetationFeatures.back()->m_branchWidth = m_branchWidth * m_species.m_widthSide;
			m_vegetationFeatures.back()->m_terminal = false;
			m_vegetationFeatures.back()->m_sylleptic = false;

			if(((m_depth + m_treeID) % 4) <= 1)
				m_vegetationFeatures.back()->SetLocalRotation(DirectX::XMQuaternionRotationRollPitchYawFromVector({ 0, 0, LHS ? DirectX::XMConvertToRadians(m_species.m_spreadSide) : -DirectX::XMConvertToRadians(m_species.m_spreadSide)}));
			else
				m_vegetationFeatures.back()->SetLocalRotation(DirectX::XMQuaternionRotationRollPitchYawFromVector({ LHS ? DirectX::XMConvertToRadians(m_species.m_spreadSide) : -DirectX::XMConvertToRadians(m_species.m_spreadSide), 0, 0 }));
		}
	}
}

void VegetationNode::Update(float growth, float time)
{
	float totalGrowthFactor = GetGrowthFactor();

	m_previousGrowth = totalGrowthFactor;

	// Append Pass
	for (auto& v : m_vegetationFeatures)
	{
		v->Update(growth * v->GetGrowthFactor() / totalGrowthFactor, time);
	}
	
	for (auto& v : m_childNodes)
	{
		v->Update(growth * v->GetGrowthFactor() / totalGrowthFactor, time);
	}

	if (m_complexity > MAX_COMPLEXITY || m_branchWidth < m_species.m_minBranchWidth)
		return;

	// Shed Pass
	for (size_t i = m_vegetationFeatures.size(); i--; )
	{
		if (m_vegetationFeatures[i]->GetFate())
		{
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

	node->m_complexity = m_complexity + (growthBud.m_sylleptic ? 1 : 0);
	node->m_simplicity = growthBud.m_sylleptic ? 0 : (m_simplicity + 1);
	node->m_terminal = growthBud.m_terminal;
	node->SetBranchWidth(growthBud.m_branchWidth);
	node->Start(this, m_species, m_deviceResources, m_rendererResources);

	const float maxDistance = 7.0f;
	float distance = 7.0f * pow(30.0f, growthBud.m_branchWidth - 1);

	node->SetLocalPosition(DirectX::XMVectorAdd(growthBud.GetLocalPosition(), { 0, distance, 0 }));

	auto rotationQuaternion = growthBud.GetTropismDirectionQuaternion();
	auto budRotationQuaternion = growthBud.GetRotation();

	auto adjustedQuaternion = DirectX::XMQuaternionSlerp(budRotationQuaternion, rotationQuaternion, growthBud.GetTropismFactor());

	node->SetRotation(adjustedQuaternion);

	m_childNodes.push_back(node);
}

void VegetationNode::SetBranchWidth(float branchWidth)
{
	m_branchWidth = branchWidth;
}

bool VegetationNode::GetRemove()
{
	return !IsNodeAlive();
}

float VegetationNode::GetBranchWidth()
{
	return m_branchWidth;
}

float VegetationNode::GetBiomass()
{
	float localBiomass = 0.0f;

	float avgRadius = m_branchWidth + (m_childNodes.size() > 0 ? m_childNodes[0]->GetBranchWidth() : 0);

	localBiomass += 0.33f * std::_Pi * avgRadius * avgRadius * 7.0f * pow(30.0f, m_branchWidth - 1);

	for (auto& vFeature : m_vegetationFeatures)
	{
		localBiomass += vFeature->GetBiomass();
	}

	for (auto& vNode : m_childNodes)
	{
		localBiomass += vNode->GetBiomass();
	}

	return localBiomass;
}

float VegetationNode::GetGrowthFactor()
{
	float totalGrowthFactor = 0.0f;

	for (auto& vFeature : m_vegetationFeatures)
	{
		totalGrowthFactor += vFeature->GetGrowthFactor() * m_branchWidth;
	}

	for (auto& vNode : m_childNodes)
	{
		totalGrowthFactor += vNode->GetGrowthFactor() - vNode->GetLifeCost();
	}

	return totalGrowthFactor;
}

float VegetationNode::GetLifeCost()
{
	float totalLifeCost = 0.0f;

	totalLifeCost += m_branchWidth * UPKEEP_COEFFICIENT;

	for (auto& vNode : m_childNodes)
	{
		totalLifeCost += vNode->GetLifeCost();
	}

	return totalLifeCost;
}

void VegetationNode::GetAllFeatures(std::vector<VegetationFeature*>* allFeatures)
{
	for (auto& vFeature : m_vegetationFeatures)
	{
		if(!vFeature->IsDormant())
			allFeatures->push_back(vFeature);
	}

	for (auto& vNode : m_childNodes)
	{
		vNode->GetAllFeatures(allFeatures);
	}
}

bool VegetationNode::IsNodeAlive()
{
	std::vector<VegetationFeature*> features;

	GetAllFeatures(&features);

	bool anyAlive = false;

	for (auto& f : features)
		if(!f->IsDormant())
			anyAlive = true;

	return anyAlive;
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
