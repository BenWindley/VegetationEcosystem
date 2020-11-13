#include "pch.h"
#include "VegetationParameters.h"

#include <iostream>
#include <fstream>

Species::Species(std::string name)
{
	std::ifstream file(name + ".txt");
	std::string value;
	std::string arg;

	while (std::getline(file, value))
	{
		std::getline(file, arg);

		if (value == "Proleptic Chance")
			m_prolepticChance = std::stof(arg);
		else if (value == "Branch Proleptic Chance")
			m_branchProlepticChance = std::stof(arg);
		else if (value == "Growth Rate")
			m_growthRate = std::stof(arg);
		else if (value == "Initial Branch Width")
			m_initialBranchWidth = std::stof(arg);
		else if (value == "Spread Main")
			m_spreadMain = std::stof(arg);
		else if (value == "Spread Branch")
			m_spreadBranch = std::stof(arg);
		else if (value == "Spread Side")
			m_spreadSide = std::stof(arg);
		else if (value == "Randomness")
			m_randomness = std::stof(arg);
		else if (value == "Width Straight")
			m_widthStraight = std::stof(arg);
		else if (value == "Width Main")
			m_widthMain = std::stof(arg);
		else if (value == "Width Branch")
			m_widthBranch = std::stof(arg);
		else if (value == "Width Side")
			m_widthSide = std::stof(arg);
		else if (value == "Min Branch Width")
			m_minBranchWidth = std::stof(arg);
		else if (value == "Tropism Factor")
			m_tropismFactor = std::stof(arg);
		else if (value == "Phototropism Factor")
			m_photoTropismFactor = std::stof(arg);
		else if (value == "Spatialtropism Factor")
			m_spatialTropismFactor = std::stof(arg);
		else if (value == "Gravitropism Factor")
			m_gravitropismFactor = std::stof(arg);
		else if (value == "Min Light Required")
			m_minLightRequired = std::stof(arg);
	}

	file.close();
}
