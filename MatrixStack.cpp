#include "MatrixStack.h"

void MatrixStack::add_identity()
{
    m_matrices.push_back(Matrix4f::identity());
}

MatrixStack::MatrixStack()
{
	// Initialize the matrix stack with the identity matrix.
    this->add_identity();
}

void MatrixStack::clear()
{
	// Revert to just containing the identity matrix.
    m_matrices.empty();
    this->add_identity();
}

Matrix4f MatrixStack::top()
{
	// Return the top of the stack
    return m_matrices.back();
}

void MatrixStack::push( const Matrix4f& m )
{
	// Push m onto the stack.
	// Your stack should have OpenGL semantics:
	// the new top should be the old top multiplied by m
    Matrix4f product =  this->top() * m;
    m_matrices.push_back(product);
}

void MatrixStack::lpush( const Matrix4f& m)
{
    // push m onto the stack, but multiply on the left hand side.
    Matrix4f product = m * this->top();
    m_matrices.push_back(product);
}

void MatrixStack::pop()
{
	// Remove the top element from the stack
    m_matrices.pop_back();
}
