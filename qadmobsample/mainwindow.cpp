#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qadmobwidget.h"

#include <QLayout>



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    iAdMobWidget( new QAdMobWidget(this))
{
    ui->setupUi(this);
    ui->centralWidget->layout()->addWidget(iAdMobWidget);

    iAdMobWidget->ad().setPublisherId("<put your publisher id here>");
    iAdMobWidget->ad().setTestMode(QBool(true));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void MainWindow::on_pushButton_clicked()
{
    iAdMobWidget->fetchAd();
}
