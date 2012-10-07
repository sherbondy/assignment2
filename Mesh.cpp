#include "Mesh.h"
#define MAX_BUFFER_SIZE 4096

using namespace std;

void Mesh::load( const char* filename )
{
	// 2.1.1. load() should populate bindVertices, currentVertices, and faces
    ifstream meshfile(filename);

    char buffer[MAX_BUFFER_SIZE];
    while (meshfile.getline(buffer, MAX_BUFFER_SIZE)){
        stringstream ss(buffer); // a buffer corresponds to one line
        
        Vector3f v;
        string s;
        unsigned idx[3]; // indices for faces
        Tuple3u faceVertices; // populated by idx
        
        ss >> s; // put a token into s: indicates *v*ertex or *f*ace
        if (s == "v") {
            ss >> v[0] >> v[1] >> v[2];
            bindVertices.push_back(v);
        } else if (s == "f") {
            ss >> idx[0] >> idx[1] >> idx[2];
            faceVertices = Tuple3u(idx[0],idx[1],idx[2]);
            faces.push_back(faceVertices);
        }
    }
    
	// make a copy of the bind vertices as the current vertices
	currentVertices = bindVertices;
}

void Mesh::draw()
{
	// Since these meshes don't have normals
	// be sure to generate a normal per triangle.
	// Notice that since we have per-triangle normals
	// rather than the analytical normals from
	// assignment 1, the appearance is "faceted".
    // render from currentVertices, NOT bindVertices
    
    FaceVec::iterator iter = faces.begin();
    vector<Vector3f>normals;

    while (iter != faces.end()){
        Tuple3u idxes = *iter;
        vector<Vector3f> faceVertices;
        glBegin(GL_TRIANGLES);
        
        for (unsigned i = 0; i < 3; ++i){
            // face vertices are 1-indexed in obj files
            Vector3f vertex = currentVertices[idxes[i] - 1];
            faceVertices.push_back(vertex);
            glVertex3f(vertex[0], vertex[1], vertex[2]);
        }
        
        // calculate normals on the fly
        Vector3f u = faceVertices[1] - faceVertices[0];
        Vector3f v = faceVertices[2] - faceVertices[0];
        Vector3f normal = Vector3f::cross(u, v).normalized();
        glNormal3f(normal[0], normal[1], normal[2]);
        
        glEnd();
        iter++;
    }
}

void Mesh::loadAttachments( const char* filename, int numJoints )
{
	// 2.2. Implement this method to load the per-vertex attachment weights
	// this method should update m_mesh.attachments
    
    ifstream attachfile(filename);
    
    char buffer[MAX_BUFFER_SIZE];
    while (attachfile.getline(buffer, MAX_BUFFER_SIZE)){
        stringstream ss(buffer); // a buffer corresponds to one line
        
        float tmpWeight;
        vector<float> vertexWeights;
        
        while(ss >> tmpWeight){
            vertexWeights.push_back(tmpWeight);
        }
        
        attachments.push_back(vertexWeights);
    }    
}
