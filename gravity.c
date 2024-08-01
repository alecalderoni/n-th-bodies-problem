#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define GAMMA 40
#define A 5
#define B 0.0000000365
#define C 0.03
#define D 0.0000000365
#define DT 0.01
#define T 1000

struct vector {
    double x;
    double y;
};

struct phaseSpace {
    struct vector r;
    struct vector v;
};

struct bodies {
    struct phaseSpace uno;
    struct phaseSpace due;
    struct phaseSpace tre;
    struct phaseSpace quattro;
};

double modulo(struct vector a);
struct vector linear_combination(struct vector a, struct vector b, double alpha, double beta);
struct vector acceleration(struct vector a, struct vector b, double alpha, double beta);
struct bodies euler_cromer(struct bodies planets);

///////////////////////////////////////////////////////

int main() {

    int n_of_steps = (int)(T / DT);

    struct phaseSpace first_body;
    struct phaseSpace second_body;
    struct phaseSpace third_body;
    struct phaseSpace fourth_body;

    struct bodies planets;
    // condizioni iniziali //////////
    struct vector r_uno_0 = {0, 0};
    struct vector v_uno_0 = {0, 0};

    struct vector r_due_0 = {10, 0};
    struct vector v_due_0 = {0, 5};

    struct vector r_tre_0 = {15, 0};
    struct vector v_tre_0 = {0, 4};
    
    struct vector r_quattro_0 = {15.13, 0};
    struct vector v_quattro_0 = {0, 8};

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
    fprintf(out, "%g,%g,%g,%g,%g,%g,%g,%g,%g\n", (double)0*DT, planets.uno.r.x, planets.uno.r.y, planets.due.r.x, planets.due.r.y, planets.tre.r.x, planets.tre.r.y, planets.quattro.r.x, planets.quattro.r.y);

    for (int i = 1; i <= n_of_steps; i++) {
        planets = euler_cromer(planets);

        fprintf(out, "%g,%g,%g,%g,%g,%g,%g,%g,%g\n", (double)i*DT, planets.uno.r.x, planets.uno.r.y, planets.due.r.x, planets.due.r.y, planets.tre.r.x, planets.tre.r.y, planets.quattro.r.x, planets.quattro.r.y);

    }
    
    fclose(out);
    return 0;
}

////////////////////////////////////////////////////////

double modulo(struct vector a) {
    double temp;

    temp = sqrt(a.x * a.x + a.y * a.y);

    return temp;
}

struct vector linear_combination(struct vector a, struct vector b, double alpha, double beta) {
    struct vector temp;

    temp.x = alpha * a.x + beta * b.x;
    temp.y = alpha * a.y + beta * b.y;

    return temp;
}

struct vector acceleration(struct vector a, struct vector b, double alpha, double beta) {
    struct vector temp;
    struct vector ab;

    ab = linear_combination(a, b, -1, 1);

    temp.x = (GAMMA * beta / (pow(modulo(ab), 3))) * ab.x;
    temp.y = (GAMMA * beta / (pow(modulo(ab), 3))) * ab.y;

    return temp;
}

struct bodies euler_cromer(struct bodies planets) {

    struct vector acc_uno, acc_due, acc_tre, acc_quattro;
    ///////////////////////////////////////////////////////////////////////
    acc_uno = acceleration(planets.uno.r, planets.due.r, A, B);
    acc_uno = linear_combination(acc_uno, acceleration(planets.uno.r, planets.tre.r, A, C), 1, 1);
    acc_uno = linear_combination(acc_uno, acceleration(planets.uno.r, planets.quattro.r, A, D), 1, 1);

    acc_due = acceleration(planets.due.r, planets.uno.r, B, A);
    acc_due = linear_combination(acc_due, acceleration(planets.due.r, planets.tre.r, B, C), 1, 1);
    acc_due = linear_combination(acc_due, acceleration(planets.due.r, planets.quattro.r, B, D), 1, 1);

    acc_tre = acceleration(planets.tre.r, planets.uno.r, C, A);
    acc_tre = linear_combination(acc_tre, acceleration(planets.tre.r, planets.due.r, C, B), 1, 1);
    acc_tre = linear_combination(acc_tre, acceleration(planets.tre.r, planets.quattro.r, D, D), 1, 1);
    
    acc_quattro = acceleration(planets.quattro.r, planets.uno.r, D, A);
    acc_quattro = linear_combination(acc_quattro, acceleration(planets.quattro.r, planets.due.r, D, B), 1, 1);
    acc_quattro = linear_combination(acc_quattro, acceleration(planets.quattro.r, planets.tre.r, D, C), 1, 1);
    ///////////////////////////////////////////////////////////////////////
    planets.uno.v = linear_combination(planets.uno.v, acc_uno, 1, DT);
    planets.due.v = linear_combination(planets.due.v, acc_due, 1, DT);
    planets.tre.v = linear_combination(planets.tre.v, acc_tre, 1, DT);
    planets.quattro.v = linear_combination(planets.quattro.v, acc_quattro, 1, DT);
    ///////////////////////////////////////////////////////////////////////
    planets.uno.r = linear_combination(planets.uno.r, planets.uno.v, 1, DT);
    planets.due.r = linear_combination(planets.due.r, planets.due.v, 1, DT);
    planets.tre.r = linear_combination(planets.tre.r, planets.tre.v, 1, DT);
    planets.quattro.r = linear_combination(planets.quattro.r, planets.quattro.v, 1, DT);

    return planets;
}