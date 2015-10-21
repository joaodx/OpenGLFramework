/*
*Author: João Pedro Cova Regateiro
*No: 445954
*
*	Module 08966 Games Development Architectures
*	Department of Computer Science
*	UNIVERSITY OF HULL
*/

/*
*	Class that represents a physic object, this mean that this class contains the geometric class of the object,
*	with this class its possible to abstract to diferente type os objects, for example dynamic object and static objects.
*	Although, also represents a node in the graph.
*/

#include "Node.h"
#include <string>

#ifndef _NODEOBJ_HPP
#define _NODEOBJ_HPP

class NodeObj : public Node
{
public:
	NodeObj(Mesh* const );
	~NodeObj();

	virtual void accept(Visitor&);

	Mesh *getMesh() { return _obj; }

private:
	Mesh *_obj;
};

#endif