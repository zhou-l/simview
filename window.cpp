#include "global.h"
#include "window.h"
#include <QDebug>
#include <QString>
#include <QOpenGLDebugLogger>
#include <QOpenGLShaderProgram>
#include <QOpenGLDebugMessage>
#include <QKeyEvent>
#include "vertex.h"
#include "input.h"
#include "profiler.h"
#include "debugdraw.h"
#include "frameresult.h"
#include "dataloader.h"
#include "dataAnalyzer.h"
//#include "RegularGrid.h"
#include <iostream>
// Custom includes
#include <OpenGLError>

//#include <OpenGLShaderProgram>

using namespace std;

// Front Verticies
#define VERTEX_FTR Vertex( QVector3D( 0.5f,  0.5f,  0.5f), QVector3D( 1.0f, 0.0f, 0.0f ) )
#define VERTEX_FTL Vertex( QVector3D(-0.5f,  0.5f,  0.5f), QVector3D( 0.0f, 1.0f, 0.0f ) )
#define VERTEX_FBL Vertex( QVector3D(-0.5f, -0.5f,  0.5f), QVector3D( 0.0f, 0.0f, 1.0f ) )
#define VERTEX_FBR Vertex( QVector3D( 0.5f, -0.5f,  0.5f), QVector3D( 0.0f, 0.0f, 0.0f ) )

// Back Verticies
#define VERTEX_BTR Vertex( QVector3D( 0.5f,  0.5f, -0.5f), QVector3D( 1.0f, 1.0f, 0.0f ) )
#define VERTEX_BTL Vertex( QVector3D(-0.5f,  0.5f, -0.5f), QVector3D( 0.0f, 1.0f, 1.0f ) )
#define VERTEX_BBL Vertex( QVector3D(-0.5f, -0.5f, -0.5f), QVector3D( 1.0f, 0.0f, 1.0f ) )
#define VERTEX_BBR Vertex( QVector3D( 0.5f, -0.5f, -0.5f), QVector3D( 1.0f, 1.0f, 1.0f ) )

// Create a colored cube
static const Vertex sg_vertexes[] = {
  // Face 1 (Front)
    VERTEX_FTR, VERTEX_FTL, VERTEX_FBL,
    VERTEX_FBL, VERTEX_FBR, VERTEX_FTR,
  // Face 2 (Back)
    VERTEX_BBR, VERTEX_BTL, VERTEX_BTR,
    VERTEX_BTL, VERTEX_BBR, VERTEX_BBL,
  // Face 3 (Top)
    VERTEX_FTR, VERTEX_BTR, VERTEX_BTL,
    VERTEX_BTL, VERTEX_FTL, VERTEX_FTR,
  // Face 4 (Bottom)
    VERTEX_FBR, VERTEX_FBL, VERTEX_BBL,
    VERTEX_BBL, VERTEX_BBR, VERTEX_FBR,
  // Face 5 (Left)
    VERTEX_FBL, VERTEX_FTL, VERTEX_BTL,
    VERTEX_FBL, VERTEX_BTL, VERTEX_BBL,
  // Face 6 (Right)
    VERTEX_FTR, VERTEX_FBR, VERTEX_BBR,
    VERTEX_BBR, VERTEX_BTR, VERTEX_FTR
};

#undef VERTEX_BBR
#undef VERTEX_BBL
#undef VERTEX_BTL
#undef VERTEX_BTR

#undef VERTEX_FBR
#undef VERTEX_FBL
#undef VERTEX_FTL
#undef VERTEX_FTR

// Define our profiler access
#define GL_PROFILER m_profiler

const float Z_NEAR = 0.01f;
const float Z_FAR = 500.0f;
const float FOV = 90.0f; // field of view in degrees
const FLOATVECTOR3 CAM_POS(0.0f, 0.0f, 2.0f);
QPoint curMousePos;
QPoint prevMousePos;

/*******************************************************************************
 * OpenGL Events
 ******************************************************************************/

