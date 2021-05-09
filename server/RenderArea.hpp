#ifndef INC_8BITCPPGAME_RENDERAREA_HPP
#define INC_8BITCPPGAME_RENDERAREA_HPP

#include <QBrush>
#include <QPen>
#include <QPixmap>
#include <QWidget>
#include <unordered_set>

class RenderArea : public QWidget {
Q_OBJECT

public:
	static const int AREA_HEIGHT = 30;
	static const int AREA_WIDTH = 30;

	enum Color { BLACK, WHITE, BLUE, GREEN, RED };
	static const std::vector<Color> COLORS;

	explicit RenderArea(QWidget *parent = nullptr);

	QSize minimumSizeHint() const override;

	QSize sizeHint() const override;

public slots:
	void setDraw(std::vector<std::vector<Color>> color);

protected:
	void paintEvent(QPaintEvent *event) override;

private:
	std::vector<std::vector<Color>> display;
	QPen pen;
	QBrush brush;
	bool antialiased;
	QPixmap pixmap;
};

#endif //INC_8BITCPPGAME_RENDERAREA_HPP
