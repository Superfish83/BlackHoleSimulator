#include <iostream>
#include <cmath>
#include "rays.h"

using namespace std;
using namespace vec;

class camera{
public:
    // resolution (w x h)
    double aspect_ratio = 0.75;
    int wres;
    int hres;

    // field of view (in degrees)
    double fov;

    void render(){
        char buffer[1024];
        double c;
        for(int j = 1; j <= hres; j++){
            char *p = buffer;
            for(int i = 1; i <= wres; i++){
                c = getColor(i, j)[0];
                int val = (int)round(c);
                if(val < 0) p += sprintf(p, "X");
                else p += sprintf(p, "%d", val);
            }
            cout << buffer << endl;
        }
    }

    color getColor(int i, int j){
        // Create a new ray object (for the ith row, jth col of image)
        
        double vp_dist = fov/20;

        double pix_x = ((double)i - wres/2);
        double pix_y = ((double)j - hres/2);

        double beta = atan(pix_y/pix_x);
        double alpha = acos(vp_dist/sqrt(1 + pix_x*pix_x + pix_y*pix_y));

        double D = 3.0;

        ray ray_i(D, alpha, beta);

        return ray_i.getColor();
    }


    camera(): wres(256), aspect_ratio(0.75), hres(192), fov(Pi) { }
    camera(int wres, double aspect_ratio, double wfov, double hfov):
        wres(wres), hres((int)(wres * aspect_ratio)), fov(fov) { }

    void renderToPPM(){
        cout << "P3\n" << wres << ' ' << hres << "\n255\n";

        char buffer[8192];
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

int main(void){
    camera cam;
    cam.renderToPPM();

    return 0;
}