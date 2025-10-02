#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QInputDialog>
#include <QNetworkCookieJar>

MainWindow::MainWindow(QWidget *parent)
		: QMainWindow(parent), ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	ui->seekSlider->setMediaObject(ui->videoPlayer->mediaObject());
	ui->volumeSlider->setAudioOutput(ui->videoPlayer->audioOutput());
	connect(ui->videoPlayer->mediaObject(),
			  SIGNAL(stateChanged(Phonon::State, Phonon::State)),
			  this,
			  SLOT(phononStateChanged(Phonon::State, Phonon::State))
			  );

	m_ua = new QNetworkAccessManager(this);
	m_ua->setCookieJar(new QNetworkCookieJar());
	connect(m_ua, SIGNAL(finished(QNetworkReply*)),
			  this, SLOT(networkRequestFinished(QNetworkReply*)));

	m_sb = new QBuffer(this);
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::phononStateChanged(Phonon::State newstate, Phonon::State oldstate)
{
	if(newstate == Phonon::ErrorState){
		qDebug() << ui->videoPlayer->mediaObject()->errorString();
	}
}

void MainWindow::on_toolButton_clicked(bool checked)
{
	QString v =	QFileDialog::getOpenFileName(this, tr("Select a Video"));
	if(!v.isNull()){
		ui->videoPlayer->play(Phonon::MediaSource(v));
	}
}

void MainWindow::on_toolButton_2_clicked(bool checked)
{ 			
	QString v =
			QInputDialog::getItem(this, tr("Select Video Location"), tr("Video URL"),
										 QStringList()
										 << "http://192.168.0.46/~l4t3n8/video.php"
										 << "http://192.168.0.46/~l4t3n8/phonon.mp4"
										 );
	QNetworkReply *r = m_ua->get(QNetworkRequest(v));
	r->setReadBufferSize(0);
	connect(r,SIGNAL(readyRead()),this,SLOT(networkRequestReadyRead()));
}

void MainWindow::networkRequestFinished(QNetworkReply *r)
{
}

void MainWindow::networkRequestReadyRead()
{
	QNetworkReply* r = dynamic_cast<QNetworkReply*>(this->sender());
	disconnect(r, SIGNAL(readyRead()), this, SLOT(networkRequestReadyRead()));
	Phonon::MediaSource s(dynamic_cast<QIODevice*>(r));
	ui->videoPlayer->play(s);
}
