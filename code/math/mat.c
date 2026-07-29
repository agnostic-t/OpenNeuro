#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "mat.h"

char NEU_MAT_ERROR_STRING[2000] = {0};

static inline bool is_in_bounds(const matnn *m, int x, int y) {
    return x > 0 && x < m->dim_x && y > 0 && y < m->dim_y;
}

static inline float get_mat_value_at(const matnn *m, int x, int y) {
    return m->data[y * m->dim_x + x];
}

static inline void set_mat_value_at(matnn *m, int x, int y, float v) {
    m->data[y * m->dim_x + x] = v;
}

static inline int mx_inx(const matnn *m) {
    return m->dim_x * m->dim_y;
}

static inline void set_mat_value_ati(matnn *m, int inx, float v) {
    m->data[inx] = v;
}

static inline void set_error(const char *msg) {
    assert(strlen(msg) < 2000);

    strcpy(NEU_MAT_ERROR_STRING, msg);
}

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

#define neu_error(msg) (set_error(__FILE__ ":" STR(__LINE__) ": " msg))

const char *neu_mat_error() {
    return NEU_MAT_ERROR_STRING;
}

int neu_mat_new(unsigned int dim_x, unsigned int dim_y, float fill_v, matnn *out){
    if (!(dim_x > 0 && dim_y > 0)) {
        neu_error("dimensions cannot be equal to zero");
        return -1;
    }

    if (!out) {
        neu_error("out matrix cannot be NULL");
        return -1;
    }

    out->data = malloc(sizeof(float) * dim_x * dim_y);
    if (!out->data) {
        neu_error("malloc() failed");
        return -1;
    }

    out->dim_x = dim_x;
    out->dim_y = dim_y;

    for (int i = 0; i < mx_inx(out); i++){
        set_mat_value_ati(out, i, fill_v);
    }

    return 0;
}

int neu_mat_copy(const matnn *src, matnn *dest) {
    if (!src || !dest) {
        neu_error("for copying src and dest must be non-NULL");
        return -1;
    }

    if (0 > neu_mat_new(src->dim_x, src->dim_y, 0, dest))
        return -1;

    memcpy(dest->data, src->data, sizeof(float) * src->dim_x * src->dim_y);
    return 0;
}

int neu_mat_replace(const matnn *src, matnn *dest) {
    if (!src || !dest) {
        neu_error("for replacing src and dest must be non-NULL");
        return -1;
    }

    neu_mat_free(dest);
    return neu_mat_copy(src, dest);
}

void neu_mat_free(matnn *m) {
    if (!m) return;

    free(m->data);
    m->data = NULL;
    m->dim_x = 0;
    m->dim_y = 0;
}

char *neu_mat_str(const matnn *mat) {
    if (!mat || !mat->data || mat->dim_x <= 0 || mat->dim_y <= 0) {
        char *empty = malloc(3 * sizeof(char));
        if (empty) {
            strcpy(empty, "[]");
        }
        return empty;
    }

    int rows = mat->dim_y;
    int cols = mat->dim_x;

    int *col_widths = malloc(cols * sizeof(int));
    if (!col_widths) {
        return NULL;
    }

    for (int c = 0; c < cols; c++) {
        col_widths[c] = 0;
        for (int r = 0; r < rows; r++) {
            float val = mat->data[r * cols + c];
            char buf[64];
            int len = snprintf(buf, sizeof(buf), "%.4f", val);
            if (len > col_widths[c]) {
                col_widths[c] = len;
            }
        }
    }

    size_t total_len = 2;
    for (int r = 0; r < rows; r++) {
        if (r > 0) {
            total_len += 3;
        }
        for (int c = 0; c < cols; c++) {
            total_len += col_widths[c];
            if (c < cols - 1) {
                total_len += 1;
            }
        }
        total_len += 1;
    }
    total_len += 2;

    char *result = malloc(total_len * sizeof(char));
    if (!result) {
        free(col_widths);
        return NULL;
    }

    char *ptr = result;
    ptr += sprintf(ptr, "[[");

    for (int r = 0; r < rows; r++) {
        if (r > 0) {
            ptr += sprintf(ptr, "\n [");
        }
        for (int c = 0; c < cols; c++) {
            if (c > 0) {
                ptr += sprintf(ptr, " ");
            }
            float val = mat->data[r * cols + c];
            ptr += sprintf(ptr, "%*.*f", col_widths[c], 4, val);
        }
        ptr += sprintf(ptr, "]");
    }
    ptr += sprintf(ptr, "]");

    free(col_widths);
    return result;
}

int neu_mat_mul(const matnn *m_a, const matnn *m_b, matnn *m_out){
    if (!(m_a && m_b)) {
        neu_error("input matricies cannot be NULL");
        return -1;
    }

    if (!m_out) {
        neu_error("output matrix cannot be NULL");
        return -1;
    }

    if (m_a->dim_x != m_b->dim_y) {
        neu_error("cannot multiply matricies with given dimensions");
        return -1;
    }

    int new_dim_x = m_b->dim_x;
    int new_dim_y = m_a->dim_y;

    if (0 > neu_mat_new(new_dim_x, new_dim_y, 0, m_out))
        return -1;

    for (int a_y = 0; a_y < m_a->dim_y; a_y++) {
        for (int b_x = 0; b_x < m_b->dim_x; b_x++){
            float sum = 0;
            for (int a_x = 0; a_x < m_a->dim_x; a_x++) {
                float a_v = get_mat_value_at(m_a, a_x, a_y);
                float b_v = get_mat_value_at(m_b, b_x, a_x);

                sum += a_v * b_v;
            }
            set_mat_value_at(m_out, b_x, a_y, sum);
        }
    }

    return 0;
}

