
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>



#define da_set(A,I,V) da_setVal(A,I,V)
#define da_setYX(A,Y,X,V) da_setVal(A, Y*da_getShape(A,DA_X)+X, V)

#define da_get(A,I) da_getVal(A,I)
#define DA_X 0
#define DA_Y 1
#define DA_Z 2

#define da_setShapeX(A,V) da_setShape(A,DA_X,V)
#define da_setShapeY(A,V) da_setShape(A,DA_Y,V)
#define da_setShapeZ(A,V) da_setShape(A,DA_Z,V)

#define da_setShapeYX(A, Y,X) da_setShapeY(A,Y); da_setShapeX(A,X)
#define da_setShapeZYX(A, Z,Y,X) da_setShapeZ(A,Z); da_setShapeYX(A,Y,X)

#define da_newYX(A,Y,X) A = da_new(2,Y*X); da_setShapeYX(A,Y,X)
#define da_newZYX(A,Z,Y,X) A = da_new(3,Z*Y*X); da_setShapeZYX(AZ,Y,X)


#define foreach_da(a)  for (int FOREACH_DA = 0; FOREACH_DA < da_getSize(a); FOREACH_DA++)
#define foreach_x(a)  for (int FOREACH_X = 0; FOREACH_X < da_getShape(a, DA_X); FOREACH_X++)
#define foreach_y(a)  for (int FOREACH_Y = 0; FOREACH_Y < da_getShape(a, DA_Y); FOREACH_Y++)
#define foreach_z(a)  for (int FOREACH_Z = 0; FOREACH_Z < da_getShape(a, DA_Z); FOREACH_Z++)

#define foreach_setDA(A,V) da_setVal(A, FOREACH_DA, V)
#define foreach_setX(A,V) da_setX(A, FOREACH_X, V)
#define foreach_setYX(A,V) da_setYX(A, FOREACH_Y, FOREACH_X, V)


#define foreach_getDA(a) da_get(a, FOREACH_DA)
#define foreach_getX(a) da_get(a, FOREACH_X)
#define foreach_getYX(a) da_getYX(a, FOREACH_Y, FOREACH_X)


typedef struct
{
    size_t arrSize;
    size_t shapeSize;
    size_t *shape;
    double *arr;
} da_t;


da_t *da_new(size_t shapeSize, size_t arrSize);
da_t *da_free(da_t *ptr);
size_t da_getSize(da_t *ptr);
size_t da_getShapeSize(da_t *ptr);
size_t da_getShape(da_t *ptr, size_t d);
void da_setShape(da_t *ptr, size_t d, size_t size);
void da_setVal(da_t *ptr, size_t i, double val);
double da_getVal(da_t *ptr, size_t i);
double da_getYX(da_t *ptr, size_t r, size_t c);
da_t *da_new_shape(size_t shape[]);
da_t *da_new_like(da_t *a);
da_t *da_free_new_like(da_t *ptr, da_t *a);
void da_fill(da_t *ptr, double val);
da_t *da_full(size_t shape[], double val);
da_t *da_full_like(da_t *a, double val);
da_t *da_ones(size_t shape[]);
da_t *da_zeros(size_t shape[]);
da_t *da_ones_like(da_t *a);
da_t *da_zeros_like(da_t *a);
da_t *da_identity(size_t n);
void da_printYX(da_t *ptr);
bool da_same_shape(da_t *a, da_t *b);
da_t *da_init_addsub(da_t *c, da_t *a, da_t *b);
da_t *da_add(da_t *c, da_t *a, da_t *b);
da_t *da_sub(da_t *c, da_t *a, da_t *b);
da_t *da_scalar_mult(da_t *c, da_t *a, double val);
da_t *da_scalar_add(da_t *c, da_t *a, double val);
da_t *da_mult(da_t *c, da_t *b, da_t *a);


