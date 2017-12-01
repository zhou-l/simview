#pragma once
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <vector>

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

	std::vector<GLuint>  _texAdjMatrixList; // array of textures storing all levels of the adjacency matrices

	std::vector<Eigen::Matrix>
};

