#include "vectors.h"
#include "rays.h"
#include <cmath>

using namespace std;
using namespace rays;
using namespace vectors;

void schwMetric::computeMetric(vec4 *x, vec4 *Dx, vec4 *D2x){
    double r = (*x)[1];
    double th = (*x)[2];

    double tmp;

    // (1) Compute tensor
    g[0] = rS / (r + d) - 1.0;
    g[1] = r / (r - rS) - 1;
    g[2] = r * r;
    tmp = r * sin(th);
    g[3] = tmp * tmp;

    // (2) Compute inverse tensor
    for(int i = 0; i < 4; i++){
        g_inv[i] = 1 / (g[i]+d);
    }
    
    // (3) Compute partial
    part_g[1][0] = -rS / (r*r+d);
    tmp = (r-rS);
    part_g[1][1] = -rS / (tmp*tmp+d);
    part_g[1][2] = 2*r;
    tmp = sin(th);
    part_g[1][3] = 2*r * tmp*tmp;
    part_g[2][3] = r*r * sin(2*th);

    // (4) Compute Christoffel
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            for(int k = 0; k < 4; k++){
                christoffel[i][j][k] = 0.0;
                if(i==k) christoffel[i][j][k] += g_inv[i] * part_g[j][i];
                if(i==j) christoffel[i][j][k] += g_inv[i] * part_g[k][i];
                if(j==k) christoffel[i][j][k] += g_inv[i] * part_g[i][j];
            }
        }
    }

    // (5) Compute and update D2x using the geodesic equation
    for(int i = 0; i < 4; i++){
        double sum = 0.0;
        for(int j = 0; j < 4; j++){
            for(int k = 0; k < 4; k++){
                sum -= christoffel[i][j][k] * (*Dx)[j] * (*Dx)[k];
            }
        }
        (*D2x).set(i, sum);
    }
}