#include <assert.h>
#include <string.h>

#include "errors.h"

char _NEU_ERROR_MSG[OPENNEURO_ERROR_MAX_LEN] = {0};

void neu_set_error(const char *msg) {
    assert(strlen(msg) < 2000);

    strcpy(_NEU_ERROR_MSG, msg);
}

const char *neu_get_error() {
    return _NEU_ERROR_MSG;
}
