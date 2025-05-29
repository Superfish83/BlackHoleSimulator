#include <iostream>
#include <cmath>
#include "rays.h"

using namespace std;
using namespace vec;
using namespace rays;

class camera{
private:
public:
    // resolution (w x h)
    int wres;
    int hres;

    // field of view (in degrees)
    double wfov;
    double hfov;

    void render(){
        char buffer[1024];
        double c;
        for(int j = 1; j <= hres; j++){
            char *p = buffer;
            for(int i = 1; i <= wres; i++){
                c = getColor(i, j);
                int val = (int)c;
                if(val < 0) p += sprintf(p, "X");
                else p += sprintf(p, "%d", val);
            }
            cout << buffer << endl;
        }
    }

    double getColor(int i, int j){
        // Create a new ray object (for the ith row, jth col of image)
        double r0 = 5.0;
        double th0 = 0;
        double phi0 = 0;

        double d_r0 = -1;

        double tmp1 = wfov * ((double)i/(wres+1) - 0.5);
        double tmp2 = hfov * ((double)j/(hres+1) - 0.5);
        double d_th0 = sqrt(tmp1*tmp1 + tmp2*tmp2);
        double d_phi0 = 0; //hfov * ((double)j/(hres+1) - 0.5);

        vec4 x0(0, r0, th0, phi0, CoordType::SPHERICAL);
        vec4 Dx0(0, d_r0, d_th0, d_phi0, CoordType::SPHERICAL);

        ray ray_i(x0, Dx0);

        color c_ij(0,0,0);
        bool horizon = true;

        // Compute ray's path, and retrive the color
        for(int step = 0; step < 200; step++){
            ray_i.update();
            if(ray_i.x[1] < 1.1*ray_i.metric.rS){
                break;
            }
            if(ray_i.x[1] > 10*ray_i.metric.rS){
                horizon = false;
                break;
            }
        }

        vec4 xf(0, ray_i.x[1], ray_i.x[2], (double)j/(hres+1) - 0.5); // final position of the ray
        if(horizon){
            return -1;
        }
        if(!horizon){
            return 20*abs(xf[3]);
        }
    }


    camera() { 
        wres = 128;
        hres = 64;
        wfov = 3.141592;
        hfov = wfov * ((double)hres/wres);
    }
    camera(int wres, int hres, double wfov, double hfov):
        wres(wres), hres(hres), wfov(wfov), hfov(hfov) { }
};

int main(void){
    cout << "Hello World" << endl;

    camera cam;
    cam.render();

    return 0;
}