#ifndef OPENNEURO_MODELS_ABSTRACT_H
#define OPENNEURO_MODELS_ABSTRACT_H

typedef struct {
  void *state;
} neu_vector;

typedef struct {
  void *params;
} neu_biophysp;

typedef struct {
  void *sparams;
} neu_neuron;

void neu_vector_destroy(neu_vector *vec);
void neu_biophysp_destroy(neu_biophysp *params);
void neu_neuron_destroy(neu_neuron *nr);

#endif
