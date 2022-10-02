#ifndef LINALG_C
#define LINALG_C
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include<time.h>

struct Matrix {
    double * data;
    size_t ncol;
    size_t nraw;
};

struct Vector {
    double * data;
    size_t length;
};

typedef enum { 
    IJK, 
    IKJ,  
    JIK,
    JKI,
    KIJ,
    KJI
} MMStrategy;

typedef enum {
    IJ,
    JI
} MVStrategy;

void print_matrix(struct Matrix mat);

void random_mat_(struct Matrix mat){
    srand(time(NULL));
    for (int i = 0; i < mat.nraw; i++)
        for (int j = 0; j < mat.ncol; j++)
            mat.data[i * mat.ncol + j] = (double)rand() / RAND_MAX;
}

void random_vec_(struct Vector vec){
    srand(time(NULL));
    for (int i = 0; i < vec.length; i++)
        vec.data[i] = (double)rand() / RAND_MAX;
}

void fill_mat_(struct Matrix mat, double value){
    for (size_t i = 0; i < mat.nraw; i++)
        for (size_t j = 0; j < mat.ncol; j++)
            mat.data[i * mat.ncol + j] = value;
}

void fill_vec_(struct Vector vec, double value){
    for (size_t i = 0; i < vec.length; i++)
        vec.data[i] = value;
}

struct Matrix make_random_matrix(int nraw, int ncol){
    double * arr = (double *) malloc(nraw * ncol * sizeof(double));
    struct Matrix mat;
    mat.data = arr;
    mat.nraw = nraw;
    mat.ncol = ncol;
    random_mat_(mat);
    return mat;
}

struct Vector make_random_vec(int length){
    double * arr = (double *) malloc(length * sizeof(double));
    struct Vector vec;
    vec.data = arr;
    vec.length = length;
    random_vec_(vec);
    return vec;
}

struct Matrix make_const_matrix(int nraw, int ncol, double value){
    double * arr = (double *) malloc(nraw * ncol * sizeof(double));
    struct Matrix mat;
    mat.data = arr;
    mat.nraw = nraw;
    mat.ncol = ncol;
    fill_mat_(mat, value);
    return mat;
}

struct Vector make_const_vec(int length, double value){
    double * arr = (double *) malloc(length * sizeof(double));
    struct Vector res;
    res.data = arr;
    res.length = length;
    fill_vec_(res, value);
    return res;
}

void print_matrix(struct Matrix mat)
{
    for (size_t i=0; i<mat.nraw; ++i) {
        for (size_t j=0; j<mat.ncol; ++j) {
            if(j==0) printf("%lf", mat.data[i*mat.ncol+j]);
            else printf(", %lf", mat.data[i*mat.ncol+j]);
        }
        putchar('\n');
    }
}


struct Matrix matmul_ijk(struct Matrix left, struct Matrix right)
{
    struct Matrix result = make_const_matrix(left.nraw, right.ncol, 0.0);
    if(left.ncol==right.nraw){
        for (size_t i=0; i<left.nraw; i++)
            for(size_t j=0; j<right.ncol; j++)
                for(size_t k = 0; k < left.ncol; k++)
                    result.data[i*result.ncol+j]=
                        result.data[i*result.ncol+j]
                        +
                        left.data[i*left.ncol+k]
                        *
                        right.data[k*right.ncol+j];
                
    }
    else printf("matmul_ijk: left.ncol!=right.nraw");
    return result;
}

struct Matrix matmul_ikj(struct Matrix left, struct Matrix right)
{
    struct Matrix result = make_const_matrix(left.nraw, right.ncol, 0.0);
    if(left.ncol==right.nraw){
        for (size_t i=0; i<left.nraw; i++)
            for(size_t k = 0; k < left.ncol; k++)
                for(size_t j=0; j<right.ncol; j++)
                    result.data[i*result.ncol+j]=
                        result.data[i*result.ncol+j]
                        +
                        left.data[i*left.ncol+k]
                        *
                        right.data[k*right.ncol+j];
                
    }
    else printf("matmul_ijk: left.ncol!=right.nraw");
    return result;
}

struct Matrix matmul_jik(struct Matrix left, struct Matrix right)
{
    struct Matrix result = make_const_matrix(left.nraw, right.ncol, 0.0);
    if(left.ncol==right.nraw){
        for(size_t j=0; j<right.ncol; j++)
            for (size_t i=0; i<left.nraw; i++)
                for(size_t k = 0; k < left.ncol; k++)
                    result.data[i*result.ncol+j]=
                        result.data[i*result.ncol+j]
                        +
                        left.data[i*left.ncol+k]
                        *
                        right.data[k*right.ncol+j];
                
    }
    else printf("matmul_ijk: left.ncol!=right.nraw");
    return result;
}

