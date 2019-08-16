#pragma once

#include <QGLWidget>
#include <QTime>
#include <QTimer>

class QMouseEvent;
class QWheelEvent;

class GLWindow : public QGLWidget
{
   Q_OBJECT

public:
   GLWindow(QWidget* parent, const QGLFormat& format);

protected:
   void initializeGL();
   void resizeGL(int w, int h);
   void paintGL();
   void mousePressEvent(QMouseEvent* me);
   void mouseReleaseEvent(QMouseEvent* me);
   void mouseMoveEvent(QMouseEvent* me);
   void wheelEvent(QWheelEvent* we);

private:
   QTime  mTime;
   QTimer mAnimate;
   QPoint mMousePos;
   bool   mReloadFlag = false;
   bool   mRotate = false;
   bool   mMove = false;
};


