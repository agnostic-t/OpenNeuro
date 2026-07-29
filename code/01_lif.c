/*
 * Illustrates Leaky Integrate-and-Fire algorithm
 * on single neuron
 */

#define SIM_STEPS 1000 // 1000 * 0.1 (dt) = 100ms of simulation

#include <stdio.h>
#include "gnuplot/gnuplot_i.h"

float LIF_dv_dt(
    float v, float i_ext,
    float tau_m, float v_rest, float r_m
) {
    return (-(v - v_rest) + r_m * i_ext) / tau_m;
}

void plot(double x[], double y[], size_t dots);
int main() {
    float euler_dt = 0.1;      // Step of the Euler's ODE method (ms)

    float tau_m = 20.f;        // Neuron's time constant (ms)
    float resist_m = 10.f;     // Membrane resistance (in MOm)
    float v_rest = -70;        // Rest voltage (in mV)
    float v_threshold = -55.f; // Spike's threshold (in mV)

    float v = v_rest;     // Voltage at start
    float i_ext = 2.0f;   // Input voltage (nA), at least 1.5 nA

    double x[SIM_STEPS], y[SIM_STEPS];
    for (int i = 0; i < SIM_STEPS; i++){
        float dv_dt = LIF_dv_dt(v, i_ext, tau_m, v_rest, resist_m);
        v += dv_dt * euler_dt;

        if (v >= v_threshold) {
            y[i] = 20.f;
            v = v_rest;
        } else {
            y[i] = (double)v;
        }

        x[i] = i * euler_dt;
    }

    // x - time in ms
    // y - voltage in mV
    plot(x, y, SIM_STEPS);

    return 0;
}


void plot(double x[], double y[], size_t dots){
    gnuplot_ctrl *h = gnuplot_init();

    gnuplot_setstyle(h, "lines");
    gnuplot_set_xlabel(h, "X axis");
    gnuplot_set_ylabel(h, "Y axis");

    gnuplot_plot_xy(h, x, y, dots, "Data");

    printf("Press Enter to exit...\n");
    getchar();

    gnuplot_close(h);
}
