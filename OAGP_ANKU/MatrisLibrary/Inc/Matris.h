#ifndef MATRIS_H_
#define MATRIS_H_

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* ------------ MATRIS YAPISI ------------ */
typedef struct {
    uint16_t rows;
    uint16_t cols;
    float* data;
} MatrixF;

/* ------------ FONKSİYONLAR ------------ */
MatrixF* create_matrix(uint16_t rows, uint16_t cols);
void     free_matrix(MatrixF* m);

void matrix_zero(MatrixF* m);
void matrix_identity(MatrixF* m);

void matrix_copy(MatrixF* src, MatrixF* dst);
void matrix_add(MatrixF* a, MatrixF* b, MatrixF* result);
void matrix_sub(MatrixF* a, MatrixF* b, MatrixF* result);
void matrix_mult(MatrixF* a, MatrixF* b, MatrixF* result);
void matrix_transpose(MatrixF* a, MatrixF* result);

/* 2x2 ters alma */
uint8_t matrix_inv2x2(MatrixF* m, MatrixF* out);

/* ------------ KALMAN ------------ */
typedef struct {
    MatrixF *X, *P;
    MatrixF *A, *B, *H;
    MatrixF *Q, *R;
    MatrixF *K;
} KalmanF;

KalmanF* kalman_create(uint16_t state_size, uint16_t meas_size);
void     kalman_free(KalmanF* kf);

void kalman_predict(KalmanF* kf, MatrixF* u);
void kalman_update(KalmanF* kf, MatrixF* z);

#endif /* MATRIS_H_ */
