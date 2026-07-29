/*
 * Illustrates Exponential Integrate-and-Fire algorithm
 * on single neuron
 */

#include <math.h>
#define SIM_STEPS 1000 // 1000 * 0.1 (dt) = 100ms of simulation

#include <stdio.h>
#include "gnuplot/gnuplot_i.h"

float EIF_dv_dt(
    float v, float i_ext,
    float c_m, float g_L, float dt_T, float e_L, float v_t
) {
    return (-g_L * (v - e_L) + g_L * dt_T * exp((v - v_t) / dt_T) + i_ext) / c_m;
}

void plot(double x[], double y[], size_t dots);
int main() {
    float euler_dt = 0.1;  // Step of the Euler's ODE method (ms)

    float c_m = 200.f;     // Membranes capacity (in pF)
    float g_L = 10.f;      // How well voltage leaks (in nS)
    float dt_T = 2.f;      // Steepness of the spike (in mV)
    float e_L = -70.f;     // Voltage level in neuron without stimules (in mV)
    float v_t = -50.f;     // Threshold for the spike (in mV)
    float v_rest = -58.f;  // Voltage after spike (in mV)
    float v_peak = 20.f;   // Voltage at spike moment (in mV)

    float v = e_L;
    float i_ext = 200.0f;   // Input voltage (nA), at least 200 nA

    double x[SIM_STEPS], y[SIM_STEPS];
    for (int i = 0; i < SIM_STEPS; i++){
        float dv_dt = EIF_dv_dt(v, i_ext, c_m, g_L, dt_T, e_L, v_t);
        v += dv_dt * euler_dt;

        if (v >= v_t) {
            y[i] = v_peak;
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
