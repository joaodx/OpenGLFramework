/*
*Author: João Pedro Cova Regateiro
*No: 445954
*
*	Module 08966 Games Development Architectures
*	Department of Computer Science
*	UNIVERSITY OF HULL
*/
/** 



The scene manager must perform many tasks, some which include communicating between many managers
such as the render manager, audio manager, resource manager, and others, keeping track of time for the purpose of firing events and 
coordinating motion on the objects of the game, and enumerating the objects of the scene to allow interaction with them
( Reference: Game engine Design and Implementation , Alan Thorn ).

**/

#ifndef _SCENEMANAGER_HPP
#define _SCENEMANAGER_HPP

// Include GLM
#include <glm\glm.hpp>

#include <hash_map>
#include <stack>
#include <vector>
#include "NodeMatrix.h"


#include "Visitor.h"

class SceneManager : public Visitor
{
public:
    typedef NodeObj::gl_LightSourceParameters lightParameters;
	struct Renderable
	{
		NodeObj *object;
		glm::mat4 matrix;

		
		vector<unsigned> textures;
		Shader *shader;
		glm::vec3 colour;

		Renderable(NodeObj *obj, const glm::mat4 &mat, vector<unsigned>& tex, Shader *sh, const glm::vec3 &col)
			: object(obj), matrix(mat), textures(tex), shader(sh), colour(col) { }
	}; //Renderable

	SceneManager(NodeMatrix* );
	~SceneManager();

	glm::mat4 getMatrix();

	virtual void visit(NodeMatrix&);
	virtual void visit(NodeGroup&);
	virtual void visit(NodeObj&);
	virtual void visit(NodeTexture&);
	virtual void visit(NodeShader&);
	virtual void visit(NodeColour&);

	void initScene();

	std::vector<Renderable> &getObjList();
    std::vector<lightParameters *> getLights() const;

    void addLight(lightParameters * light);

private:
	 NodeMatrix *_graph;
	 std::vector<Renderable> _objlist;
	 std::stack<glm::mat4> _matstack;

	 Shader *_shader;
	 vector<unsigned> _textures;
	 glm::vec3 _colour;
     std::vector<lightParameters *> m_ligths;
};

#endif