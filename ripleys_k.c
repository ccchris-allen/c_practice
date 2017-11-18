#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TRUE 1
#define FALSE 0
#define INF INFINITY
#define MAX(x, y) (x > y) ? x : y
#define MIN(x, y) (x < y) ? x : y

typedef struct BOUNDS {
    double minx;
    double miny;
    double maxx;
    double maxy;
} BOUNDS;

double mean(int arr[], int size) {
    int i;
    double sum = 0.0;

    for (i = 0; i < size; i++) {
        sum += arr[i];
    }

    return sum / (double) size;
}
    
BOUNDS bounds(double **points, int size) {
    int i;
    BOUNDS bound = { INF, INF, -INF, -INF };

    for (i = 0; i < size; i++) { 
        bound.minx = MIN(bound.minx, points[i][0]);
        bound.miny = MIN(bound.miny, points[i][1]);
        bound.maxx = MAX(bound.maxx, points[i][0]);
        bound.maxy = MAX(bound.maxy, points[i][1]);
    }

    return bound;
}

double random_double(const double min, const double max) {
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    srand(t.tv_nsec * t.tv_nsec);

    return (min + (rand() / (double) RAND_MAX) * (max - min));
}

double **random_pts(int size, double minx, double miny, double maxx, double maxy) {
    double **points = (double **) malloc(size * sizeof(double *));
    int i;

    for (i = 0; i < size; i++) {
        points[i] = (double *) malloc(2 * sizeof(double));
        points[i][0] = random_double(minx, maxx);
        points[i][1] = random_double(miny, maxy);
    }

    return points;
}

void free_points(double **points, int size) {
    int i;
    for (i = 0; i < size; i++) free(points[i]);
    free(points);
}

double euclidean_distance(double x1, double y1, double x2, double y2) {
    double dx = (x2 - x1);
    double dy = (y2 - y1);

    // note: must compile with -lm to work (using gcc)
    return sqrt((dx * dx) + (dy * dy));
}

int within_radius(double x1, double y1, double x2, double y2,  double radius) {
    double dx = abs(x2 - x1);
    double dy = abs(y2 - y1);

    if (dx > radius || dy > radius) {
        return FALSE;
    } 
    
    if ((dx + dy) <= radius) {
        return TRUE;
    } 

    double dxdx = (dx * dx);
    double dydy = (dy * dy);
    double rr = (radius * radius);
     
    return (dxdx + dydy) <= rr;
}

double kest(double **points, int size, double radius) {
    int i, j;
    int total = 0;
    //double distances[size][size] = { -INF }; //cache these?? not implemented currently
    BOUNDS bound = bounds(points, size);
    double area = (bound.maxx - bound.minx) * (bound.maxy - bound.miny);

    for (i = 0; i < size; i++) {
        double x1 = points[i][0];
        double y1 = points[i][1];

        for (j = 0; j < size; j++) {
            if (i == j) continue;

            double x2 = points[j][0];
            double y2 = points[j][1];

            if (within_radius(x1, y1, x2, y2, radius)) {
                total++;
            }
        }
    }

    double density = size / area;
    double kd = (1 / density) *  (total / (double) size);

    return kd;
}

double lest(double **points, int size, double radius) {
    double k = kest(points, size, radius);

    return sqrt(k / M_PI);
}
    
double lest_zero(double **points, int size, double radius) {
    double l = lest(points, size, radius);

    return radius - l;
}

int main(int argc, char *argv[]) { 

    if (argc != 2) {
        printf("Need two args: min and max!\n");
        return 1;
    }

    double min = (double) atof(argv[1]);
    double max = (double) atof(argv[2]);
    int i, j;
   
    int size = 5000;
    int num_folds = 99;
    double radius = 10.0; 
    double scores[5] = { 0 };
    double radii[5] = { 50.0, 100.0, 150.0, 200.0, 250.0 };

    for (j = 0; j < num_folds; j++) {
        double **points = random_pts(size, min, min, max, max);

        for (i = 0; i < 5; i++) {
            double v = lest_zero(points, size, radii[i]);
            scores[i] += v;
        }

        free_points(points, size);
    }

    for (i = 0; i < 5; i++) {
        printf("r=%f, v=%f\n", radii[i], scores[i] / (double) num_folds); 
    }
}
