#pragma once

#include "vec3.h"
#include "mat4.h"
#include "quat.h"
#include <string>
#include <vector>

using std::string;
using std::vector;

struct PositionKey
{
	vec3 position;
	float frame;
};

struct ScaleKey
{
	vec3 position;
	float frame;
};

struct RotationKey
{
	quat rotation;
	float frame;
};

class Joint
{
public:
	string name;
	vec3 position;
	vec3 scale;
	quat rotation;
	vector<Joint*> children;
	Joint* parent;

	vector<PositionKey> positionKeys;
	vector<RotationKey> rotationKeys;
	vector<ScaleKey>    scaleKeys;
	vector<int>         vertexIndices;
	vector<float>       vertexWeights;

	mat4 globalAnimatedMatrix;
	mat4 globalInvMatrix;

	mat4 globalPosition;

	void GetPositionKeyFramesAtFrame(float frame, PositionKey* preKey, PositionKey* nextKey);
	void GetScaleKeyFramesAtFrame(float frame, ScaleKey* preKey, ScaleKey* nextKey);
	void GetRotationKeyFramesAtFrame(float frame, RotationKey* preKey, RotationKey* nextKey);
	void GetFrame(float frame, vec3& position, quat& rotation);
};
