/*
*Author: João Pedro Cova Regateiro
*No: 445954
*
*	Module 08966 Games Development Architectures
*	Department of Computer Science
*	UNIVERSITY OF HULL
*/

/*
* This class gives the possibility to other's class that extend subject to 
* run over all the nodes of the graph, instead of iterate with a vector
* each node of the graph spread the message to their suns
*
*/
#ifndef _VISITOR_HPP
#define _VISITOR_HPP

#include "NodeMatrix.h"
#include "NodeObj.h"
#include "NodeTexture.h"
#include "NodeShader.h"
#include "NodeColour.h"

class Subject;

class Visitor
{
public:
	virtual ~Visitor();

	virtual void visit(NodeMatrix&) = 0;
	virtual void visit(NodeGroup&) = 0;
	virtual void visit(NodeObj&) = 0;
	virtual void visit(NodeTexture&) = 0;
	virtual void visit(NodeShader&) = 0;
	virtual void visit(NodeColour&) = 0;

private:
protected:
	Visitor();
};

#endif