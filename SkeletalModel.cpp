#include "SkeletalModel.h"

#include <FL/Fl.H>

#define MAX_BUFFER_SIZE 4096

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
		// Clear out any weird matrix we may have been using for drawing
        // the bones and revert to the camera matrix.
		glLoadMatrixf(m_matrixStack.top());

		// Tell the mesh to draw itself.
		m_mesh.draw();
	}
}

void SkeletalModel::loadSkeleton( const char* filename )
{
	// Load the skeleton from file here.
    ifstream skelefile(filename);
    
    char buffer[MAX_BUFFER_SIZE];
    while (skelefile.getline(buffer, MAX_BUFFER_SIZE)){
        stringstream ss(buffer); // a buffer corresponds to one line
                            
        float dx, dy, dz;
        int parent;
        ss >> dx >> dy >> dz >> parent;
        
        Joint *joint = new Joint();
        Matrix4f translation = Matrix4f::translation(dx, dy, dz);
        joint->transform = translation;
        joint->rotation = Matrix4f::identity();

        m_joints.push_back(joint);
        if (parent == -1){
            m_rootJoint = joint;
        } else {
            m_joints[parent]->children.push_back(joint);
        }
    }
    skelefile.close();
}

/* Recursively draws the child joints of joint */
void SkeletalModel::drawChildJoints(Joint *joint)
{
    m_matrixStack.push(joint->rotatedTransform());
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
    m_matrixStack.push(joint->rotatedTransform());
    
    for (int i = 0; i < joint->children.size(); ++i){
        Joint *child = joint->children[i];
        Vector3f child_offset = child->rotatedTransform().getCol(3).xyz();
        float child_distance = child_offset.abs();
        
        // push cube transformations
        Matrix4f translate = Matrix4f::translation(0, 0, 0.5); // translate up 0.5;
        Matrix4f scale     = Matrix4f::scaling(0.05, 0.05, child_distance);
        
        // z is the important rotation. The other two are somewhat arbitrary.
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
    this->drawChildBones(m_rootJoint);
}

void SkeletalModel::setJointTransform(int jointIndex, float rX, float rY, float rZ)
{
	// Set the rotation part of the joint's transformation matrix
    // based on the passed in Euler angles.
    Matrix4f new_rotation = Matrix4f::rotateX(rX) *
                            Matrix4f::rotateY(rY) *
                            Matrix4f::rotateZ(rZ);
    
    m_joints[jointIndex]->rotation = new_rotation;
}


// recursive helper function for B
void SkeletalModel::computeBindWorldToJoint(Joint *joint)
{
    Matrix4f B_inv = joint->rotatedTransform().inverse();
    m_matrixStack.push(B_inv);
    
    joint->bindWorldToJointTransform = m_matrixStack.top();
        
    for (unsigned i = 0; i < joint->children.size(); i++){
        Joint *child = joint->children[i];
        this->computeBindWorldToJoint(child);
    }
    
    m_matrixStack.pop();
}

void SkeletalModel::computeBindWorldToJointTransforms()
{
	// Note that this needs to be computed only once since there is only
	// a single bind pose.
	//
	// This method should update each joint's bindWorldToJointTransform.
	// You will need to add a recursive helper function to traverse the joint hierarchy.

    m_matrixStack.clear();
    this->computeBindWorldToJoint(m_rootJoint);
}

// recursive helper function for T
void SkeletalModel::updateCurrentJointToWorld(Joint *joint)
{
    m_matrixStack.push(joint->rotatedTransform());
    
    joint->currentJointToWorldTransform = m_matrixStack.top();
    
    for (unsigned i = 0; i < joint->children.size(); i++){
        Joint *child = joint->children[i];
        this->updateCurrentJointToWorld(child);
    }
    
    m_matrixStack.pop();
}

void SkeletalModel::updateCurrentJointToWorldTransforms()
{
	// The current pose is defined by the rotations you've applied to the
	// joints and hence needs to be *updated* every time the joint angles change.
	//
	// This method should update each joint's currentJointToWorldTransform.
	// You will need to add a recursive helper function to traverse the joint hierarchy.

    m_matrixStack.clear();
    updateCurrentJointToWorld(m_rootJoint);
}

void SkeletalModel::updateMesh()
{
	// 2.3.2. This is the core of SSD.
	// Implement this method to update the vertices of the mesh
	// given the current state of the skeleton.
	// You will need both the bind pose world --> joint transforms.
	// and the current joint --> world transforms.
    unsigned jointSize = (unsigned) m_joints.size();
    unsigned long vertexCount = m_mesh.bindVertices.size();
        
    for (unsigned long i = 0; i < vertexCount; ++i) {
        Vector3f p = m_mesh.bindVertices[i];
        Vector3f pActive = Vector3f();
        vector<float> weights = m_mesh.attachments[i];
        
        for (unsigned j = 0; j < jointSize; ++j) {
            Joint *joint = m_joints[j];
            float weight = weights[j];
            
            // note: bindWorldToJointTransform is actually B^-1 since that's
            // all we actually make use of here.
            if (weight > 0) {
                Vector4f contribution = joint->currentJointToWorldTransform *
                                        joint->bindWorldToJointTransform *
                                        Vector4f(p, 1);
                
                pActive += weight * contribution.xyz();
            }
        }
        
        m_mesh.currentVertices[i] = pActive;
    }    
}

