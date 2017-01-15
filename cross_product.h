#ifndef CROSS_PRODUCT_H
#define CROSS_PRODUCT_H

void cross_product(const float* v1, const float* v2, float* v3) {
    v3[0] = (v1[1] * v2[2]) - (v1[2] * v2[1]);
    v3[1] = (v1[2] * v2[0]) - (v1[0] * v2[2]);
    v3[2] = (v1[0] * v2[1]) - (v1[1] * v2[0]);
}

void cross_product(point3f *v1, point3f *v2, point3f *v3) {
    v3->x = (v1->y * v2->z) - (v1->z * v2->y);
    v3->y = (v1->z* v2->x) - (v1->x * v2->z);
    v3->z = (v1->x * v2->y) - (v1->y * v2->x);
}

#endif
