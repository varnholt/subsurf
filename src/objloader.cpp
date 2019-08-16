#include "mesh.h"
#include <QQueue>
#include <QFile>
#include <QMap>

// a set of vertex indices (position, normal, uv)
class IndexSet
{
public:
   int mVertex;
   int mNormal;
   int mTexture;

   // compare operator so we can store it in a map
   bool operator < (const IndexSet& other) const
   {
      if (mVertex < other.mVertex)
         return true;
      else if (mVertex > other.mVertex)
         return false;

      if (mNormal < other.mNormal)
         return true;
      else if (mNormal > other.mNormal)
         return false;

      if (mTexture < other.mTexture)
         return true;
      else if (mTexture > other.mTexture)
         return false;

      return false;
   }

};

Mesh* loadObj(const char* filename)
{
   Mesh* mesh= 0;
   QQueue<Vector3> vertexQueue;
   QQueue<Vector3> normalQueue;
   QQueue<Vector2> texcoordQueue;
   QQueue<IndexSet> triangleQueue;

   QFile stream(filename);

   if (!stream.open(QIODevice::ReadOnly))
   {
      qDebug("obj file not found: %s", filename);
      return 0;
   }

   // identifiers
   const QByteArray commentId("#");
   const QByteArray groupId("g ");
   const QByteArray vertexId("v ");
   const QByteArray textureId("vt ");
   const QByteArray normalId("vn ");
   const QByteArray faceId("f ");

   // sequentially load obj file
   while (!stream.atEnd())
   {
      QByteArray line= stream.readLine();

      // skip empty line
      if (line.isEmpty())
         continue;

      // skip comments
      if (line.startsWith(commentId))
         continue;

      // start behind first space
      int start= line.indexOf(' ') + 1;

      if (start<=0) // no space? wrong line.
         continue;

      QByteArray src= line.mid(start);

      // group: name
      if (line.startsWith(groupId))
      {
         // TODO: create new mesh
      }

      // vertex: x y z
      if (line.startsWith(vertexId))
      {
         Vector3 v;
         sscanf(src.data(), "%f %f %f", &v.x, &v.y, &v.z);
         vertexQueue.append(v);
      }

      // normal: x y z
      else if (line.startsWith(normalId))
      {
         Vector3 n;
         sscanf(src.data(), "%f %f %f", &n.x, &n.y, &n.z);
         normalQueue.append(n);
      }

      // texcoord: u v
      else if (line.startsWith(textureId))
      {
         Vector2 uv;
         sscanf(src.data(), "%f %f", &uv.x, &uv.y);
         uv.y= 1.0f - uv.y;
         texcoordQueue.append(uv);
      }

      // face: v/vt/vn v/vt/vn v/vt/vn ...
      else if (line.startsWith(faceId))
      {
         Array<IndexSet> poly(16);  // typically n-gons with n=3,4,5

         int size= line.size();
         while (start < size)
         {
            int prev= start;
            start= line.indexOf(' ', start+1);
            if (start<0)
               start= size;
            QByteArray face= line.mid(prev, start-prev);
            while (start<size && line[start]==' ') start++;

            while (line[size-1]<' ') size--;

            // parse v/vt/vn indices
            {
               int start= 0;
               int size= face.size();
               int num= 0;
               IndexSet indexSet;
               indexSet.mVertex= -1;
               indexSet.mTexture= -1;
               indexSet.mNormal= -1;

               while (start < size)
               {
                  int index= -1;
                  sscanf(face.data() + start, "%d", &index);

                  if (index>=0)
                  {
                     // indices start at 1 (not 0!)
                     index--;
                     switch (num)
                     {
                        case 0: indexSet.mVertex= index; break;
                        case 1: indexSet.mTexture= index; break;
                        case 2: indexSet.mNormal= index; break;
                     }
                  }
                  num++;

                  // start behind next /
                  start= face.indexOf('/', start) + 1;
                  if (start<=0)
                     start= size;
               }
               poly.add(indexSet);
            }
         }

         // triangulate:
         for (int i=2;i<poly.size();i++)
         {
            triangleQueue.append( poly[0] );
            triangleQueue.append( poly[i-1] );
            triangleQueue.append( poly[i] );
         }
      }
   }

   stream.close();

   mesh= new Mesh();
   qDebug("number of vertices: %d ", vertexQueue.size());
   qDebug("number of normals: %d ", normalQueue.size());
   qDebug("number of texcoords: %d ", texcoordQueue.size());
   qDebug("number of triangles: %d ", triangleQueue.size()/3);


   Array<int> uniqueIndices( triangleQueue.size() );

   QMap<IndexSet, int> indexMap;

   // create new index-buffer for every unique vertex <pos, normal, uv>
   for (int tri=0; tri<triangleQueue.size(); tri++)
   {
      int index;
      const IndexSet& indexSet= triangleQueue[tri];

      // find current set of indices in index-map
      QMap<IndexSet, int>::Iterator it= indexMap.find( indexSet );

      // if <index,index,index> already exists: use index
      if ( it != indexMap.end() )
      {
         // if this <int,int,int> already has an index: use it
         index= it.value();
      }
      else
      {
         // ...otherwise add new vertex and index
         index= indexMap.count();
         indexMap.insert( indexSet, index );
      }

      // add new index
      uniqueIndices.add( index );
   }

   // create separate array for position, normal, texcoord - one entry per shared index
   Array<Vector3> uniqueVertices( indexMap.count(), true );
   Array<Vector3> uniqueNormals( indexMap.count(), true );
   Array<Vector2> uniqueTexcoords( indexMap.count(), true );

   for (int tri=0; tri<triangleQueue.size(); tri++)
   {
      const IndexSet& indexSet= triangleQueue[tri];
      int index= uniqueIndices[tri];

      if (indexSet.mVertex>=0)
      {
         uniqueVertices[index]= vertexQueue[ indexSet.mVertex ];
      }
      if (indexSet.mNormal>=0)
      {
         uniqueNormals[index]= normalQueue[ indexSet.mNormal ];
      }
      if (indexSet.mTexture>=0)
      {
         uniqueTexcoords[index]= texcoordQueue[ indexSet.mTexture ];
      }
   }

   if (mesh)
   {
      mesh->setIndices( uniqueIndices );
      mesh->setVertices( uniqueVertices );
      mesh->setNormals( uniqueNormals );
      mesh->setTexcoords( uniqueTexcoords );
   }

   return mesh;
}


