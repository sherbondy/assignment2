#include "Mesh.h"
#define MAX_BUFFER_SIZE 1024

using namespace std;

void Mesh::load( const char* filename )
{
	// 2.1.1. load() should populate bindVertices, currentVertices, and faces

	// Add your code here.
    ifstream meshfile(filename);

    char buffer[MAX_BUFFER_SIZE];
    while (meshfile.getline(buffer, MAX_BUFFER_SIZE)){
        stringstream ss(buffer); // a buffer corresponds to one line
        
        Vector3f v;
        string s;
        unsigned tmp; // placeholder for face vertex indices.
        char slash;  // gobble up blank space
        vector<unsigned> idx; // indices for faces
        Tuple3u faceVertices; // populated by idx
        
        ss >> s; // put a token into s: indicates *v*ertex or *f*ace
        if (s == "v") {
            ss >> v[0] >> v[1] >> v[2];
            bindVertices.push_back(v);
        } else if (s == "f") {
            while (ss.good()) {
                ss >> tmp;
                ss >> slash;
                idx.push_back(tmp);
            }
            
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
}

void Mesh::loadAttachments( const char* filename, int numJoints )
{
	// 2.2. Implement this method to load the per-vertex attachment weights
	// this method should update m_mesh.attachments
}
