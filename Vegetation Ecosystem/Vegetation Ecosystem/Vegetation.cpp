#include "pch.h"
#include "Vegetation.h"

Vegetation::Vegetation(Species species)
{
	m_species = species;
}

Vegetation::~Vegetation()
{
	delete m_vegetationNode;
}

void Vegetation::Start()
{
	m_vegetationNode = new VegetationNode();
	m_vegetationNode->Start(this);
}

void Vegetation::Update(float time)
{
	m_growth += time * m_species.m_growthRate;

	m_vegetationNode->Update(m_growth);

	m_growth = 0;
}

void Vegetation::Render()
{

}
