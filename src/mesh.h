#pragma once

#include "array.h"
#include "vector2.h"
#include "vector3.h"

class Mesh
{
public:
   Mesh() = default;

   int                   getIndexCount() const;
   int*                  getIndexData() const;
   int                   getVertexCount() const;
   Vector3*              getVertexData() const;
   Vector3*              getNormalData() const;
   Vector2*              getTexcoordData() const;

   void                  setIndices(const Array<int>& indices);
   const Array<int>&     getIndices() const;
   void                  setVertices(const Array<Vector3>& vertices);
   const Array<Vector3>& getVertices() const;
   void                  setNormals(const Array<Vector3>& normals);
   const Array<Vector3>& getNormals() const;
   void                  setTexcoords(const Array<Vector2>& texcoords);
   const Array<Vector2>& getTexcoords() const;

   void                  symmetryX(int axis, float plane, float eps); // axis: 0=x, 1=y, 2=z
   void                  subDivide(Mesh* mesh);

   void                  calcVertexNormals();

private:
   Array<Vector3>        mVertices;     //!< vertex positions
   Array<Vector3>        mNormals;      //!< vertex normals (1 per vertex position)
   Array<Vector2>        mTexcoords;    //!< texture coordinates (1 per vertex psoition)
   Array<int>            mIndices;      //!< triangles (3 vertex indices per triangle)
};

