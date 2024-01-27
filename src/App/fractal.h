#pragma once

#include <Base/GLWindow.h>

#include <QMatrix4x4>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLWidget>
#include <QQuaternion>
#include <QVector2D>
#include <QVector3D>

#include <memory>

class MandelbrotWidget final : public QOpenGLWidget,
							   protected QOpenGLFunctions
{
	Q_OBJECT
public:
	constexpr static int MIN_ITERS = 10;
	constexpr static int MAX_ITERS = 1000;
	constexpr static int DEFAULT_ITERS = 100;

	constexpr static int MIN_C_X = -1000;
	constexpr static int MAX_C_X = 1000;
	constexpr static int DEFAULT_C_X = -800; //-800

	constexpr static int MIN_C_Y = -1000;
	constexpr static int MAX_C_Y = 1000;
	constexpr static int DEFAULT_C_Y = 156; //156

	constexpr static int MIN_R0 = 0;
	constexpr static int MAX_R0 = 100;
	constexpr static int DEFAULT_R0 = 50; //50

	constexpr static int MIN_G0 = 0;
	constexpr static int MAX_G0 = 100;
	constexpr static int DEFAULT_G0 = 95; //95

	constexpr static int MIN_B0 = 0;
	constexpr static int MAX_B0 = 100;
	constexpr static int DEFAULT_B0 = 30; //30

public:
	MandelbrotWidget();

	void initializeGL() override;
	void paintGL() override;

signals:
	void updateFPS(float);

public slots:
	void setIters(int iters);
	void setc_x(int x);
	void setR(int x);
	void setG(int x);
	void setB(int x);
	void setc_y(int y);
	void fpsTimer();

protected:
	void wheelEvent(QWheelEvent *event) override;
	void mouseMoveEvent(QMouseEvent *event) override;
	void mousePressEvent(QMouseEvent *event) override;

private:
	GLint itersUniform_ = -1;
	GLint windowSizeUniform_ = -1;
	GLint sizesUniform_ = -1;
	GLint fromPosUniform_ = -1;
	GLint cxUniform_ = -1;
	GLint cyUniform_ = -1;
	GLint r0Uniform_ = -1;
	GLint g0Uniform_ = -1;
	GLint b0Uniform_ = -1;

	QVector2D cord = {0.0, 0.0};

	size_t framePoint_ = 0;
	int iters_ = DEFAULT_ITERS;
	int c_x_ = DEFAULT_C_X;
	int c_y_ = DEFAULT_C_Y;
	int r0_ = DEFAULT_R0;
	int g0_ = DEFAULT_G0;
	int b0_ = DEFAULT_B0;

	QVector2D fromPos_{-0.789136f, -0.150316f};
	GLfloat sizeX = 2.0;

	GLfloat zoomSpeed = 1.0;

	QOpenGLBuffer vbo_{QOpenGLBuffer::Type::VertexBuffer};
	QOpenGLBuffer ibo_{QOpenGLBuffer::Type::IndexBuffer};
	QOpenGLVertexArrayObject vao_;

	std::unique_ptr<QOpenGLShaderProgram> program_ = nullptr;

	QTimer* updateTimer_ {};
	QTimer* frameTimer_ {};
	std::chrono::steady_clock::time_point lastFrame_;

	size_t frame_ = 0;

	QVector2D mousePressPosition_{0., 0.};
	QVector2D positionChange_{0., 0.};

	bool animate_ = false;
};