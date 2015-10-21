#pragma once

#include "NodeGroup.h"

class NodeColour : public NodeGroup
{
	glm::vec3 m_colour;

public:
	NodeColour(const glm::vec3 &colour);
	~NodeColour();

	glm::vec3 getColour() { return m_colour; }

	virtual void accept(Visitor &v);

	void setColour(float red, float green, float blue);
};

