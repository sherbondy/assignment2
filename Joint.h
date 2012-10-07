#ifndef JOINT_H
#define JOINT_H

#include <vector>
#include <vecmath.h>

struct Joint
{
	Matrix4f transform; // transform relative to its parent
    Matrix4f rotation;  // keep track of the rotation separately
	std::vector< Joint* > children; // list of children

	// This matrix transforms world space into joint space for the initial ("bind") configuration of the joints. (B)
	Matrix4f bindWorldToJointTransform;

	// This matrix maps joint space into world space for the *current* configuration of the joints. (T)
	Matrix4f currentJointToWorldTransform;
    
    Matrix4f rotatedTransform();
};

#endif
