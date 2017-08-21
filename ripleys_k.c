#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TRUE 1
#define FALSE 0


double random_double(const double min, const double max) {
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    srand(t.tv_nsec % 3451);

    return (min + (rand() / (double) RAND_MAX) * (max - min));
}

double euclidean_distance(double x1, double y1, double x2, double y2) {
    double dx = (x2 - x1);
    double dy = (y2 - y1);

    // note: must compile with -lm to work (using gcc)
    return sqrt((dx * dx) + (dy * dy));
}

int within_radius(double x1, double y1, double x2, double y2,  double radius) {
    // if outside range, eliminate automatically (before doing calculation)
    if (x2 < x1 - radius || x2 > x1 + radius || y2 < y1 - radius || y2 > y1 + radius) {
        return FALSE;
    }

    double dist = euclidean_distance(x1, y1, x2, y2);

    return (dist <= radius) ? TRUE : FALSE;
}

void calculate_k(double points[][2], int size, double radius) {
    int i, j;
    int count = 0;

    for (i = 0; i < size; i++) {
        double x1 = points[i][0];
        double y1 = points[i][1];

        for (j = 0; j < size; j++) {
            if (i == j) continue;

            double x2 = points[j][0];
            double y2 = points[j][1];

            if (within_radius(x1, y1, x2, y2, radius)) {
                count++;
            }
        }
        printf("COUNT FOR %d: %d\n", i, count);
        count = 0;
    }
}

int main(int argc, char *argv[]) { 
    double min = (double) atof(argv[1]);
    double max = (double) atof(argv[2]);
    int i;

    for (i = 0; i < 100; i++) {
        printf("RANDOM NUMBER: %f\n", random_double(min, max));
    }

    printf("DISTANCE: %f\n", euclidean_distance(0.0, 12.2, 31.2, 3.2));
    printf("ISWITHIN???: %i\n", within_radius(0.0, 12.2, 31.2, 3.2, 33.0));


    double points[1000][2];
    
    for (i = 0; i < 1000; i++) {
        double x = random_double(min, max);
        double y = random_double(min, max);
        points[i][0] = x;
        points[i][1] = y;
    }

    calculate_k(points, 1000, 60.0);
}
