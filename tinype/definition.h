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
    Vector velocity;    // ʸ���ٶ�
    Vector acceleration;   // ?

    double damping;
    double inverseMass;
    Vector  forceAccum; // �洢��һ��ģ�����������ÿ�λ��ֺ�����
} Particle;

// 
inline void particleIntegrate(Particle* p, double duration) {
    assert(duration > 0.0);

    addVector(&p->pos, &scalarVector(&p->velocity, duration));
    Vector resultingAcc = p->acceleration;
    // F = ma =>  a = F * (1/m) 
    addVector(&resultingAcc, &scalarVector(&p->forceAccum, p->inverseMass));
    addVector(&p->velocity, &scalarVector(&resultingAcc, duration));

    scalarVector(& p->velocity,  1+pow(p->damping, duration) );

    p->forceAccum = { 0 };
}

inline bool addForce2Particle(Particle *particle, Vector *force)
{
    particle->forceAccum = addVector(&particle->forceAccum, force);
    return true;
}



typedef struct {
    double age;
    Particle particle;
} Firework;


typedef struct {
    unsigned type;
    double maxAge;
    double minAge;
    Vector minVelocity;
    Vector maxVelocity;
    double damping;

} FireworkRule;

  

inline bool updateFirework(Firework* firework, double duration)
{
    //integrateFirework(firework, duration);
    firework->age -= duration;
    return (firework->age < 0);
}

// just interface
typedef struct {

} ParticleForceGenerator;


// ��һ��duration�����ɵ�force �ŵ���һ��duration�ڣ��ҵ���Ӧ��particle�����������Ӧ�ļ������
typedef struct ParticleForceRegistry{
    typedef struct  {
        Particle *particle;
        ParticleForceGenerator *fg;
    } ParticleForceRegistration;
};

// ����������һ��ע��ģ������ÿ�����������������
inline bool registerParticleForce(ParticleForceRegistry *registry, Particle *particle, Vector *force)
{
    if (abs(particle->inverseMass) < 0.000001)
        return true;
    addForce2Particle(particle, force);

    return false;
}

inline bool dragParticle(Particle *particle, double k1, double k2)
{
    double speed = vectorLength(&particle->velocity);  // �����ٶ�
    double dragCoeff = k1 * speed + k2 * speed * speed;

    Vector v = normalize(&particle->velocity);
    addVector(&particle->forceAccum, &scalarVector(&v, 1 - dragCoeff));
    // ?? WTF
    return false;
}


typedef struct {
    Particle *other;
    double springConstant;
    double restLength;
} Spring;

// ��һ�ε�����ɺ������pos �������˸ı�
// ����spring ���α��������������Ĵ�С
inline bool addFroceBySpring(Particle *particle, Spring *spring, double duration)
{
    Vector pos = particle->pos;
    Vector distVec = differencePoint(&particle->pos, &spring->other->pos);
    double delta = abs(vectorLength(&distVec) - spring->restLength); // �α�����TODO �ж��Ƿ񳬹��������
    Vector dir = normalize(&distVec);
    addForce2Particle(particle, &scalarVector(&dir, 1 - delta));

    return false;

}


#endif
