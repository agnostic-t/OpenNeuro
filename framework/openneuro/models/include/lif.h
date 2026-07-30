#ifndef OPENNEURO_MODELS_LIF_H
#define OPENNEURO_MODELS_LIF_H

#include "abstract.h"

typedef struct {
  float tau_m;
  float resist_m;
  float v_rest;
  float v_threshold;
} lif_params;

typedef struct {
  float v;
} lif_vector;

int lif_new_vector(neu_vector *vec);
int lif_new_neuron(neu_neuron *nr, lif_params params);
int lif_vec_update(const neu_vector *changed, neu_vector *new);

lif_vector *lif_vec(const neu_vector *vec);

int lif_sim_step(const neu_neuron *nr, const neu_vector *vec,
                 neu_vector *new_vec, float i_ext, float euler_dt);

int lif_is_spiked(const neu_neuron *nr, const neu_vector *vec);

#endif
