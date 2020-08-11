#include "pch.h"

#include "VegetationNode.h"
#include "VegetationBud.h"

VegetationNode::~VegetationNode()
{
	while (m_vegetationFeatures.size() > 0)
	{
		delete m_vegetationFeatures[m_vegetationFeatures.size() - 1];
		m_vegetationFeatures.pop_back();
	}
}

void VegetationNode::Start(Transform* parent)
{
	m_parent = parent;

	m_vegetationFeatures.push_back(new VegetationBud(this, true));
}

void VegetationNode::Start(VegetationNode* parent)
{
	m_parentNode = parent;
	m_parent = parent;

	m_vegetationFeatures.push_back(new VegetationBud(this, true));
	m_vegetationFeatures.push_back(new VegetationBud(this, false));
}

void VegetationNode::Update(float growth)
{
	return;

	// Append Pass
	for (auto& v : m_vegetationFeatures)
	{
		v->Update();
	}

	for (auto& v : m_childNodes)
	{
		v.Update(growth);
	}

	// Shed Pass
	for (int i = m_vegetationFeatures.size() - 1; i >= 0 ; --i)
	{
		if (m_vegetationFeatures[i]->GetRemove())
		{
			if(m_vegetationFeatures[i]->GetFate())
				CreateNewNode(*m_vegetationFeatures[i]);

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
	for (auto& vFeature : m_vegetationFeatures)
	{
		vFeature->Render();
	}

	for (auto& vNode : m_childNodes)
	{
		vNode.Render();
	}
}

void VegetationNode::CreateNewNode(VegetationFeature growthBud)
{
	m_childNodes.push_back(VegetationNode());

	VegetationNode* node = &m_childNodes.back();

	node->SetLocalPosition(growthBud.GetLocalPosition());
	node->SetLocalRotation(growthBud.GetLocalRotation());

	node->Start(this);
}

bool VegetationNode::GetRemove()
{
	return m_remove;
}
