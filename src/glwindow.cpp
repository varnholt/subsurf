#include "glwindow.h"
#include "gldevice.h"

#include <QMouseEvent>
#include <QWheelEvent>


extern void initDemo();
extern void drawDemoFrame(float time);
extern float rotX, rotY, posX, posY, posZ;


GLWindow::GLWindow(QWidget* parent, const QGLFormat& format)
   : QGLWidget(format, parent)
{
   mAnimate.setInterval(1000 / 60);
   connect(&mAnimate, SIGNAL(timeout()), this, SLOT(update()));
}


void GLWindow::mousePressEvent(QMouseEvent* me)
{
   if (me->button() == Qt::LeftButton)
   {
      mMousePos= me->pos();
      mRotate = true;
   }
   else if (me->button() == Qt::RightButton)
   {
      mMousePos= me->pos();
      mMove = true;
   }
}


void GLWindow::mouseReleaseEvent(QMouseEvent* me)
{
   if (me->button() == Qt::LeftButton)
      mRotate = false;
   else if (me->button() == Qt::RightButton)
      mMove = false;
}


void GLWindow::mouseMoveEvent(QMouseEvent* me)
{
   if (mRotate)
   {
      auto pos= me->pos();
      rotY += (pos.x() - mMousePos.x()) * 0.1f;
      rotX += (pos.y() - mMousePos.y()) * 0.1f;
      mMousePos= pos;
   }
   else if (mMove)
   {
      auto pos= me->pos();
      posX += (pos.x() - mMousePos.x()) * 0.03f;
      posY -= (pos.y() - mMousePos.y()) * 0.03f;
      mMousePos= pos;
   }
}

void GLWindow::wheelEvent(QWheelEvent* we)
{
   posZ += we->delta() * 0.01f;
}


void GLWindow::initializeGL()
{
   if (!initExtensions())
   {
      qDebug("Ok, we're doomed and will crash in a moment...");
   }

   // init some global gl settings:
   glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

   // setup the z buffer
   glClearDepth(1.0f);
   glEnable(GL_DEPTH_TEST);
   glDepthMask(GL_TRUE);
   glDepthFunc(GL_LESS);

   // setup alpha test
   glEnable(GL_ALPHA_TEST);
   glAlphaFunc(GL_GREATER, 0.0f);

   // enable backface culling
   glEnable(GL_CULL_FACE);

   glPolygonOffset(1.0, 1.0);
   glEnable(GL_POLYGON_OFFSET_FILL);

   initDemo();

   mTime.start();
   mAnimate.start();
}


void GLWindow::resizeGL(int w, int h)
{
   glViewport(0, 0, w, h);
}


void GLWindow::paintGL()
{
   if (mReloadFlag)
   {
      mReloadFlag = false;
   }

   const auto time = mTime.elapsed() / 1000.0;
   drawDemoFrame(time);
}

