#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_bRender_clicked();

    void on_hsXoff_sliderMoved(int position);

    void on_hsYoff_sliderMoved(int position);

    void on_hsYoff_valueChanged(int value);

    void on_hsXoff_valueChanged(int value);

private:
    Ui::MainWindow *ui;
    int starOffsetX;
    int starOffsetY;
};
#endif // MAINWINDOW_H
