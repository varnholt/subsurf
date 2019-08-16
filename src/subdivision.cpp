// implements the loop subdivision sheme on triangles

#include "subdivision.h"
#include "map.h"

/*
  Subdivision concept:
  Each triangle is defined by three vertex indices (i1,i2,i3)
  and is subdivided into four new triangles by creating a new vertex on each edge -> e1,e2,e3
  These new vertices are also used by the neighbouring triangle on the shared edge

          i3
          + - - - - - +
         / \
        /   \       /
    e3 O-----O e2
      / \   / \   /
     /   \ /   \
 i1 +-----O-----+ i2
          e1

new vertex buffer:
...numVerts: old vertices
numVerts...: new vertices (on edge)
*/



/*
  Shared edge concept:
  Defines an edge by two vertex indices (i1,i2)
  and holds the neighbouring triangles (usually two, one for boundaries)
  by storing the "missing" vertex indices to build the triangles

       i3     <- always exist (because edges are extracted from triangles)
       +
      / \
     /   \
    /     \
i1 +=======+ i2    edge i1 -> i2
    \     /        neighbouring triangles: (i1,i2,i3) (i1,i4,i2)
     \   /
      \ /
       +
       i4     <- absent on boundary edges (-1)
*/

class SharedEdge
{
public:
   SharedEdge()
      : i1(-1), i2(-1), i3(-1), i4(-1)
   {
   }

   SharedEdge(int v1, int v2, int v3)
   {
      i1= v1;
      i2= v2;
      i3= v3;
      i4= -1;
   }

   void addTri(int index)
   {
      i4=index;
   }

   int i1,i2;
   int i3,i4;
};

struct EdgeMap {
   int vtxIndex;
   int edgeIndex;
};


inline int addEdge(Array<EdgeMap>* edgeMap, Array<SharedEdge>& edgeList, int** vtxNeighbours, int i1, int i2, int i3)
{
   int edgeIndex;

   // create unique edges so that (i1, i2) has always i1 < i2
   if (i1 > i2)
   {
      int t = i1;
      i1 = i2;
      i2 = t;
   }

   Array<EdgeMap>& map= edgeMap[i1];
   edgeIndex= -1;

   for (int i=0; i<map.size(); i++)
   {
      if (map[i].vtxIndex==i2)
      {
         edgeIndex= map[i].edgeIndex;
         break;
      }
   }

   if (edgeIndex < 0)
   {
      // new edge
      SharedEdge edge(i1,i2, i3);
      edgeIndex= edgeList.add( edge );
      EdgeMap em;
      em.vtxIndex= i2;
      em.edgeIndex= edgeIndex;
      map.add( em );

      // i1 is connected to i2
      // i2 is connected to i1
      int* n1= vtxNeighbours[i1];
      int* n2= vtxNeighbours[i2];
      n1[ ++n1[0] ]= i2;
      n2[ ++n2[0] ]= i1;
   }
   else
   {
      // edge already exists
      // -> i1/i2 are already connected
      // but add shared from this triangle
      edgeList[edgeIndex].addTri(i3);
   }

   return edgeIndex;
}

