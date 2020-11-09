#pragma once

struct Species
{
public:
	float m_prolepticChance;
	float m_growthRate;

	Species()
	{
		m_prolepticChance = 0.5f;
		m_growthRate = 1.0f;
	}

	Species(float syllepticChance, float growthRate) : m_prolepticChance(syllepticChance), m_growthRate(growthRate) {};
};
