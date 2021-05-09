#include <QApplication>
#include "Window.hpp"

Q_DECLARE_METATYPE(std::vector<std::vector<RenderArea::Color> >);

int main(int argc, char *argv[]) {
	//Q_INIT_RESOURCE(basicdrawing);

	QApplication app(argc, argv);
	qRegisterMetaType<std::vector<std::vector<RenderArea::Color> >>();
	Window window;
	window.show();
	return app.exec();
}
