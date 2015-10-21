#pragma once

#undef min
#undef max

#include <string>
#include <cmath>
#include <cstdarg>
#include <limits>
#include <sstream>
// find library for vector matrizes etc...
#include <glm/glm.hpp>




using namespace std;

namespace utils
{
	const float PI = 3.14159265359f;
	const float TAU = 6.28318530718f;
	const float EPSILON = 0.0001f;

	void errorQuit(wstring error);

	unsigned short reverse(unsigned short num);
	unsigned reverse(unsigned num);

	inline float toRadians(float deg) { return (deg * PI) / 180.0f; }
	inline float toDegrees(float rad) { return (180.0f * rad) / PI; }

	bool inline fequals(float lhs, float rhs) { return fabs(lhs - rhs) < EPSILON; }

	template<typename T> string toString(T val);
	template<typename T> wstring toWString(T val);

	template<typename T> T maximum(unsigned count, ...);
	template<typename T> T minimum(unsigned count, ...);
	template<typename T> T clamp(T value, T lbound, T ubound);

	
	glm::vec3 rotatePoint(float angle, const glm::vec3 &point);
};

template<typename T>
string utils::toString(T val)
{
	stringstream ss;
	ss << val;
	return ss.str();
}

template<typename T>
wstring utils::toWString(T val)
{
	wstringstream ss;
	ss << val;
	return ss.str();
}

template<typename T>
T utils::maximum(unsigned count, ...)
{
	va_list args;
    va_start(args, count);
	T output(numeric_limits<T>::min());

	for (unsigned c = 0; c < count; ++c)
	{
		 T val = va_arg(args, T);
		 if (val > output)
			 output = val;
	}

	va_end(args);
	return output;
}

template<typename T>
T utils::minimum(unsigned count, ...)
{
	va_list args;
    va_start(args, count);
	T output(numeric_limits<T>::max());

	for (unsigned c = 0; c < count; ++c)
	{
		 T val = va_arg(args, T);
		 if (val < output)
			 output = val;
	}

	va_end(args);
	return output;
}

template<typename T>
T utils::clamp(T value, T lbound, T ubound)
{
	if (value > ubound)
		return ubound;
	if (value < lbound)
		return lbound;
	return value;
}