Window::Window() :
  m_frameCount(0)
{
#ifdef    GL_DEBUG
  m_profiler = new Profiler(this);
  m_profiler->setBorder(10, 10, 0, 10);
  m_profiler->setOffset(0.0f, 0.0f, 0.95f, 0.0f);
  Profiler::setProfiler(m_profiler);
#endif // GL_DEBUG
  //m_camera.translate(CAM_POS.x, CAM_POS.y, CAM_POS.z);
  OpenGLError::pushErrorHandler(this);
  m_frameTimer.start();

  m_dataLoader = new DataLoader();
  m_dataAnalyzer = new DataAnalyzer();
}

Window::~Window()
{
    SAFE_DELETE(m_dataLoader);
    SAFE_DELETE(m_dataAnalyzer);

  makeCurrent();
  OpenGLError::popErrorHandler();
}
void Window::initializeGL()
{
	// load data
	vector<Vertex> verts;
	if (loadData())
	{
		cout << "Data loaded successfully! Num verts = " << g_params.vertices().size() << endl;
	}

	verts = g_params.vertices();
	// compute the proper scale/bias matrix for the vertices to place them in BBox [-0.5->0.5]
	//FLOATAABB3D aabb;
	//aabb.vMin = FLOATVECTOR3(FLT_MAX, FLT_MAX, FLT_MAX);
	//aabb.vMax = FLOATVECTOR3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
	//for (size_t i = 0; i < verts.size(); i++)
	//{
	//	aabb.vMin.x = MIN(aabb.vMin.x, verts[i].position().x());
	//	aabb.vMin.y = MIN(aabb.vMin.y, verts[i].position().y());
	//	aabb.vMin.z = MIN(aabb.vMin.z, verts[i].position().z());

	//	aabb.vMax.x = MAX(aabb.vMax.x, verts[i].position().x());
	//	aabb.vMax.y = MAX(aabb.vMax.y, verts[i].position().y());
	//	aabb.vMax.z = MAX(aabb.vMax.z, verts[i].position().z());
	//}
	//FLOATVECTOR3 aabbSize = aabb.vMax - aabb.vMin;
	//FLOATVECTOR3 invAAbbSize = FLOATVECTOR3(aabbSize.x == 0.f ? 0.f : 1.f / aabbSize.x, aabbSize.y == 0.f ? 0.f : 1.f / aabbSize.y, aabbSize.z == 0.f ? 0.f : 1.f / aabbSize.z);
	//FLOATVECTOR3 aabbCtr = 0.5f * aabbSize + aabb.vMin;
	//cout << "AABB of vertices = " << aabb.vMin << ";" << aabb.vMax << endl;
	//cout << "AABB center = " << aabbCtr << endl;
	//m_transform.setTranslation(QVector3D(-aabbCtr.x, -aabbCtr.y, -aabbCtr.z));

	//m_transform.setScale(1.f*invAAbbSize.x, 1.f * invAAbbSize.y, 1.f * invAAbbSize.z);
	// Initialize OpenGL Backend
	initializeOpenGLFunctions();
	connect(context(), SIGNAL(aboutToBeDestroyed()), this, SLOT(teardownGL()), Qt::DirectConnection);
	connect(this, SIGNAL(frameSwapped()), this, SLOT(update()));
	printVersionInformation();

	// Set global information
	glEnable(GL_CULL_FACE);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

#ifdef    GL_DEBUG
	m_debugLogger = new QOpenGLDebugLogger(this);
	if (m_debugLogger->initialize())
	{
		qDebug() << "GL_DEBUG Debug Logger" << m_debugLogger << "\n";
		connect(m_debugLogger, SIGNAL(messageLogged(QOpenGLDebugMessage)), this, SLOT(messageLogged(QOpenGLDebugMessage)));
		m_debugLogger->startLogging();
	}
	else
	{
		qDebug() << "GL_DEBUG Debug Logger (NONE)\n";
	}
	connect(m_profiler, SIGNAL(onFrameResult(FrameResult)), this, SLOT(onFrameResult(FrameResult)));
#endif // GL_DEBUG

	// Application-specific initialization
	{
		// Create Shader (Do not release until VAO is created)
		//m_program = new OpenGLShaderProgram(this);
		m_program = new QOpenGLShaderProgram(this);
		m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/simple.vert");
		m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/simple.frag");
		m_program->link();
		m_program->bind();

		// Cache Uniform Locations
		u_modelToWorld = m_program->uniformLocation("modelToWorld");
		u_worldToCamera = m_program->uniformLocation("worldToCamera");
		u_cameraToView = m_program->uniformLocation("cameraToView");

		// Create Buffer (Do not release until VAO is created)
		m_vertex.create();
		m_vertex.bind();
		m_vertex.setUsagePattern(QOpenGLBuffer::StaticDraw);
		//m_vertex.allocate(&sg_vertexes, sizeof(sg_vertexes));
		m_vertex.allocate(&verts[0], verts.size() * sizeof(Vertex));


		// Create Vertex Array Object
		m_object.create();
		m_object.bind();
		m_program->enableAttributeArray(0);
		m_program->enableAttributeArray(1);
		m_program->setAttributeBuffer(0, GL_FLOAT, Vertex::positionOffset(), Vertex::PositionTupleSize, Vertex::stride());
		m_program->setAttributeBuffer(1, GL_FLOAT, Vertex::colorOffset(), Vertex::ColorTupleSize, Vertex::stride());

		// Release (unbind) all
		m_object.release();
		m_vertex.release();
		m_program->release();


		m_vertexAddOn.create();
		m_vertexAddOn.bind();
		m_vertexAddOn.setUsagePattern(QOpenGLBuffer::StaticDraw);
		m_vertexAddOn.allocate(&sg_vertexes, sizeof(sg_vertexes));
		//m_vertexAddOn.allocate(&verts[0], verts.size() * sizeof(Vertex));
		m_objectAddOn.release();
		m_vertexAddOn.release();
	}

	DebugDraw::initialize(this);
}

