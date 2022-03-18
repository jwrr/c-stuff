
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include  "da_t.h"


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
    size_t shapeSize = da_getShapeSize(a);
    printf("num dimensions = %zu\n", shapeSize);

    da_setShapeYX(a, 50, 20);
    da_printYX(a);

    for (size_t i = 0; i < shapeSize; i++)
    {
        size_t shape = da_getShape(a, i);
        printf("%zu ", shape);
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
    c = da_add(c, a, b);
    da_printYX(c);

    c = da_sub(c,c,a);
    da_printYX(c);
    c = da_sub(c,c,a);
    da_printYX(c);
    c = da_sub(c,c,a);
    da_printYX(c);
    c = da_scalar_mult(c,c,9);
    da_printYX(c);
    c = da_scalar_add(c,c,7);
    da_printYX(c);
    da_free(c);

    size_t d2x2[] = {2,2,0};
    size_t d4x5[] = {4,5,0};
    da_t *d = da_full(d4x5, 33);
    da_printYX(d);
    da_free(d);

    d = da_zeros(d4x5);
    da_printYX(d);
    da_free(d);

    d = da_ones(d4x5);
    da_printYX(d);

    da_t *e = da_full_like(d, 88);
    da_printYX(e);
    da_free(e);

    e = da_ones_like(d);
    da_printYX(e);
    da_free(e);

    e = da_zeros_like(d);
    da_printYX(e);
    da_free(e);

    da_t *sq = da_identity(10);
    da_printYX(sq);
    da_free(sq);



//     b = da_new(2, 9);
//     c = da_mult(a, b);

    printf("hello\n");
    return 0;
}

