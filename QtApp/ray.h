#ifndef RAY_H
#define RAY_H

#include "vecN.h"

const double Pi = 3.1415926536;

typedef struct _pixelData {
    bool outOfBounds;
    int x;
    int y;
} pixelData;

class ray {
public:
    double u;
    double Du;
    double D2u;

    double alpha;
    double beta;
    
    double phi = 0;
    double dphi = 0.003;
    const double rS = 1.0; // Schwarzschild radius
    const double bgR = 500.0; // distance from center to background (celestial sphere)

    bool finished = false;
    bool reachedBg = false;
    bool reachedHorizon = false;
    double phi_final;

    ray(double D_, double alpha_, double beta_):
        u(1/D_), alpha(alpha_), beta(beta_), Du(1/(D_ * tan(alpha_))) { }

    pixelData getBgPixel(int bgW, int bgH){
        int max_iter = 2000;
        for(int i = 0; i < max_iter; i++){
            update();
            if(1/u > bgR){
                finished = true;
                reachedBg = true;
                phi_final = phi;
                break;
            }
            if(1/u <= rS){
                finished = true;
                reachedHorizon = true;
                phi_final = phi;
                break;
            }
        }

        vec3 bgDir = getBgDir();

        // set Color according to bgDir
        if(reachedBg) return getBgProj(bgDir, bgW, bgH);
        else return pixelData {true};
    }

    // Get the position of the ray's destination in spherical coordinates 
    vec3 getBgDir(){
        // transform (rotate along x axis by beta)
        double r_beta = bgR;
        double th_beta = acos(sin(phi_final) * sin(beta));
        double phi_beta = atan(tan(phi_final) * cos(beta)) + Pi;

        return vec3(r_beta, th_beta, phi_beta, CoordType::SPHERICAL);
    }

    pixelData getBgProj(vec3 &dir, int bgW, int bgH){
        vec3 pos;
        pos = dir;

        pos.set(0, -(bgW)/5 / (cos(dir[2])));
        pos.convToCartesian();

        // Get projection to 2d plane
        int x = (int)(bgW * (0.5 - pos[1] / (bgW)));
        int y = (int)(bgH * (0.5 - pos[2] / (bgH)));
        //x = (x + 100*bg_w) % bg_w; // Wrap around
        //y = (y + 100*bg_h) % bg_h; // Wrap around
        if(x < 0 || x >= bgW || y < 0 || y >= bgH){
            return pixelData {true}; // for pixels out of bounds
        }
        else{
            return pixelData {false, x, y};
        }
    }

    void update(){
        D2u = 1.5 * rS * u*u - u;

        // Euler's method
        phi += dphi;
        Du += dphi * D2u;
        u += dphi * Du;
    }
};

#endif
