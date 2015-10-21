/*
*Author: João Pedro Cova Regateiro
*No: 445954
*
*	Module 08966 Games Development Architectures
*	Department of Computer Science
*	UNIVERSITY OF HULL
*/
#include "NodeGroup.h"
#include "Visitor.h"

NodeGroup::NodeGroup() : childs()
{
}

NodeGroup::~NodeGroup()
{
	childs.clear();
}


void NodeGroup::addChild(Node *child)
{
	childs.push_back(child);
}

void NodeGroup::removeChild(Node *node)
{
	childs.erase(std::remove(childs.begin(), childs.end(), node), childs.end());
}

void NodeGroup::accept(Visitor& v)
{
	v.visit(*this);
}

std::vector<Node*>& NodeGroup::getChilds()
{
	return childs;
}