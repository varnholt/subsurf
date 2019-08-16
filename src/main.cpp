#include <QApplication>

#include "glwindow.h"
#include "gldevice.h"
#include "mesh.h"
#include "objloader.h"
#include "vector3.h"
#include "vector2.h"


// vertex buffer
unsigned int vertexBuffer;
unsigned int indexBuffer;
int indexCount;
Mesh* baseMesh;
Mesh* faceMesh;
float rotX = 0.0f, rotY = 0.0f, posX = 0.0f, posY = 0.0f, posZ = -50.0f;


struct Vertex
{
   Vector3 pos;
   Vector3 normal;
   float  u,v;
};


// helper function to set up the opengl projection matrix
void setPerspective(float scale, float zNear, float zFar)
{
   const auto aspect = 16.0f / 9.0f;

   auto ymin = -zNear * scale;
   auto ymax = -ymin;

   auto xmax = ymax * aspect;
   auto xmin = ymin * aspect;

   glFrustum( xmin, xmax, ymin, ymax, zNear, zFar );
}



void initDemo()
{
   baseMesh = loadObj("data/face.obj");
   if (!baseMesh)
   {
      qFatal("obj file not found!");
   }

   baseMesh->symmetryX(0, 0.0f, 0.04f);
   faceMesh = baseMesh;

   for (auto i = 0; i < 3; i++)
   {
      auto temp = faceMesh;
      faceMesh = new Mesh();
      faceMesh->subDivide(temp);

      if (temp != baseMesh)
      {
         delete temp;
      }
   }

}

void drawSurface(Mesh* mesh)
{
   auto idx = mesh->getIndexData();
   const auto idxCount = mesh->getIndexCount();
   auto pos = mesh->getVertexData();

   glBegin(GL_TRIANGLES);

   for (auto i = 0; i < idxCount; i++)
   {
      auto index = idx[i];
      auto& v = pos[index];
      glVertex3fv(v);
   }

   glEnd();
}


void drawWireframe(Mesh* mesh)
{
   glBegin(GL_LINES);

   auto idx = mesh->getIndexData();
   const auto idxCount = mesh->getIndexCount();
   auto vtx = mesh->getVertexData();

   for (auto i = 0; i<idxCount; i += 3)
   {
      auto i1 = idx[i];
      auto i2 = idx[i + 1];
      auto i3 = idx[i + 2];

      const auto& v1 = vtx[i1];
      const auto& v2 = vtx[i2];
      const auto& v3 = vtx[i3];

      glVertex3fv( v1 ); glVertex3fv( v2 );
      glVertex3fv( v2 ); glVertex3fv( v3 );
      glVertex3fv( v3 ); glVertex3fv( v1 );
   }

   glEnd();
}


void drawDemoFrame(float /*time*/)
{
   // clear back-buffer & z-buffer
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   // projection: transform from world space to camera space)
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   setPerspective(
      0.5,   // zoom factor
      1.0,   // smallest z
      100.0f // largest z
   );

   // modelview matrix: transform object into 3d space
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();

   // move object back a little
   glTranslatef(posX, posY, posZ);

   glRotatef(rotX, 1, 0, 0);
   glRotatef(rotY, 0, 1, 0);

   glColor4f(0, 1, 0, 1);
   drawSurface(faceMesh);

   glColor4f(1, 0.3f, 0, 1);
   drawWireframe(faceMesh);

   // draw base mesh (wireframe)
   glColor4f(0, 0, 0, 1);
   drawWireframe(baseMesh);
}


int main(int argc, char **argv)
{
   QApplication app(argc, argv);

   QGLFormat format= QGLFormat::defaultFormat();
   format.setSampleBuffers(true);
   auto win = new GLWindow(0, format);
   win->resize(1024,576);
   win->show();

   auto res = app.exec();
   delete win;
   return res;
}
