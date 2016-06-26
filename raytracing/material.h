//
//  material.h
//  raytracing
//
//  Created by Vincent on 16/6/26.
//  Copyright © 2016年 许开平. All rights reserved.
//

#ifndef material_h
#define material_h

#include "ray.h"
#include "hitable.h"

vec3 reflect(const vec3& v, const vec3& n) {
    // 因为这里n是法向量，||n|| = 1
    // 所以dot(v, n) = ||v|| * ||n|| * cos(theta) = ||v|| * cos(theta)
    return v - 2 * dot(v, n) * n;
}


vec3 random_in_unit_sphere() {
    vec3 p;
    do {
        p = 2.0 * vec3(drand48(), drand48(), drand48()) - vec3(1, 1, 1);
    } while (p.squared_length() >= 1.0);
    return p;
}

class material  {
public:
    virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const = 0;
};

class lambertian : public material {
public:
    lambertian(const vec3& a) : albedo(a) {}
    virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const  {
        // 以p + n为圆心，random点可以看做新园的法向量N
        // 则新的hitpoint = p + n + N
        vec3 target = rec.p + rec.normal + random_in_unit_sphere();
        // 新建一个ray，从原有的p射向新的hitpoint，就相当于随机反射（漫反射）
        scattered = ray(rec.p, target - rec.p);
        attenuation = albedo;
        return true;
    }
    
    vec3 albedo;
};

class metal : public material {
public:
    metal(const vec3& a, float f) : albedo(a) { if (f < 1) fuzz = f; else fuzz = 1; }
    virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const  {
        vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
        scattered = ray(rec.p, reflected + fuzz * random_in_unit_sphere());
        attenuation = albedo;
        return (dot(scattered.direction(), rec.normal) > 0);
    }
    vec3 albedo;
    float fuzz;
};


#endif /* material_h */
