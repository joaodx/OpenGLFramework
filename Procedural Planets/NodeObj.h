/*
*Author: João Pedro Cova Regateiro
*No: 445954
*
*	Module 08966 Games Development Architectures
*	Department of Computer Science
*	UNIVERSITY OF HULL
*/

/*
*	Class that represents a physic object, this mean that this class contains the geometric class of the object,
*	with this class its possible to abstract to diferente type os objects, for example dynamic object and static objects.
*	Although, also represents a node in the graph.
*/

#include "Node.h"
#include <string>

#ifndef _NODEOBJ_HPP
#define _NODEOBJ_HPP

class NodeObj : public Node
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
public:
    NodeObj(Mesh* const obj);
	~NodeObj();

	virtual void accept(Visitor&);

	Mesh * getMesh() { return _obj; }
    gl_LightSourceParameters * getLight() { return _light; }

    void setLight(gl_LightSourceParameters* const light)
    {
        _light = light;
    }

    bool isLight() { return _light; }

private:
	Mesh *_obj;
    gl_LightSourceParameters *_light;
};

#endif