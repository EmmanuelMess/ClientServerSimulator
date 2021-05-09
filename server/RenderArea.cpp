#include "RenderArea.hpp"
#include <QPainter>
#include <QPainterPath>
#include <utility>

const std::vector<RenderArea::Color> RenderArea::COLORS = {RenderArea::BLACK, RenderArea::WHITE, RenderArea::BLUE, RenderArea::GREEN, RenderArea::RED };

RenderArea::RenderArea(QWidget *parent) : QWidget(parent) {
	antialiased = false;
	pixmap.load(":/images/qt-logo.png");

	setBackgroundRole(QPalette::Base);
	setAutoFillBackground(true);
}

QSize RenderArea::minimumSizeHint() const {
	return QSize(300, 300);
}

QSize RenderArea::sizeHint() const {
	return QSize(300, 300);
}

void RenderArea::setDraw(std::vector<std::vector<Color>> colors) {
	this->display = std::move(colors);
	update();
}

void RenderArea::paintEvent(QPaintEvent * /* event */) {
	QRect rect(0, 0, 9, 9);

	std::unordered_map<Color, QPainter> painters(COLORS.size());

	for (auto c : COLORS) {
		auto color = Qt::green;
		switch (c) {
			case BLACK:
				color = Qt::black;
				break;
			case WHITE:
				color = Qt::white;
				break;
			case BLUE:
				color = Qt::blue;
				break;
			case GREEN:
				color = Qt::green;
				break;
			case RED:
				color = Qt::red;
				break;
		}

		pen = QPen(Qt::black, 1, Qt::SolidLine,Qt::SquareCap, Qt::BevelJoin);

		auto style = Qt::BrushStyle(Qt::BrushStyle::SolidPattern);
		brush = QBrush(color, style);

		painters.emplace(c, this);
		painters[c].setPen(pen);
		painters[c].setBrush(brush);
		if (antialiased) {
			painters[c].setRenderHint(QPainter::Antialiasing, true);
		}
	}


	for (int x = 0; x < AREA_WIDTH; x++) {
		for (int y = 0; y < AREA_HEIGHT; y++) {
			auto &painter = painters[display[x][y]];

			painter.save();
			painter.translate(x * 10, y * 10);

			painter.drawRect(rect);

			painter.restore();
		}
	}
}
