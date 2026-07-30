#include "hodjkin_haxly.h"
#include "abstract.h"
#include "errors.h"
#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

// math --

#define HH_BIO_SHIFT 65.f

float alpha_m(float v) { return 0.1f * (v + 40.0f) / (1.0f - expf(-(v + 40.0f) / 10.0f)); }
float beta_m(float v)  { return 4.0f * expf(-(v + HH_BIO_SHIFT) / 18.0f); }

float alpha_h(float v) { return 0.07f * expf(-(v + HH_BIO_SHIFT) / 20.0f); }
float beta_h(float v)  { return 1.0f / (1.0f + expf(-(v + 35.0f) / 10.0f)); }

float alpha_n(float v) { return 0.01f * (v + 55.0f) / (1.0f - expf(-(v + 55.0f) / 10.0f)); }
float beta_n(float v)  { return 0.125f * expf(-(v + HH_BIO_SHIFT) / 80.0f); }

float HH_dv_dt(float v, float m, float h, float n, float i_ext,
               float c_m, float g_Na, float g_K, float g_L,
               float e_Na, float e_K, float e_L) {
    float i_Na = g_Na * m * m * m * h * (v - e_Na);
    float i_K  = g_K * n * n * n * n * (v - e_K);
    float i_L  = g_L * (v - e_L);
    return (i_ext - i_Na - i_K - i_L) / c_m;
}

float HH_dm_dt(float m, float v) { return alpha_m(v) * (1.0f - m) - beta_m(v) * m; }
float HH_dh_dt(float h, float v) { return alpha_h(v) * (1.0f - h) - beta_h(v) * h; }
float HH_dn_dt(float n, float v) { return alpha_n(v) * (1.0f - n) - beta_n(v) * n; }

// --

int hh_new_vector(neu_vector *vec){
    if (!vec) {
        neu_error("vec cannot be NULL");
        return -1;
    }

    hh_vector* lvec = malloc(sizeof *lvec);
    if (!lvec) {
        neu_error("malloc failed");
        return -1;
    }

    lvec->v = -65.0f;
    lvec->m = 0.05f;
    lvec->h = 0.6f;
    lvec->n = 0.32f;
    vec->state = lvec;
    return 0;
}

int hh_new_neuron(neu_neuron *nr, hh_params params){
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

int hh_new_biophys(neu_biophysp *bp, hh_biohys params) {
    if (!bp) {
        neu_error("biophysp cannot be NULL");
        return -1;
    }

    bp->params = malloc(sizeof(params));
    if (!bp->params) {
        neu_error("malloc failed");
        return -1;
    }

    memcpy(bp->params, &params, sizeof(params));
    return 0;
}

int hh_sim_step(const neu_biophysp *bp, const neu_neuron *nr, const neu_vector *vec,
                 neu_vector *new_vec, float i_ext, float euler_dt)
{
    if (!nr || !vec || !new_vec) {
        neu_error("input params cannot be NULL");
        return -1;
    }

    hh_params *params = nr->sparams;
    hh_vector *lvec = vec->state;
    hh_vector *lvec_new = new_vec->state;
    hh_biohys *lbp = bp->params;

    float dv_dt = HH_dv_dt(lvec->v, lvec->m, lvec->h, lvec->n, i_ext, lbp->c_m, lbp->g_Na, lbp->g_K, lbp->g_L, params->e_Na, params->e_K, params->e_L);
    float dm_dt = HH_dm_dt(lvec->m, lvec->v);
    float dh_dt = HH_dh_dt(lvec->h, lvec->v);
    float dn_dt = HH_dn_dt(lvec->n, lvec->v);

    lvec_new->v = lvec->v + dv_dt * euler_dt;
    lvec_new->m = lvec->m + dm_dt * euler_dt;
    lvec_new->h = lvec->h + dh_dt * euler_dt;
    lvec_new->n = lvec->n + dn_dt * euler_dt;

    return 0;
}

hh_vector *hh_vec(const neu_vector *vec) {
    if (!vec) return NULL;

    return vec->state;
}

int hh_vec_update(const neu_vector *changed, neu_vector *new){
    if (!changed || !new) {
        neu_error("inputs cannot be NULL");
        return -1;
    }

    hh_vector *new_v = new->state;
    hh_vector *ch_v = changed->state;

    (*new_v) = (*ch_v);
    return 0;
}
