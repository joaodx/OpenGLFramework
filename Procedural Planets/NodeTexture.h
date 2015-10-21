#pragma once

#include "NodeGroup.h"

class NodeTexture : public NodeGroup
{
	unsigned m_texture;

public:
	NodeTexture(unsigned texture);
	~NodeTexture();

	void setTexture(unsigned texture) { m_texture = texture; }
	unsigned getTexture() { return m_texture; }

	virtual void accept(Visitor &v);
};

