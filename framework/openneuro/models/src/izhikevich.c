#include "izhikevich.h"
#include "errors.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

// math --

float Izhekevich_dv_dt(
    float v, float u, float I
) {
    return 0.04 * v * v + 5 * v + 140 - u + I;
}

float Izhekevich_du_dt(
    float v, float u, float a, float b
) {
    return a * (b * v - u);
}
// --

int izh_new_vector(neu_vector *vec, izh_params params) {
    if (!vec) {
        neu_error("vec cannot be NULL");
        return -1;
    }

    izh_vector* izhvec = malloc(sizeof *izhvec);
    if (!izhvec) {
        neu_error("malloc failed");
        return -1;
    }

    izhvec->v = params.c;
    izhvec->u = params.b * params.c;
    vec->state = izhvec;
    return 0;
}

int izh_new_neuron(neu_neuron *nr, izh_params params){
    if (!nr) {
        neu_error("neuron cannot be NULL");
        return -1;
    }

    nr->sparams = malloc(sizeof(params));
    if (!nr->sparams) {
        neu_error("malloc failed");
        return -1;
    }

    memcpy(nr->sparams, &params, sizeof(params));
    return 0;
}


int izh_sim_step(const neu_neuron *nr, const neu_vector *vec,
                 neu_vector *new_vec, float i_ext, float euler_dt)
{
    if (!nr || !vec || !new_vec) {
        neu_error("input params cannot be NULL");
        return -1;
    }

    izh_params *params = nr->sparams;
    izh_vector *lvec = vec->state;
    izh_vector *lvec_new = new_vec->state;

    lvec_new->v = lvec->v;
    lvec_new->u = lvec->u;

    float dv_dt = Izhekevich_dv_dt(lvec->v, lvec->u, i_ext);
    lvec_new->v += dv_dt * euler_dt;

    float du_dt = Izhekevich_du_dt(lvec->v, lvec->u, params->a, params->b);
    lvec_new->u += du_dt * euler_dt;

    if (lvec_new->v >= IZHIKEVICH_TRESHOLD) {
        lvec_new->v = params->c;
        lvec_new->u += params->d;
        return 1;
    }

    return 0;
}

int izh_is_spiked(const neu_neuron *nr, const neu_vector *vec){
    if (!nr || !vec) {
        neu_error("input params cannot be NULL");
        return -1;
    }

    izh_params *params = nr->sparams;
    izh_vector *lvec = vec->state;

    return lvec->v >= IZHIKEVICH_TRESHOLD;
}

izh_vector *izh_vec(const neu_vector *vec) {
    if (!vec) return NULL;

    return vec->state;
}

int izh_vec_update(const neu_vector *changed, neu_vector *new){
    if (!changed || !new) {
        neu_error("inputs cannot be NULL");
        return -1;
    }

    izh_vector *new_v = new->state;
    izh_vector *ch_v = changed->state;

    (*new_v) = (*ch_v);
    return 0;
}
