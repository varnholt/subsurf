#include "mesh.h"
#include "subdivision.h"

const Array<int>& Mesh::getIndices() const
{
   return mIndices;
}

void Mesh::setIndices(const Array<int>& indices)
{
   mIndices= indices;
}

const Array<Vector3>& Mesh::getVertices() const
{
   return mVertices;
}

void Mesh::setVertices(const Array<Vector3>& vertices)
{
   mVertices= vertices;
}

const Array<Vector3>& Mesh::getNormals() const
{
   return mNormals;
}

void Mesh::setNormals(const Array<Vector3>& normals)
{
   mNormals= normals;
}

const Array<Vector2>& Mesh::getTexcoords() const
{
   return mTexcoords;
}

void Mesh::setTexcoords(const Array<Vector2>& texcoords)
{
   mTexcoords= texcoords;
}

int Mesh::getVertexCount() const
{
   return mVertices.size();
}

Vector3* Mesh::getVertexData() const
{
   return mVertices.data();
}

Vector3* Mesh::getNormalData() const
{
   return mNormals.data();
}

Vector2* Mesh::getTexcoordData() const
{
   return mTexcoords.data();
}

int* Mesh::getIndexData() const
{
   return mIndices.data();
}

int Mesh::getIndexCount() const
{
   return mIndices.size();
}

void Mesh::calcVertexNormals()
{
   int numIndices= mIndices.size();
   int numVerts= mVertices.size();

   // init all vertex normals to 0,0,0
   mNormals.init(numVerts, true);
   Vector3* vtxNormal= mNormals.data();
   memset(vtxNormal, 0, numVerts*sizeof(Vector3));

   // calculate face normals and add them to all adjacent faces
   int* idx= mIndices.data();
   Vector3* vtx= mVertices.data();
   while (numIndices-=3)
   {
      int i1= *idx++;
      int i2= *idx++;
      int i3= *idx++;

      const Vector3& v1= vtx[i1];
      const Vector3& v2= vtx[i2];
      const Vector3& v3= vtx[i3];

      Vector3 faceNormal= (v2-v1) % (v3-v1);

      vtxNormal[i1] += faceNormal;
      vtxNormal[i2] += faceNormal;
      vtxNormal[i3] += faceNormal;
   }

   // normalize vertex normals
   while (numVerts--)
   {
      vtxNormal->normalize();
      vtxNormal++;
   }
}

void Mesh::symmetryX(int axis, float plane, float eps)
{
   int i;
   const int numVerts= mVertices.size();

   // alle vertices werden kopiert.
   // bis auf die die auf der Symmetrie-Ebene liegen (weld)
   // jeder vertex wird also auf sich selbst oder auf sein spiegelbild abgebildet
   Array<int> vertexRemap(numVerts);
   mVertices.resize(numVerts*2);
   for (i=0; i<numVerts; i++)
   {
      Vector3 v= mVertices[i];
      float* data= v.data();
      if (data[axis] < plane+eps) // am rand -> nicht kopieren
      {
         vertexRemap[i]= i;
      }
      else
      {
         data[axis]= plane*2.0f-data[axis];
         vertexRemap[i]= mVertices.add( v );
      }
   }

   const int numIndices= mIndices.size();
   mIndices.resize(numIndices*2);
   for (i=0; i<numIndices; i+=3)
   {
      int i1= mIndices[i];
      int i2= mIndices[i+1];
      int i3= mIndices[i+2];
      int m1= vertexRemap[i1];
      int m2= vertexRemap[i2];
      int m3= vertexRemap[i3];
      mIndices.add(m1); // flip winding!
      mIndices.add(m3);
      mIndices.add(m2);
   }
}


void Mesh::subDivide(Mesh* mesh)
{
   loopSubdivision(
            mVertices,
            mIndices,
            mesh->getVertices(),
            mesh->getIndices()
   );

   // no normals!
   // no uvs!
}
