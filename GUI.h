#pragma once
#include <QtWidgets/qwidget.h>
#include <qmainwindow.h>
#include <qlist.h>
#include "Paint.h"

class GUI: public QMainWindow
{
	Q_OBJECT
protected:
	void closeEvent(QCloseEvent* event) override;
private slots:
	void open();
	void save();
	void penColor();
	void penWidth();
	void about();

private:
	// Function that creates the actions
	void createActions();

	// Function that creates the menus
	void createMenus();

	// Function that checks if there were made changes
	bool maybeSave();

	// Function that saves the drawing to a file
	// fileFormat - QByteArray ( the format of the file )
	bool saveFile(const QByteArray& fileFormat);

	PaintArea* area = new PaintArea;
	QMenu* saveAsMenu;
	QMenu* fileMenu;
	QMenu* optionMenu;
	QMenu* helpMenu;
	QAction* openAct;
	QList<QAction*> saveAsActs;
	QAction* penColorAct;
	QAction* penWidthAct;
	QAction* exitAct;
	QAction* clearScreenAct;
	QAction* aboutAct;

	// Function that sets the initial state of the GUI
	void setInitialState();
public:

	// Constructor for GUI class
	GUI() {
		setInitialState();
	}
};

