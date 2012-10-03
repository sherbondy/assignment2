#include "SkeletalModel.h"

#include <FL/Fl.H>

using namespace std;

void SkeletalModel::load(const char *skeletonFile, const char *meshFile, const char *attachmentsFile)
{
	loadSkeleton(skeletonFile);

	m_mesh.load(meshFile);
	m_mesh.loadAttachments(attachmentsFile, (int)m_joints.size());

	computeBindWorldToJointTransforms();
	updateCurrentJointToWorldTransforms();
}

void SkeletalModel::draw(Matrix4f cameraMatrix, bool skeletonVisible)
{
	// draw() gets called whenever a redraw is required
	// (after an update() occurs, when the camera moves, the window is resized, etc)

	m_matrixStack.clear();
	m_matrixStack.push(cameraMatrix);

	if( skeletonVisible )
	{
		drawJoints();

		drawSkeleton();
	}
	else
	{
		// Clear out any weird matrix we may have been using for drawing the bones and revert to the camera matrix.
		glLoadMatrixf(m_matrixStack.top());

		// Tell the mesh to draw itself.
		m_mesh.draw();
	}
}

void SkeletalModel::loadSkeleton( const char* filename )
{
	// Load the skeleton from file here.
    ifstream skelefile(filename);
    string line;
    
    if (skelefile.is_open()){
        while (skelefile.good()) {
            getline(skelefile, line);
            stringstream ss(line);
            
            float dx, dy, dz;
            int parent;
            ss >> dx >> dy >> dz >> parent;
            
            Joint *joint = new Joint();
            Matrix4f translation = Matrix4f::translation(dx, dy, dz);
            joint->transform = translation;
            
            m_joints.push_back(joint);
            if (parent == -1){
                m_rootJoint = joint;
            } else {
                m_joints[parent]->children.push_back(joint);
            }
        }

        skelefile.close();
    } else {        
        cout << "Unable to open file " << filename;
    }
}

/* Recursively draws the child joints of joint */
void SkeletalModel::drawChildJoints(Joint *joint)
{
    m_matrixStack.push(joint->transform);
    glLoadMatrixf(m_matrixStack.top());
    glutSolidSphere(0.025f, 12, 12);
    
    for (int i = 0; i < joint->children.size(); ++i){
        Joint *child = joint->children[i];
        this->drawChildJoints(child);
    }
    
    m_matrixStack.pop();
}

void SkeletalModel::drawJoints( )
{
    this->drawChildJoints(m_rootJoint);
}

void SkeletalModel::drawChildBones(Joint *joint)
{
    m_matrixStack.push(joint->transform);
    
    for (int i = 0; i < joint->children.size(); ++i){
        Joint *child = joint->children[i];
        Vector3f child_offset = child->transform.getCol(3).xyz();
        float child_distance = child_offset.abs();
        
        // push cube transformations
        Matrix4f translate = Matrix4f::translation(0, 0, 0.5); // translate up 0.5;
        Matrix4f scale     = Matrix4f::scaling(0.05, 0.05, child_distance);
        
        Vector3f rotateZ   = child_offset.normalized();
        Vector3f rotateY   = Vector3f::cross(rotateZ, Vector3f::RIGHT).normalized();
        Vector3f rotateX   = Vector3f::cross(rotateY, rotateZ).normalized();
        Matrix4f rotate    = Matrix4f(Vector4f(rotateX, 0),
                                      Vector4f(rotateY, 0),
                                      Vector4f(rotateZ, 0),
                                      Vector4f(0, 0, 0, 1));
        
        Matrix4f transformations = rotate * scale * translate;
        
        m_matrixStack.push(transformations);
        glLoadMatrixf(m_matrixStack.top());
        glutSolidCube(1.0f);
        m_matrixStack.pop();
        
        this->drawChildBones(child);
    }
    
    m_matrixStack.pop();
}

void SkeletalModel::drawSkeleton( )
{
	// Draw boxes between the joints. You will need to add a recursive helper function to traverse the joint hierarchy.
    this->drawChildBones(m_rootJoint);
}

static float inline degToRad(float degrees) {
    return 2 * M_PI * degrees / 360.0;
}

void SkeletalModel::setJointTransform(int jointIndex, float rX, float rY, float rZ)
{
	// Set the rotation part of the joint's transformation matrix based on the passed in Euler angles.
    Matrix4f *transform = &(m_joints[jointIndex]->transform);
    transform->rotateX(rX);
    transform->rotateY(rY);
    transform->rotateZ(rZ);
}


void SkeletalModel::computeBindWorldToJointTransforms()
{
	// 2.3.1. Implement this method to compute a per-joint transform from
	// world-space to joint space in the BIND POSE.
	//
	// Note that this needs to be computed only once since there is only
	// a single bind pose.
	//
	// This method should update each joint's bindWorldToJointTransform.
	// You will need to add a recursive helper function to traverse the joint hierarchy.
}

void SkeletalModel::updateCurrentJointToWorldTransforms()
{
	// 2.3.2. Implement this method to compute a per-joint transform from
	// joint space to world space in the CURRENT POSE.
	//
	// The current pose is defined by the rotations you've applied to the
	// joints and hence needs to be *updated* every time the joint angles change.
	//
	// This method should update each joint's bindWorldToJointTransform.
	// You will need to add a recursive helper function to traverse the joint hierarchy.
}

void SkeletalModel::updateMesh()
{
	// 2.3.2. This is the core of SSD.
	// Implement this method to update the vertices of the mesh
	// given the current state of the skeleton.
	// You will need both the bind pose world --> joint transforms.
	// and the current joint --> world transforms.
}