void Window::resizeGL(int width, int height)
{
  m_projection.setToIdentity();
  m_projection.perspective(FOV * 0.5f, width / float(height), Z_NEAR, Z_FAR);
  PROFILER_RESIZE_GL(width, height);
}

void Window::paintGL()
{

  // Clear
  PROFILER_SYNC_FRAME();
  glClearColor(1, 1, 1, 1);
  glClear(GL_COLOR_BUFFER_BIT);

  // Render the scene
  PROFILER_PUSH_GPU_MARKER("Render Scene");
  {
    PROFILER_PUSH_GPU_MARKER("Prepare Scene");
    m_program->bind();
    m_program->setUniformValue(u_worldToCamera, m_camera.toMatrix());
    m_program->setUniformValue(u_cameraToView, m_projection);
    PROFILER_POP_GPU_MARKER();
    {
		QMatrix4x4 model2world;
		model2world.setToIdentity();
      PROFILER_PUSH_GPU_MARKER("Draw Object");
      m_object.bind();
      m_program->setUniformValue(u_modelToWorld, m_transform.toMatrix());
	  //m_program->setUniformValue(u_modelToWorld, model2world);
      //glDrawArrays(GL_TRIANGLES, 0, sizeof(sg_vertexes) / sizeof(sg_vertexes[0]));
      //glDrawArrays(GL_TRIANGLES, 0, g_params.vertices().size());
      glDrawArrays(GL_POINTS, 0, GLsizei(g_params.vertices().size()));
      m_object.release();
      PROFILER_POP_GPU_MARKER();
    }
    m_program->release();
  }
  PROFILER_POP_GPU_MARKER();
  PROFILER_EMIT_RESULTS();

  // Render the profiler
  PROFILER_PAINT_GL();
  DebugDraw::draw();

}

void Window::teardownGL()
{
  // Actually destroy our OpenGL information
  m_object.destroy();
  m_vertex.destroy();
}

