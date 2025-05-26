#ifndef VECTORS_H
#define VECTORS_H

namespace vectors {
    enum class Coordtype {
        CARTESIAN, // Cartesian coordinates
        SPHERICAL, // Spherical coordinates
    };

    class vecN {
    protected:
        double* data;
        int dim; // Dimension
        Coordtype type;
    public:
        // Constructor and Destructor
        vecN() : data(nullptr), dim(0) {} // Default constructor
        vecN(int size) : dim(size), type(Coordtype::CARTESIAN){ 
            data = new double[size]();
        }
        ~vecN() { delete[] data; }
        // Operator Overloading
        double operator[](int index) { return data[index]; }
        double& operator[](int index) const { return data[index]; }
        inline vecN& operator=(const vecN& other);
        // Methods
        inline double getDim() const { return dim; }
        inline Coordtype getType() const { return type; }
        double norm() const;
    };

    class vec3 : public vecN {
        public:
        vec3(float x = 0.0f, float y = 0.0f, float z = 0.0f,
            Coordtype type=Coordtype::CARTESIAN){
            data = new double[3]{x, y, z};
            dim = 3;
            type = type;
        }
    };

    class vec4 : public vecN {
        public:
        vec4(float t = 0.0f, float x = 0.0f, float y = 0.0f, float z = 0.0f,
            Coordtype type=Coordtype::CARTESIAN) {
            data = new double[4]{t, x, y, z};
            dim = 4;
            type = type;
        }
    };

    
    // Common functions for vecN
    double dot(const vecN& a, const vecN& b);

    // Operator Overloading
    // They are inline functions, since we don't want the new vec3 object destroyed
    // as the call stack unwinds. 
    inline std::ostream& operator<<(std::ostream& out, const vecN& v){
        out << "Vector Dim: " << v.getDim() << ", Data: [";
        for (int i = 0; i < v.getDim(); i++) {
            out << v[i] << (i < v.getDim() - 1 ? ", " : "");
        }
        out << "]";
        return out;
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