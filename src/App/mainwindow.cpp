#include "mainwindow.h"
#include "fractal.h"

#include <QSlider>
#include <QFormLayout>
#include <QDockWidget>

namespace
{
constexpr auto g_sampels = 8;
constexpr auto g_gl_major_version = 3;
constexpr auto g_gl_minor_version = 3;
}// namespace


MainWindow::MainWindow()
{
	QSlider* iterSlider = new QSlider(Qt::Horizontal);

	QLabel *iLabel = new QLabel(tr("iterations:"));
	iLabel->setBuddy(iterSlider);

	iterSlider->setMinimum(MandelbrotWidget::MIN_ITERS);
	iterSlider->setMaximum(MandelbrotWidget::MAX_ITERS);
	iterSlider->setValue(MandelbrotWidget::DEFAULT_ITERS);
	iterSlider->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

	QSlider* c_xSlider = new QSlider(Qt::Horizontal);

	QLabel *c_xLabel = new QLabel(tr("const_x:"));
	c_xLabel->setBuddy(c_xSlider);

	c_xSlider->setMinimum(MandelbrotWidget::MIN_C_X);
	c_xSlider->setMaximum(MandelbrotWidget::MAX_C_X);
	c_xSlider->setValue(MandelbrotWidget::DEFAULT_C_X);
	c_xSlider->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

	QSlider* c_ySlider = new QSlider(Qt::Horizontal);

	QLabel *c_yLabel = new QLabel(tr("const_y:"));
	c_yLabel->setBuddy(c_ySlider);

	c_ySlider->setMinimum(MandelbrotWidget::MIN_C_Y);
	c_ySlider->setMaximum(MandelbrotWidget::MAX_C_Y);
	c_ySlider->setValue(MandelbrotWidget::DEFAULT_C_Y);
	c_ySlider->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

	QSlider* rSlider = new QSlider(Qt::Horizontal);

	QLabel *rLabel = new QLabel(tr("Hue:"));
	rLabel->setBuddy(rSlider);

	rSlider->setMinimum(MandelbrotWidget::MIN_R0);
	rSlider->setMaximum(MandelbrotWidget::MAX_R0);
	rSlider->setValue(MandelbrotWidget::DEFAULT_R0);

	QSlider* gSlider = new QSlider(Qt::Horizontal);

	QLabel *gLabel = new QLabel(tr("Saturation:"));
	gLabel->setBuddy(gSlider);

	gSlider->setMinimum(MandelbrotWidget::MIN_G0);
	gSlider->setMaximum(MandelbrotWidget::MAX_G0);
	gSlider->setValue(MandelbrotWidget::DEFAULT_G0);
	gSlider->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

	QSlider* bSlider = new QSlider(Qt::Horizontal);

	QLabel *bLabel = new QLabel(tr("Brightness:"));
	bLabel->setBuddy(bSlider);

	bSlider->setMinimum(MandelbrotWidget::MIN_B0);
	bSlider->setMaximum(MandelbrotWidget::MAX_B0);
	bSlider->setValue(MandelbrotWidget::DEFAULT_B0);

	fpsLabel_ = new QLabel();
	fpsLabel_->setText("...");

	QGridLayout* formLayout = new QGridLayout();

	formLayout->addWidget(rLabel, 0, 0);
	formLayout->addWidget(rSlider, 0, 1);

	formLayout->addWidget(gLabel, 1, 0);
	formLayout->addWidget(gSlider, 1, 1);

	formLayout->addWidget(bLabel, 2, 0);
	formLayout->addWidget(bSlider, 2, 1);

	formLayout->addWidget(c_xLabel, 3, 0);
	formLayout->addWidget(c_xSlider, 3, 1);

	formLayout->addWidget(c_yLabel, 4, 0);
	formLayout->addWidget(c_ySlider, 4, 1);

	formLayout->addWidget(iLabel, 5, 0);
	formLayout->addWidget(iterSlider, 5, 1);

	formLayout->addWidget(fpsLabel_, 6, 0);

//	formLayout->addRow("Saturation: ", gSlider);
//	formLayout->addRow("Brightness: ", bSlider);
//	formLayout->addRow("const_x: ", c_xSlider);
//	formLayout->addRow("const_y: ", c_ySlider);
//	formLayout->addRow("Iterations: ", iterSlider);
//	formLayout->addRow("", fpsLabel_);

	QSurfaceFormat format;
	format.setSamples(g_sampels);
	format.setVersion(g_gl_major_version, g_gl_minor_version);
	format.setProfile(QSurfaceFormat::CoreProfile);
	QSurfaceFormat::setDefaultFormat(format);

	MandelbrotWidget* mandelbrotWidget = new MandelbrotWidget;
	mandelbrotWidget->setFormat(format);

	connect(rSlider, &QSlider::valueChanged, mandelbrotWidget, &MandelbrotWidget::setR);
	connect(gSlider, &QSlider::valueChanged, mandelbrotWidget, &MandelbrotWidget::setG);
	connect(bSlider, &QSlider::valueChanged, mandelbrotWidget, &MandelbrotWidget::setB);
	connect(c_xSlider, &QSlider::valueChanged, mandelbrotWidget, &MandelbrotWidget::setc_x);
	connect(c_ySlider, &QSlider::valueChanged, mandelbrotWidget, &MandelbrotWidget::setc_y);
	connect(iterSlider, &QSlider::valueChanged, mandelbrotWidget, &MandelbrotWidget::setIters);
	connect(mandelbrotWidget, &MandelbrotWidget::updateFPS, this, &MainWindow::updateFPS);

	setCentralWidget(mandelbrotWidget);

	auto dock = new QDockWidget;
	auto settingsWidget = new QWidget;
	settingsWidget->setFocusPolicy(Qt::NoFocus);
	dock->setFocusPolicy(Qt::NoFocus);
	settingsWidget->setLayout(formLayout);
	dock->setWidget(settingsWidget);

	this->addDockWidget(Qt::BottomDockWidgetArea, dock);
}

void MainWindow::updateFPS(float fps)
{
	fpsLabel_->setText(QString::asprintf("FPS:%.2d", int(fps)));
}