bool Window::loadData()
{
    QString filePrefix = tr("C:\\MyData\\Utah_heart_ischema\\201701_Conductivity\\mesh\\");
    QString fileName = tr("heartPts.csv"); //tr("heartPts.csv");
    vector<Vertex> verts;
    // Load mesh file
    cout << "Loading mesh data...";
    bool fileLoaded = m_dataLoader->loadCSVtoPointCloud(filePrefix+fileName);
    cout << "finished!" << endl;
    if(!fileLoaded || m_dataLoader->attrib_names().size() < 3)
    {
         verts.assign(sg_vertexes, sg_vertexes + sizeof(sg_vertexes)/sizeof(Vertex));
         g_params.setVertices(verts);
         cout << "Failed to load csv data!" << endl;
         return false;
    }
    g_params.setPointData(m_dataLoader->pointData());

    // Load run files
    cout << "Loading ensemble runs...";
    QString runFileFolder = tr("C:/MyData/Utah_heart_ischema/201701_Conductivity/simRuns/dataset_test");
    bool runFilesLoaded = m_dataLoader->loadEnsembleRunsTxt(runFileFolder);
    g_params.setEnsembleData(m_dataLoader->ensembleData());
    cout << "finished!" << endl;
    //clean data in dataloader
    m_dataLoader->cleanData();



    // Convert to point data
    cout << "Creating vertices for rendering...";
    vector<vector<float> > pointData = g_params.pointData();
    verts.resize(g_params.pointData().size());
    // prepare for KD-tree data
    vector<FLOATVECTOR3> fvPtData(pointData.size());
    for(vector<vector<float> >::const_iterator IT = pointData.begin(); IT!= pointData.end(); ++IT)
    {
        size_t id = IT - pointData.begin();
        FLOATVECTOR3 fv = FLOATVECTOR3((*IT)[0], (*IT)[1], (*IT)[2]);
        fvPtData[id] = fv;
        QVector3D posV = QVector3D(fv.x, fv.y, fv.z)/50.0f;
        QVector3D colV = QVector3D(abs(posV.x()), abs(posV.y()), abs(posV.z())) ;
        verts[id] = Vertex(posV, colV);
    }
    g_params.setVertices(verts);
    cout << "finished!" << endl;

    // Rasterize to regular grid
    vector<UINT64VECTOR3> posTable;
    UINT64VECTOR3 gridDim(128, 128, 128);
    VolumeData* ptCntVol = NULL;
    m_dataAnalyzer->convertMeshIdToGridPos(posTable, &ptCntVol, fvPtData, gridDim);
    // convert ensembles to volumes
    vector<VolumeData*> ensembleVols;
    m_dataAnalyzer->convertEnsembleRunsToVol(ensembleVols, g_params.ensembleData(), gridDim, posTable, *ptCntVol);
    // set to global varaible
    g_params.setEnsembleVols(ensembleVols);

    // Write out ensemble volumes
    QString runVolFilePrefix = runFileFolder + "/run";
    for (size_t i = 0; i < ensembleVols.size(); i++)
    {
        QString filename = runVolFilePrefix + QString("%1").arg(i);
        ensembleVols[i]->writeToFile(filename.toStdString().c_str(), NULL);
    }
	// Build ensemble octree
	
	m_dataAnalyzer->createEnsembleOctree(ensembleVols/*g_params.ensembleVols()*/, g_params.ensembleOctree());


    // build KD-tree
    KD<spatialDataPt*>* meshKDtree = new KD<spatialDataPt*>(3);
    m_dataAnalyzer->buildKDtree(fvPtData, *meshKDtree);
    cout << "KD tree node #= " << meshKDtree->size()<<endl;
    g_params.meshKDTree(*meshKDtree);
    cout << "global KD tree node #= " << g_params.meshKDTree_unsafe().size() << endl;

    return true;
}


void Window::analyzeData()
{
    // build octree and compute statistics for blocks
}

QVector3D Window::get_arcball_vector(int x, int y)
{
    QVector3D P(float(x) / float(width()) * 2.0f - 1.0f, float(y) / float(height()) * 2.0f - 1.0f, 0.0f);
    P.setY(-P.y());
    float OP_squared = P.x() * P.x() + P.y() * P.y();
    if (OP_squared <= 1.0f)
        P.setZ(sqrtf(1.0f - OP_squared));
    else
        P.normalize();
    return P;
}

