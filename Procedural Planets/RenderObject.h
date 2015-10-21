#ifndef _RENDEROBJECT_HPP
#define _RENDEROBJECT_HPP


#include "RenderManager.h"
#include "NodeObj.h"
#include "NodeMatrix.h"
#include "Shader.h"
#include <vector>
#include <gl\GL.h>

// Include GLM
#include <glm\glm.hpp>
#include <glm\gtc\quaternion.hpp>
#include <glm\gtc\matrix_transform.hpp>


class RenderObject 
{
public:
    typedef SceneManager::gl_LightSourceParameters lightParameters;

	RenderObject(RenderManager *renderer, Mesh *mesh, Shader *shader, unsigned texture, unsigned normalMap=0, unsigned cellTexture=0);
	~RenderObject();
	
	void translate(float x, float y, float z);
	void rotate(const glm::dquat &quat);
	void scale(float x, float y, float z);
	void colour(float red, float green, float blue);
	void visible(bool visible) { _nodeMatrix->setVisible(visible); }
	void transparency(bool alphaOn) { _mesh->setTransparency(alphaOn); }
	void setTexture(unsigned texture) { _nodeTexture->setTexture(texture); }
	void setNormalMap(unsigned texture) { _nodeNormalMap->setTexture(texture); }
	void setCellTexture(unsigned texture) { _nodeCellTexture->setTexture(texture); }

	float getX() { return _x; }
	float getY() { return _y; }
	float getZ() { return _z; }
	glm::vec3 & getPosition() { return position; }
	float getWidth() { return _mesh->getWidth(); }
	float getHeight() { return _mesh->getHeight(); }
	float getDepth() { return _mesh->getDepth(); }
	glm::dquat getRotation() { return _rotation; }
	bool getVisible() { return _nodeMatrix->isVisible(); }
	bool getTransparency() { return _mesh->getTransparent(); }
	glm::vec3 getColour() { return _nodeColour->getColour(); }
	void addChild(RenderObject*);
	void addToScene();
	//btCollisionShape *createBoundingPoly() { return _mesh->createBoundingPoly(); }

	NodeMatrix *getNodeMatrix() const;
    //Light Operations
	void setAsLight();
    void setAmbientColour(glm::vec4 amb){ _light.setAmbientColour(amb); }
    void setdiffuseColour(glm::vec4 diff){ _light.setdiffuseColour(diff); }
    void setSpecularColour(glm::vec4 spec){ _light.setSpecularColour(spec); }
    void setPosition(glm::vec3 pos){ _light.setPosition(pos); }

private:
	RenderManager *render;

	Mesh *_mesh;
	glm::mat4 _transformation;
	glm::dquat _rotation;
	float  _x, _y, _z;
	glm::vec3 position;
	glm::vec3 _scale;

	NodeMatrix *_nodeMatrix;
	NodeShader *_nodeShader;
	NodeTexture *_nodeTexture;
	NodeTexture *_nodeNormalMap;
	NodeTexture *_nodeCellTexture;
	NodeColour *_nodeColour;
	NodeObj *_nodeObject;

	bool isLigth;
    lightParameters _light;
};
#endif