#define WIDTH_AND_HEIGHT 100, 100
#define FILE_START "P6 100 100 255\n"

// HIT() is for debugging.
#define HIT() printf("hit\n");
#define HANDLE_NULL_RET(a, ret) \
  if(!(a)) { \
    return ret; \
  }

#define HANDLE_NULL_VOID(a) \
  if(!(a)) { \
    return; \
  }

#include <stdlib.h> // For malloc()
#include <stdio.h> // For file management.
#include <string.h> // For strcat().
#include <math.h> // For path tracing.

// Section pixmap.
#define RGB_LENGTH 3
struct pixmap {
    char *data;
    int width;
    int height;
};

int pixmap_data_index(
    int arg_x,
    int arg_y,
    struct pixmap *arg_pixmap
) {
    HANDLE_NULL_RET(arg_pixmap, -1)
    if(
        !arg_pixmap ||
        arg_x > arg_pixmap->width ||
        arg_y > arg_pixmap->height    
    ) {
        return -1;
    }
    
    return ((arg_y * arg_pixmap->width) + arg_x) * RGB_LENGTH;
}

int pixmap_area(
    struct pixmap *arg_pixmap
) {
    HANDLE_NULL_RET(arg_pixmap, -1)
    return (
        arg_pixmap->width *
        arg_pixmap->height *
        RGB_LENGTH
    );
}

void pixmap_setup(
    int arg_width,
    int arg_height,
    struct pixmap *arg_pixmap
) {
    arg_pixmap->width = arg_width;
    arg_pixmap->height = arg_height;
    arg_pixmap->data = (char*)malloc(
        sizeof(char*) *
        arg_pixmap->width *
        arg_pixmap->height *
        RGB_LENGTH
    );
}

void pixmap_output(
    const char *arg_filename,
    struct pixmap *arg_pixmap
) {
    HANDLE_NULL_VOID(arg_pixmap)
    FILE *file = fopen(arg_filename, "wb+");
    fputs(FILE_START, file);
    for(
        int i = 0;
        i < pixmap_area(arg_pixmap) - 1;
        i++
    ) {
      fputc(arg_pixmap->data[i], file);  
    }

    fclose(file);
}

// Section vector3.
struct vector3 {
  double x;
  double y; 
  double z;
};

struct vector3 vector3_create(
  double arg_x,
  double arg_y,
  double arg_z
) {
  struct vector3 result = {
    .x = arg_x,
    .y = arg_y,
    .z = arg_z
  };

  return result;
}

struct vector3 vector3_neg(
  struct vector3 arg_vector3
) {
  return vector3_create(
    -arg_vector3.x,
    -arg_vector3.y,
    -arg_vector3.z
  );
}

#define VECTOR3_MATH_ASSIGN(a, b, operator) \
  a.x operator b.x; \
  a.y operator b.y; \
  a.z operator b.z;

#define VECTOR3_MATH(a, b, c, operator) \
  c.x = a.x operator b.x; \
  c.y = a.y operator b.y; \
  c.z = a.z operator b.z;

double vector3_dot(
  struct vector3 a,
  struct vector3 b
) {
  return
    a.x * b.x +
    a.y * b.y +
    a.z * b.z;    
}

struct vector3 vector3_cross(
  struct vector3 a,
  struct vector3 b
) {
  return vector3_create(
    a.y * b.z - a.z * b.y,
    a.z * b.x - a.x * b.z,
    a.x * b.y - a.y * b.y
  );  
}

struct vector3 vector3_normalize(
  struct vector3 a
) {
  double temp = sqrt(vector3_dot(a, a));
  struct vector3 b = vector3_create(temp, temp, temp);
  VECTOR3_MATH_ASSIGN(a, b, /=)
  return a;
}

// Section ray.
struct ray {
  struct vector3 origin;
  struct vector3 direction;
};

struct ray ray_create(
  struct vector3 origin,
  struct vector3 direction  
) {
  struct ray result = {
    .origin = origin,
    .direction = direction
  };

  return result;
}

// Section sphere.
enum reflection_type {
  DIFFUSE,
  SPECULAR,
  REFRACTION  
};

struct sphere {
  double radius;
  struct vector3 position;
  struct vector3 emission;
  struct vector3 color;
  enum reflection_type reflection;
};

struct sphere sphere_create(
  double radius,
  struct vector3 position,
  struct vector3 emission,
  struct vector3 color,
  enum reflection_type reflection
) {
  struct sphere result = {
    .radius = radius,
    .position = position,
    .emission = emission,
    .color = color,
    .reflection = reflection
  };

  return result;
}

