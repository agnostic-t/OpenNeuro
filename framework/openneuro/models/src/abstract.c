#include "abstract.h"
#include <stdlib.h>

void neu_vector_destroy(neu_vector *vec){
    if (!vec) return;

    free(vec->state);
}

void neu_biophysp_destroy(neu_biophysp *params){
    if (!params) return;

    free(params->params);
}

void neu_neuron_destroy(neu_neuron *nr){
    if (!nr) return;

    free(nr->sparams);
}
