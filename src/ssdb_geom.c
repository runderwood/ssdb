#include <stdlib.h>
#include <math.h>
#include "ssdb_geom.h"

double ssdb_lambert_flatten(double lat) {
    double rlat = (2*M_PI*lat)/360.0;
    double tan_psi = ((GEOM_LAMBERT_R-1)/ GEOM_LAMBERT_R) * tan(rlat);
    double psi = atan(tan_psi);
    return (psi * 360.0) / (2 * M_PI);
}

void ssdb_point_init(ssdb_point_t* p) {
    p->type = GEOM_POINT;
    p->lat = 0.00;
    p->lon = 0.00;
}

ssdb_point_t* ssdb_point_new(void) {
    ssdb_point_t* p = malloc(sizeof(ssdb_point_t));
    if(!p) abort();
    ssdb_point_init(p);
    return p;
}

void ssdb_point_destroy(ssdb_point_t* p) {
    free(p);
    p = NULL;
    return;
}

int ssdb_point_equals(ssdb_point_t* p1, ssdb_point_t* p2) {
    return p1->lon == p2->lon && p1->lat == p2->lat;
}

double ssdb_point_distance(ssdb_point_t* p1, ssdb_point_t* p2) {
    double lon_arc = (p2->lon - p1->lon) * M_PI / 180.0;
    double lat_arc = (p2->lat - p1->lat) * M_PI / 180.0;
    double lon_h = sin(lon_arc*0.5);
    lon_h *= lon_h;
    double lat_h = sin(lat_arc*0.5);
    lat_h *= lat_h;
    double d = 2 * asin(sqrt(lat_h + cos(p1->lat*M_PI/180.0)*cos(p2->lat*M_PI/180.0) * lon_h)) * GEOM_R_EARTH;
    return d;
}

double ssdb_point_distance_lambert(ssdb_point_t* p1, ssdb_point_t* p2) {
    double flat_lat1 = ssdb_lambert_flatten(p1->lat);
    double flat_lat2 = ssdb_lambert_flatten(p2->lat);
    double lon_arc = (p2->lon - p1->lon) * M_PI / 180.0;
    double lat_arc = (flat_lat2 - flat_lat1) * M_PI / 180.0;
    double lon_h = sin(lon_arc*0.5);
    lon_h *= lon_h;
    double lat_h = sin(lat_arc*0.5);
    lat_h *= lat_h;
    double d = 2 * asin(sqrt(lat_h + cos(flat_lat1*M_PI/180.0)*cos(flat_lat2*M_PI/180.0) * lon_h)) * GEOM_R_EARTH;
    return d;
}

void ssdb_linestring_init(ssdb_linestring_t* l) {
    l->type = GEOM_LINESTRING;
    l->num_points = 0;
    l->points = NULL;
}

ssdb_linestring_t* ssdb_linestring_new(void) {
    ssdb_linestring_t* l = malloc(sizeof(ssdb_linestring_t));
    if(l == NULL) abort();
    ssdb_linestring_init(l);
    return l;
}

void ssdb_linestring_destroy(ssdb_linestring_t* l) {
    int i;
    for(i=0; i<l->num_points; i++) {
        ssdb_point_destroy(l->points[i]);
        l->points[i] = NULL;
    }
    free(l->points);
    l->points = NULL;
    free(l);
    l = NULL;
    return;
}

int ssdb_linestring_append(ssdb_linestring_t* l, ssdb_point_t* p) {
    l->points = realloc(l->points, sizeof(ssdb_point_t*)*(l->num_points+1));
    if(l->points) {
        l->num_points++;
    } else {
        abort();
    }
    l->points[l->num_points-1] = p;
    return l->num_points;
}

double ssdb_linestring_length(ssdb_linestring_t* l) {
    double len = 0.0;
    int i;
    for(i=1; i<l->num_points; i++) {
        len += ssdb_point_distance(l->points[i-1], l->points[i]);
    }
    return len;
}
