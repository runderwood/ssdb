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

ssdb_point_t* ssdb_point_destroy(ssdb_point_t* p) {
    free(p);
    p = NULL;
    return p;
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

ssdb_bbox_t* ssdb_point_bbox(ssdb_point_t* p) {
    ssdb_bbox_t* b = ssdb_bbox_new();
    b->latmax = p->lat;
    b->lonmax = p->lon;
    b->latmin = p->lat;
    b->lonmin = p->lon;
    return b;
}

void ssdb_linestr_init(ssdb_linestr_t* l) {
    l->type = GEOM_LINESTRING;
    l->num_points = 0;
    l->points = NULL;
}

ssdb_linestr_t* ssdb_linestr_new(void) {
    ssdb_linestr_t* l = malloc(sizeof(ssdb_linestr_t));
    if(l == NULL) abort();
    ssdb_linestr_init(l);
    return l;
}

ssdb_linestr_t* ssdb_linestr_destroy(ssdb_linestr_t* l) {
    int i;
    for(i=0; i<l->num_points; i++) {
        ssdb_point_destroy(l->points[i]);
        l->points[i] = NULL;
    }
    free(l->points);
    l->points = NULL;
    free(l);
    l = NULL;
    return l;
}

int ssdb_linestr_append(ssdb_linestr_t* l, ssdb_point_t* p) {
    l->points = realloc(l->points, sizeof(ssdb_point_t*)*(l->num_points+1));
    if(l->points) {
        l->num_points++;
    } else {
        abort();
    }
    l->points[l->num_points-1] = p;
    return l->num_points;
}

double ssdb_linestr_length(ssdb_linestr_t* l) {
    double len = 0.0;
    int i;
    for(i=1; i<l->num_points; i++) {
        len += ssdb_point_distance(l->points[i-1], l->points[i]);
    }
    return len;
}

ssdb_bbox_t* ssdb_linestr_bbox(ssdb_linestr_t* l) {
    int i;
    ssdb_point_t* p;
    ssdb_bbox_t* pb;
    ssdb_bbox_t* b = ssdb_bbox_new();
    for(i=0; i<l->num_points; i++) {
        p = l->points[i];
        pb = ssdb_point_bbox(p);
        ssdb_bbox_extend(b, pb);
        ssdb_bbox_destroy(pb);
    }
    return b;
}

ssdb_bbox_t* ssdb_bbox_new(void) {
    ssdb_bbox_t* b = malloc(sizeof(ssdb_bbox_t));
    if(b == NULL) abort();
    ssdb_bbox_init(b);
    return b;
}

ssdb_bbox_t* ssdb_bbox_destroy(ssdb_bbox_t* b) {
    free(b);
    b = NULL;
    return b;
}

void ssdb_bbox_init(ssdb_bbox_t* b) {
    b->latmin = GEOM_UNSET;
    b->lonmin = GEOM_UNSET;
    b->latmax = GEOM_UNSET;
    b->lonmax = GEOM_UNSET;
}

void ssdb_bbox_extend(ssdb_bbox_t* b0, ssdb_bbox_t* b1) {
    if(b0->latmin == GEOM_UNSET || b0->latmin > b1->latmin) {
        b0->latmin = b1->latmin;
    }
    if(b0->lonmin == GEOM_UNSET || b0->lonmin > b1->lonmin) {
        b0->lonmin = b1->lonmin;
    }
    if(b0->latmax == GEOM_UNSET || b0->latmax < b1->latmax) {
        b0->latmax = b1->latmax;
    }
    if(b0->lonmax == GEOM_UNSET || b0->lonmax < b1->lonmax) {
        b0->lonmax = b1->lonmax;
    }
    return;
}
