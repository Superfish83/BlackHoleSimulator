#include <stdexcept>
#include <cmath>
#include <cstdio>
#include <iostream>

using namespace std;

enum class CoordType {
    UNDEFINED,
    CARTESIAN, // Cartesian coordinates
    SPHERICAL, // Spherical coordinates
};

class vecN {
protected:
    double* data;
    int dim; // Dimension
    CoordType cType = CoordType::UNDEFINED; // Coordinate type
public:
    // Constructor and Destructor
    vecN() : data(nullptr), dim(0) {} // Default constructor
    vecN(int size) : dim(size){ 
        data = new double[size]();
    }
    ~vecN() { delete[] data; }
    // Operator Overloading
    inline double operator[](int index) { return data[index]; }
    inline double& operator[](int index) const { return data[index]; }
    inline vecN& operator=(const vecN& other){
        if (this != &other) { 
            delete[] data; // Free existing memory
            dim = other.dim;
            data = new double[dim];
            for (int i = 0; i < dim; i++) {
                data[i] = other.data[i]; // Copy data
            }
        }
        return *this;
    };
    // Methods
    inline double getDim() const { return dim; }
    inline CoordType getCType() const { return cType; }
    double norm() const;
    inline void set(int index, double value) { data[index] = value; }
};

class vec3 : public vecN {
    public:
    vec3(float x = 0.0f, float y = 0.0f, float z = 0.0f,
        CoordType type=CoordType::UNDEFINED){
        data = new double[3]{x, y, z};
        dim = 3;
        cType = type;
    }
    inline vec3& operator=(const vec3& other){
        if (this != &other) { 
            data[0] = other[0];
            data[1] = other[1];
            data[2] = other[2];
            cType = other.getCType();
        }
        return *this;
    };
    void convToSpherical(); // Manipulate the vector to Spherical coordinates
    void convToCartesian(); // Manipulate the vector to Cartesian coordinates
};
    
// Common functions for vecN
double dot(const vecN& a, const vecN& b);
double distance(const vec3& a_, const vec3& b_);

// Operator Overloading
// They are inline functions, since we don't want the new vec3 object destroyed
// as the call stack unwinds. 
inline ostream& operator<<(ostream& out, const vecN& v);
inline vec3 operator-(const vec3& a) {
    return vec3(-a[0], -a[1], -a[2], a.getCType());
}
inline vec3 operator+(const vec3& a, const vec3& b) {
    return vec3(a[0] + b[0], a[1] + b[1], a[2] + b[2]);
}
inline vec3 operator-(const vec3& a, const vec3& b) {
    return vec3(a[0] - b[0], a[1] - b[1], a[2] - b[2]);
}
inline vec3 operator*(const vec3& v, const double c) {
    return vec3(v[0] * c, v[1] * c, v[2] * c);
}
inline vec3 operator*(const double c, const vec3& v) {
    return vec3(v[0] * c, v[1] * c, v[2] * c);
}

using color = vec3;
const double Pi = 3.1415926536;

class ray {
public:
    double u;
    double Du;
    double D2u;

    double alpha;
    double beta;
    
    double phi = 0;
    const double dphi = 0.05;
    const double rS = 1.0;
    const double bgR = 15.0; // distance from center to background (celestial sphere)

    bool finished = false;
    bool background = false;
    bool horizon = false;
    double phi_final;

    ray(double D_, double alpha_, double beta_):
        u(1/D_), alpha(alpha_), beta(beta_), Du(1/(D_ * tan(alpha_))) { }

    color getColor(){
        int max_iter = 100;
        for(int i = 0; i < max_iter; i++){
            update();
            if(1/u > bgR){
                finished = true;
                background = true;
                phi_final = phi;
                break;
            }
            if(1/u <= rS){
                finished = true;
                horizon = true;
                phi_final = phi;
                break;
            }
        }

        vec3 bgPos = getBgPos();

        // set Color according to bgPos
        
        /*
        double tmp = max(min(abs(bgPos[1]) / (Pi/2), 1.0), 0.0);
        if(background) return color(0.7*tmp, 0.3, 1- 0.7*tmp);
        else return color(0,0,0);
        */

        double delta = 0.0;
        vec3 starpos(bgR, Pi/2 + delta, Pi + delta, CoordType::SPHERICAL);
        if(background){
            const double dist = distance(starpos, bgPos);
            const double tmp = max(0.1, 1 - 0.5 * dist);
            return color(tmp, tmp, tmp);
        }
        else return color(0,0,0);
    }

    // Get the position of the ray's destination in spherical coordinates 
    vec3 getBgPos(){
        double r_rot = bgR;
        //double th_rot = Pi/2 - beta;
        double sp2 = sin(phi_final) * sin(phi_final);
        double th_rot = Pi/2 - abs(atan(sin(phi_final) * tan(beta)));
        double phi_rot = phi_final;

        return vec3(r_rot, th_rot, phi_rot, CoordType::SPHERICAL);
    }
    
    void update(){
        phi += dphi;
        D2u = 1.5 * rS * u*u - u;

        // Euler's method
        u += dphi * Du + 0.5 * (dphi*dphi) * D2u;
        Du += dphi * D2u;
    }
};