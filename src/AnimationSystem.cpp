#include "AnimationSystem.hpp"
#include "Property.hpp"
#include "utility.hpp"

#include <fstream>

using namespace kocmoc;

AnimationSystem::AnimationSystem(void)
{
	fullPath = (std::string)util::Property("scriptsRootFolder") + (std::string)util::Property("animationFileName");
}

AnimationSystem::~AnimationSystem(void)
{
}

AnimationSystem& AnimationSystem::getInstance()
{
	static AnimationSystem instance;
	return instance;
}

bool AnimationSystem::isspacesonly(const string& line)
{
	for (unsigned int i = 0; i < line.length(); ++i)
	{
		if (!isspace(line[i]))
			return false;
	}

	return true;
}

void AnimationSystem::getnextline(std::istream& is, std::string& line) 
{
	while (!is.eof())
	{
		getline(is, line);
		
		if (!isspacesonly(line))
			return;
	}
}


bool AnimationSystem::parseAnimationFile()
{
	std::ifstream file(fullPath.c_str());
	if (!file)
	{
		cout << "failed to load file: " << fullPath << endl;
		return false;
	}

	try
	{
		string line = "";
		string key, value;

		while(!file.eof())
		{
			getnextline(file, line);
			const char* cString = line.c_str();
		
			// skip comment lines
			if (cString[0] == '#' || cString[0] == '!')
				continue;

			std::vector<std::string > tokens;
			util::tokenize(line, tokens, "\t");
			if (tokens.size() == 3)
			{
				std::string name = tokens[1];
				float time = 0.0f;
				sscanf(tokens[0].c_str(), "%f", &time);
				float scalar = 0.0f;
				sscanf(tokens[2].c_str(), "%f", &scalar);

				scalarMap[name].push_back(ScalarPair(time, scalar));

			}

			// TODO parse vec3
		}

	} catch (...) {
		file.close();
		return false;
	}
	//cout << "parsing successful!" << endl;
	file.close();
	return true;
}


float AnimationSystem::getScalar(double time, std::string name)
{
	// get list
	ScalarValues values = scalarMap[name];
	if (values.size() == 0)
		return 0.0f;

	if (values.size() == 1)
		return values[0].second;

	// binary search
	unsigned int lowerIndex = binarySearch(values, time, 0, values.size());

	// interpolate
	if (lowerIndex == values.size() - 1)
		return values[lowerIndex].second;
	else
	{
		float timeA = values[lowerIndex].first;
		float valueA = values[lowerIndex].second;

		float timeB = values[lowerIndex + 1].first;
		float valueB = values[lowerIndex + 1].second;

		if (time < timeA)
			return valueA;

		if (time > timeB)
			return valueB;

		// we are right in the middle

		float t = (timeB - time) / (timeB - timeA);

		return t * valueA + (1 - t) * valueB;
	}
}

unsigned int AnimationSystem::binarySearch(ScalarValues values, float needle, unsigned int lower, unsigned int upper)
{
	if (upper - lower <= 1)
		return lower;

	unsigned int middle = lower + (upper - lower)/2;

	if (needle < values[middle].first) // branch left
		return binarySearch(values, needle, lower, middle);
	else // branch right
		return binarySearch(values, needle, middle, upper);
}

void AnimationSystem::reload()
{
	scalarMap.clear();
	vecMap.clear();
	parseAnimationFile();
}