int neu_mat_transpose_ip(matnn *m_a) {
    if (!m_a) {
        neu_error("input matrix cannot be NULL");
        return -1;
    }

    matnn m_buf;

    if (0 > neu_mat_new(m_a->dim_y, m_a->dim_x, 0, &m_buf))
        return -1;

    for (int y = 0; y < m_a->dim_y; y++) {
        for (int x = 0; x < m_a->dim_x; x++){
            float v = get_mat_value_at(m_a, x, y);
            set_mat_value_at(&m_buf, y, x, v);
        }
    }

    if (0 > neu_mat_replace(&m_buf, m_a))
        return -1;

    neu_mat_free(&m_buf);
    return 0;
}

int neu_mat_transpose(const matnn *m_a, matnn *m_out){
    if (!m_a) {
        neu_error("input matrix cannot be NULL");
        return -1;
    }

    if (!m_out) {
        neu_error("output matrix cannot be NULL");
        return -1;
    }

    if (0 > neu_mat_new(m_a->dim_y, m_a->dim_x, 0, m_out))
        return -1;

    for (int y = 0; y < m_a->dim_y; y++) {
        for (int x = 0; x < m_a->dim_x; x++){
            float v = get_mat_value_at(m_a, x, y);
            set_mat_value_at(m_out, y, x, v);
        }
    }

    return 0;
}


int neu_mat_sum(const matnn *m_a, const matnn *m_b, matnn *m_out){
    if (!(m_a && m_b)) {
        neu_error("input matricies cannot be NULL");
        return -1;
    }

    if (!m_out) {
        neu_error("output matrix cannot be NULL");
        return -1;
    }

    if (m_a->dim_x != m_b->dim_x || m_a->dim_y != m_b->dim_y) {
        neu_error("for sum dimensions of input matricies must match");
        return -1;
    }

    if (0 > neu_mat_copy(m_a, m_out))
        return -1;

    for (int y = 0; y < m_a->dim_y; y++) {
        for (int x = 0; x < m_a->dim_x; x++){
            float v = get_mat_value_at(m_out, x, y);
            float v2 = get_mat_value_at(m_b, x, y);

            set_mat_value_at(m_out, x, y, v + v2);
        }
    }

    return 0;
}

int neu_mat_hmul(const matnn *m_a, const matnn *m_b, matnn *m_out){
    if (!(m_a && m_b)) {
        neu_error("input matricies cannot be NULL");
        return -1;
    }

    if (!m_out) {
        neu_error("output matrix cannot be NULL");
        return -1;
    }

    if (m_a->dim_x != m_b->dim_x || m_a->dim_y != m_b->dim_y) {
        neu_error("for hmul dimensions of input matricies must match");
        return -1;
    }

    if (0 > neu_mat_copy(m_a, m_out))
        return -1;

    for (int y = 0; y < m_a->dim_y; y++) {
        for (int x = 0; x < m_a->dim_x; x++){
            float v = get_mat_value_at(m_out, x, y);
            float v2 = get_mat_value_at(m_b, x, y);

            set_mat_value_at(m_out, x, y, v * v2);
        }
    }

    return 0;
}


int neu_mat_sc_mul(const matnn *m_a, float scalar, matnn *m_out){
    if (!m_a) {
        neu_error("input matrix cannot be NULL");
        return -1;
    }

    if (!m_out) {
        neu_error("output matrix cannot be NULL");
        return -1;
    }

    if (0 > neu_mat_copy(m_a, m_out))
        return -1;

    for (int y = 0; y < m_a->dim_y; y++) {
        for (int x = 0; x < m_a->dim_x; x++){
            float v = get_mat_value_at(m_out, x, y);

            set_mat_value_at(m_out, x, y, v * scalar);
        }
    }

    return 0;
}

int neu_mat_sc_sum(const matnn *m_a, float scalar, matnn *m_out){
    if (!m_a) {
        neu_error("input matrix cannot be NULL");
        return -1;
    }

    if (!m_out) {
        neu_error("output matrix cannot be NULL");
        return -1;
    }

    if (0 > neu_mat_copy(m_a, m_out))
        return -1;

    for (int y = 0; y < m_a->dim_y; y++) {
        for (int x = 0; x < m_a->dim_x; x++){
            float v = get_mat_value_at(m_out, x, y);

            set_mat_value_at(m_out, x, y, v + scalar);
        }
    }

    return 0;
}


// vec (mat Nx1) ---
int neu_vec_new(unsigned int dim, float fill_v, matnn *out){
    if (!out) {
        neu_error("output vector cannot be NULL");
        return -1;
    }

    return neu_mat_new(dim, 1, fill_v, out);
}

static inline bool is_a_vector(const matnn *m) {
    return m->dim_y == 1;
}

int new_vec_dot(const matnn *v_a, const matnn *v_b, float *v_out){
    if(!(v_a && v_b)) {
        neu_error("input vectors cannot be NULL");
        return -1;
    }

    if (!v_out) {
        neu_error("output value pointer cannot be NULL");
        return -1;
    }

    if (v_a->dim_x != v_b->dim_x || v_a->dim_y != v_b->dim_y) {
        neu_error("for dot product dimensions of input vectors must match");
        return -1;
    }

    if (!is_a_vector(v_a) || !is_a_vector(v_b)) {
        neu_error("dot product cannot be done on matricies, use regular matmul");
        return -1;
    }

    float sum = 0;
    for (int i = 0; i < v_a->dim_x; i++) {
        sum += v_a->data[i] * v_b->data[i];
    }

    *v_out = sum;
    return 0;
}
