#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>

#include <cmath>

#include "rays.h"
#include "camera.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

QPixmap getRenderQPixmap(QProgressBar *pb, double fov, double D,
                         int starOffsetX, int starOffsetY, int w, double aspect_ratio=0.75){
    QImage imagen(w, (int)(w*aspect_ratio), QImage::Format_RGB666);

    camera cam(fov, D, starOffsetX, starOffsetY);

    color c;

    for(int j = 0; j < cam.hres; j++){
        for(int i = 0; i < cam.wres; i++){
            c = cam.getColor(i+1, j+1);
            imagen.setPixel(i, j, qRgb((int)(255*c[0]), (int)(255*c[1]), (int)(255*c[2])));
        }
        pb->setValue((int)(100.0 * ((double)(j+1) / cam.hres)));
    }

    QPixmap pixmap_scene = QPixmap::fromImage(imagen);

    return pixmap_scene;
}

void MainWindow::on_bRender_clicked()
{
    QGraphicsView *gView = ui->graphicsView;
    QGraphicsScene *scene = new QGraphicsScene();
    QProgressBar *pb = ui->pbRenderProgress;

    double fov = (Pi * (double)ui->hsFOV->value()) / 180.0; // convert fov to radian
    double D = (double)(ui->hsD->value());

    starOffsetX = ui->hsXoff->value();
    starOffsetY = ui->hsYoff->value();

    QPixmap pixmap_scene = getRenderQPixmap(pb, fov, D, starOffsetX, starOffsetY, 512);
    scene->addPixmap(pixmap_scene);
    gView->setScene(scene);
    gView->show();

    //QMessageBox msg;
    //msg.setText("Test");
    //msg.exec();
}



void MainWindow::on_hsYoff_valueChanged(int value)
{
    on_bRender_clicked();
}


void MainWindow::on_hsXoff_valueChanged(int value)
{
    on_bRender_clicked();
}

