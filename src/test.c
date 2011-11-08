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
        points[j] = ssdb_point_destroy(points[j]);
        printf("destroyed point (%d)\n", j);
    }
    free(points);
    points = NULL;
    printf("done.\n");
    return;
}

static void test_linestrs(int num, int max_points) {
    int j;
    int k;
    int lp;
    ssdb_point_t* p;
    ssdb_bbox_t* lb;
    for(j=0; j<num; j++) {
        ssdb_linestr_t* ls = ssdb_linestr_new();
        printf("created linestr (%d): t: %d, num_points: %d\n", j, ls->type, ls->num_points);
        lp = rand() % max_points;
        for(k=0; k<lp; k++) {
            p = ssdb_point_new();
            p->lon = (double)((360/2) - (rand()%(int)360));
            p->lat = (double)((180/2) - (rand()%(int)180));
            ssdb_linestr_append(ls, p);
        }
        printf("\tlinestr (%d): t: %d, num_points: %d, len: %f\n", j, ls->type, ls->num_points, ssdb_linestr_length(ls));
        lb = ssdb_linestr_bbox(ls);
        printf("min: %f, %f. max: %f, %f.\n", lb->latmin, lb->lonmin, lb->latmax, lb->lonmax);
        lb = ssdb_bbox_destroy(lb);
        ssdb_linestr_destroy(ls);
        printf("destroyed linestr...(%d)\n", j);
    }
    return;
}

static void test_distance() {
        ssdb_point_t* p0 = ssdb_point_new();
        p0->lat = 42.357778;
        p0->lon = -71.061667;
        ssdb_point_t* p1 = ssdb_point_new();
        p1->lat = 29.762778;
        p1->lon = -95.383056;
        double d = ssdb_point_distance(p0, p1);
        printf("distance from %f,%f to %f,%f = %f\n", p0->lat, p0->lon, p1->lat, p1->lon, d);
        p0 = ssdb_point_destroy(p0);
        p1 = ssdb_point_destroy(p1);
        return;
}

int main(int argc, char** argv) {
    
    srand(time(NULL));
    //test_points(10, 360, 180);
    test_linestrs(10, 100);
    test_distance();

    return EXIT_SUCCESS;
}
