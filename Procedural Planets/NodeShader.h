#pragma once

#include "NodeGroup.h"
#include "shader.h"

class NodeShader : public NodeGroup
{
	Shader *m_shader;

public:
	NodeShader(Shader *shader);
	~NodeShader();

	Shader *getShader() { return m_shader; }

	virtual void accept(Visitor&);
};

