
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>



#define da_set(A,I,V) da_setVal(A,I,V)
#define da_setYX(A,Y,X,V) da_setVal(A, Y*da_getDim(A,DA_X)+X, V)

#define da_get(A,I) da_getVal(A,I)
#define DA_X 0
#define DA_Y 1
#define DA_Z 2

#define da_setDimX(A,V) da_setDim(A,DA_X,V)
#define da_setDimY(A,V) da_setDim(A,DA_Y,V)
#define da_setDimZ(A,V) da_setDim(A,DA_Z,V)

#define da_setDimYX(A, Y,X) da_setDimY(A,Y); da_setDimX(A,X)
#define da_setDimZYX(A, Z,Y,X) da_setDimZ(A,Z); da_setDimYX(A,Y,X)

#define da_newYX(A,Y,X) A = da_new(2,Y*X); da_setDimYX(A,Y,X)
#define da_newZYX(A,Z,Y,X) A = da_new(3,Z*Y*X); da_setDimZYX(AZ,Y,X)


#define foreach_da(a)  for (int FOREACH_DA = 0; FOREACH_DA < da_getSize(a); FOREACH_DA++)
#define foreach_x(a)  for (int FOREACH_X = 0; FOREACH_X < da_getDim(a, DA_X); FOREACH_X++)
#define foreach_y(a)  for (int FOREACH_Y = 0; FOREACH_Y < da_getDim(a, DA_Y); FOREACH_Y++)
#define foreach_z(a)  for (int FOREACH_Z = 0; FOREACH_Z < da_getDim(a, DA_Z); FOREACH_Z++)

#define foreach_setDA(A,V) da_setVal(A, FOREACH_DA, V)
#define foreach_setX(A,V) da_setX(A, FOREACH_X, V)
#define foreach_setYX(A,V) da_setYX(A, FOREACH_Y, FOREACH_X, V)


#define foreach_getDA(a) da_get(a, FOREACH_DA)
#define foreach_getX(a) da_get(a, FOREACH_X)
#define foreach_getYX(a) da_getYX(a, FOREACH_Y, FOREACH_X)


typedef struct
{
    size_t arrSize;
    size_t dimSize;
    size_t *dim;
    double *arr;
} da_t;


da_t *da_new(size_t dimSize, size_t arrSize)
{
    da_t *struct_p = (da_t *) malloc(sizeof(da_t));
    if (struct_p == NULL) return NULL;

    size_t *dim_p = malloc(dimSize*sizeof(size_t));
    if (dim_p == NULL)
    {
        free(struct_p);
        return NULL;
    }

    double *arr = malloc(arrSize*sizeof(double));
    if (arr == NULL)
    {
        free(dim_p);
        free(struct_p);
        return NULL;
    }

    struct_p->arrSize = arrSize;
    struct_p->dimSize = dimSize;
    dim_p[0] = arrSize;
    struct_p->dim = dim_p;
    struct_p->arr = arr;
    return struct_p;
}

da_t *da_free(da_t *ptr)
{
    free(ptr->arr);
    free(ptr->dim);
    free(ptr);
    return NULL;
}

size_t da_getSize(da_t *ptr)
{
    return ptr->arrSize;
}

size_t da_getDimSize(da_t *ptr)
{
    return ptr->dimSize;
}

size_t da_getDim(da_t *ptr, size_t d)
{
    if (d < 0 || d >= ptr->dimSize) return 0;
    return ptr->dim[d];
}

void da_setDim(da_t *ptr, size_t d, size_t size)
{
    if (d < 0 || d >= ptr->dimSize) return;
    ptr->dim[d] = size;
}

void da_setVal(da_t *ptr, size_t i, double val)
{
    if (i >= ptr->arrSize) return;
    ptr->arr[i] = val;
}

double da_getVal(da_t *ptr, size_t i)
{
    if (i >= ptr->arrSize) return 0.0;
    return ptr->arr[i];
}

double da_getYX(da_t *ptr, size_t r, size_t c)
{
    if (da_getDimSize(ptr) != 2) return 0.0;
    size_t numCol = da_getDim(ptr, 0);
    size_t i = r * numCol + c;
    return da_getVal(ptr, i);
}

void da_fill(da_t *ptr, double val)
{
    size_t size = ptr->arrSize;
    for (int i=0; i<size; i++)
    {
        da_set(ptr, i, val);
    }
}

da_t *da_addYX(da_t *c, da_t *a, da_t *b)
{
    size_t asize = a->arrSize;
    size_t bsize = b->arrSize;
    if (asize != bsize) return c;
    
    if (c != NULL) {
        da_free(c);
    }
    
    c = da_new(a->dimSize, a->arrSize);
    if (c == NULL) return NULL;
    
    for (int i=0; i<a->dimSize; i++)
    {
        c->dim[i] = a->dim[i];
    }
    
    foreach_da(a)
    {
        double sum = foreach_getDA(a) + foreach_getDA(b);
        foreach_setDA(c, sum);
    }
    return c;
}

void da_printYX(da_t *ptr)
{
    foreach_y(ptr)
    {
        foreach_x(ptr)
        {
           printf("%5.0f ", foreach_getYX(ptr));
        }
        printf("\n");
    }
}


// ========================================================
// ========================================================


int main()
{

    da_t *a = da_new(2, 1000);
    if (a == NULL) {
        printf("Error: failed to malloc\n");
        return 1;
    }

    size_t size = da_getSize(a);
    for (size_t i=0; i < size; i++)
    {
        da_setVal(a, i, 5.0*i);
    }

    printf("size = %zu\n", da_getSize(a));
    size_t dimSize = da_getDimSize(a);
    printf("num dimensions = %zu\n", dimSize);

    da_setDimYX(a, 50, 20);
    da_printYX(a);

    for (size_t i = 0; i < dimSize; i++)
    {
        size_t dim = da_getDim(a, i);
        printf("%zu ", dim);
    }
    printf("\n");

    a = da_free(a);
    
    da_t *b;

    da_newYX(a,5,3);
    da_newYX(b,5,3);
    
    da_fill(a, 3);
    da_fill(b, 5);
    da_printYX(a);
    da_printYX(b);

    da_t *c = NULL;
    c = da_addYX(c, a, b);
    da_printYX(c);

//     b = da_new(2, 9);
//     c = da_mult(a, b);

    printf("hello\n");
    return 0;
}

