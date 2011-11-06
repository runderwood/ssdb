#ifndef SSDB_GEOM_H

#define GEOM_NULL 0
#define GEOM_POINT 1
#define GEOM_LINESTRING 2
#define GEOM_POLYGON 4

#define GEOM_LAMBERT_R 298.257223563
#define GEOM_R_EARTH 6371137.0

struct ssdb_point_s {
    int type;
    double lat;
    double lon;
};

typedef struct ssdb_point_s ssdb_point_t;

struct ssdb_linestring_s {
    int type;
    int num_points;
    ssdb_point_t** points;
};

typedef struct ssdb_linestring_s ssdb_linestring_t;

double ssdb_lambert_flatten(double lat);

ssdb_point_t* ssdb_point_new(void);
void ssdb_point_init(ssdb_point_t* p);
void ssdb_point_destroy(ssdb_point_t* p);

int ssdb_point_equals(ssdb_point_t* p1, ssdb_point_t* p2);
double ssdb_point_distance(ssdb_point_t* p1, ssdb_point_t* p2);
double ssdb_point_distance_lambert(ssdb_point_t* p1, ssdb_point_t* p2);

ssdb_linestring_t* ssdb_linestring_new(void);
void ssdb_linestring_init(ssdb_linestring_t* l);
void ssdb_linestring_destroy(ssdb_linestring_t* l);

double ssdb_linestring_length(ssdb_linestring_t* l);
int ssdb_linestring_append(ssdb_linestring_t* l, ssdb_point_t* p);

#define SSDB_GEOM_H
#endif
