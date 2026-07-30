#ifndef OPENNEURO_ERRORS_H
#define OPENNEURO_ERRORS_H

#define OPENNEURO_ERROR_MAX_LEN 2000

void neu_set_error(const char *msg);

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

#define neu_error(msg) (neu_set_error(__FILE__ ":" STR(__LINE__) ": " msg))

const char *neu_get_error();

#endif
