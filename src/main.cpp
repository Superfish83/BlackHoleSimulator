#include <iostream>
#include "vectors.h"


using namespace std;
using namespace vectors;

int main(void){
    cout << "Hello World" << endl;
    vec4 v1(0, 1.0f, 2.0f, 3.0f);
    vec4 v2(0, 2.0f, 1.0f, 5.0f);
    vec4 v3 = 0.1 * (v1 + v2);

    cout << v1 << endl;
    cout << v2 << endl;
    cout << v3 << endl;
    cout << "Norm of v3: " << v3.norm() << endl;
    cout << "Dot product of v1 and v2: " << dot(v1, v2) << endl;

    return 0;
}