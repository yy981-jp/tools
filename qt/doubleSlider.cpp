#include "doubleSlider.h"
#include <QtWidgets/QStyleOptionSlider>

DoubleSlider::DoubleSlider(Qt::Orientation orientation, QWidget *parent)
	: QSlider(orientation, parent), m_lowerValue(0), m_upperValue(maximum()) {}

int DoubleSlider::lowerValue() const {
	return m_lowerValue;
}

int DoubleSlider::upperValue() const {
	return m_upperValue;
}

void DoubleSlider::setLowerValue(int value) {
	if (value < minimum()) value = minimum();
	if (value > m_upperValue) value = m_upperValue;
	m_lowerValue = value;
	update();
}

void DoubleSlider::setUpperValue(int value) {
	if (value > maximum()) value = maximum();
	if (value < m_lowerValue) value = m_lowerValue;
	m_upperValue = value;
	update();
}

void DoubleSlider::paintEvent(QPaintEvent *event) {
	QSlider::paintEvent(event);
	QPainter painter(this);

	QStyleOptionSlider opt;
	initStyleOption(&opt);

	QRect groove = style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderGroove, this);
	QRect handleLower = style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderHandle, this);
	QRect handleUpper = style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderHandle, this);

	handleLower.moveLeft(lowerValueToPos());
	handleUpper.moveLeft(upperValueToPos());

	painter.fillRect(groove, Qt::gray);
	painter.fillRect(handleLower, Qt::blue);
	painter.fillRect(handleUpper, Qt::blue);
}

int DoubleSlider::lowerValueToPos() const {
	return static_cast<int>((m_lowerValue - minimum()) / static_cast<double>(maximum() - minimum()) * width());
}

int DoubleSlider::upperValueToPos() const {
	return static_cast<int>((m_upperValue - minimum()) / static_cast<double>(maximum() - minimum()) * width());
}
