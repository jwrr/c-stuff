
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include "da_t.h"



da_t *da_new(size_t shapeSize, size_t arrSize)
{
    da_t *struct_p = (da_t *) malloc(sizeof(da_t));
    if (struct_p == NULL) return NULL;

    size_t *shape_p = malloc(shapeSize*sizeof(size_t));
    if (shape_p == NULL)
    {
        free(struct_p);
        return NULL;
    }

    double *arr = malloc(arrSize*sizeof(double));
    if (arr == NULL)
    {
        free(shape_p);
        free(struct_p);
        return NULL;
    }

    struct_p->arrSize = arrSize;
    struct_p->shapeSize = shapeSize;
    shape_p[0] = arrSize;
    struct_p->shape = shape_p;
    struct_p->arr = arr;
    return struct_p;
}


da_t *da_free(da_t *ptr)
{
    if (ptr == NULL) return NULL;
    free(ptr->arr);
    free(ptr->shape);
    free(ptr);
    return NULL;
}


size_t da_getSize(da_t *ptr)
{
    return ptr->arrSize;
}


size_t da_getShapeSize(da_t *ptr)
{
    return ptr->shapeSize;
}


size_t da_getShape(da_t *ptr, size_t d)
{
    if (d < 0 || d >= ptr->shapeSize) return 0;
    return ptr->shape[d];
}


void da_setShape(da_t *ptr, size_t d, size_t size)
{
    if (d < 0 || d >= ptr->shapeSize) return;
    ptr->shape[d] = size;
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
    if (da_getShapeSize(ptr) != 2) return 0.0;
    size_t numCol = da_getShape(ptr, 0);
    size_t i = r * numCol + c;
    return da_getVal(ptr, i);
}


da_t *da_new_shape(size_t shape[])
{
    const size_t errMax = 20;
    size_t arrSize = 1;
    size_t i = 0;
    for (; i < errMax; i++)
    {
        if (shape[i] == 0) break;
        arrSize *= shape[i];
    }
    if (i == 0) return NULL;
    
    size_t shapeSize = i;
    da_t *a = da_new(shapeSize, arrSize);
    if (a == NULL) return NULL;
    
    for (i = 0; i < shapeSize; i++)
    {
        da_setShape(a, i, shape[i]);
    }
    
    return a;
}


da_t *da_new_like(da_t *a)
{
    if (a == NULL) return NULL;

    da_t *ptr = da_new(a->shapeSize, a->arrSize);
    if (ptr == NULL) return NULL;
    
    for (int i = 0; i < a->shapeSize; i++)
    {
        da_setShape(ptr, i, a->shape[i]);
    }
    return ptr;
}


da_t *da_free_new_like(da_t *ptr, da_t *a)
{
    da_free(ptr);
    ptr = da_new_like(a);
    return ptr;
}


void da_fill(da_t *ptr, double val)
{
    if (ptr == NULL) return;
    size_t size = ptr->arrSize;
    for (int i=0; i<size; i++)
    {
        da_set(ptr, i, val);
    }
}


da_t *da_full(size_t shape[], double val)
{
    da_t *a = da_new_shape(shape);
    da_fill(a, val);
    return a;
}


da_t *da_full_like(da_t *a, double val)
{
    if (a == NULL) return NULL;
    da_t *ptr = da_new_like(a);
    da_fill(ptr, val);
    return ptr;
}


da_t *da_ones(size_t shape[])
{
    return da_full(shape, 1.0);
}


da_t *da_zeros(size_t shape[])
{
    return da_full(shape, 0.0);
}


da_t *da_ones_like(da_t *a)
{
    return da_full_like(a, 1.0);
}


da_t *da_zeros_like(da_t *a)
{
    return da_full_like(a, 0.0);
}


da_t *da_identity(size_t n)
{
    size_t arrSize = n * n;
    size_t shapeSize = 2;
    size_t shape[3];
    shape[0] = n;
    shape[1] = n;
    shape[2] = 0;
    da_t *a = da_zeros(shape);
    if (a == NULL) return NULL;

    for (size_t i = 0; i < n; i++)
    {
        size_t ii = i*n + i;
        da_set(a, ii, 1);
    }
    return a;
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


bool da_same_shape(da_t *a, da_t *b)
{
    if (a == NULL || b == NULL) return false;
    if (a->shapeSize != b->shapeSize) return false;
    for (int i=0; i < a->shapeSize; i++)
    {
        if (a->shape[i] != b->shape[i]) return false;
    }
    return true;
}


da_t *da_init_addsub(da_t *c, da_t *a, da_t *b)
{
    if (!da_same_shape(a, b)) return da_free(c);
    if (da_same_shape(c, a)) return c;
    return da_free_new_like(c, a);
}


#define da_addsub_macro(OP,c,a,b)\
do {\
    c = da_init_addsub(c, a, b);\
    if (c == NULL) return NULL;\
    foreach_da(a)\
    {\
        double result = foreach_getDA(a) OP foreach_getDA(b);\
        foreach_setDA(c, result);\
    }\
} while(0)


da_t *da_add(da_t *c, da_t *a, da_t *b)
{
    da_addsub_macro(+, c, a, b);
    return c;
}


da_t *da_sub(da_t *c, da_t *a, da_t *b)
{
    da_addsub_macro(-, c, a, b);
    return c;
}


da_t *da_scalar_mult(da_t *c, da_t *a, double val)
{
    if (a == NULL) return NULL;
    if (!da_same_shape(c, a))
    {
        c = da_free_new_like(c, a);
    } 
    if (c == NULL) return NULL;

    foreach_da(a)
    {
        double result = foreach_getDA(a) * val;
        foreach_setDA(c, result);
    }
    return c;
}


da_t *da_scalar_add(da_t *c, da_t *a, double val)
{
    if (a == NULL) return NULL;
    if (!da_same_shape(c, a))
    {
        c = da_free_new_like(c, a);
    } 
    if (c == NULL) return NULL;

    foreach_da(a)
    {
        double result = foreach_getDA(a) + val;
        foreach_setDA(c, result);
    }
    return c;
}


da_t *da_mult(da_t *c, da_t *b, da_t *a)
{
    if (a == NULL || b == NULL) return da_free(c);

    if (a->shapeSize != 2 || b->shapeSize != 2) return da_free(c);

    if (a->shape[0] != b->shape[1]) return da_free(c);
    if (a->shape[1] != b->shape[0]) return da_free(c);
    
    for (int r = 0; r < a->shape[DA_Y]; r++)
    {
        for (int c = 0; c < a->shape[DA_X]; c++)
        {
            // FIXME
            
        }
    }
}


