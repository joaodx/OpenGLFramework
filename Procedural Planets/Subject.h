/*
*Author: João Pedro Cova Regateiro
*No: 445954
*
*	Module 08966 Games Development Architectures
*	Department of Computer Science
*	UNIVERSITY OF HULL
*/


/*
*	Class that say's iterate trought the graph. Start saying that it wants to visit the root
*	so the root spread his messange to all his childs and so on. Easly interate all nodes in the graph
*
*
*/
#ifndef _SUBJECT_HPP
#define _SUBJECT_HPP


class Visitor;

class Subject
{
public:
	virtual ~Subject();

	virtual void accept(Visitor&) = 0;
private:
protected:
	Subject();
};

#endif