double sphere_intersect(
  struct sphere arg_sphere,
  struct ray arg_ray
) {
  struct vector3 op;
  VECTOR3_MATH(arg_sphere.position, arg_ray.origin, op, -)
  double t;
  double b = vector3_dot(op, arg_ray.direction);
  double detection = 
    (b * b) - 
    vector3_dot(op, op) + 
    (
      arg_sphere.radius *
      arg_sphere.radius
    );

  if(detection < 0) {
    return -1;
  }

  detection = sqrt(detection);
  return 
    (t = b - detection) > 0 
    ? t 
    : b + detection;
}

// struct vector3 sphere_normal(
  // struct sphere arg_sphere,
  // struct vector3 a
// ) {
  // VECTOR3_MATH_ASSIGN(a, arg_sphere.position, -=)
  // double temp = arg_sphere.radius;
  // struct vector3 b = vector3_create(temp, temp, temp);
  // VECTOR3_MATH_ASSIGN(a, b, /=)
  // return a;
// }

// Section path tracing.
inline double clamp(double x) {
  return
    x < 0
    ? 0
    : x > 1
      ? 1
      : x;
}

inline int to_int(double x) {
  return (int)(
    pow(
      clamp(x),
      1 / 2.2
    ) * 255 + .5
  );
}

inline int instersect(
  const struct ray r,
  double t,
  int *id,
  struct sphere *spheres
) {
  double n = sizeof(spheres) / sizeof(struct sphere*);
  double d;
  double inf = t = 1e20;
  for(int i = (int)n; i--;) {
    if(
      (d = sphere_intersect(spheres[i], r)) &&
      d < t
    ) {
      t = d;
      *id = i;
    }
  }

  return t < inf;
}

// Section main.
int main(
  int argc,
  char **argv
) {
  struct sphere spheres[] = {
    sphere_create(1e5, vector3_create(1e5 + 1, 40.8, 81.6), vector3_create(0, 0, 0), vector3_create(.75, .25, .25), DIFFUSE),
    sphere_create(1e5, vector3_create(-1e5 + 99, 40.8, 81.6), vector3_create(0, 0, 0), vector3_create(.25, .25, .75), DIFFUSE),
    sphere_create(1e5, vector3_create(50, 40.8, 1e5), vector3_create(0, 0, 0), vector3_create(.75, .75, .75), DIFFUSE),
    sphere_create(1e5, vector3_create(50, 40.8, -1e5 + 170), vector3_create(0, 0, 0), vector3_create(0, 0, 0), DIFFUSE),
    sphere_create(1e5, vector3_create(50, 1e5, 81.6), vector3_create(0, 0, 0), vector3_create(.75, .75, .75), DIFFUSE),
    sphere_create(1e5, vector3_create(50, -1e5 + 81.6, 81.6), vector3_create(0, 0, 0), vector3_create(.75, .75, .75), DIFFUSE),
    sphere_create(1e5, vector3_create(27, 16.5, 47), vector3_create(0, 0, 0), vector3_create(.999, .999, .999), SPECULAR),
    sphere_create(1e5, vector3_create(73, 16.5, 78), vector3_create(0, 0, 0), vector3_create(.999, .999, .999), REFRACTION),
    sphere_create(1e5, vector3_create(50, 81.6 - 16.5, 81.6), vector3_create(400, 400, 400), vector3_create(0, 0, 0), DIFFUSE),
  };

  int spheresLength = sizeof(spheres) / sizeof(struct sphere);
  int width = 512;
  int height = 382;
  int samples = 
    argc == 2
    ? atoi(argv[1]) / 4
    : 1;

  struct ray camera = ray_create(
    vector3_create(50, 52, 295.6), 
    vector3_normalize(
      vector3_create(
        0,
        -0.042612,
        -1
      )
    )
  );

  double temp = width * .5135 / height;
  struct vector3 camera_x = vector3_create(temp, temp, temp);
  struct vector3 camera_y = vector3_normalize(
    vector3_cross(
      camera_x, 
      camera.direction
    )
  );

  VECTOR3_MATH_ASSIGN(camera_y, vector3_create(.5135, .5135, .5135), *)
  struct vector3 r;
  struct pixmap current_pixmap;
  // Then create image.
  pixmap_setup(WIDTH_AND_HEIGHT, &current_pixmap);
  pixmap_output("test.ppm", &current_pixmap);
  return 0;
}