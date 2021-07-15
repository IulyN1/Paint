#pragma once
#include <qwidget.h>

class PaintArea : public QWidget {
	Q_OBJECT
private:
	void drawLineTo(const QPoint& endPoint);
	void resizeImage(QImage* image, const QSize& newSize);
	bool modified;
	bool drawing;
	int myPenWidth;
	QColor myPenColor;
	QImage image;
	QPoint lastPoint;

	// Function that sets the initial state of the paint area
	void setInitialState();
public:
	PaintArea() {
		setInitialState();
	}

	// Function that opens an image
	// filename - QString
	bool openImage(const QString& filename);

	// Function that saves an image
	// filename - QString
	// fileFormat - char*
	bool saveImage(const QString& filename, const char* fileFormat);

	// Function that sets the pen color
	// newColor - QColor
	void setPenColor(const QColor& newColor);

	// Function that sets the pen width
	// newWidth - int
	void setPenWidth(int newWidth);

	// Function that checks if the drawing is modified
	bool isModified() const { return modified; }

	// Function that returns the pen color
	QColor penColor() const { return myPenColor; }

	// Function that returns the pen width
	int penWidth() const { return myPenWidth; }
public slots:
	void clearImage();
protected:
	void mousePressEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;
	void paintEvent(QPaintEvent* event) override;
	void resizeEvent(QResizeEvent* event) override;
};

