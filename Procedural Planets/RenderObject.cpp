#include "RenderObject.h"

RenderObject::RenderObject(RenderManager *render, Mesh *mesh, Shader *shader, unsigned texture,unsigned normalMap, unsigned cellTexture)
    : render(render), _mesh(mesh), _rotation(glm::dquat()), _scale(1.0f, 1.0f, 1.0f), _nodeNormalMap(NULL), _nodeCellTexture(NULL), _light(position)
{
	shader->LoadShaders();

	_transformation = glm::mat4();
	_nodeMatrix = new NodeMatrix(_transformation);

	_nodeShader = new NodeShader(shader);
	_nodeTexture = new NodeTexture(texture);
	_nodeColour = new NodeColour(glm::vec3(1.0f, 1.0f, 1.0f));

	if (normalMap != 0)
	{
		_nodeNormalMap = new NodeTexture(normalMap);
	}
	if (cellTexture != 0)
	{
		_nodeCellTexture = new NodeTexture(cellTexture);
	}
	_nodeObject = new NodeObj(mesh);

	if (_nodeCellTexture != NULL)
	{
		_nodeCellTexture->addChild(_nodeObject);
	}
	if (_nodeNormalMap != NULL)
	{
		if (_nodeCellTexture != NULL)
		{
			_nodeNormalMap->addChild(_nodeCellTexture);
		}
		else
		{
			_nodeNormalMap->addChild(_nodeObject);
		}
	}

	if(_nodeNormalMap)
	{
		_nodeTexture->addChild(_nodeNormalMap);
	}
	else
	{
		_nodeTexture->addChild(_nodeObject);
	}

	_nodeColour->addChild(_nodeTexture);
	_nodeShader->addChild(_nodeColour);
	_nodeMatrix->addChild(_nodeShader);

	
}


RenderObject::~RenderObject()
{
	render->RemoveNode(_nodeMatrix);

	delete _nodeMatrix;
	delete _nodeShader;
	delete _nodeTexture;
	delete _nodeNormalMap;
	delete _nodeCellTexture;
	delete _nodeColour;
	delete _nodeObject;
}

void RenderObject::addToScene()
{
	render->AddNode(_nodeMatrix);
}

NodeMatrix* RenderObject::getNodeMatrix() const
{
	return _nodeMatrix;
}

void RenderObject::addChild(RenderObject* object)
{
	_nodeMatrix->addChild(object->getNodeMatrix());
}

void RenderObject::translate(float x, float y, float z)
{

	glm::vec3 mPosition(x, y, z);

	_transformation = glm::translate(_transformation,mPosition);
    _x = _transformation[3].x;
    _y = _transformation[3].y;
    _z = _transformation[3].z;
    position = glm::vec3(_x, _y, _z);
	_nodeMatrix->setMatrix(_transformation);
}

void RenderObject::setAsLight()
{
	isLigth = true;    
    _light.setPosition(position);
    if (isLigth)
        render->addLight(&_light);
}

void RenderObject::rotate(const glm::dquat &quat)
{
	_rotation = quat;

    glm::vec3 axis(quat.x, quat.y, quat.z);
    float angle = quat.w;
    _transformation = glm::rotate(_transformation, angle, axis);
	
    _x = _transformation[3].x;
    _y = _transformation[3].y;
    _z = _transformation[3].z;
    glm::vec4 PosCoordinates(_transformation * glm::vec4( 1, 1, 1, 1));
    position = glm::vec3(PosCoordinates.x / PosCoordinates.w, 
        PosCoordinates.y / PosCoordinates.w,
        PosCoordinates.z / PosCoordinates.w);
    _nodeMatrix->setMatrix(_transformation);
}

void RenderObject::scale(float x, float y, float z)
{
	_scale = (glm::vec3(x, y, z));
	_transformation = glm::scale(_transformation,_scale);	
	_nodeMatrix->setMatrix(_transformation);
}

void RenderObject::colour(float red, float green, float blue)
{
	_nodeColour->setColour(red, green, blue);
}
