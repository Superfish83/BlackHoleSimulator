#include <iostream>
#include <cmath>
#include "rays.h"

using namespace std;
using namespace vec;
using namespace rays;

class camera{
private:
    ray ray_i;
    color pixel_i;
public:
    // resolution (w x h)
    int wres;
    int hres;

    // field of view (in degrees)
    double wfov;
    double hfov;

    void getPoint(int i, int j){
        // Create a new ray object (for the ith row, jth col of image)
        double r0 = 5.0;
        double th0 = 0;
        double phi0 = 0;

        double d_r0 = -1;
        double d_th0 = wfov * (i/wres - 0.5);
        double d_phi0 = hfov * (j/hres - 0.5);

        vec4 x0(0, r0, th0, phi0, CoordType::SPHERICAL);
        vec4 Dx0(0, d_r0, d_th0, d_phi0, CoordType::SPHERICAL);

        ray ray_i(x0, Dx0);
    }


    camera() { 
        wres = 128;
        hres = 1;
        wfov = 3.141592 / 2;
        hfov = 3.141592 / 2;
    }
    camera(int wres, int hres, double wfov, double hfov):
        wres(wres), hres(hres), wfov(wfov), hfov(hfov) { }
};

int main(void){
    cout << "Hello World" << endl;

    while(true){
        double a;
        cin >> a;
    ray testRay( 5, 0, 0,
                -cos(a), sin(a), 0);

    for(int i = 0; i < 1000; i++){
        testRay.update();
        
        if(testRay.x[1] < 1.001*testRay.metric.rS){
            cout << "ray reached event horizon" << endl;
            cout << "x:\t" << testRay.x << endl;
            break;
        }
        if(testRay.x[1] > 20*testRay.metric.rS){
            cout << "ray reached celestial sphere" << endl;
            cout << "x:\t" << testRay.x << endl;
            break;
        }

        /*
        if(i%10 != 0) continue;
        cout << "x:\t" << testRay.x << endl;
        cout << "Dx:\t" << testRay.Dx << endl;
        cout << "D2x:\t" << testRay.D2x << endl << endl;
        */
    }
}

    return 0;
}