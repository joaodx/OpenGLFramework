/*
*Author: João Pedro Cova Regateiro
*No: 445954
*
*	Module 08966 Games Development Architectures
*	Department of Computer Science
*	UNIVERSITY OF HULL
*/
#ifndef _NODEGROUP_HPP
#define _NODEGROUP_HPP

#include <algorithm>
#include "Node.h"

class NodeGroup : public Node
{

public:
	NodeGroup();
	~NodeGroup();

	void addChild(Node *child);
	void removeChild(Node *node);
	std::vector<Node*>& getChilds();

	virtual void accept(Visitor&);

private:		
	

protected:
	std::vector<Node*> childs;


};

#endif