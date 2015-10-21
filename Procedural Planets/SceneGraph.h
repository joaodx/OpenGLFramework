#ifndef _SCENEGRAPH_HPP
#define _SCENEGRAPH_HPP


#include "Node.h"
#include "NodeMatrix.h"
#include "NodeObj.h"
#include <vector>

class SceneGraph 
{
public:
	SceneGraph();	
	~SceneGraph();

	void addNode(Node *child);
	void removeNode(Node *node);
	NodeMatrix *getRoot() const;

private:
	NodeMatrix *root;
};
#endif