#include "Window.hpp"

#include "RenderArea.hpp"
#include "Comunication.hpp"

#include <QtWidgets>

const int IdRole = Qt::UserRole;

Window::Window(): simulation(std::vector<std::vector<RenderArea::Color>>(RenderArea::AREA_WIDTH, std::vector<RenderArea::Color>(RenderArea::AREA_HEIGHT, RenderArea::Color::BLACK))) {
	renderArea = new RenderArea;
	renderArea->setDraw(simulation);

	auto mainLayout = new QGridLayout;
	mainLayout->setColumnStretch(0, 1);
	mainLayout->setColumnStretch(3, 1);
	mainLayout->addWidget(renderArea, 0, 0, 1, 4);
	setLayout(mainLayout);

	setWindowTitle(tr("Basic Drawing"));

	auto *workerThread = new CommunicationThread(this);
	connect(workerThread, &CommunicationThread::resultReady, this, &Window::update);
	workerThread->start();
}

void Window::update(const std::vector<std::vector<RenderArea::Color>>& simulationNext) {
	renderArea->setDraw(simulationNext);
}