#include "pch.h"

#include "VegetationNode.h"
#include "VegetationBud.h"

void VegetationNode::Start(Transform* parent)
{
	m_parent = parent;

	m_vegetationFeatures.push_back(VegetationBud(this, true));
}

void VegetationNode::Start(VegetationNode* parent)
{
	m_parentNode = parent;
	m_parent = parent;

	m_vegetationFeatures.push_back(VegetationBud(this, true));
	m_vegetationFeatures.push_back(VegetationBud(this, false));
}

void VegetationNode::Update(float growth)
{
	// Append Pass
	for (auto& v : m_vegetationFeatures)
	{
		v.Update();
	}

	for (auto& v : m_childNodes)
	{
		v.Update(growth);
	}

	// Shed Pass
	for (int i = m_vegetationFeatures.size() - 1; i >= 0 ; --i)
	{
		if (m_vegetationFeatures[i].GetRemove())
		{
			if(m_vegetationFeatures[i].GetFate())
				CreateNewNode(m_vegetationFeatures[i]);

			m_vegetationFeatures.erase(m_vegetationFeatures.begin() + i);
		}
	}

	for (int i = m_childNodes.size() - 1; i >= 0; --i)
	{
		if (m_childNodes[i].GetRemove())
			m_childNodes.erase(m_childNodes.begin() + i);
	}
}

void VegetationNode::Render()
{

}

void VegetationNode::CreateNewNode(VegetationFeature growthBud)
{
	m_childNodes.push_back(VegetationNode());

	VegetationNode* node = &m_childNodes.back();

	node->SetLocalPosition(growthBud.GetLocalPosition());
	node->SetLocalRotation(growthBud.GetLocalRotation());
}

bool VegetationNode::GetRemove()
{
	return m_remove;
}
