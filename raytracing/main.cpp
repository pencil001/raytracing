//
//  main.cpp
//  raytracing
//
//  Created by 许开平 on 16/6/12.
//  Copyright © 2016年 许开平. All rights reserved.
//

#include <iostream>
#include <fstream>
#include "sphere.h"
#include "hitable_list.h"
#include "camera.h"

vec3 random_in_unit_sphere() {
    vec3 p;
    do {
        p = 2 * vec3(drand48(), drand48(), drand48()) - vec3(1, 1, 1);
    } while (p.squared_length() >= 1.0);
    return p;
}

vec3 color(const ray& r, hitable *world) {
    hit_record rec;
    if (world->hit(r, 0.0, MAXFLOAT, rec)) {
        // 以p + n为圆心，random点可以看做新园的法向量N
        // 则新的hitpoint = p + n + N
        vec3 target = rec.p + rec.normal + random_in_unit_sphere();
        // 新建一个ray，从原有的p射向新的hitpoint，就相当于随机反射（漫反射）
        // 0.5是表示每反射一次吸收50%的光
        return 0.5 * color(ray(rec.p, target - rec.p), world);
    } else {
        vec3 unit_direction = unit_vector(r.direction());
        float t = 0.5 * (unit_direction.y() + 1.0);
        return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
    }
}

int main(int argc, const char * argv[]) {
    int nx = 400;
    int ny = 200;
    int ns = 100;
    
    std::ofstream testfile;
    testfile.open("test.ppm");
    testfile << "P3\n" << nx << " " << ny << "\n255\n";
    
    hitable *list[2];
    list[0] = new sphere(vec3(0, 0, -1), 0.5);
    list[1] = new sphere(vec3(0, -100.5, -1), 100);
    hitable *world = new hitable_list(list, 2);
    
    camera cam;
    for (int j = ny - 1; j >= 0; j--) {
        for (int i  = 0; i < nx; i++) {
            vec3 col(0, 0, 0);
            for (int s = 0; s < ns; s++) {
                float u = float(i + drand48()) / float(nx);
                float v = float(j + drand48()) / float(ny);
                ray r = cam.get_ray(u, v);
                col += color(r, world);
            }
            col /= float(ns);
            col = vec3(sqrtf(col.r()), sqrtf(col.g()), sqrtf(col.b()));
            int ir = int(255.99 * col.r());
            int ig = int(255.99 * col.g());
            int ib = int(255.99 * col.b());
            
            testfile << ir << " " << ig << " " << ib << "\n";
        }
    }
    return 0;
}
