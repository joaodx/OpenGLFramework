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
    // light Structure based on [link: https://www.opengl.org/sdk/docs/tutorials/ClockworkCoders/lighting.php ]
    struct gl_LightSourceParameters
    {
        glm::vec4 ambient;              // Aclarri   
        glm::vec4 diffuse;              // Dcli   
        glm::vec4 specular;             // Scli   
        glm::vec3 & position;             // Ppli   
       /* glm::vec4 halfVector;           // Derived: Hi   
        glm::vec3 spotDirection;        // Sdli   
        float spotExponent;        // Srli   
        float spotCutoff;          // Crli                              
        // (range: [0.0,90.0], 180.0)   
        float spotCosCutoff;       // Derived: cos(Crli)                 
        // (range: [1.0,0.0],-1.0)   
        float constantAttenuation; // K0   
        float linearAttenuation;   // K1   
        float quadraticAttenuation;// K2  */

        gl_LightSourceParameters(glm::vec3 &pos) : ambient(0, 0, 0, 0), diffuse(0, 0, 0, 0), specular(0, 0, 0, 0), position(pos)/*, halfVector(0, 0, 0, 0),
            spotDirection(0, 0, 0), spotExponent(0), spotCutoff(0), spotCosCutoff(0), constantAttenuation(0),
            linearAttenuation(0), quadraticAttenuation(0)*/ { }

        gl_LightSourceParameters(glm::vec4 &amb, glm::vec4 &diff, glm::vec4 &spec, glm::vec3 &pos) : ambient(amb), diffuse(diff), specular(spec), position(pos)/*, halfVector(0, 0, 0, 0),
            spotDirection(0, 0, 0), spotExponent(0), spotCutoff(0), spotCosCutoff(0), constantAttenuation(0),
            linearAttenuation(0), quadraticAttenuation(0)*/ { }

        void setAmbientColour(glm::vec4 amb){ ambient = amb; }
        void setdiffuseColour(glm::vec4 diff){ diffuse = diff; }
        void setSpecularColour(glm::vec4 spec){ specular = spec; }
        void setPosition(glm::vec3 & pos){ position = pos; }
        // TODO: add the other modifiers.

    }; // gl_LightSourceParameters

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
    std::vector<gl_LightSourceParameters *> getLights() const;

    void addLight(gl_LightSourceParameters * light);

private:
	 NodeMatrix *_graph;
	 std::vector<Renderable> _objlist;
	 std::stack<glm::mat4> _matstack;

	 Shader *_shader;
	 vector<unsigned> _textures;
	 glm::vec3 _colour;
     std::vector<gl_LightSourceParameters *> m_ligths;
};

#endif