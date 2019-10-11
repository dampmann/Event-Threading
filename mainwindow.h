#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
	Q_OBJECT

	public:
		MainWindow(QWidget *parent = nullptr);
		~MainWindow();

	private:
		Ui::MainWindow *ui;
		volatile bool stopped;
		static void wasteTime(QObject *receiver, int n, volatile bool *stopped, unsigned long seconds, int times);

	private slots:
		void clicked();
		void is_done();

	protected:
		bool event(QEvent*);
		void closeEvent(QCloseEvent *event);
};
#endif // MAINWINDOW_H