void Window::update()
{
  // Update FPS
  if (++m_frameCount > FPSFrameDelay)
  {
    qint64 ms = m_frameTimer.elapsed();
    int frames = m_frameCount / (ms / 1000.0f);
    setTitle(QString("FPS: %1").arg(frames));
    m_frameCount = 0;
    m_frameTimer.start();
  }

  // Update input
  Input::update();

  // Camera Transformation
  static const float transSpeed = 0.2f;
  if (curMousePos != prevMousePos)
  {
	  if(Input::buttonPressed(Qt::RightButton))
	  {
		  QVector2D Pa(float(curMousePos.x()) / float(width()) * 2.0f - 1.0f, 
			  float(curMousePos.y()) / float(height()) * 2.0f - 1.0f);
		  QVector2D Pb(float(prevMousePos.x()) / float(width()) * 2.0f - 1.0f,
			  float(prevMousePos.y()) / float(height()) * 2.0f - 1.0f);

		  QVector2D Pab = Pa - Pb;
		  Pab.setY(-Pab.y());
		  QVector3D translation = m_camera.LocalUp * Pab.y();
		  translation += m_camera.LocalRight * Pab.x();
		  m_camera.translate(transSpeed * translation);
	

	  }
	  else if(Input::buttonPressed(Qt::LeftButton))
	  {

		  //static const float rotSpeed   = 0.5f;

		  // Handle rotations using the arcball method

		  QPoint curPos = curMousePos;
		  QPoint lastPos = prevMousePos;

		  QVector3D va = get_arcball_vector(curPos.x(), curPos.y());
		  QVector3D vb = get_arcball_vector(lastPos.x(), lastPos.y());

		  float dotProd = QVector3D::dotProduct(vb, va);
		  float angle = acosf(MIN(1.0f, dotProd)) *180.0f / M_PI;
		  QVector3D axis_in_camera_coord = QVector3D::crossProduct(vb, va);
		  //QMatrix4x4 cam2obj = m_camera.toMatrix().inverted() * m_transform.toMatrix();
		  //QVector3D axis_in_obj_coord = cam2obj * axis_in_camera_coord;
		     //m_transform.rotate(angle, axis_in_obj_coord);
		  m_camera.rotate(QQuaternion(dotProd, axis_in_camera_coord));
		  prevMousePos = curMousePos;
	  }
  }

  
  // Handle translations
  if (Input::keyPressed(Qt::Key_R))
  {
	  m_camera.reset();
  }
  else
  {
	  QVector3D translation;
	  if (Input::keyPressed(Qt::Key_W))
	  {
		  translation -= m_camera.LocalForward;//m_camera.forward();
	  }
	  if (Input::keyPressed(Qt::Key_S))
	  {
		  translation += m_camera.LocalForward; //m_camera.forward();
	  }
	  if (Input::keyPressed(Qt::Key_A))
	  {
		  translation += m_camera.LocalRight; //m_camera.right();
	  }
	  if (Input::keyPressed(Qt::Key_D))
	  {
		  translation -= m_camera.LocalRight; //m_camera.right();
	  }
	  if (Input::keyPressed(Qt::Key_Q))
	  {
		  translation -= m_camera.LocalUp; //m_camera.up();
	  }
	  if (Input::keyPressed(Qt::Key_E))
	  {
		  translation += m_camera.LocalUp; //m_camera.up();
	  }
	  m_camera.translate(transSpeed * translation);


  }
 
  // Update instance information
  //m_transform.rotate(1.0f, QVector3D(0.4f, 0.3f, 0.3f));

  // Schedule a redraw
  QOpenGLWindow::update();
}

