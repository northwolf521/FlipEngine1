#ifndef __PIPELINEP_H__
#define __PIPELINEP_H__
#include "glutils.h"
#include "Pipeline.h"

class PipelineP:public Pipeline
{
public:
	PipelineP(renderBuffer_t* rb);
	~PipelineP();

	void Init() {}
	void DrawScene();
	void DrawMesh(array<Mesh*>* meshs);

	Shader* _shader;
};


#endif