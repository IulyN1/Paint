#include "Paint.h"
#include <qevent.h>
#include <qpainter.h>

void PaintArea::setInitialState() {
	setAttribute(Qt::WA_StaticContents);
	modified = false;
	drawing = false;
	myPenColor = Qt::blue;
	myPenWidth = 1;
}

bool PaintArea::openImage(const QString& filename) {
	QImage loadedImg;
	if (!loadedImg.load(filename)) {
		return false;
	}
	QSize newSize = loadedImg.size().expandedTo(size());
	resizeImage(&loadedImg, newSize);
	image = loadedImg;
	modified = false;
	update();
	return true;
}

bool PaintArea::saveImage(const QString& filename, const char* fileFormat) {
	QImage visibleImage = image;
	resizeImage(&visibleImage, size());
	if (visibleImage.save(filename, fileFormat)) {
		modified = false;
		return true;
	}
	else return false;
}

void PaintArea::setPenColor(const QColor& newColor) {
	myPenColor = newColor;
}

void PaintArea::setPenWidth(int newWidth) {
	myPenWidth = newWidth;
}

void PaintArea::clearImage() {
	image.fill(qRgb(255, 255, 255));
	modified = true;
	update();
}

void PaintArea::mousePressEvent(QMouseEvent* event) {
	if (event->button() == Qt::LeftButton) {
		lastPoint = event->pos();
		drawing = true;
	}
}

void PaintArea::mouseMoveEvent(QMouseEvent* event) {
	if ((event->buttons() & Qt::LeftButton) && drawing){
		drawLineTo(event->pos());
	}
}

void PaintArea::mouseReleaseEvent(QMouseEvent* event) {
	if (event->button() == Qt::LeftButton && drawing) {
		drawLineTo(event->pos());
		drawing = false;
	}
}

void PaintArea::paintEvent(QPaintEvent* event) {
	QPainter painter(this);
	QRect dirtyRect = event->rect();
	painter.drawImage(dirtyRect, image, dirtyRect);
}

void PaintArea::resizeEvent(QResizeEvent* event) {
	if (width() > image.width() || height() > image.height()) {
		int newWidth = qMax(width() + 128, image.width());
		int newHeight = qMax(height() + 128, image.height());
		resizeImage(&image, QSize(newWidth, newHeight));
		update();
	}
	QWidget::resizeEvent(event);
}

void PaintArea::drawLineTo(const QPoint& endPoint) {
	QPainter painter(&image);
	painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
	painter.drawLine(lastPoint, endPoint);
	modified = true;
	int rad = (myPenWidth / 2) + 2;
	update(QRect(lastPoint, endPoint).normalized().adjusted(-rad, -rad, +rad, +rad));
	lastPoint = endPoint;
}

void PaintArea::resizeImage(QImage* image, const QSize& newSize) {
	if (image->size() == newSize) {
		return;
	}
	QImage newImage(newSize, QImage::Format_RGB32);
	newImage.fill(qRgb(255, 255, 255));
	QPainter painter(&newImage);
	painter.drawImage(QPoint(0, 0), *image);
	*image = newImage;
}
