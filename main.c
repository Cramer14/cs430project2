#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct {
    int kind; // 0 = sphere, 1 = plane
    double color[3];
    union {
        struct {
            double center[3];
            double radius;
        } sphere;

        struct {
            double length;
            double width;
        } plane;
    }
} Objects;

static inline double sqr(double v) {
    return v*v;
}

static inline void normalize (double* v) {
    double len = sqrt(sqr(v[0]) + sqr(v[1]) + sqr(v[2]));
    v[0] /= len;
    v[1] /= len;
    v[2] /= len;
}

double sphere_interection(double* Ro, double* Rd, double* C, double r) {
    // Step 1. Find the equation for the object you are interested in.
    //
    // 4*pi*r^3 = V
    //
    // Step 2 Parameterize the equation with a center point if needed
    //
    // 2(x-Cx) + 2(y - Cy) + 2(z - Cz) - r^2 = 0
    //
    // Step 3. Substitute the eq for a ray into our object
    //
    // 2(Rox + t * Rdx -Cx) + 2(Roy + t * Rdy - Cy) + 2(Roz + t * Rdz - Cz) - r^2 = 0
    //
    // Step 4a. Write the equation in forms of t
    //
    // t * ( 2 (Rox * Rdx - Cx + Roy * Rdy - Cy + Roz * Rdz - Cz)) - r^2 = 0
    //
    // quadratric formula?

    double a = (sqr(Rd[0] + Rd[1] + Rd[2]));
    double b = (2 * (Ro[0] * Rd[0] - Rd[0] * C[0] + Ro[2] * Rd[2] - Rd[2] * C[2] + Ro[1] * Rd[1] - Rd[1] * C[1]));
    double c = sqr(Ro[0]) - 2*Ro[0]*C[0] + sqr(C[0]) + sqr(Ro[2]) - 2*Ro[2]*C[2] + sqr(C[2]) + sqr(Ro[1]) -2*Ro[1]*C[1] + sqr(C[1]) - sqr(r);

    double det = sqr(b) - 4 * a * c;
    if(det < 0) return -1;

    det = sqrt(det);

    double t0 = (-b - det) / (2*a);
    if (t0>0) return t0;

    double t1 = (-b + det) / (2*a);
    if (t1>0) return t1;


    return -1;



}

int main()
{
    Objects** objects;
    objects = malloc(sizeof(Objects*)*2);
    objects[0] = malloc(sizeof(Objects));
    objects[0]->kind = 0;
    objects[0]->sphere.radius = 2;
    objects[0]->sphere.center[0] = 0;
    objects[0]->sphere.center[1] = 0;
    objects[0]->sphere.center[2] = 20;
    objects[1] = NULL;

    double cx = 0;
    double cy = 0;
    double h = 0.7;
    double w = 0.7;

    int M = 20;
    int N = 20;

    double pixheight = h / M;
    double pixwidth = w / N;

    int y;
    int x;
    int i;

    for(y = 0; y < M; y += 1){
        for(x = 0; x < N; x += 1){
            double Ro[3] = {0, 0, 0};
            double Rd[3] = {cx - (w/2) + pixwidth * (x + 0.5), cy - (h/2) + pixheight * (y + 0.5), 1};

            normalize(Rd);

            double best_t = INFINITY;
            for (i = 0; objects[i]->kind;) {
                double t = 0;

                switch(objects[i]->kind){
                    case 0:
                        t = sphere_interection(Ro, Rd, objects[i]->sphere.center, objects[i]->sphere.radius);
                        break;
                    default:
                        //horrible error
                        exit(1);
                }

                if (t > 0 && t < best_t) best_t = t;

            }

            if(best_t > 0 && best_t != INFINITY){
                printf("#");
            }
            else{
                printf(".");
            }

        }
        printf("\n");

    }

    return 0;
}








