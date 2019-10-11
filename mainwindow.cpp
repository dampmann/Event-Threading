#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ProgressEvent.h"

#include <QThread>
#include <QtConcurrent/QtConcurrent>

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
		, ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	stopped = false;
	connect(ui->pushButton1, SIGNAL(clicked()), this, SLOT(clicked()));
	connect(ui->pushButton2, SIGNAL(clicked()), this, SLOT(clicked()));
	connect(ui->pushButton3, SIGNAL(clicked()), this, SLOT(clicked()));
}

void MainWindow::clicked() {
	QObject *sender = QObject::sender();
	if(sender->objectName() == "pushButton1") {
		QPushButton *pb = static_cast<QPushButton*>(sender);
		pb->setEnabled(false);
		ui->pushButton3->setEnabled(true);
		QtConcurrent::run(wasteTime, this, 1, &stopped, 2, 25);
	} else if(sender->objectName() == "pushButton2") {
		QPushButton *pb = static_cast<QPushButton*>(sender);
		pb->setEnabled(false);
		ui->pushButton3->setEnabled(true);
		QtConcurrent::run(wasteTime, this, 2, &stopped, 1, 15);
	} else {
		stopped = true;
		ui->pushButton3->setEnabled(false);
		is_done();
        ui->plainTextEdit->clear();
    }
}

void MainWindow::is_done() {
	if(QThreadPool::globalInstance()->activeThreadCount()) {
		QTimer::singleShot(100, this, SLOT(is_done()));
	} else {
		ui->pushButton1->setEnabled(true);
		ui->pushButton2->setEnabled(true);
		stopped = false;
	}
}

bool MainWindow::event(QEvent *event) {
	if(event->type() == static_cast<QEvent::Type>(ProgressEvent::EventId)) {
		ProgressEvent *e = static_cast<ProgressEvent*>(event);
		ui->plainTextEdit->appendPlainText(e->message);
		return(true);
	}
	return(QMainWindow::event(event));
}

void MainWindow::closeEvent(QCloseEvent *event) {
	stopped = true;
	if(QThreadPool::globalInstance()->activeThreadCount()) {
		QThreadPool::globalInstance()->waitForDone();
	}

	event->accept();
}

void MainWindow::wasteTime(
		QObject *receiver,
		int n,
		volatile bool *stopped,
		unsigned long seconds,
		int times) {
	for(int i = 0; i < times; ++i) {
		if(*stopped) return;
		QThread::currentThread()->sleep(seconds);
		if(*stopped) return;
		QString message = QObject::tr("Thread %1 -> iteration %2\n").arg(n).arg(i);
		QApplication::postEvent(receiver, new ProgressEvent(message));
	}
}

MainWindow::~MainWindow()
{
	delete ui;
}

