#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "ssdb_core.h"

static void test_points(int i, double xmax, double ymax) {
    int j = 0;
    ssdb_point_t** points = malloc(sizeof(ssdb_point_t*) * i);
    for(j=0; j<i; j++) {
        points[j] = ssdb_point_new();
        points[j]->lon = (double)((xmax/2) - (rand()%(int)xmax));
        points[j]->lat = (double)((ymax/2) - (rand()%(int)ymax));
        printf("created point (%d): t: %d, x: %f, y: %f\n", 
            j, points[j]->type, points[j]->lon, points[j]->lat
        );
        if(j>0) {
            printf("\tdistance from point %d: %f\n", j-1, ssdb_point_distance(points[j-1], points[j]));
            printf("\tdistance from point %d: %f (w. Lambert flattening)\n", j-1, ssdb_point_distance_lambert(points[j-1], points[j]));
        }
    }
    printf("created %d points...\n", j);
    for(j=0; j<i; j++) {
        ssdb_point_destroy(points[j]);
        printf("destroyed point (%d)\n", j);
    }
    free(points);
    points = NULL;
    printf("done.\n");
    return;
}

static void test_linestrings(int num, int max_points) {
    int j;
    int k;
    int lp;
    ssdb_point_t* p;
    for(j=0; j<num; j++) {
        ssdb_linestring_t* ls = ssdb_linestring_new();
        printf("created linestring (%d): t: %d, num_points: %d\n", j, ls->type, ls->num_points);
        lp = rand() % max_points;
        for(k=0; k<lp; k++) {
            p = ssdb_point_new();
            p->lon = (double)((360/2) - (rand()%(int)360));
            p->lat = (double)((180/2) - (rand()%(int)180));
            ssdb_linestring_append(ls, p);
        }
        printf("\tlinestring (%d): t: %d, num_points: %d, len: %f\n", j, ls->type, ls->num_points, ssdb_linestring_length(ls));
        ssdb_linestring_destroy(ls);
        printf("destroyed linestring...(%d)\n", j);
    }
    return;
}

int main(int argc, char** argv) {
    
    srand(time(NULL));
    test_points(10, 360, 180);
    test_linestrings(10, 10);

    return EXIT_SUCCESS;
}
