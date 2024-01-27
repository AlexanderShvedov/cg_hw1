#include "fractal.h"

#include <QMouseEvent>
#include <QOpenGLFunctions>
#include <QScreen>
#include <QTimer>
#include <iostream>

#include <array>

namespace
{

constexpr std::array<GLfloat, 8u> vertices = {
	-1.0f, -1.0f,
	1.0f, -1.0f,
	-1.0f, 1.0f,
	1.0f, 1.0f,
};
constexpr std::array<GLuint, 6u> indices = {0, 1, 2, 2, 1, 3};

}// namespace

MandelbrotWidget::MandelbrotWidget()
{
	setFocusPolicy(Qt::StrongFocus);
}

void MandelbrotWidget::initializeGL()
{
	initializeOpenGLFunctions();

	// Configure shaders
	program_ = std::make_unique<QOpenGLShaderProgram>(this);
	program_->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/Shaders/fractal.vs");
	program_->addShaderFromSourceFile(QOpenGLShader::Fragment,
									  ":/Shaders/fractal.fs");
	program_->link();

	// Create VAO object
	vao_.create();
	vao_.bind();

	// Create VBO
	vbo_.create();
	vbo_.bind();
	vbo_.setUsagePattern(QOpenGLBuffer::StaticDraw);
	vbo_.allocate(vertices.data(), static_cast<int>(vertices.size() * sizeof(GLfloat)));

	// Create IBO
	ibo_.create();
	ibo_.bind();
	ibo_.setUsagePattern(QOpenGLBuffer::StaticDraw);
	ibo_.allocate(indices.data(), static_cast<int>(indices.size() * sizeof(GLuint)));

	// Bind attributes
	program_->bind();

	program_->enableAttributeArray(0);
	program_->setAttributeBuffer(0, GL_FLOAT, 0, 2, static_cast<int>(2 * sizeof(GLfloat)));

	itersUniform_ = program_->uniformLocation("iters"); // int
	windowSizeUniform_ = program_->uniformLocation("windowSize"); // vec2
	sizesUniform_ = program_->uniformLocation("sizes"); // vec2
	fromPosUniform_ = program_->uniformLocation("fromPos"); // vec2
	cxUniform_ = program_->uniformLocation("cx"); // int
	cyUniform_ = program_->uniformLocation("cy"); // int
	r0Uniform_ = program_->uniformLocation("r0"); // int
	g0Uniform_ = program_->uniformLocation("g0"); // int
	b0Uniform_ = program_->uniformLocation("b0"); // int

	// Release all
	program_->release();

	vao_.release();

	ibo_.release();
	vbo_.release();

	// Clear all FBO buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Start frame timer
	frameTimer_ = new QTimer;
	connect(frameTimer_, &QTimer::timeout,
			this, &MandelbrotWidget::fpsTimer);
	frameTimer_->setTimerType(Qt::TimerType::PreciseTimer);
	frameTimer_->start(1000);
}

void MandelbrotWidget::paintGL()
{
	// Configure viewport
	const auto retinaScale = devicePixelRatio();
	glViewport(0, 0, static_cast<GLint>(width() * retinaScale),
			   static_cast<GLint>(height() * retinaScale));

	// Clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Calculate position change
	fromPos_ += positionChange_ * sizeX * 0.01;

	// Get window size
	QVector2D windowSize{(GLfloat)width(), (GLfloat)height()};

	// Calculate x and y sizes
	sizeX /= zoomSpeed;

	GLfloat sizeY = sizeX * ((GLfloat) height()) / ((float) width());

	QVector2D sizes{sizeX, sizeY};

	// Bind VAO and shader program
	program_->bind();
	vao_.bind();

	// Update uniform values

	program_->setUniformValue(r0Uniform_, r0_);
	program_->setUniformValue(g0Uniform_, g0_);
	program_->setUniformValue(b0Uniform_, b0_);
	program_->setUniformValue(cxUniform_, c_x_);
	program_->setUniformValue(cyUniform_, c_y_);
	program_->setUniformValue(itersUniform_, iters_);
	program_->setUniformValue(fromPosUniform_, fromPos_);
	program_->setUniformValue(windowSizeUniform_, windowSize);
	program_->setUniformValue(sizesUniform_, sizes);

	// Draw
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

	// Release VAO and shader program
	vao_.release();
	program_->release();

	// Increment frame counter
	++frame_;

	// Update instant FPS counter
	auto time = std::chrono::steady_clock::now();
	auto dur = std::chrono::duration_cast<std::chrono::milliseconds>(time - lastFrame_);
	auto fps = 1000.f / dur.count();
	emit updateFPS(fps);
	lastFrame_ = time;
	update();
}

void MandelbrotWidget::wheelEvent(QWheelEvent *event) 
{
	if (event->angleDelta().y() > 0) {
		sizeX /= 1.1;
	} else if (event->angleDelta().y() < 0) {
		sizeX /= 0.9;
	}
}

template <typename T> float sgn(T val) {
	return (T(0) < val) - (val < T(0));
}

void MandelbrotWidget::mousePressEvent(QMouseEvent *event)
{
	cord = {static_cast<float>(event->x()), static_cast<float>(event->y())};
}

void MandelbrotWidget::mouseMoveEvent(QMouseEvent *event)
{
	if(event->buttons() == Qt::LeftButton)
	{
		QVector2D change = {-sgn(event->x() - cord.x()), sgn(event->y() - cord.y())};
		cord = {static_cast<float>(event->x()), static_cast<float>(event->y())};
		fromPos_ += change * sizeX * 0.007;
	}
}

void MandelbrotWidget::setIters(int iters) {
	iters_ = iters;
	update();
}

void MandelbrotWidget::fpsTimer() {
	framePoint_ = frame_;
}
void MandelbrotWidget::setc_x(int x)
{
	c_x_ = x;
	update();
}
void MandelbrotWidget::setc_y(int y)
{
	c_y_ = y;
	update();
}
void MandelbrotWidget::setR(int x)
{
	r0_ = x;
	update();
}

void MandelbrotWidget::setG(int x)
{
	g0_ = x;
	update();
}

void MandelbrotWidget::setB(int x)
{
	b0_ = x;
	update();
}
