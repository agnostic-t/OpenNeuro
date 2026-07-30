#include "lif.h"
#include "errors.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

// math --

static inline float LIF_dv_dt(
    float v, float i_ext,
    float tau_m, float v_rest, float r_m
) {
    return (-(v - v_rest) + r_m * i_ext) / tau_m;
}

// --

int lif_new_vector(neu_vector *vec){
    if (!vec) {
        neu_error("vec cannot be NULL");
        return -1;
    }

    lif_vector* lvec = malloc(sizeof *lvec);
    if (!lvec) {
        neu_error("malloc failed");
        return -1;
    }

    lvec->v = 0.f;
    vec->state = lvec;
    return 0;
}

int lif_new_neuron(neu_neuron *nr, lif_params params){
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


int lif_sim_step(const neu_neuron *nr, const neu_vector *vec,
                 neu_vector *new_vec, float i_ext, float euler_dt)
{
    if (!nr || !vec || !new_vec) {
        neu_error("input params cannot be NULL");
        return -1;
    }

    lif_params *params = nr->sparams;
    lif_vector *lvec = vec->state;
    lif_vector *lvec_new = new_vec->state;

    float dv_dt = LIF_dv_dt(
        lvec->v, i_ext,
        params->tau_m, params->v_rest, params->resist_m
    ) * euler_dt;

    lvec_new->v = lvec->v + dv_dt;

    if (lvec->v >= params->v_threshold) {
        lvec_new->v = params->v_rest;
        return 1;
    }

    return 0;
}

int lif_is_spiked(const neu_neuron *nr, const neu_vector *vec){
    if (!nr || !vec) {
        neu_error("input params cannot be NULL");
        return -1;
    }

    lif_params *params = nr->sparams;
    lif_vector *lvec = vec->state;

    return lvec->v >= params->v_threshold;
}

lif_vector *lif_vec(const neu_vector *vec) {
    if (!vec) return NULL;

    return vec->state;
}

int lif_vec_update(const neu_vector *changed, neu_vector *new){
    if (!changed || !new) {
        neu_error("inputs cannot be NULL");
        return -1;
    }

    lif_vector *new_v = new->state;
    lif_vector *ch_v = changed->state;

    (*new_v) = (*ch_v);
    return 0;
}
