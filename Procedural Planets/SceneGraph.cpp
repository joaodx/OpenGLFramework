#include "SceneGraph.h"
// Include GLM
#include <glm\glm.hpp>

SceneGraph::SceneGraph()
{
	glm::mat4 id = glm::mat4 (); // Identity into ID
	root = new NodeMatrix(id);

}

SceneGraph::~SceneGraph()
{
}

void SceneGraph::addNode(Node *child)
{
	root->addChild(child);
}

void SceneGraph::removeNode(Node *node)
{
	root->removeChild(node);
}

NodeMatrix *SceneGraph::getRoot() const
{
	return root;
}