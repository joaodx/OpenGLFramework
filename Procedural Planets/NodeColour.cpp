#include "NodeColour.h"
#include "Visitor.h"

NodeColour::NodeColour(const glm::vec3 &colour)
	: m_colour(colour)
{
}

NodeColour::~NodeColour()
{
}

void NodeColour::accept(Visitor &v)
{
	v.visit(*this);
}

void NodeColour::setColour(float red, float green, float blue)
{
	m_colour.x = red;
	m_colour.y = green;
	m_colour.z = blue;
}
