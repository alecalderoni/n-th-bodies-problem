#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define GAMMA 40
#define A 5
#define B 0.0000000365
#define C 0.03
#define D 0.0000000365

typedef struct vector {
    double x;
    double y;
} vector;

typedef struct phaseSpace {
    vector r;
    vector v;
} phaseSpace;

typedef struct bodies {
    phaseSpace uno;
    phaseSpace due;
    phaseSpace tre;
    phaseSpace quattro;
} bodies;

double modulo (vector a);
vector linear_combination(vector a, vector b, double alpha, double beta);
vector acceleration(vector a, vector b, double alpha, double beta);
void euler_cromer(double dt, bodies* planets);

///////////////////////////////////////////////////////

int main() {

    double t = 1000;
    double dt = 0.01;
    int n_of_steps = t / dt;

    phaseSpace first_body;
    phaseSpace second_body;
    phaseSpace third_body;
    phaseSpace fourth_body;

    bodies planets;
    // condizioni iniziali //////////
    vector r_uno_0 = {0, 0};
    vector v_uno_0 = {0, 0};

    vector r_due_0 = {10, 0};
    vector v_due_0 = {0, 5};

    vector r_tre_0 = {15, 0};
    vector v_tre_0 = {0, 4};
    
    vector r_quattro_0 = {15.13, 0};
    vector v_quattro_0 = {0, 8};

    first_body.r = r_uno_0;
    first_body.v = v_uno_0;

    second_body.r = r_due_0;
    second_body.v = v_due_0;

    third_body.r = r_tre_0;
    third_body.v = v_tre_0;

    fourth_body.r = r_quattro_0;
    fourth_body.v = v_quattro_0;

    planets.uno = first_body;
    planets.due = second_body;
    planets.tre = third_body;
    planets.quattro = fourth_body;

    //////////////////////////////////
    FILE *out = fopen("dati.csv", "w");

    fprintf(out, "t,x_uno,y_uno,x_due,y_due,x_tre,y_tre,x_quattro,y_quattro\n");
    fprintf(out, "%g,%g,%g,%g,%g,%g,%g,%g,%g\n", (double)0*dt, planets.uno.r.x, planets.uno.r.y, planets.due.r.x, planets.due.r.y, planets.tre.r.x, planets.tre.r.y, planets.quattro.r.x, planets.quattro.r.y);

    for (int i = 1; i <= n_of_steps; i++) {
        euler_cromer(dt, &planets);

        fprintf(out, "%g,%g,%g,%g,%g,%g,%g,%g,%g\n", (double)i*dt, planets.uno.r.x, planets.uno.r.y, planets.due.r.x, planets.due.r.y, planets.tre.r.x, planets.tre.r.y, planets.quattro.r.x, planets.quattro.r.y);

    }
    
    fclose(out);
    return 0;
}

////////////////////////////////////////////////////////

double modulo(vector a) {
    return sqrt(a.x * a.x + a.y * a.y);
}

vector linear_combination(vector a, vector b, double alpha, double beta) {
    vector temp = {alpha * a.x + beta * b.x, alpha * a.y + beta * b.y};
    return temp;
}

vector acceleration(vector a, vector b, double alpha, double beta) {
    vector temp;
    vector ab;

    ab = linear_combination(a, b, -1, 1);

    temp.x = (GAMMA * beta / (pow(modulo(ab), 3))) * ab.x;
    temp.y = (GAMMA * beta / (pow(modulo(ab), 3))) * ab.y;

    return temp;
}

void euler_cromer(double dt, bodies* planets) {

    vector acc_uno, acc_due, acc_tre, acc_quattro;
    ///////////////////////////////////////////////////////////////////////
    acc_uno = acceleration(planets->uno.r, planets->due.r, A, B);
    acc_uno = linear_combination(acc_uno, acceleration(planets->uno.r, planets->tre.r, A, C), 1, 1);
    acc_uno = linear_combination(acc_uno, acceleration(planets->uno.r, planets->quattro.r, A, D), 1, 1);

    acc_due = acceleration(planets->due.r, planets->uno.r, B, A);
    acc_due = linear_combination(acc_due, acceleration(planets->due.r, planets->tre.r, B, C), 1, 1);
    acc_due = linear_combination(acc_due, acceleration(planets->due.r, planets->quattro.r, B, D), 1, 1);

    acc_tre = acceleration(planets->tre.r, planets->uno.r, C, A);
    acc_tre = linear_combination(acc_tre, acceleration(planets->tre.r, planets->due.r, C, B), 1, 1);
    acc_tre = linear_combination(acc_tre, acceleration(planets->tre.r, planets->quattro.r, D, D), 1, 1);
    
    acc_quattro = acceleration(planets->quattro.r, planets->uno.r, D, A);
    acc_quattro = linear_combination(acc_quattro, acceleration(planets->quattro.r, planets->due.r, D, B), 1, 1);
    acc_quattro = linear_combination(acc_quattro, acceleration(planets->quattro.r, planets->tre.r, D, C), 1, 1);
    ///////////////////////////////////////////////////////////////////////
    planets->uno.v = linear_combination(planets->uno.v, acc_uno, 1, dt);
    planets->due.v = linear_combination(planets->due.v, acc_due, 1, dt);
    planets->tre.v = linear_combination(planets->tre.v, acc_tre, 1, dt);
    planets->quattro.v = linear_combination(planets->quattro.v, acc_quattro, 1, dt);
    ///////////////////////////////////////////////////////////////////////
    planets->uno.r = linear_combination(planets->uno.r, planets->uno.v, 1, dt);
    planets->due.r = linear_combination(planets->due.r, planets->due.v, 1, dt);
    planets->tre.r = linear_combination(planets->tre.r, planets->tre.v, 1, dt);
    planets->quattro.r = linear_combination(planets->quattro.r, planets->quattro.v, 1, dt);
}