struct Matrix matmul_jki(struct Matrix left, struct Matrix right)
{
    struct Matrix result = make_const_matrix(left.nraw, right.ncol, 0.0);
    if(left.ncol==right.nraw){
        for(size_t j=0; j<right.ncol; j++)
            for(size_t k = 0; k < left.ncol; k++)
                for (size_t i=0; i<left.nraw; i++)
                    result.data[i*result.ncol+j]=
                        result.data[i*result.ncol+j]
                        +
                        left.data[i*left.ncol+k]
                        *
                        right.data[k*right.ncol+j];
                
    }
    else printf("matmul_ijk: left.ncol!=right.nraw");
    return result;
}

struct Matrix matmul_kij(struct Matrix left, struct Matrix right)
{
    struct Matrix result = make_const_matrix(left.nraw, right.ncol, 0.0);
    size_t res_offset, left_offset,right_offset;
    if(left.ncol==right.nraw){
        for(size_t k = 0; k < left.ncol; k++)
            for (size_t i=0; i<left.nraw; i++) {
                res_offset=i*result.ncol;
                left_offset=i*left.ncol;
                right_offset=k*right.ncol;
                for(size_t j=0; j<right.ncol; j++)
                    result.data[res_offset+j]=
                        result.data[res_offset+j]
                        +
                        left.data[left_offset+k]
                        *
                        right.data[right_offset+j];
            }
                
    }
    else printf("matmul_ijk: left.ncol!=right.nraw");
    return result;
}

struct Matrix matmul_kji(struct Matrix left, struct Matrix right)
{
    struct Matrix result = make_const_matrix(left.nraw, right.ncol, 0.0);
    if(left.ncol==right.nraw){
        for(size_t k = 0; k < left.ncol; k++)
            for(size_t j=0; j<right.ncol; j++){
                for (size_t i=0; i<left.nraw; i++)
                    result.data[i*result.ncol+j]=
                        result.data[i*result.ncol+j]
                        +
                        left.data[i*left.ncol+k]
                        *
                        right.data[k*right.ncol+j];
            }
                
    }
    else printf("matmul_ijk: left.ncol!=right.nraw");
    return result;
}

struct Matrix matmul(struct Matrix left, struct Matrix right, MMStrategy strategy){
    struct Matrix res;
    switch (strategy)
    {
        case IJK:
            res = matmul_ijk(left, right);
            // printf("!!!!!!ijk");
            return res;
        case IKJ:
            res = matmul_ikj(left, right);
            return res;
        case JIK:
            res = matmul_jik(left, right);
            return res;
        case JKI:
            res = matmul_jki(left, right);
            return res;
        case KIJ:
            res = matmul_kij(left, right);
            return res;
        case KJI:
            res = matmul_kji(left, right);
            return res;
        // default:
        //     break;
    }
}

struct Vector matvecmul_ij(struct Matrix left, struct Vector right)
{
    struct Vector result = make_const_vec(left.nraw, 0.0);
    if(left.ncol==right.length){
        for (size_t i=0; i<left.nraw; i++)
            for(size_t j=0; j<right.length; j++)
                result.data[i]=
                    result.data[i]
                    +
                    left.data[i*left.ncol+j]*right.data[j];
    }
    else printf("matmul_ijk: left.ncol!=right.nraw");
    return result;
}

struct Vector matvecmul_ji(struct Matrix left, struct Vector right)
{
    struct Vector result = make_const_vec(left.nraw, 0.0);
    if(left.ncol==right.length){
        for(size_t j=0; j<right.length; j++)
            for (size_t i=0; i<left.nraw; i++)
                result.data[i]=
                    result.data[i]
                    +
                    left.data[i*left.ncol+j]
                    *
                    right.data[j];
    }
    else printf("matmul_ijk: left.ncol!=right.nraw");
    return result;
}

struct Vector matvecmul(struct Matrix left, struct Vector right, MVStrategy strategy)
{
    struct Vector res;
    switch (strategy)
    {
        case IJ:
            res = matvecmul_ij(left, right);
            return res;
        case JI:
            res = matvecmul_ji(left, right);
            return res;
    }
}
#endif

