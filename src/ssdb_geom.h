#ifndef SSDB_GEOM_H
#include <float.h>

#define GEOM_NULL 0
#define GEOM_POINT 1
#define GEOM_LINESTRING 2
#define GEOM_POLYGON 4

#define GEOM_LAMBERT_R 298.257223563
#define GEOM_R_EARTH 6371137.0

#define GEOM_UNSET (-DBL_MAX)

struct ssdb_bbox_s {
    double latmin;
    double latmax;
    double lonmin;
    double lonmax;
};

typedef struct ssdb_bbox_s ssdb_bbox_t;

struct ssdb_point_s {
    int type;
    double lat;
    double lon;
};

typedef struct ssdb_point_s ssdb_point_t;

struct ssdb_linestr_s {
    int type;
    int num_points;
    ssdb_point_t** points;
};

typedef struct ssdb_linestr_s ssdb_linestr_t;

double ssdb_lambert_flatten(double lat);

ssdb_bbox_t* ssdb_bbox_new(void);
void ssdb_bbox_init(ssdb_bbox_t* b);
ssdb_bbox_t* ssdb_bbox_destroy(ssdb_bbox_t* b);
void ssdb_bbox_extend(ssdb_bbox_t* b0, ssdb_bbox_t* b1);

ssdb_point_t* ssdb_point_new(void);
void ssdb_point_init(ssdb_point_t* p);
ssdb_point_t* ssdb_point_destroy(ssdb_point_t* p);
ssdb_bbox_t* ssdb_point_bbox(ssdb_point_t* p);

int ssdb_point_equals(ssdb_point_t* p1, ssdb_point_t* p2);
double ssdb_point_distance(ssdb_point_t* p1, ssdb_point_t* p2);
double ssdb_point_distance_lambert(ssdb_point_t* p1, ssdb_point_t* p2);

ssdb_linestr_t* ssdb_linestr_new(void);
void ssdb_linestr_init(ssdb_linestr_t* l);
ssdb_linestr_t* ssdb_linestr_destroy(ssdb_linestr_t* l);
ssdb_bbox_t* ssdb_linestr_bbox(ssdb_linestr_t* p);

double ssdb_linestr_length(ssdb_linestr_t* l);
int ssdb_linestr_append(ssdb_linestr_t* l, ssdb_point_t* p);

#define SSDB_GEOM_H
#endif
