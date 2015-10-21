/*
*Author: João Pedro Cova Regateiro
*No: 445954
*
*	Module 08966 Games Development Architectures
*	Department of Computer Science
*	UNIVERSITY OF HULL
*/

/*
* This class represents a node of the graph scene, that contains the index and the name of the node
*
*
*
*/

#ifndef _NODE_HPP
#define _NODE_HPP

#include "Mesh.h"
#include <string>
#include "Subject.h"

class Node : public Subject
{
public:
	Node();
	
	virtual ~Node();
	bool isVisible() { return visible; };
	void setVisible(bool flag) { visible = flag; };

	virtual void accept(Visitor&) = 0;

private:
	bool visible;
};

#endif
