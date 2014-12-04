/**
 * This sphere class was derived from Professor Nussbaum's solution.
 *
 * It was modified by Ryan Seys to add colors to the spheres and
 * rotation transformation.
 */

#ifndef RYAN_CYLINDER
#define RYAN_CYLINDER
#include <stdlib.h>
#include <cmath>
#include "ryan_vector.h"
#include "ryan_matrix.h"

struct Vertex{
    Vertex(){}
    Vertex( const Vector3f& _pos, const Vector3f& _nor ){
        pos[0] = _pos.x;
        pos[1] = _pos.y;
        pos[2] = _pos.z;
        pos[3] = 1.0f;
        norm[0] = _nor.x;
        norm[1] = _nor.y;
        norm[2] = _nor.z;
        norm[3] = 1.0f;
    }

    float pos[4];
    float norm[4];
};

class Cylinder {
protected:
    std::vector<Vertex> m_vertices;
    std::vector<GLuint> m_indices;
    GLuint m_vbo;
    GLuint m_idxVbo;

    Matrix4f modelMat = Matrix4f::identity();
    Matrix4f rotMat = Matrix4f::identity();
public:
  Vector4f materialAmbient;
  Vector4f materialDiffuse;
  Vector4f materialSpecular;

  Cylinder(int sectors) {

    m_vertices.resize( (sectors * 2) + 2 );
    m_indices.resize( (sectors - 1) * 12 );

    //create the vertices
    float S = 1.0f / ( sectors - 1.0f );
    int i = 0;
    for( int s = 0; s < sectors; ++s ){
        float x = (float) cos( 2 * M_PI * s * S );
        float z = (float) sin( 2 * M_PI * s * S );
        m_vertices[s]           = Vertex( Vector3f( x, 1.0f, z ), Vector3f( x, 1.0f, z ) );
        m_vertices[s+sectors]   = Vertex( Vector3f( x, -1.0f, z ), Vector3f( x, 1.0f, z ) );
    }
    m_vertices[sectors*2]           = Vertex( Vector3f( 0, 1.0f, 0 ), Vector3f( 0, 1.0f, 0 ));
    m_vertices[sectors*2 + 1]       = Vertex( Vector3f( 0, -1.0f, 0 ), Vector3f( 0, -1.0f, 0 ));

    //create the indices
    i = 0;
    for( int s = 0; s < sectors - 1; ++s ){
        m_indices[i++] = s + 1;
        m_indices[i++] = s + sectors;
        m_indices[i++] = s;

        m_indices[i++] = s + 1 + sectors;
        m_indices[i++] = s + sectors;
        m_indices[i++] = s + 1;
    }
    for( int s = 0; s < sectors - 1; ++s ){
        m_indices[i++] = s + 1;
        m_indices[i++] = s;
        m_indices[i++] = sectors * 2;

        m_indices[i++] = s + 1 + sectors;
        m_indices[i++] = s + sectors;
        m_indices[i++] = sectors * 2 + 1;
    }
    glGenBuffers(1, &m_vbo);
    glBindBuffer( GL_ARRAY_BUFFER, m_vbo );
    glBufferData( GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), m_vertices.data(), GL_STATIC_DRAW);

    //create index buffer
    glGenBuffers( 1, &m_idxVbo );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_idxVbo );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(GLuint), m_indices.data(), GL_STATIC_DRAW );
  }

  /**
   * Rotate the sphere around the y-axis.
   *
   * @param degrees Degrees to rotate it.
   */
  void rotateY(GLfloat angle, GLuint deg) {
    Matrix4f tempRot = Matrix4f::rotateY(angle, deg);
    rotMat = rotMat * tempRot;
    modelMat = modelMat * tempRot;
  }

  void rotateX(GLfloat angle, GLuint deg) {
    Matrix4f tempRot = Matrix4f::rotateX(angle, deg);
    rotMat = rotMat * tempRot;
    modelMat = modelMat * tempRot;
  }

  void rotateZ(GLfloat angle, GLuint deg) {
    Matrix4f tempRot = Matrix4f::rotateZ(angle, deg);
    rotMat = rotMat * tempRot;
    modelMat = modelMat * tempRot;
  }

  void rotateVector(Vector3f v, GLfloat angle, int degrees) {
    Matrix4f tempRot = Matrix4f::rotateVector(v, angle, degrees);
    rotMat = rotMat * tempRot;
    modelMat = modelMat * tempRot;
  }

  /**
   * Draw the sphere using the supplied shaderProgram.
   * @param shaderProg Shader program to use.
   */
  void draw(GLuint shaderProg) {
    Matrix4f normalMat = Matrix4f::transpose(Matrix4f::inverse(this->rotMat));
    // modelMat = modelMat * Matrix4f::scale(4, 4, 4);

    GLuint modelLoc = glGetUniformLocation(shaderProg,  "modelMat");
    glUniformMatrix4fv(modelLoc, 1, 1, (float *) modelMat.vm);

    GLuint normalMatLoc = glGetUniformLocation(shaderProg,  "normalMat");
    glUniformMatrix4fv(normalMatLoc, 1, 1, (float *) normalMat.vm);

    GLuint matAmbLoc = glGetUniformLocation(shaderProg,  "materialAmb");
    glUniform4fv(matAmbLoc, 1, (float *) &materialAmbient);

    GLuint matDiffLoc = glGetUniformLocation(shaderProg,  "materialDiff");
    glUniform4fv(matDiffLoc, 1, (float *) &materialDiffuse);

    GLuint matSpecLoc = glGetUniformLocation(shaderProg,  "materialSpec");
    glUniform4fv(matSpecLoc, 1, (float *) &materialSpecular);

    //copy the vertex position
    GLint vtxLoc = glGetAttribLocation( shaderProg, "vPosition");
    GLuint normalLoc = glGetAttribLocation(shaderProg, "vNormal");
    glEnableVertexAttribArray( vtxLoc );
    glEnableVertexAttribArray(normalLoc);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_idxVbo);

    glVertexAttribPointer(vtxLoc, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, pos));
    glVertexAttribPointer(normalLoc, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, norm));

    //copy the vertex color
    // GLint colLoc = glGetAttribLocation( shaderProg, "color");
    // glEnableVertexAttribArray( colLoc );

    glDrawElements( GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, NULL );
    this->clear();
  }

  void scale(GLfloat xAmt, GLfloat yAmt, GLfloat zAmt) {
    modelMat = modelMat * Matrix4f::scale(xAmt, yAmt, zAmt);
  }

  void translate(GLfloat x, GLfloat y, GLfloat z) {
    modelMat = modelMat * Matrix4f::translation(x, y, z);
  }

  void rotateVector(Vector3f v, float deg) {
    modelMat = modelMat * Matrix4f::rotateVector(v, deg, 1);
  }

  void setAmbient(GLfloat r, GLfloat g, GLfloat b) {
    this->materialAmbient = Vector4f(r, g, b, 0.0);
  }

  void setDiffuse(GLfloat r, GLfloat g, GLfloat b) {
    this->materialDiffuse = Vector4f(r, g, b, 0.0);
  }

  void setSpecular(GLfloat r, GLfloat g, GLfloat b) {
    this->materialSpecular = Vector4f(r, g, b, 0.0);
  }

  void clear() {
    this->modelMat = Matrix4f::identity();
    this->rotMat = Matrix4f::identity();
  }
};

#endif
