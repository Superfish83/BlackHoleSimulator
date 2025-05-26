#include <iostream>

void render() {
    // Image
    int image_width = 128;
    int image_height = 128;

    // Render

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = 0; j < image_height; j++) {
        for (int i = 0; i < image_width; i++) {
            auto r = double(i) / (image_width-1);
            auto g = double(j) / (image_height-1);
            auto b = 0.0;

            int ir = int(127.999 * r);
            int ig = int(127.999 * g);
            int ib = int(127.999 * b);

            std::cout << ir << ' ' << ig << ' ' << ib << '\n';
        }
    }
}