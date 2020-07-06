#pragma once
#include <vector>

#include "VegetationNode.h"
#include "VegetationParameters.h"

class Vegetation : public Transform
{
public:
	Vegetation(Species species);
	virtual ~Vegetation();

public:
	void Start();
	void Update(float time);
	void Render();

private:
	VegetationNode* m_vegetationNode;

	float m_growth = 0.0f;
	Species m_species;
};
