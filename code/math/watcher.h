#include "mat.h"
#include <stdio.h>
#include <stdlib.h>

#ifndef OPENNEU_MATH_WATCHER_H
#define OPENNEU_MATH_WATCHER_H

typedef struct {
  matnn *ptr;
} __dfobject;

static inline __dfobject *__make_base_deferer(matnn *ptr) {
  __dfobject *df = (__dfobject *)malloc(sizeof(__dfobject));
  df->ptr = ptr;
  return df;
}

static inline void __base_deffering(__dfobject **dfobject) {
  __dfobject *obj = *dfobject;

  neu_mat_free(obj->ptr);
  free(obj);
}

#define GLUE_HELPER(x, y) x##y
#define GLUE(x, y) GLUE_HELPER(x, y)

#define neu_destroy(mat)                                                       \
  __attribute__((cleanup(__base_deffering)))                                   \
  __dfobject *GLUE(__dfobject, __LINE__) = __make_base_deferer(mat);           \
  (void)GLUE(__dfobject, __LINE__);

static inline void neu_check(int code) {
  if (code < 0) {
    printf("[neu] failed: %s\n", neu_mat_error());
    exit(-1);
  }
}

#define neu_magic_mat(dim_x, dim_y, fill, mat)                                 \
  neu_check(neu_mat_new(dim_x, dim_y, fill, mat));                             \
  neu_destroy(mat);

#define neu_magic_vec(dim, fill, vec)                                          \
  neu_check(neu_vec_new(dim, fill, vec));                                      \
  neu_destroy(vec);

#endif
