#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>
#include <QFileDialog>

#include <cmath>

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

QPixmap getRenderQPixmap(QProgressBar *pb, double fov, double D, background bg, int w, double aspect_ratio=0.75){

    QImage imagen(w, (int)(w*aspect_ratio), QImage::Format_RGB666);
    QColor c;

    camera cam(fov, D, bg);

    for(int j = 0; j < cam.hres; j++){
        for(int i = 0; i < cam.wres; i++){
            c = cam.getColor(i+1, j+1);
            imagen.setPixel(i, j, c.rgb());
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

    int fov_deg = ui->hsFOV->value();
    double fov = (Pi * (double)fov_deg) / 180.0; // convert fov to radian
    double D = (double)(ui->hsD->value());

    int offsetX = ui->sbOffsetX->value();
    int offsetY = ui->sbOffsetY->value();

    // Set background data
    background bg;
    bool isImage = ui->rbImage->isChecked();
    if(isImage){ // image(QImage) for background
        if(!bgImageLoaded){
            QMessageBox msg;
            msg.setText("You should select background image file!");
            msg.exec();
            return;
        }

        bg.setImage(bgImage.width(), bgImage.height(), &bgImage, offsetX, offsetY);
    }
    else { // single star background
        double brightness = (double)(ui->hsBrightness->value()) * 0.001;
        bg.setStar(4000, 3000, brightness, offsetX, offsetY);
    }

    QPixmap pixmap_scene = getRenderQPixmap(pb, fov, D, bg, 512);
    scene->addPixmap(pixmap_scene);
    gView->setScene(scene);
    gView->show();

    ui->lbRenderOption->setText(QString("D = %1R, FOV=%2º").arg(D).arg(fov_deg));

    //QMessageBox msg;
    //msg.setText("Test");
    //msg.exec();
}

void MainWindow::on_rbStar_clicked()
{
    ui->hsBrightness->setEnabled(true);
    ui->bLoadImage->setEnabled(false);
    ui->bRender->setEnabled(true);
}


void MainWindow::on_bLoadImage_clicked()
{
    QString url = QFileDialog::getOpenFileName(this,"Select Background Image","C:\\Users\\User\\Pictures","Image Files (*.jpg *.jpeg *.png)");
    ui->lbImgPath->setText(QString("Loaded: %1").arg(url));

    bgImageLoaded = true;
    bgPixmap.load(url);
    bgImage = bgPixmap.toImage();
}


void MainWindow::on_rbImage_clicked()
{
    ui->hsBrightness->setEnabled(false);
    ui->bLoadImage->setEnabled(true);
    ui->bRender->setEnabled(true);
}

