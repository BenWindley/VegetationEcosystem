#pragma once
#include <string>

struct Species
{
public:
	// Growth
	float m_prolepticChance;
	float m_branchProlepticChance;
	float m_initialBranchWidth;
	float m_growthRate;
	float m_minBranchWidth;
	float m_minLightRequired;

	// Spatial
	float m_spreadMain;
	float m_spreadBranch;
	float m_spreadSide;
	float m_randomness;

	float m_widthStraight;
	float m_widthMain;
	float m_widthBranch;
	float m_widthSide;
	
	// Tropisms
	float m_tropismFactor;
	float m_photoTropismFactor;
	float m_spatialTropismFactor;
	float m_gravitropismFactor;

	// Visual
	float m_leafSize = 7.0f;

	Species() = default;
	Species(std::string name);
};
