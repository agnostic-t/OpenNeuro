#ifndef OPENNEURO_MATH_MAT_H
#define OPENNEURO_MATH_MAT_H

#include <stdlib.h>
typedef struct {
  float *data;

  int dim_x, dim_y;
} matnn;

int neu_mat_new(unsigned int dim_x, unsigned int dim_y, float fill_v,
                matnn *out);
void neu_mat_free(matnn *m);
int neu_mat_copy(const matnn *src, matnn *dest);
int neu_mat_replace(const matnn *src, matnn *dest);

int neu_mat_mul(const matnn *m_a, const matnn *m_b, matnn *m_out);
int neu_mat_transpose(const matnn *m_a, matnn *m_out);
int neu_mat_transpose_ip(matnn *m_a);

int neu_mat_sum(const matnn *m_a, const matnn *m_b, matnn *m_out);
int neu_mat_hmul(const matnn *m_a, const matnn *m_b, matnn *m_out);

int neu_mat_sc_mul(const matnn *m_a, float scalar, matnn *m_out);
int neu_mat_sc_sum(const matnn *m_a, float scalar, matnn *m_out);

// vec (mat 1xN) ---
int neu_vec_new(unsigned int dim, float fill_v, matnn *out);
int neu_vec_dot(const matnn *v_a, const matnn *v_b, float *v_out);
char *neu_mat_str(const matnn *mat);

// utility
const char *neu_mat_error();

#endif
