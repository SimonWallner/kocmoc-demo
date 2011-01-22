#ifndef ANIMATION_SYSTEM_HPP
#define ANIMATION_SYSTEM_HPP

#include "common.hpp"
#include <map>
#include <vector>

namespace kocmoc
{

	typedef std::pair<double, double > ScalarPair;
	typedef std::pair<double, vec3 > VecPair;

	typedef std::vector<ScalarPair> ScalarValues;
	typedef std::vector<VecPair> VecValues;

	typedef std::map<std::string, ScalarValues > ScalarMap;
	typedef std::map<std::string, VecValues > VecMap;

	/**
	 * singleton animation system class. parses animation files and
	 * interpolates named properties
	 */
	class AnimationSystem
	{
	public:

		static AnimationSystem& getInstance(void);
		~AnimationSystem(void);

		bool parseAnimationFile(void);

		/**
		 * Get the interpolated value at the given time with the given name
		 */
		float getScalar(double time, std::string name);
		vec3 getVec3(double time, std::string name);

		/**
		 * reload the animation file and do all neccessary housekeeeping
		 */
		void reload(void);

	private:
		AnimationSystem(void);
		AnimationSystem(const AnimationSystem &cc);

		std::string fullPath;

		bool isspacesonly(const string& line);
		void getnextline(std::istream& is, std::string& line);

		/** two maps one for lists of scalar values, one for vector values */
		ScalarMap scalarMap;
		VecMap vecMap;

		unsigned int binarySearch(ScalarValues values, float needle, unsigned int lower, unsigned int upper);
	};
}


#endif