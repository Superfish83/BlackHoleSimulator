#include <iostream>
#include <cmath>
#include "rays.h"

using namespace std;

class camera{
public:
    // resolution (w x h)
    double aspect_ratio = 0.75;
    int wres;
    int hres;

    // field of view (in degrees)
    double fov;

    color getColor(int i, int j){
        // Create a new ray object (for the ith row, jth col of image)
        
        double vp_dist = (wres/2.0) / tan(fov/2);

        double pix_x = ((double)i - (wres/2+1));
        double pix_y = ((double)j - (hres/2+1));

        vec3 v(vp_dist, pix_x, pix_y);
        double beta = atan2(pix_y, pix_x);
        double alpha = acos(vp_dist / v.norm());

        double D = 10.0;

        ray ray_i(D, alpha, beta);

        return ray_i.getColor();
    }


    camera(): wres(256), aspect_ratio(0.75), hres(192), fov(Pi * 0.6) { }
    camera(int wres):
        wres(wres), aspect_ratio(0.75), hres((int)(wres * 0.75)), fov(Pi * 0.6) { }
    camera(int wres, double aspect_ratio, double fov):
        wres(wres), hres((int)(wres * aspect_ratio)), fov(fov) { }

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

/*
int main(void){
    camera cam(256, 0.75, Pi * 0.6);//384);
    cam.renderToPPM();

    return 0;
}*/
