#include <stdio.h>
#include <stdlib.h>
#include <math.h>
/////////////////////////
#define N 4
#define G 1 //UA/giorni/masse in unità solari
#define DT 0.01
#define TMAX 100.
///////////////////////// 3
typedef struct vector_3 {
    double x;
    double y;
    double z;
} vector_3;

vector_3 lin_combination_3(double alpha, vector_3 a, double beta, vector_3 b);
double norma(vector_3 a);
vector_3 force(double t, vector_3 a, vector_3 b, double m_a, double m_b);
///////////////////////// 3 * N
typedef struct vector_3n {
   vector_3 system[N];
} vector_3n;

vector_3n lin_combination_3n(double alpha, vector_3n a, double beta, vector_3n b);
vector_3n phi(double t, vector_3n bodies, double m[]);
vector_3n verlet(double t, vector_3n bodies, vector_3n *bodies_old, double m[]);
/////////////////////////

int main() {
    /////////inizializzo/////////////////////////////
    vector_3n bodies;
    vector_3n bodies_old;
    vector_3n v0;
    double m[N];

    m[0] = 10.0;
    bodies.system[0].x = 0.;
    bodies.system[0].y = 0.;
    bodies.system[0].z = 0.;
    v0.system[0].x = 0.;
    v0.system[0].y = 0.;
    v0.system[0].z = 0.;

    m[1] = 1.; 
    bodies.system[1].x = 1.000; 
    bodies.system[1].y = 0.000;  
    bodies.system[1].z = 0.000;
    v0.system[1].x = 0.000;
    v0.system[1].y = 0.0172;     
    v0.system[1].z = 0.000;

    m[2] = 0.1; 
    bodies.system[2].x = 1.1; 
    bodies.system[2].y = 0.000;  
    bodies.system[2].z = 0.000;
    v0.system[2].x = 0.000;
    v0.system[2].y = 0.0172;     
    v0.system[2].z = 0.000;

    m[3] = 1.; 
    bodies.system[3].x = -1.000; 
    bodies.system[3].y = 0.000;  
    bodies.system[3].z = 0.000;
    v0.system[3].x = 0.000;
    v0.system[3].y = -0.0172;     
    v0.system[3].z = 0.000;
    /////////////////////////////////////////////////
    FILE *out = fopen("dati.csv", "w");
    /////////passo zero//////////////////////////////
    fprintf(out, "%lf, ", 0*DT);
    for (int j = 0; j <= N; j++) {
            fprintf(out, "%lf, %lf, %lf, ",bodies.system[j].x, bodies.system[j].y, bodies.system[j].z);
        }
        fprintf(out, "\n");
    bodies_old = bodies;
    bodies = lin_combination_3n(1,lin_combination_3n(1, bodies, DT, v0), DT*DT/2, phi(0, bodies, m));
    /////////////////////////////////////////////////
    for (int i = 1; i <= TMAX/DT; i++) {
        fprintf(out, "%lf, ", i*DT);
        for(int j = 0; j <= N; j++) {
            fprintf(out, "%lf, %lf, %lf, ",bodies.system[j].x, bodies.system[j].y, bodies.system[j].z);
        }
        fprintf(out, "\n");

        bodies = verlet(i * DT, bodies, &bodies_old, m);
    }
    /////////////////////////////////////////////////
    fclose(out);
    return 0;
}

///////////////////////// 3
vector_3 lin_combination_3(double alpha, vector_3 a, double beta, vector_3 b) {
    vector_3 c;
    c.x = alpha * a.x + beta * b.x;
    c.y = alpha * a.y + beta * b.y;
    c.z = alpha * a.z + beta * b.z;
    return c;
}

double norma(vector_3 a) {
    return (sqrt(a.x*a.x + a.y*a.y + a.z*a.z));
}

vector_3 force(double t, vector_3 a, vector_3 b, double m_a, double m_b) {
    //per evitare che il sistema esploda si può implementare un limite superiore alla forza o introdurre urti
    vector_3 r = lin_combination_3(-1, a, +1, b);
    double GMbMa_r_a_b_3;
    double eps;
    if(norma(r) < 0.01) {
        eps = 0.;
    }
    else {
        eps = 0.;
    }
    GMbMa_r_a_b_3 = G * m_b * m_a / pow(norma(r) + eps,3);
    return lin_combination_3(-GMbMa_r_a_b_3, a, +GMbMa_r_a_b_3, b);
}
///////////////////////// 3 * N
vector_3n lin_combination_3n(double alpha, vector_3n a, double beta, vector_3n b) {
    vector_3n c;
    for (int i = 0; i < N; i++) {
        c.system[i].x = alpha * a.system[i].x + beta * b.system[i].x;
        c.system[i].y = alpha * a.system[i].y + beta * b.system[i].y;
        c.system[i].z = alpha * a.system[i].z + beta * b.system[i].z;
    }
    return c;
}

vector_3n phi(double t, vector_3n bodies, double m[]) {
    vector_3n phi = {0};
    vector_3 f;
    // 0.5 * N * (N-1) forze da calcolare (O(N^2)), per ridurre i calcoli si potrebbe implementare Barnes-Hut (O(NlogN))
    for(int i = 0; i < N - 1; i++) {
        for(int j = i + 1; j < N; j++) {
            f = force(t, bodies.system[i], bodies.system[j], m[i], m[j]);
            phi.system[i] = lin_combination_3(1, phi.system[i], 1/m[i], f);
            phi.system[j] = lin_combination_3(1, phi.system[j], -1/m[j], f);
        }
    }
    return phi;
}

vector_3n verlet(double t, vector_3n bodies, vector_3n *bodies_old, double m[]) {
    vector_3n bodies_new;
    bodies_new = lin_combination_3n(2, bodies, -1, *bodies_old);
    bodies_new = lin_combination_3n(1, bodies_new, DT*DT, phi(t, bodies, m));
    *bodies_old = bodies;
    return bodies_new;
}
/////////////////////////
