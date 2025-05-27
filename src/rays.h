#ifndef RAYS_H
#define RAYS_H

#include "vectors.h"
#include <cmath>

using namespace std;
using namespace vectors;

namespace rays {
    using color = vec3;

    class schwMetric {
    // class that stores and computes data related to the
    // Schwarzschild metric at a given position
    public:
        // ****************************************************************
        // Arrays that store the metric tensor, partial derivitives,
        // and Christoffel symbols at given point and direction

        // (1) metric tensor
        // g[i] := g_(ii)
        double g[4];

        // (2) inverse metric tensor
        // g_inv[i] := g^(ii) = 1/g_(ii)
        double g_inv[4];
        
        // (3) partial derivatives of the metric
        // part_g[i][j] := \partial_(i) g_(jj)
        double part_g[4][4];

        // (4) Christoffel symbols of the metric
        // christoffel[i][j]][k] := \Gamma^(i)_(jk) 
        double christoffel[4][4][4];

        // ****************************************************************

        double rS = 1.0; // Schwarzchild radius
        const double d = 10e-5; // small value added in division to prevent division by zero

        schwMetric() {
            // Initialize all the arrays
            for(int i = 0; i < 4; i++){
                g[i] = 0;
                g_inv[i] = 0;
                for(int j = 0; j < 4; j++){
                    part_g[i][j] = 0;
                    for(int k = 0; k < 4; k++){
                        christoffel[i][j][k] = 0;
                    }
                }
            }
        }

        void computeMetric(vec4 *x, vec4 *Dx, vec4* D2x);
    };

    class ray {
    public:
        int l; // the parameter (lambda) of the ray path curve

        vec4 x; // position
        vec4 Dx; // derivative of position with respect to lambda
        vec4 D2x; // second derivative of position with respect to lambda

        color rayColor;
        schwMetric metric; // Stores and computes the metric data at ray's position
        
        const double dl = 0.01; // update step size (='delta lambda')
        
        ray (double x1,  double x2,  double x3,
             double Dx1, double Dx2, double Dx3) {
            x   = vec4(0, x1,  x2,  x3,  CoordType::SPHERICAL);
            Dx  = vec4(0, Dx1, Dx2, Dx3, CoordType::SPHERICAL);
            D2x = vec4(0, 0,   0,   0,   CoordType::SPHERICAL);
            metric.computeMetric(&x, &Dx, &D2x);
        }

        void update(){
            // Update x and Dx by Euler's method
            for(int i = 0; i < 4; i++){
                Dx.set(i, 
                    Dx[i] + (dl)*D2x[i]);
                x.set(i, 
                    x[i] + (dl)*Dx[i] + (dl*dl/2) * D2x[i]);
            }
            // Compute metric at updated position
            metric.computeMetric(&x, &Dx, &D2x);
        }
    };
}

#endif