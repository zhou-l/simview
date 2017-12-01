#include "QGLAdjMatrixView.h"
#include "global.h"
#include "volume\octree.h"
using namespace std;


QGLAdjMatrixView::QGLAdjMatrixView(QWidget* parent):
	QOpenGLWidget(parent)
{

	initializeGL();
}


QGLAdjMatrixView::~QGLAdjMatrixView()
{
}

void QGLAdjMatrixView::initializeGL()
{
	makeCurrent();
	initializeOpenGLFunctions();
}

void QGLAdjMatrixView::resizeGL(int width, int height)
{
}

void QGLAdjMatrixView::paintGL()
{
	QOpenGLWidget::paintGL();
}

void QGLAdjMatrixView::init()
{
	// Destroy the list of textures if there's anything inside
	if (!_texAdjMatrixList.empty())
	{
		destroyTexList();
		// destroy CPU storage
		_adjMatList.clear();
	}

	// get data from the octree
	octree* oct_tree = g_params.ensembleOctree();

	// get the dimension of leaf nodes
	int levels = oct_tree->levels();
	// create textures
	_texAdjMatrixList.reserve(levels);

	// create CPU storage
	_adjMatList.reserve(levels);

	// allocate memories
	for (int i = 0; i < levels; i++)
	{
		// create GPU tex handle
		GLuint texId = 0;
		glGenTextures(1, &texId);
		glBindTexture(GL_TEXTURE_2D, texId);
		int nodesPerSide = 1 << i;
		int lw = nodesPerSide * nodesPerSide;
		// texture with a single mipmap level
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, lw, lw, 0, GL_RGBA, GL_FLOAT, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
		_texAdjMatrixList.push_back(texId);

		// allocate CPU memories
		Eigen::MatrixXf M(lw, lw);
		_adjMatList.push_back(M);

	}

	// set adjacent matrix content
	oct_tree->bfsSetLevelNodesInfo(_adjMatList);

	// upload to the GPU

	for (int i = 0; i < levels; i++)
	{
		int nodesPerSide = 1 << i;
		int lw = nodesPerSide * nodesPerSide;
		GLuint texId = _texAdjMatrixList[i];
		glBindTexture(GL_TEXTURE_2D, texId);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, lw, lw, GL_RGBA, GL_FLOAT, _adjMatList.at(i).data());
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void QGLAdjMatrixView::setAdjMatrix(const octree* octTree, int level, Eigen::MatrixXd& M)
{
	
}

void QGLAdjMatrixView::destroyTexList()
{
	for (size_t i = 0; i < _texAdjMatrixList.size(); i++)
	{
		GLuint texid = _texAdjMatrixList.at(i);
		if (texid != 0)
		{
			glBindTexture(GL_TEXTURE_2D, texid);
			glDeleteTextures(1, &texid);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}
	_texAdjMatrixList.clear();
}