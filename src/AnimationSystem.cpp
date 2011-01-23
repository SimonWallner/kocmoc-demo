#include "AnimationSystem.hpp"
#include "Property.hpp"
#include "utility.hpp"

#include <fstream>
#include <gtx/spline.hpp>

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
				float time, offset, scalar;

				if (tokens[0].c_str()[0] == '+')
				{
					sscanf(tokens[0].c_str(), "%f", &offset);
					time = scalarLastKeyTime[name] + offset;
				}
				else
					sscanf(tokens[0].c_str(), "%f", &time);

				sscanf(tokens[2].c_str(), "%f", &scalar);

				scalarMap[name].push_back(ScalarPair(time, scalar));
				scalarLastKeyTime[name] = time;

			}
			else if (tokens.size() == 5)
			{
				std::string name = tokens[1];
				float time, offset, v0, v1, v2;

				if (tokens[0].c_str()[0] == '+')
				{
					sscanf(tokens[0].c_str(), "%f", &offset);
					time = vecLastKeyTime[name] + offset;
				}
				else
					sscanf(tokens[0].c_str(), "%f", &time);

				sscanf(tokens[2].c_str(), "%f", &v0);
				sscanf(tokens[3].c_str(), "%f", &v1);
				sscanf(tokens[4].c_str(), "%f", &v2);

				vecMap[name].push_back(VecPair(time, vec3(v0, v1, v2)));
				vecLastKeyTime[name] = time;
			}
			else 
				std::cout << "failed to parse line: '" << line << "'" << std::endl;
			
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

vec3 AnimationSystem::getVec3(double time, std::string name)
{
	// get list
	VecValues values = vecMap[name];
	if (values.size() == 0) // default value for 0 values
		return vec3(0.0f);

	if (values.size() == 1) // constant interpolation for a single value
		return values[0].second;

	// binary search
	unsigned int lowerIndex = binarySearch(values, time, 0, values.size());



	// interpolate
	// time is left or right of all samples
	if ((lowerIndex == 0 && time < values[lowerIndex].first) || lowerIndex == values.size() - 1)
	{
		return values[lowerIndex].second; // constant interpolation
	}
	// only one sample is left or right of time
	else if (lowerIndex == 0 || lowerIndex == values.size() - 2) // linearily interpolate
	{
		float timeA = values[lowerIndex].first;
		vec3 valueA = values[lowerIndex].second;

		float timeB = values[lowerIndex + 1].first;
		vec3 valueB = values[lowerIndex + 1].second;

		float t = (timeB - time) / (timeB - timeA);

		return valueA * t + valueB * (1 - t);
	}

	else // enougth samples on both sides
	{
		// A--B-(time)-C--D
		vec3 valueA = values[lowerIndex -1].second;
		vec3 valueB = values[lowerIndex].second;
		vec3 valueC = values[lowerIndex + 1].second;
		vec3 valueD = values[lowerIndex + 2].second;

		float timeB = values[lowerIndex].first;
		float timeC = values[lowerIndex + 1].first;
		
		float t = (time - timeB) / (timeC - timeB);

		vec3 result = glm::gtx::spline::catmullRom(valueA, valueB, valueC, valueD, t);
		return result;
	}
}

template <class T> unsigned int AnimationSystem::binarySearch(T values, float needle, unsigned int lower, unsigned int upper)
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