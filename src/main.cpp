#include <iostream>
#include "vectors.h"
#include "rays.h"

using namespace std;
using namespace vectors;
using namespace rays;

int main(void){
    cout << "Hello World" << endl;

    ray testRay( 5, 1, 0,
                -1, 0.1, 0);

    for(int i = 0; i < 100; i++){
        testRay.update();
        if(i%10 != 0) continue;

        cout << "x:\t" << testRay.x << endl;
        cout << "Dx:\t" << testRay.Dx << endl;
        cout << "D2x:\t" << testRay.D2x << endl << endl;
    }

    return 0;
}