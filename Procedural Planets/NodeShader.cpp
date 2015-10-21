#include "NodeShader.h"
#include "Visitor.h"

NodeShader::NodeShader(Shader *shader)
	: NodeGroup(), m_shader(shader)
{
}

NodeShader::~NodeShader()
{
}

void NodeShader::accept(Visitor &v)
{
	v.visit(*this);
}
