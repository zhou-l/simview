#pragma once
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <vector>
#include "Eigen/SparseCore"

typedef Eigen::SparseMatrix<double> SpDbMat;

class QGLAdjMatrixView : public QOpenGLWidget, protected QOpenGLFunctions
{
	Q_OBJECT
public:
	QGLAdjMatrixView(QWidget* parent);
	~QGLAdjMatrixView();

	void initializeGL();
	void resizeGL(int width, int height);
	void paintGL();

	void init();

	
private:
	void destroyTexList();
	void setAdjMatrix(const octree* octTree, int level, Eigen::MatrixXd& M);

	std::vector<GLuint>  _texAdjMatrixList; // array of textures storing all levels of the adjacency matrices

	// The list of adjacent matrices for graph representation
	std::vector<Eigen::MatrixXf> _adjMatList; 
	// sparse matrix version for later extension
	std::vector<SpDbMat>         _adjSpMatList;
};

