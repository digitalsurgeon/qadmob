#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>


class QAdMobWidget;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::MainWindow *ui;
    QAdMobWidget*   iAdMobWidget;

private slots:
    void on_pushButton_clicked();
};

#endif // MAINWINDOW_H
