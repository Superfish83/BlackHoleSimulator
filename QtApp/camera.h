#ifndef CAMERA_H
#define CAMERA_H

#include "ray.h"
#include <QImage>

using color = vec3;

typedef struct background_ {
public:
    bool isImage;
    int bgW;
    int bgH;

    int offsetX;
    int offsetY;

    // * single star at background (isImage == False)
    double starBrightness;

    // * background Image (isImage == True)
    QImage *bgImage;

    void setBg(int bgW_, int bgH_, int offsetX_, int offsetY_){
        bgW = bgW_;
        bgH = bgH_;
        offsetX = offsetX_;
        offsetY = offsetY_;
    }
    void setStar(int bgW_, int bgH_, double starBrightness_=0.02, int offsetX_=0, int offsetY_=0){
        isImage = false;
        setBg(bgW_, bgH_, offsetX_, offsetY_);
        starBrightness = starBrightness_;
    }
    void setImage(int bgW_, int bgH_, QImage *bgImage_, int offsetX_=0, int offsetY_=0){
        isImage = true;
        setBg(bgW_, bgH_, offsetX_, offsetY_);
        bgImage = bgImage_;
    }

    QColor getColor(pixelData pix){
        if(isImage == false) { // single star background
            if(pix.outOfBounds){
                return QColor::fromRgb(0,0,0);
            }

            double l1 = max(0.1, 1-1/starBrightness
                                        *sqrt(
                                            pow(((double)pix.x/bgW - 0.5 + ((double)offsetX * 0.01)), 2) +
                                            pow(((double)pix.y/bgH - 0.5 + ((double)offsetY * 0.01)), 2)
                                            ));
            int l255 = (int)(l1*255);
            return QColor::fromRgb(l255, l255, l255);
        }
        else { // image background
            int x = (bgW-pix.x) - (offsetX * bgW)/40;
            int y = (bgH-pix.y) - (offsetY * bgH)/40;

            if(pix.outOfBounds || (x < 0 || x >= bgW) || (y < 0 || y >= bgH)){
                return QColor::fromRgb(0,0,0);
            }

            QColor c = bgImage->pixel(x, y);
            return c;
        }
    }
} background;


class camera{
public:
    // resolution (w x h)
    double aspect_ratio = 0.75;
    int wres = 512;
    int hres = 384;

    // The background object which saves the mapping from pixelData to colors

    // *** rendering parameters ***
    // * field of view (in degrees)
    double fov;
    // * distance to black hole
    double D;
    // * the mapping from pixelData to color
    background bg;

    QColor getColor(int i, int j){
        // Create a new ray object (for the ith row, jth col of image)

        double vp_dist = (wres/2.0) / tan(fov/2);

        double pix_x = ((double)i - (wres/2+1));
        double pix_y = ((double)j - (hres/2+1));

        vec3 v_(vp_dist, pix_x, pix_y);
        double beta = atan2(pix_y, pix_x);
        double alpha = acos(vp_dist / v_.norm());

        ray ray_ij(D, alpha, beta);

        // *** perform ray tracing for ray_ij ***
        pixelData pix = ray_ij.getBgPixel(bg.bgW, bg.bgH);

        return bg.getColor(pix);
    }

    camera(double fov, double D, background bg): fov(fov), D(D), bg(bg) { }

    // Unused (used in testing on console)
    /*
    void renderToPPM(){
        cout << "P3\n" << wres << ' ' << hres << "\n255\n";

        char buffer[32768];
        color c;
        for(int j = 1; j <= hres; j++){
            char *p = buffer;
            for(int i = 1; i <= wres; i++){
                c = getColor(i, j);
                p += sprintf(p, "%d %d %d\n",
                             int(255*c[0]), int(255*c[1]), int(255*c[2]));
            }
            cout << buffer;
        }
    }*/
};

#endif // CAMERA_H
