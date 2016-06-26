//
//  hitable.h
//  raytracing
//
//  Created by Vincent on 16/6/26.
//  Copyright © 2016年 许开平. All rights reserved.
//

#ifndef hitable_h
#define hitable_h

#include "ray.h"

struct hit_record {
    float t;
    vec3 p;
    vec3 normal;
};

class hitable {
public:
    virtual bool hit(const ray &r, float t_min, float t_max, hit_record &rec) const = 0;
};


#endif /* hitable_h */