void loopSubdivision(
      Array<Vector3>& dstVertices,
      Array<int>& dstIndices,
      const Array<Vector3>& srcVertices,
      const Array<int>& srcIndices )
{
   int i;
   const int numIndices= srcIndices.size();
   const int numVerts= srcVertices.size();

   Vector3* srcVtx= srcVertices.data();
   int* srcIdx= srcIndices.data();

   Array<int> neighbourCount(numVerts);
   memset(neighbourCount.data(), 0, numVerts*sizeof(int)); // clear
   int total= 0;

   for (i=0; i<numIndices; i+=3)
   {
      const int i1= srcIdx[i+0];
      const int i2= srcIdx[i+1];
      const int i3= srcIdx[i+2];

      // this happens twice for all neighbouring triangles
      // so we are about twice
      neighbourCount[i1]+=2; // i1 -> i2,i3
      neighbourCount[i2]+=2; // i2 -> i1,i3
      neighbourCount[i3]+=2; // i3 -> i1,i2
      total+=6;
   }

   // allocate enough space to hold a list for each vertex
   int* buffer= new int[total+numVerts];
   int** vertexNeighbours= new int*[numVerts];
   int pos= 0;
   for (i=0; i<numVerts; i++)
   {
      int* list= buffer + pos;
      list[0]= 0;
      vertexNeighbours[i]= list;
      pos += neighbourCount[i]+1;
   }

   // find all unique edges, for each triangle: (i1,i2) (i2,i3) (i3,i1)
   // this process will create e1,e2,e3 so we can build the new index buffer on the fly
   dstIndices.init(numIndices*4); // each triangle (3 indices) becomes 4 triangles (12 indices)
   Array<SharedEdge> edges(numIndices); // typically about 1.5 * numTriangles
   Array<EdgeMap>* edgeMap= new Array<EdgeMap>[numVerts];
   for (i=0; i<numVerts; i++) edgeMap[i].init(neighbourCount[i]);

   for (i=0; i<numIndices; i+=3)
   {
      const int i1= srcIdx[i+0];
      const int i2= srcIdx[i+1];
      const int i3= srcIdx[i+2];

      int e1= addEdge(edgeMap, edges, vertexNeighbours, i1,i2,  i3);
      int e2= addEdge(edgeMap, edges, vertexNeighbours, i2,i3,  i1);
      int e3= addEdge(edgeMap, edges, vertexNeighbours, i3,i1,  i2);

      e1+=numVerts;
      e2+=numVerts;
      e3+=numVerts;

      dstIndices.add(i1); dstIndices.add(e1); dstIndices.add(e3);
      dstIndices.add(i2); dstIndices.add(e2); dstIndices.add(e1);
      dstIndices.add(i3); dstIndices.add(e3); dstIndices.add(e2);
      dstIndices.add(e1); dstIndices.add(e2); dstIndices.add(e3);
   }

   // smooth old vertices
   const int numEdges= edges.size();
   dstVertices.init(numVerts + numEdges);
   for (i=0; i<numVerts; i++)
   {
      Vector3 v(0.0f, 0.0f, 0.0f);

      int* list= vertexNeighbours[i];
      const int n= list[0];
      for (int j=0; j<n; j++)
      {
         int nIndex= list[j+1];
         v+= srcVtx[ nIndex ];
      }

      float b;
      if (n>3)
         b= 3.0f / (8.0f * n);
      else
         b= 3.0f / 16.0f;

      v= v*b + srcVtx[i]*(1.0f-n*b);

      dstVertices.add(v);
   }

   // create new vertices
   for (i=0; i<numEdges; i++)
   {
      Vector3 v;
      const SharedEdge& e= edges[i];
      const Vector3& v1= srcVtx[e.i1];
      const Vector3& v2= srcVtx[e.i2];
      const Vector3& v3= srcVtx[e.i3];

      if (e.i4 == -1)
      {
         v = (v1 + v2) * 0.4285f + v3 * 0.143f;
      }
      else
      {
         const Vector3& v4= srcVtx[e.i4];
         v = (v1 + v2) * 0.375f + (v3 + v4) * 0.125f;
      }

      dstVertices.add(v);
   }

   // average edges per index
   total= 0;
   int max= 0;
   for (i=0; i<numVerts; i++)
   {
      const Array<EdgeMap>& map= edgeMap[i];
      int size= map.size();
      total += size;
      if (size > max) max=size;
   }

   delete[] edgeMap;
   delete[] buffer;

   // qDebug("vertices: %d -> %d", numVerts, dstVertices.size());
   // qDebug("triangles:%d -> %d", numIndices/3, dstIndices.size()/3);
   // qDebug("edges:    %d -> ~%f (%d)", edges.size(), (float)total/numVerts, max);
}

