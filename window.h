#ifndef WINDOW_H
#define WINDOW_H

#include <QOpenGLWindow>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLDebugMessage>
#include <QMatrix4x4>
#include <QElapsedTimer>
#include "transform3d.h"
#include "camera3d.h"

// Custom Includes
#include <OpenGLBuffer>
#include <OpenGLFunctions>
#include <OpenGLVertexArrayObject>

class Profiler;
class FrameResult;
class OpenGLError;
//class OpenGLShaderProgram;
class QOpenGLShaderProgram;
class QOpenGLDebugLogger;
class QMoveEvent;
class DataLoader;
class Window : public QOpenGLWindow,
               protected OpenGLFunctions
{
  Q_OBJECT

// OpenGL Events
public:
  static const uint64_t FPSFrameDelay = 60;
  Window();
  ~Window();
  void initializeGL();
  void resizeGL(int width, int height);
  void paintGL();
protected slots:
  void teardownGL();
  void update();
  void messageLogged(const QOpenGLDebugMessage &msg);
  void onFrameResult(const FrameResult& result);

protected:
  bool event(QEvent *event);
  void errorEventGL(OpenGLError *event);
  void keyPressEvent(QKeyEvent *event);
  void keyReleaseEvent(QKeyEvent *event);
  void mousePressEvent(QMouseEvent *event);
  void mouseReleaseEvent(QMouseEvent *event);
  void moveEvent(QMoveEvent *ev);

  bool loadData();
private:
  // OpenGL State Information
  OpenGLBuffer m_vertex;
  OpenGLVertexArrayObject m_object;
  //OpenGLShaderProgram *m_program;
  QOpenGLShaderProgram *m_program;
  QOpenGLDebugLogger *m_debugLogger;
  Profiler *m_profiler;
  uint64_t m_frameCount;
  QElapsedTimer m_frameTimer;

  // Shader Information
  int u_modelToWorld;
  int u_worldToCamera;
  int u_cameraToView;
  QMatrix4x4 m_projection;
  Camera3D m_camera;
  Transform3D m_transform;

  // Data loader
  DataLoader* m_dataLoader;

  // Private Helpers
  void printVersionInformation();
};

#endif // WINDOW_H
