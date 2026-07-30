#include "adex.h"
#include "errors.h"
#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

// math --

float AdEx_dv_dt(
    float v, float i_ext, float w,
    float c_m, float g_L, float dt_T, float e_L, float v_t
) {
    return (-g_L * (v - e_L) + g_L * dt_T * expf((v - v_t) / dt_T) - w + i_ext) / c_m;
}

float AdEx_dw_dt(
    float w, float v,
    float tau_w, float e_L, float a
) {
    return (a * (v - e_L) - w) / tau_w;
}

// --

int adex_new_vector(neu_vector *vec, adex_params params) {
    if (!vec) {
        neu_error("vec cannot be NULL");
        return -1;
    }

    adex_vector* adexvec = malloc(sizeof *adexvec);
    if (!adexvec) {
        neu_error("malloc failed");
        return -1;
    }

    adexvec->v = params.e_L;
    adexvec->w = 0;
    adexvec->in_rest = 0;
    vec->state = adexvec;
    return 0;
}

int adex_new_neuron(neu_neuron *nr, adex_params params){
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


int adex_sim_step(const neu_neuron *nr, const neu_vector *vec,
                 neu_vector *new_vec, float i_ext, float euler_dt)
{
    if (!nr || !vec || !new_vec) {
        neu_error("input params cannot be NULL");
        return -1;
    }

    adex_params *params = nr->sparams;
    adex_vector *lvec = vec->state;
    adex_vector *lvec_new = new_vec->state;

    lvec_new->v = lvec->v;
    lvec_new->in_rest = lvec->in_rest;

    if (lvec->in_rest > 0) {
        lvec_new->in_rest = lvec->in_rest - 1;
        lvec_new->v = params->v_rest;

        float dw_dt = AdEx_dw_dt(
            lvec->w, lvec->v,
            params->tau_w, params->e_L, params->a
        ) * euler_dt;
        lvec_new->w = lvec->w + dw_dt;

        return 0;
    }

    float dw_dt = AdEx_dw_dt(
        lvec->w, lvec->v,
        params->tau_w, params->e_L, params->a
    ) * euler_dt;
    lvec_new->w = lvec->w + dw_dt;

    float dv_dt = AdEx_dv_dt(
        lvec->v, i_ext, lvec->w,
        params->c_m, params->g_L, params->dt_T, params->e_L, params->v_t
    ) * euler_dt;

    lvec_new->v = lvec->v + dv_dt;

    if (lvec_new->v >= params->v_peak) {
        lvec_new->v = params->v_rest;
        lvec_new->in_rest = (int)(params->tau_r / euler_dt);
        lvec_new->w += params->b;
        return 1;
    }

    return 0;
}

int adex_is_spiked(const neu_neuron *nr, const neu_vector *vec){
    if (!nr || !vec) {
        neu_error("input params cannot be NULL");
        return -1;
    }

    adex_params *params = nr->sparams;
    adex_vector *lvec = vec->state;

    return lvec->v >= params->v_peak;
}

adex_vector *adex_vec(const neu_vector *vec) {
    if (!vec) return NULL;

    return vec->state;
}

int adex_vec_update(const neu_vector *changed, neu_vector *new){
    if (!changed || !new) {
        neu_error("inputs cannot be NULL");
        return -1;
    }

    adex_vector *new_v = new->state;
    adex_vector *ch_v = changed->state;

    (*new_v) = (*ch_v);
    return 0;
}
