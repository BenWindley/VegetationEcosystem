#pragma once

struct Species
{
public:
	float m_syllepticChance;
	float m_growthRate;

	Species()
	{
		m_syllepticChance = 0.5f;
		m_growthRate = 1.0f;
	}

	Species(float syllepticChance, float growthRate) : m_syllepticChance(syllepticChance), m_growthRate(growthRate) {};
};
