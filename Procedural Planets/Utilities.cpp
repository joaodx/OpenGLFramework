#include "Utilities.h"

void utils::errorQuit(wstring error)
{
	printf("Error: %ls\n", error.c_str());
	exit(0);
}

unsigned short utils::reverse(unsigned short num)
{
	unsigned char *bytes = reinterpret_cast<unsigned char *>(&num);
	unsigned char output[2];
	output[0] = bytes[1];
	output[1] = bytes[0];
	return *reinterpret_cast<unsigned short *>(output);
}

unsigned utils::reverse(unsigned num)
{
	unsigned char *bytes = reinterpret_cast<unsigned char *>(&num);
	unsigned char output[4];
	output[0] = bytes[3];
	output[1] = bytes[2];
	output[2] = bytes[1];
	output[3] = bytes[0];
	return *reinterpret_cast<unsigned *>(output);
}

glm::vec3 utils::rotatePoint(float angle, const glm::vec3 &point)
{
	float s = sin(utils::toRadians(angle));
	float c = cos(utils::toRadians(angle));

	glm::vec3 output(point);

	output.x = (point.x * c - point.x * s);
	output.y = (point.x * s + point.y * c);

	return output;
}
