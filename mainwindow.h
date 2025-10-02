#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <phonon/mediaobject.h>
#include <QNetworkReply>
#include <QNetworkAccessManager>

#include <QBuffer>

namespace Ui
{
	class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();

private:
	QBuffer *m_sb;
	Ui::MainWindow *ui;
	QNetworkAccessManager *m_ua;

private slots:
	void phononStateChanged(Phonon::State, Phonon::State);
	void on_toolButton_clicked(bool checked);
	void on_toolButton_2_clicked(bool checked);
	void networkRequestFinished(QNetworkReply *r);
	void networkRequestReadyRead();
};

#endif // MAINWINDOW_H
