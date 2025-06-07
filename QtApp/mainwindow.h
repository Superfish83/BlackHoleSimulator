#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>

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

    void on_rbStar_clicked();

    void on_bLoadImage_clicked();

    void on_rbImage_clicked();

private:
    Ui::MainWindow *ui;
    bool bgImageLoaded = false;
    QPixmap bgPixmap;
    QImage bgImage;
};
#endif // MAINWINDOW_H
