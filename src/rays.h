#ifndef VECN_H
#define VECN_H

#include <stdexcept>
#include <cmath>
#include <cstdio>
#include <iostream>

namespace vec {
    enum class CoordType {
        UNDEFINED,
        CARTESIAN, // Cartesian coordinates
        SPHERICAL, // Spherical coordinates
    };
    enum class VecType {
        UNDEFINED,
        COVARIANT,
        CONTRAVARIANT
    };

    class vecN {
    protected:
        double* data;
        int dim; // Dimension
        CoordType cType = CoordType::UNDEFINED; // Coordinate type
        VecType vType = VecType::UNDEFINED; // Vector type (covariant or contravariant)
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

    class vec4;
    class vec3 : public vecN {
        public:
        vec3(float x = 0.0f, float y = 0.0f, float z = 0.0f,
            CoordType type=CoordType::UNDEFINED){
            data = new double[3]{x, y, z};
            dim = 3;
            cType = type;
        }
        inline vec4 getVec4() const;
        void convToSpherical(); // Manipulate the vector to Spherical coordinates
        void convToCartesian(); // Manipulate the vector to Cartesian coordinates
    };
    
    class vec4 : public vecN {
        public:
        vec4(float t = 0.0f, float x = 0.0f, float y = 0.0f, float z = 0.0f,
            CoordType type=CoordType::UNDEFINED) {
            data = new double[4]{t, x, y, z};
            dim = 4;
            cType = type;
        }
        void setCoordType(CoordType type) {
            if(cType==CoordType::UNDEFINED) cType = type;
            else throw std::invalid_argument("Illegal coordinate conversion: cannot setCoordinate to a vector which already has a coordinate type.");
        }
        inline vec3 getVec3() const;
        void convToSpherical(); // Manipulate the vector to Spherical coordinates
        void convToCartesian(); // Manipulate the vector to Cartesian coordinates
    };

    
    // Common functions for vecN
    double dot(const vecN& a, const vecN& b);

    // Operator Overloading
    // They are inline functions, since we don't want the new vec3 object destroyed
    // as the call stack unwinds. 
    inline std::ostream& operator<<(std::ostream& out, const vecN& v){
        CoordType cType = v.getCType();
        out << "Dims: " << v.getDim() << "d, Data: [";
        for (int i = 0; i < v.getDim(); i++) {
            out << v[i] << (i < v.getDim() - 1 ? ", " : "");
        }
        out << "]";
        if(cType != CoordType::UNDEFINED) {
            switch(v.getCType()) {
                case CoordType::CARTESIAN: out << " (Cartesian)"; break;
                case CoordType::SPHERICAL: out << " (Spherical)"; break;
            }
        }
        return out;
    }
    
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

    inline vec4 operator-(const vec4& a) {
        return vec4(-a[0], -a[1], -a[2], -a[3], a.getCType());
    }
    inline vec4 operator+(const vec4& a, const vec4& b) {
        return vec4(a[0] + b[0], a[1] + b[1], a[2] + b[2], a[3] + b[3]);
    }
    inline vec4 operator-(const vec4& a, const vec4& b) {
        return vec4(a[0] - b[0], a[1] - b[1], a[2] - b[2], a[3] - b[3]);
    }
    inline vec4 operator*(const vec4& v, const double c) {
        return vec4(v[0] * c, v[1] * c, v[2] * c, v[3] * c);
    }
    inline vec4 operator*(const double c, const vec4& v) {
        return vec4(v[0] * c, v[1] * c, v[2] * c, v[3] * c);
    }
};

#endif

#ifndef RAYS_H
#define RAYS_H

using namespace std;
using namespace vec;

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

        void computeMetric(vec4 *x, vec4 *Dx, vec4 *D2x);
    };

    class ray {
    public:
        int l; // the parameter (lambda) of the ray path curve

        vec4 x; // position
        vec4 Dx; // derivative of position with respect to lambda
        vec4 D2x; // second derivative of position with respect to lambda

        color rayColor;
        schwMetric metric; // Stores and computes the metric data at ray's position
        
        double dl; // update step size (='delta lambda')
        
        ray () {}
        ray (vec4 x_, vec4 Dx0){
            if(x_.getCType() != CoordType::SPHERICAL)
                throw std::invalid_argument("ray(): x should be in spherical coordinate.");
            if(Dx0.getCType() != CoordType::SPHERICAL)
                throw std::invalid_argument("ray(): Dx should be in spherical coordinate.");
            x   = x_;
            Dx  = Dx0;
            D2x = vec4(0,   0,   0,   0,   CoordType::SPHERICAL);
            metric.computeMetric(&x, &Dx, &D2x);
        }
        ray (double x1,  double x2,  double x3,
             double Dx1, double Dx2, double Dx3) {
            x   = vec4(0,   x1,  x2,  x3,  CoordType::SPHERICAL);
            Dx  = vec4(0,   Dx1, Dx2, Dx3, CoordType::SPHERICAL);
            D2x = vec4(0,   0,   0,   0,   CoordType::SPHERICAL);
            metric.computeMetric(&x, &Dx, &D2x);
        }

        void update(){
            vec4 v(0, Dx[1], (x[1]*Dx[2]), (x[1]*sin(x[2])*Dx[3]));
            dl = 0.05 / v.norm();
            if(x[1] < 4) dl = 0.005 / v.norm();

            // Update x and Dx by Euler's method
            for(int i = 0; i < 4; i++){
                x.set(i, 
                    x[i] + (dl)*Dx[i] + (dl)*(dl)/2*D2x[i] );
                Dx.set(i, 
                    Dx[i] + (dl)*D2x[i]);
            }
            // Compute metric at updated position
            metric.computeMetric(&x, &Dx, &D2x);
        }
    };
}

#endif