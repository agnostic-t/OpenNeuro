#ifndef OPENNEURO_MODELS_IZHIKEVICH_H
#define OPENNEURO_MODELS_IZHIKEVICH_H

#include "abstract.h"

#define IZHIKEVICH_TRESHOLD 30.f

typedef struct {
  float a; // Speed of `u` going down
  float b; // How sensitive `u` is to subthreshold values
  float c; // Voltage after spike
  float d; // Spike of adaptation
} izh_params;

typedef struct {
  float v;
  float u;
} izh_vector;

int izh_new_vector(neu_vector *vec, izh_params params);
int izh_new_neuron(neu_neuron *nr, izh_params params);
int izh_vec_update(const neu_vector *changed, neu_vector *new);

izh_vector *izh_vec(const neu_vector *vec);

int izh_sim_step(const neu_neuron *nr, const neu_vector *vec,
                 neu_vector *new_vec, float i_ext, float euler_dt);

int izh_is_spiked(const neu_neuron *nr, const neu_vector *vec);

#endif
