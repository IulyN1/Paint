#include "GUI.h"
#include <qevent.h>
#include <qfiledialog.h>
#include <qbytearray.h>
#include <qcolordialog.h>
#include <qinputdialog.h>
#include <qmessagebox.h>
#include <qaction.h>
#include <qimagewriter.h>
#include <qmenu.h>
#include <qmenubar.h>


void GUI::setInitialState() {
	resize(500, 500);
	setCentralWidget(area);
	createActions();
	createMenus();
	setWindowTitle("Paint");
}

void GUI::closeEvent(QCloseEvent* event) {
	if (maybeSave()) {
		event->accept();
	}
	else {
		event->ignore();
	}
}

void GUI::open() {
	if (maybeSave()) {
		QString filename = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::currentPath());
		if (!filename.isEmpty()) {
			area->openImage(filename);
		}
	}
}

void GUI::save() {
	QAction* action = qobject_cast<QAction*>(sender());
	QByteArray fileFormat = action->data().toByteArray();
	saveFile(fileFormat);
}

void GUI::penColor() {
	QColor newColor = QColorDialog::getColor(area->penColor());
	if (newColor.isValid()) {
		area->setPenColor(newColor);
	}
}

void GUI::penWidth() {
	bool ok;
	int newWidth = QInputDialog::getInt(this, tr("Paint"), tr("Select pen width: "),
		area->penWidth(), 1, 50, 1, &ok);
	if (ok) {
		area->setPenWidth(newWidth);
	}
}

void GUI::about() {
	QMessageBox::about(this, tr("About Paint"), tr("<p><b>Paint</b> is amazing.</p>"));
}

void GUI::createActions() {
	openAct = new QAction(tr("&Open"), this);
	openAct->setShortcuts(QKeySequence::Open);	
	auto formats = QImageWriter::supportedImageFormats();
	for(QByteArray format : formats)
	{
		QString text = tr("%1...").arg(QString(format).toUpper());
		QAction* action = new QAction(text, this);
		action->setData(format);
		connect(action, &QAction::triggered, this, &GUI::save);
		saveAsActs.append(action);
	};

	exitAct = new QAction(tr("E&xit"), this);
	exitAct->setShortcuts(QKeySequence::Quit);
	penColorAct = new QAction(tr("&Pen Color..."),this);
	penWidthAct = new QAction(tr("&Pen Width..."), this);
	clearScreenAct = new QAction(tr("&Clear Screen..."),this);
	clearScreenAct->setShortcut(tr("Ctrl+L"));
	aboutAct = new QAction(tr("&About..."), this);

	connect(openAct, &QAction::triggered, this, &GUI::open);
	connect(exitAct, &QAction::triggered, this, &GUI::close);
	connect(penColorAct, &QAction::triggered, this, &GUI::penColor);
	connect(penWidthAct, &QAction::triggered, this, &GUI::penWidth);
	connect(clearScreenAct, &QAction::triggered, area, &PaintArea::clearImage);
	connect(aboutAct, &QAction::triggered, this, &GUI::about);
}

void GUI::createMenus() {
	saveAsMenu = new QMenu(tr("&Save As"), this);
	for (QAction* action : saveAsActs) {
		saveAsMenu->addAction(action);
	}
	fileMenu = new QMenu(tr("&File"), this);
	fileMenu->addAction(openAct);
	fileMenu->addMenu(saveAsMenu);
	fileMenu->addSeparator();
	fileMenu->addAction(clearScreenAct);

	optionMenu = new QMenu(tr("&Options"), this);
	optionMenu->addAction(penColorAct);
	optionMenu->addAction(penWidthAct);
	optionMenu->addSeparator();
	optionMenu->addAction(clearScreenAct);

	helpMenu = new QMenu(tr("&Help"), this);
	helpMenu->addAction(aboutAct);
	
	menuBar()->addMenu(fileMenu);
	menuBar()->addMenu(optionMenu);
	menuBar()->addMenu(helpMenu);
}

bool GUI::maybeSave() {
	if (area->isModified()) {
		auto ret = QMessageBox::warning(this, tr("Paint"),
			tr("The image has been modified.\nDo you want to save your changes?"),
			QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
		if (ret == QMessageBox::Save) {
			return saveFile("png");
		}
		else if (ret == QMessageBox::Cancel) {
			return false;
		}
	}
	return true;
}

bool GUI::saveFile(const QByteArray& fileFormat) {
	QString initialPath = QDir::currentPath() + "/untitled." + fileFormat;
	QString filename = QFileDialog::getSaveFileName(this, tr("Save As"),
		initialPath, tr("%1 Files (*.%2);; All Files(*)").arg(QString::fromLatin1(fileFormat.toUpper()))
			.arg(QString::fromLatin1(fileFormat)));
	if (filename.isEmpty()) {
		return false;
	}
	else{
		return area->saveImage(filename, fileFormat.constData());
	}
}