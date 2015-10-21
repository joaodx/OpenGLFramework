/*
*Author: João Pedro Cova Regateiro
*No: 445954
*
*	Module 08966 Games Development Architectures
*	Department of Computer Science
*	UNIVERSITY OF HULL
*/
#include "NodeObj.h"
#include "Visitor.h"

NodeObj::NodeObj(Mesh* const obj)
	: _obj(obj)
{
}

NodeObj::~NodeObj()
{
}

void NodeObj::accept(Visitor& v)
{
	v.visit(*this);
}
