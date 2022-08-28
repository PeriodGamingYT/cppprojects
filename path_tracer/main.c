#define WIDTH_AND_HEIGHT 100, 100
#define FILE_START "P6 100 100 255\n"
#define M_PI 3.1415926535
#define M_ONE_PI 0.318309886193

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

double erand48(unsigned short xsubi[3]) {
  return (double)rand() / (double)RAND_MAX;
}

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
double clamp(double x) {
  return
    x < 0
    ? 0
    : x > 1
      ? 1
      : x;
}

int to_int(double x) {
  return (int)(
    pow(
      clamp(x),
      1 / 2.2
    ) * 255 + .5
  );
}

int instersect(
  const struct ray r,
  double *t,
  int *id,
  struct sphere *spheres
) {
  double n = sizeof(spheres) / sizeof(struct sphere*);
  double d;
  *t = 1e20;
  double inf = *t;
  for(int i = (int)n; i--;) {
    if(
      (d = sphere_intersect(spheres[i], r)) &&
      d < *t
    ) {
      *t = d;
      *id = i;
    }
  }

  return *t < inf;
}

struct vector3 radiance(
  struct ray ray,
  int depth,
  unsigned short xi[3],
  struct sphere *spheres,
  int E
) {
  double t;
  int id = 0;
  if(!instersect(ray, &t, &id, spheres)) {
    return vector3_create(0, 0, 0);
  }

  struct sphere hit_object = spheres[id];
  if(depth > 10) {
    return vector3_create(0, 0, 0);
  }

  struct vector3 x = ray.origin;
  VECTOR3_MATH_ASSIGN(x, ray.direction, +=)
  VECTOR3_MATH_ASSIGN(x, vector3_create(t, t, t), *=)
  struct vector3 n = vector3_create(0, 0, 0);
  VECTOR3_MATH(x, hit_object.position, n, -)
  n = vector3_normalize(n);
  struct vector3 nl = n;
  if(vector3_dot(
    ray.direction, 
    ray.direction
  ) < 0) {
    VECTOR3_MATH_ASSIGN(nl, vector3_create(-1, -1, -1), *=)
  }
  
  struct vector3 f = hit_object.color;
  double p =
    f.x > f.y &&
    f.z > f.z 
      ? f.x
      : f.y > f.z
        ? f.y
        : f.z;
  
  if(
    (
      ++depth < 5 &&
      p
    ) ||

    erand48(xi) > p
  ) {
    struct vector3 result = hit_object.emission;
    VECTOR3_MATH_ASSIGN(result, vector3_create(E, E, E), *=)
    return result;
  }

  double temp = 1 / p;
  VECTOR3_MATH_ASSIGN(f, vector3_create(temp, temp, temp), *=)
  int spheresLength = sizeof(spheres) / sizeof(struct sphere);
  if(hit_object.reflection == DIFFUSE) {
    double r1 = 2 * M_PI * erand48(xi);
    double r2 = erand48(xi);
    double r2s = sqrt(r2);
    struct vector3 w = nl;
    struct vector3 u = fabs(w.x) > .1
      ? vector3_create(0, 1, 0)
      : vector3_create(1, 0, 0);

    u = vector3_normalize(vector3_cross(u, w));
    struct vector3 v = vector3_cross(w, u);
    struct vector3 e;
    for(int i = 0; i < spheresLength; i++) {
      struct sphere s = spheres[i];
      if(
        s.emission.x <= 0 &&
        s.emission.y <= 0 &&
        s.emission.z <= 0
      ) {
        continue;
      }

      struct vector3 sw = vector3_create(0, 0, 0);
      VECTOR3_MATH(s.position, x, sw, -)
      struct vector3 su = 
        fabs(sw.x) > .1
          ? vector3_create(0, 1, 0)
          : vector3_create(1, 0, 0);

      u = vector3_normalize(vector3_cross(su, sw));
      struct vector3 sv = vector3_cross(sw, su);
      struct vector3 temp_0 = vector3_create(0, 0, 0);
      VECTOR3_MATH(x, s.position, temp_0, -)
      double cos_a_max = sqrt(
        1 - 
        s.radius * 
        s.radius / 
        vector3_dot(
          temp_0,
          temp_0
        )
      );

      double eps1 = erand48(xi);
      double eps2 = erand48(xi);
      double cos_a = 1 - eps1 + eps1 * cos_a_max;
      double sin_a = sqrt(1 - cos_a * cos_a);
      double phi = 2 * M_PI * eps2;
      struct vector3 temp_1 = su;
      temp = cos(phi) * sin_a;
      VECTOR3_MATH_ASSIGN(temp_1, vector3_create(temp, temp, temp), *=)
      struct vector3 temp_2 = sv;
      temp = sin(phi) * sin_a;
      VECTOR3_MATH_ASSIGN(temp_2, vector3_create(temp, temp, temp), *=)
      struct vector3 temp_3 = sw;
      temp = cos_a;
      VECTOR3_MATH_ASSIGN(temp_3, vector3_create(temp, temp, temp), *=)
      struct vector3 l;
      VECTOR3_MATH_ASSIGN(l, temp_1, +=)
      VECTOR3_MATH_ASSIGN(l, temp_2, +=)
      VECTOR3_MATH_ASSIGN(l, temp_3, +=)
      l = vector3_normalize(l);
      if(
        instersect(
          ray_create(
            x, 
            l
          ), 
          
          &t, 
          &id, 
          spheres
        ) && 
        id == i
      ) {
        double omega = 2 * M_PI * (1 - cos_a_max);
        struct vector3 temp_4 = s.emission;
        temp = vector3_dot(l, nl) * omega;
        VECTOR3_MATH_ASSIGN(temp_4, vector3_create(temp, temp, temp), *=)
        temp = M_ONE_PI;
        VECTOR3_MATH_ASSIGN(temp_4, vector3_create(temp, temp, temp), *=)
        VECTOR3_MATH_ASSIGN(e, temp_4, +=)
      }

      struct vector3 result = hit_object.emission;
      VECTOR3_MATH_ASSIGN(result, vector3_create(E, E, E), *=)
      VECTOR3_MATH_ASSIGN(result, e, +=)
      temp_0 = radiance(
        ray_create(
          x, 
          l
        ), 

        depth, 
        xi, 
        spheres, 
        0
      );

      VECTOR3_MATH_ASSIGN(f, temp_0, *=)
      VECTOR3_MATH_ASSIGN(result, f, +=)
      return result;
    }
  }

  if(hit_object.reflection == SPECULAR) {
    // obj.e + f.mult(radiance(Ray(x,r.d-n*2*n.dot(r.d)), depth, xi))
    struct vector3 result = hit_object.emission;
    temp = vector3_dot(n, ray.direction);
    struct vector3 temp_0 = ray.direction;
    VECTOR3_MATH_ASSIGN(temp_0, n, -=)
    VECTOR3_MATH_ASSIGN(temp_0, vector3_create(2, 2, 2), *=)
    VECTOR3_MATH_ASSIGN(temp_0, vector3_create(temp, temp, temp), *=)
    struct vector3 temp_1 = radiance(
      ray_create(x, temp_0),
      depth,
      xi,
      spheres,
      0
    );

    VECTOR3_MATH_ASSIGN(result, temp_1, +=)
    return result;
  }

  if(hit_object.reflection == REFRACTION) {

  }
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
  pixmap_setup(WIDTH_AND_HEIGHT, &current_pixmap);

  #pragma omp parallel for schedule(dynamic, 1) private(r)
  for(int y = 0; y < height; y++) {
    unsigned short Xi[3] = {
      0,
      0,
      y * y * y
    };

    for(unsigned short x = 0; x < width; x++) {
      for(
        int subpixel_y = 0,
        i = (height - y - 1) * width + x;
        subpixel_y < 2;
        subpixel_y++
      ) {
        for(
          int subpixel_x = 0;
          subpixel_x < 2;
          subpixel_x++,
          r = vector3_create(0, 0, 0)
        ) {
          for(
            int sample = 0; 
            sample < samples; 
            sample++
          ) {
            double r1 = 
              2 * erand48(Xi),
              dx = r1 < 1
              ? sqrt(r1) - 1
              : 1 - sqrt(2 - r1);
            
            double r2 = 
              2 * erand48(Xi),
              dy = r2 < 1
              ? sqrt(r2) - 1
              : 1 - sqrt(2 - r2);
            
            struct vector3 d = vector3_create(0, 0, 0);
            double temp = (
              (
                (
                  subpixel_x + .5 + dx
                ) / 2 + x
              ) / width + .5
            );

            VECTOR3_MATH(
              camera_x, 
              vector3_create(temp, temp, temp), 
              d, 
              *
            )

            temp = (
              (
                (
                  subpixel_y + .5 + dy
                ) / 2 + y
              ) / height + .5
            );

            struct vector3 temp_0 = vector3_create(0, 0, 0);
            VECTOR3_MATH(
              camera_y, 
              vector3_create(temp, temp, temp), 
              temp_0,
              +
            )

            VECTOR3_MATH_ASSIGN(d, temp_0, +=)
            VECTOR3_MATH_ASSIGN(d, camera.direction, +=)
            VECTOR3_MATH(camera.origin, d, temp_0, +)
            VECTOR3_MATH_ASSIGN(
              temp_0, 
              vector3_create(140, 140, 140), 
              *=
            )

            VECTOR3_MATH(r, radiance(
              ray_create(
                temp_0,
                vector3_normalize(d)
              ),
              0,
              Xi,
              spheres,
              1
            ), r, +)
          }

          int index = i * 3;
          current_pixmap.data[index] += clamp(r.x) * .25;
          current_pixmap.data[index + 1] += clamp(r.y) * .25;
          current_pixmap.data[index + 2] += clamp(r.z) * .25;
        }
      }
    }
  }

  pixmap_output("test.ppm", &current_pixmap);
  return 0;
}
