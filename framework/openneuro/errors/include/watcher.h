#include "errors.h"
#include <stdio.h>
#include <stdlib.h>

#ifndef OPENNEU_ERROR_WATCHER_H
#define OPENNEU_ERROR_WATCHER_H

static inline int neu_check(int code) {
  if (code < 0) {
    printf("[neu] failed: %s\n", neu_get_error());
    exit(-1);
  }

  return code;
}

#endif
