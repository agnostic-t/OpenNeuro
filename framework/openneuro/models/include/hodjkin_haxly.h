#ifndef OPENNEURO_MODELS_HODJKIN_HAXLY_H
#define OPENNEURO_MODELS_HODJKIN_HAXLY_H

#include "abstract.h"

typedef struct {
  float c_m;
  float g_Na;
  float g_K;
  float g_L;
} hh_biohys;

typedef struct {
  float e_Na;
  float e_K;
  float e_L;
} hh_params;

typedef struct {
  float v;
  float m;
  float h;
  float n;
} hh_vector;

int hh_new_vector(neu_vector *vec);
int hh_new_neuron(neu_neuron *nr, hh_params params);
int hh_new_biophys(neu_biophysp *bp, hh_biohys params);
int hh_vec_update(const neu_vector *changed, neu_vector *new);

hh_vector *hh_vec(const neu_vector *vec);

int hh_sim_step(const neu_biophysp *bp, const neu_neuron *nr,
                const neu_vector *vec, neu_vector *new_vec, float i_ext,
                float euler_dt);

// int hh_is_spiked(const neu_neuron *nr, const neu_vector *vec);

#endif
