/*
*Author: João Pedro Cova Regateiro
*No: 445954
*
*	Module 08966 Games Development Architectures
*	Department of Computer Science
*	UNIVERSITY OF HULL
*/
#include "SceneManager.h"


SceneManager::SceneManager(NodeMatrix *graph)
	: _objlist(), _matstack(), _graph(graph), _colour(1.0f, 1.0f, 1.0f)
{
	_matstack.push(glm::mat4()); // saves the Identity in _matstack
}

SceneManager::~SceneManager()
{
	delete _graph;
	_objlist.clear();	

	while (!_matstack.empty())
		_matstack.pop();
}

glm::mat4 SceneManager::getMatrix()
{
	return _graph->getMatrix();
}

void SceneManager::visit(NodeMatrix &nm)
{
	if (nm.isVisible())
	{
		_matstack.push(_matstack.top() * nm.getMatrix());

		for (int i = 0; i < nm.getChilds().size(); i++)
			nm.getChilds()[i]->accept(*this);

		_matstack.pop();
	}
}

void SceneManager::visit(NodeGroup &ng)
{
	if(ng.isVisible())
	{
		for (int i = 0; i < ng.getChilds().size(); i++)
			ng.getChilds()[i]->accept(*this);
	}
}

void SceneManager::visit(NodeObj &nm)
{
	if (nm.isVisible())
	{
		_objlist.push_back(Renderable(&nm, _matstack.top(), _textures, _shader, _colour));
		_textures.clear();
	}
}

void SceneManager::visit(NodeTexture &nt)
{
	_textures.push_back(nt.getTexture());

	for (int i = 0; i < nt.getChilds().size(); i++)
		nt.getChilds()[i]->accept(*this);
}

void SceneManager::visit(NodeShader &ns)
{
	_shader = ns.getShader();

	for (int i = 0; i < ns.getChilds().size(); i++)
		ns.getChilds()[i]->accept(*this);
}

void SceneManager::visit(NodeColour &nc)
{
	_colour = nc.getColour();

	for (int i = 0; i < nc.getChilds().size(); i++)
		nc.getChilds()[i]->accept(*this);
}

void SceneManager::initScene()
{
	_objlist.clear();
	_graph->accept(*this);
}

std::vector<SceneManager::Renderable> &SceneManager::getObjList()
{
	return _objlist;
}

std::vector<SceneManager::gl_LightSourceParameters *> SceneManager::getLights() const
{
	return m_ligths;
}

void SceneManager::addLight(SceneManager::gl_LightSourceParameters * light)
{
	m_ligths.push_back(light);
}
