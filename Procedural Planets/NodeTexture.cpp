#include "NodeTexture.h"
#include "Visitor.h"

NodeTexture::NodeTexture(unsigned texture)
	: m_texture(texture)
{
}

NodeTexture::~NodeTexture()
{
}

void NodeTexture::accept(Visitor &v)
{
	v.visit(*this);
}
