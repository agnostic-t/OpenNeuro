#include "eif.h"
#include "errors.h"
#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

// math --

float EIF_dv_dt(
    float v, float i_ext,
    float c_m, float g_L, float dt_T, float e_L, float v_t
) {
    return (-g_L * (v - e_L) + g_L * dt_T * expf((v - v_t) / dt_T) + i_ext) / c_m;
}

// --

int eif_new_vector(neu_vector *vec, eif_params params) {
    if (!vec) {
        neu_error("vec cannot be NULL");
        return -1;
    }

    eif_vector* evec = malloc(sizeof *evec);
    if (!evec) {
        neu_error("malloc failed");
        return -1;
    }

    evec->v = params.e_L;
    evec->in_rest = 0;
    vec->state = evec;
    return 0;
}

int eif_new_neuron(neu_neuron *nr, eif_params params){
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


int eif_sim_step(const neu_neuron *nr, const neu_vector *vec,
                 neu_vector *new_vec, float i_ext, float euler_dt)
{
    if (!nr || !vec || !new_vec) {
        neu_error("input params cannot be NULL");
        return -1;
    }

    eif_params *params = nr->sparams;
    eif_vector *lvec = vec->state;
    eif_vector *lvec_new = new_vec->state;

    if (lvec->in_rest > 0) {
        lvec_new->in_rest = lvec->in_rest - 1;
        lvec_new->v = params->v_rest;

        return 0;
    }

    float dv_dt = EIF_dv_dt(
        lvec->v, i_ext,
        params->c_m, params->g_L, params->dt_T, params->e_L, params->v_t
    ) * euler_dt;

    lvec_new->v = lvec->v + dv_dt;
    lvec_new->in_rest = lvec->in_rest;

    if (lvec->v >= params->v_peak) {
        lvec_new->v = params->v_rest;
        lvec_new->in_rest = (int)(params->tau_r / euler_dt);
        return 1;
    }

    return 0;
}

int eif_is_spiked(const neu_neuron *nr, const neu_vector *vec){
    if (!nr || !vec) {
        neu_error("input params cannot be NULL");
        return -1;
    }

    eif_params *params = nr->sparams;
    eif_vector *lvec = vec->state;

    return lvec->v >= params->v_peak;
}

eif_vector *eif_vec(const neu_vector *vec) {
    if (!vec) return NULL;

    return vec->state;
}

int eif_vec_update(const neu_vector *changed, neu_vector *new){
    if (!changed || !new) {
        neu_error("inputs cannot be NULL");
        return -1;
    }

    eif_vector *new_v = new->state;
    eif_vector *ch_v = changed->state;

    (*new_v) = (*ch_v);
    return 0;
}