void Window::messageLogged(const QOpenGLDebugMessage &msg)
{
  QString error;

  // Format based on severity
  switch (msg.severity())
  {
  case QOpenGLDebugMessage::NotificationSeverity:
    error += "--";
    break;
  case QOpenGLDebugMessage::HighSeverity:
    error += "!!";
    break;
  case QOpenGLDebugMessage::MediumSeverity:
    error += "!~";
    break;
  case QOpenGLDebugMessage::LowSeverity:
    error += "~~";
    break;
  case QOpenGLDebugMessage::InvalidSeverity:
  case QOpenGLDebugMessage::AnySeverity:
    error += "??";
    break;
  }

  error += " (";

  // Format based on source
#define CASE(c) case QOpenGLDebugMessage::c: error += #c; break
  switch (msg.source())
  {
    CASE(APISource);
    CASE(WindowSystemSource);
    CASE(ShaderCompilerSource);
    CASE(ThirdPartySource);
    CASE(ApplicationSource);
    CASE(OtherSource);
    CASE(InvalidSource);
    CASE(AnySource);
  }
#undef CASE

  error += " : ";

  // Format based on type
#define CASE(c) case QOpenGLDebugMessage::c: error += #c; break
  switch (msg.type())
  {
    CASE(ErrorType);
    CASE(DeprecatedBehaviorType);
    CASE(UndefinedBehaviorType);
    CASE(PortabilityType);
    CASE(PerformanceType);
    CASE(OtherType);
    CASE(MarkerType);
    CASE(GroupPushType);
    CASE(GroupPopType);
    CASE(AnyType);
    CASE(InvalidType);
  }
#undef CASE

  error += ")";
  qDebug() << qPrintable(error) << "\n" << qPrintable(msg.message()) << "\n";
}

void Window::onFrameResult(const FrameResult &result)
{
  (void)result;
}

bool Window::event(QEvent *e)
{
  if (e->type() == OpenGLError::type())
  {
    errorEventGL(static_cast<OpenGLError*>(e));
    return true;
  }
  return QOpenGLWindow::event(e);
}

void Window::errorEventGL(OpenGLError *event)
{
  qFatal("%s::%s => Returned an error!", event->callerName(), event->functionName());
}

void Window::keyPressEvent(QKeyEvent *event)
{
  if (event->isAutoRepeat())
  {
    event->ignore();
  }
  else
  {
    Input::registerKeyPress(event->key());
  }
}

void Window::keyReleaseEvent(QKeyEvent *event)
{
  if (event->isAutoRepeat())
  {
    event->ignore();
  }
  else
  {
    Input::registerKeyRelease(event->key());
  }
}

void Window::mousePressEvent(QMouseEvent *event)
{
  Input::registerMousePress(event->button());
  curMousePos = event->pos();
  prevMousePos = curMousePos;
}

void Window::mouseReleaseEvent(QMouseEvent *event)
{
  Input::registerMouseRelease(event->button());
}

void Window::mouseMoveEvent(QMouseEvent * event)
{
    if (Input::buttonPressed(Qt::LeftButton) || Input::buttonPressed(Qt::RightButton))
        curMousePos = event->pos();
}

void Window::wheelEvent(QWheelEvent * event)
{
    float fdelta = event->delta()/1000.0f;
	QVector3D translation = QVector3D(0, 0, fdelta);/*m_camera.forward() * fdelta*/;

    m_camera.translate(translation);
    update();
}

void Window::moveEvent(QMoveEvent *ev)
{

  PROFILER_MOVE_EVENT(ev);
  QOpenGLWindow::moveEvent(ev);
}

/*******************************************************************************
 * Private Helpers
 ******************************************************************************/

void Window::printVersionInformation()
{
  QString glType;
  QString glVersion;
  QString glProfile;

  // Get Version Information
  glType = (context()->isOpenGLES()) ? "OpenGL ES" : "OpenGL";
  glVersion = reinterpret_cast<const char*>(glGetString(GL_VERSION));

  // Get Profile Information
#define CASE(c) case QSurfaceFormat::c: glProfile = #c; break
  switch (format().profile())
  {
    CASE(NoProfile);
    CASE(CoreProfile);
    CASE(CompatibilityProfile);
  }
#undef CASE

  // qPrintable() will print our QString w/o quotes around it.
  qDebug() << qPrintable(glType) << qPrintable(glVersion) << "(" << qPrintable(glProfile) << ")";
}
