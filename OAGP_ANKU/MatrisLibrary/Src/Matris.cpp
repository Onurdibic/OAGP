#include "Matris.h"
#include <math.h>

/* ------------------ MATRIX CORE ------------------ */

MatrixF* create_matrix(uint16_t rows, uint16_t cols) {
    MatrixF* m = (MatrixF*)malloc(sizeof(MatrixF));
    if (!m) return NULL;

    m->rows = rows;
    m->cols = cols;
    m->data = (float*)calloc(rows * cols, sizeof(float));

    if (!m->data) {
        free(m);
        return NULL;
    }
    return m;
}

void free_matrix(MatrixF* m) {
    if (!m) return;
    free(m->data);
    free(m);
}

void matrix_zero(MatrixF* m) {
    memset(m->data, 0, sizeof(float) * m->rows * m->cols);
}

void matrix_identity(MatrixF* m) {
    matrix_zero(m);
    for (uint16_t i = 0; i < m->rows; i++)
        m->data[i*m->cols + i] = 1.0f;
}

void matrix_copy(MatrixF* src, MatrixF* dst) {
    memcpy(dst->data, src->data, sizeof(float) * src->rows * src->cols);
}

void matrix_add(MatrixF* a, MatrixF* b, MatrixF* result) {
    uint16_t n = a->rows * a->cols;
    for (uint16_t i=0; i<n; i++)
        result->data[i] = a->data[i] + b->data[i];
}

void matrix_sub(MatrixF* a, MatrixF* b, MatrixF* result) {
    uint16_t n = a->rows * a->cols;
    for (uint16_t i=0; i<n; i++)
        result->data[i] = a->data[i] - b->data[i];
}

void matrix_transpose(MatrixF* a, MatrixF* result) {
    for (uint16_t r=0; r<a->rows; r++)
        for (uint16_t c=0; c<a->cols; c++)
            result->data[c*a->rows + r] = a->data[r*a->cols + c];
}

void matrix_mult(MatrixF* a, MatrixF* b, MatrixF* result) {
    for (uint16_t i=0; i<a->rows; i++) {
        for (uint16_t j=0; j<b->cols; j++) {
            float sum = 0;
            for (uint16_t k=0; k<a->cols; k++)
                sum += a->data[i*a->cols + k] * b->data[k*b->cols + j];
            result->data[i*b->cols + j] = sum;
        }
    }
}

/* ------------ 2x2 INVERSE ------------ */
uint8_t matrix_inv2x2(MatrixF* m, MatrixF* out) {
    float a = m->data[0];
    float b = m->data[1];
    float c = m->data[2];
    float d = m->data[3];

    float det = a*d - b*c;
    if (fabs(det) < 1e-6) return 0;

    out->data[0] =  d / det;
    out->data[1] = -b / det;
    out->data[2] = -c / det;
    out->data[3] =  a / det;

    return 1;
}

/* ------------------ KALMAN ------------------ */

KalmanF* kalman_create(uint16_t state_size, uint16_t meas_size) {
    KalmanF* k = (KalmanF*)malloc(sizeof(KalmanF));

    k->X = create_matrix(state_size,1);
    k->P = create_matrix(state_size,state_size);
    k->A = create_matrix(state_size,state_size);
    k->B = create_matrix(state_size,state_size);
    k->H = create_matrix(meas_size,state_size);
    k->Q = create_matrix(state_size,state_size);
    k->R = create_matrix(meas_size,meas_size);
    k->K = create_matrix(state_size,meas_size);

    return k;
}

void kalman_free(KalmanF* k) {
    free_matrix(k->X); free_matrix(k->P); free_matrix(k->A);
    free_matrix(k->B); free_matrix(k->H); free_matrix(k->Q);
    free_matrix(k->R); free_matrix(k->K);
    free(k);
}

/* ------------ PREDICT: X = A*X + B*u ------------ */
void kalman_predict(KalmanF* k, MatrixF* u) {

    MatrixF AX = *create_matrix(k->X->rows,1);
    matrix_mult(k->A, k->X, &AX);

    MatrixF BU = *create_matrix(k->X->rows,1);
    matrix_mult(k->B, u, &BU);

    matrix_add(&AX, &BU, k->X);

    /* P = A P A' + Q */
    MatrixF AP  = *create_matrix(k->P->rows, k->P->cols);
    matrix_mult(k->A, k->P, &AP);

    MatrixF AT = *create_matrix(k->A->cols, k->A->rows);
    matrix_transpose(k->A, &AT);

    MatrixF APA = *create_matrix(k->P->rows, k->P->cols);
    matrix_mult(&AP, &AT, &APA);

    matrix_add(&APA, k->Q, k->P);

    free_matrix(&AX); free_matrix(&BU);
    free_matrix(&AP); free_matrix(&AT); free_matrix(&APA);
}

/* ------------ UPDATE ------------ */
void kalman_update(KalmanF* k, MatrixF* z) {

    /* S = H P H' + R */
    MatrixF HP = *create_matrix(k->H->rows, k->P->cols);
    matrix_mult(k->H, k->P, &HP);

    MatrixF HT = *create_matrix(k->H->cols, k->H->rows);
    matrix_transpose(k->H, &HT);

    MatrixF S = *create_matrix(k->H->rows, k->H->rows);
    matrix_mult(&HP, &HT, &S);
    matrix_add(&S, k->R, &S);

    /* 2x2 inverse (örnek için) */
    MatrixF S_inv = *create_matrix(S.rows, S.cols);
    matrix_inv2x2(&S, &S_inv);

    /* K = P H' S^-1 */
    MatrixF PHT = *create_matrix(k->P->rows, HT.cols);
    matrix_mult(k->P, &HT, &PHT);

    matrix_mult(&PHT, &S_inv, k->K);

    /* X = X + K (z - H X) */
    MatrixF HX = *create_matrix(z->rows, 1);
    matrix_mult(k->H, k->X, &HX);

    MatrixF Y = *create_matrix(z->rows, 1);
    matrix_sub(z, &HX, &Y);

    MatrixF KY = *create_matrix(k->X->rows,1);
    matrix_mult(k->K, &Y, &KY);

    matrix_add(k->X, &KY, k->X);

    /* P = (I - K H) P */
    MatrixF KH = *create_matrix(k->K->rows, k->H->cols);
    matrix_mult(k->K, k->H, &KH);

    MatrixF I = *create_matrix(k->P->rows, k->P->cols);
    matrix_identity(&I);

    MatrixF IKH = *create_matrix(I.rows, I.cols);
    matrix_sub(&I, &KH, &IKH);

    MatrixF newP = *create_matrix(k->P->rows, k->P->cols);
    matrix_mult(&IKH, k->P, &newP);

    matrix_copy(&newP, k->P);

    /* TEMP FREE */
    free_matrix(&HP); free_matrix(&HT); free_matrix(&S);
    free_matrix(&S_inv); free_matrix(&PHT);
    free_matrix(&HX); free_matrix(&Y); free_matrix(&KY);
    free_matrix(&KH); free_matrix(&I); free_matrix(&IKH);
    free_matrix(&newP);
}
