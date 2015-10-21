/*
*Author: João Pedro Cova Regateiro
*No: 445954
*
*	Module 08966 Games Development Architectures
*	Department of Computer Science
*	UNIVERSITY OF HULL
*/

/*
* Class that contains the matrix associated to a certain object
*
*/



#ifndef _NODEMATRIX_HPP
#define _NODEMATRIX_HPP

#include "NodeGroup.h"

// Include GLM
#include <glm\glm.hpp>


using namespace std;

class NodeMatrix : public NodeGroup
{
public:
	NodeMatrix(const glm::mat4 &);
	~NodeMatrix();

	glm::mat4 getMatrix();
	void setMatrix(const glm::mat4 &);
	virtual void accept(Visitor&);

private:
	glm::mat4 _matrix;
};

#endif