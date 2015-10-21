/*
*Author: João Pedro Cova Regateiro
*No: 445954
*
*	Module 08966 Games Development Architectures
*	Department of Computer Science
*	UNIVERSITY OF HULL
*/
#include "NodeMatrix.h"
#include "Visitor.h"

NodeMatrix::NodeMatrix(const glm::mat4 &matrix) : _matrix(matrix), NodeGroup()
{
}

NodeMatrix::~NodeMatrix()
{
}


glm::mat4 NodeMatrix::getMatrix()
{
	return _matrix;
}

void NodeMatrix::accept(Visitor& v)
{
	v.visit(*this);
}

void NodeMatrix::setMatrix(const glm::mat4 &matrix)
{
	_matrix = matrix;
}


