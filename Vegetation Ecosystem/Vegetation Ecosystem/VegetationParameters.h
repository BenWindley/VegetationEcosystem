#pragma once

struct Species
{
public:
	float m_prolepticChance;
	float m_branchProlepticChance;
	float m_growthRate;

	Species()
	{
		m_prolepticChance = 0.5f;
		m_branchProlepticChance = 0.5f;
		m_growthRate = 1.0f;
	}

	Species(float prolepticChance, float growthRate) : m_prolepticChance(prolepticChance), m_growthRate(growthRate) {};
	Species(float prolepticChance, float branchProlepticChance, float growthRate) : m_prolepticChance(prolepticChance), m_branchProlepticChance(branchProlepticChance), m_growthRate(growthRate) {};
};
