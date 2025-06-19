#pragma once
#include <QtWidgets/QSlider>
#include <QtGui/QPainter>
#include <QtGui/QMouseEvent>
#include <QtWidgets/QStyle>


class DoubleSlider : public QSlider {
	Q_OBJECT

public:
	DoubleSlider(Qt::Orientation orientation, QWidget *parent = nullptr)
		: QSlider(orientation, parent), lowerValue(0), upperValue(maximum()) {}

	int lowerValue;
	int upperValue;

protected:
	void paintEvent(QPaintEvent *event) override {
		QSlider::paintEvent(event);
		QPainter painter(this);
		QRect groove = style()->subControlRect(QStyle::CC_Slider, &option(), QStyle::SC_SliderGroove, this);
		QRect handleLower = style()->subControlRect(QStyle::CC_Slider, &option(), QStyle::SC_SliderHandle, this);
		QRect handleUpper = style()->subControlRect(QStyle::CC_Slider, &option(), QStyle::SC_SliderHandle, this);

		handleLower.moveLeft(lowerValueToPos());
		handleUpper.moveLeft(upperValueToPos());

		painter.fillRect(groove, Qt::gray);
		painter.fillRect(handleLower, Qt::blue);
		painter.fillRect(handleUpper, Qt::blue);
	}

private:
	int lowerValueToPos() const {
		return static_cast<int>((lowerValue - minimum()) / static_cast<double>(maximum() - minimum()) * width());
	}

	int upperValueToPos() const {
		return static_cast<int>((upperValue - minimum()) / static_cast<double>(maximum() - minimum()) * width());
	}
};
