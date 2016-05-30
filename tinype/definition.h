#ifndef TINYPE_DEFINITION_H
#define TINYPE_DEFINITION_H

#include <math.h>
#include <assert.h>


/*
ʾ�����룬�������������˫���ȸ�����
*/


typedef struct {
    double  x;
    double 	y;
    double 	z;
} Vector;

typedef Vector Point;





inline double vectorLength(Vector* v) { return sqrt(v->x*v->x + v->y*v->y + v->z*v->z); };

inline Vector addVector(Vector* v1, Vector* v2) {
    return{ v1->x + v2->x, v1->y + v2->y, v1->z + v2->z };
}; // 

// ��ת������ ��ScalarVector(v, -1) ����

inline Vector differencePoint(Point* p1, Point* p2) {
    return{ p1->x - p2->x, p1->y - p2->y, p1->z - p2->z };
};

// ĳ�㾭��������transform�������µĵ㣬��ʵӦ������ transformPoint(Point, Vector)
inline Point addPoint(Point* p1, Vector* v) {
    return{ p1->x + v->x, p1->y + v->y, p1->z + v->z };
};

// dotProduct
inline double scalarProduct(Vector* v1, Vector* v2){
    return v1->x*v2->x + v1->y*v2->y + v1->z*v2->z;
};

// ��Ҫע��v1 Ϊx�� v2Ϊy��˳���ܸı䣬 ��������� -z   x * y =z
// z * x = y
inline Vector crossVector(Vector* v1, Vector* v2){
    return{ v1->y * v2->z - v2->y * v1->z,
        v1->z * v2->x - v2->z * v1->x,
        v1->x*v2->y - v1->y * v2->x };
};

inline Vector scalarVector(Vector* v, double scale) {
    Vector v1;
    v1.x = v->x * scale;
    v1.y = v->y * scale;
    v1.z = v->z * scale;

    return v1;
};

inline Vector normalize(Vector* v){
    Vector v1;
    double l = vectorLength(v);
    v1.x = v->x / l;
    v1.y = v->y / l;
    v1.z = v->z / l;
    return  v1;
};

typedef struct {
    Vector pos;
    Vector velocity;
    Vector acceleration;   // ?

    double damping;
    double inverseMass;
    Vector  forceAccum; // �洢��һ��ģ�����������ÿ�λ��ֺ�����
} Particle;


inline void particleIntegrate(Particle* p, double duration) {
    assert(duration > 0.0);

    addVector(&p->pos, &scalarVector(&p->velocity, duration));
    Vector resultingAcc = p->acceleration;
    // F = ma =>  a = F * (1/m) 
    addVector(&resultingAcc, &scalarVector(&p->forceAccum, p->inverseMass));
    addVector(&p->velocity, &scalarVector(&resultingAcc, duration));

    scalarVector(& p->velocity,  pow(p->damping, duration) );

}


#endif
