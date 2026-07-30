#ifndef OPENNEURO_MODELS_ADEX_H
#define OPENNEURO_MODELS_ADEX_H

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
  float tau_w;  // Time for relaxation (in ms)
  float a;      // Subthreshold adaptation (nS)
  float b;      // Spike of adaption after voltage spike (nA)
} adex_params;

typedef struct {
  float v;
  float w;
  int in_rest;
} adex_vector;

int adex_new_vector(neu_vector *vec, adex_params params);
int adex_new_neuron(neu_neuron *nr, adex_params params);
int adex_vec_update(const neu_vector *changed, neu_vector *new);

adex_vector *adex_vec(const neu_vector *vec);

int adex_sim_step(const neu_neuron *nr, const neu_vector *vec,
                  neu_vector *new_vec, float i_ext, float euler_dt);

int adex_is_spiked(const neu_neuron *nr, const neu_vector *vec);

#endif
