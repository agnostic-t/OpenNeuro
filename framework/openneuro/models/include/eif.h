#ifndef OPENNEURO_MODELS_EIF_H
#define OPENNEURO_MODELS_EIF_H

#include "abstract.h"

typedef struct {
  float c_m;    // Membranes capacity (in pF)
  float g_L;    // How well voltage leaks (in nS)
  float dt_T;   // Steepness of the spike (in mV)
  float e_L;    // Voltage level in neuron without stimules (in mV)
  float v_t;    // Threshold for the spike (in mV)
  float v_rest; // Voltage after spike (in mV)
  float v_peak; // Voltage at spike moment (in mV)
  float tau_r;  // Resting time after spike (in ms)
} eif_params;

typedef struct {
  float v;
  int in_rest;
} eif_vector;

int eif_new_vector(neu_vector *vec, eif_params params);
int eif_new_neuron(neu_neuron *nr, eif_params params);
int eif_vec_update(const neu_vector *changed, neu_vector *new);

eif_vector *eif_vec(const neu_vector *vec);

int eif_sim_step(const neu_neuron *nr, const neu_vector *vec,
                 neu_vector *new_vec, float i_ext, float euler_dt);

int eif_is_spiked(const neu_neuron *nr, const neu_vector *vec);

#endif
