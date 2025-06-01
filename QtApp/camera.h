#ifndef CAMERA_H
#define CAMERA_H

#include "rays.h"

class camera{
public:
    // resolution (w x h)
    double aspect_ratio;
    int wres;
    int hres;

    // * simulation parameters
    // * field of view (in degrees)
    double fov;
    // * distance to black hole
    double D;
    // * position of star at background
    int starOffsetX;
    int starOffsetY;

    color getColor(int i, int j){
        // Create a new ray object (for the ith row, jth col of image)

        double vp_dist = (wres/2.0) / tan(fov/2);

        double pix_x = ((double)i - (wres/2+1));
        double pix_y = ((double)j - (hres/2+1));

        vec3 v(vp_dist, pix_x, pix_y);
        double beta = atan2(pix_y, pix_x);
        double alpha = acos(vp_dist / v.norm());

        ray ray_i(D, alpha, beta);
        int bgW = 4000;
        int bgH = 3000;
        pixelData pix = ray_i.getBgPixel(bgW, bgH);

        if(pix.outOfBounds){
            return color(0,0,0);
        }
        else{
            double l = max(0.1, 1-30*sqrt(
                                        pow(((double)pix.x/bgW - 0.5 + ((double)starOffsetX * 0.02)), 2) +
                                        pow(((double)pix.y/bgH - 0.5 + ((double)starOffsetY * 0.02)), 2)
            ));
            return color(l, l, l);
        }
    }


    camera(): wres(512), aspect_ratio(0.75), hres(384), fov(Pi * 0.6), D(10.0) { }
    camera(double fov, double D, int starOffsetX, int starOffsetY):
        wres(512), aspect_ratio(0.75), hres(384),
        starOffsetX(starOffsetX), starOffsetY(starOffsetY), fov(fov), D(D) { }
    camera(int wres, double aspect_ratio, int starOffsetX, int starOffsetY, double fov, double D):
        wres(wres), aspect_ratio(aspect_ratio), hres((int)(wres * aspect_ratio)),
        starOffsetX(starOffsetX), starOffsetY(starOffsetY), fov(fov), D(D) { }

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
    }
};

#endif // CAMERA_H
