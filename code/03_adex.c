/*
 * Illustrates Adaptive Exponential Integrate-and-Fire algorithm
 * on single neuron
 */

#define SIM_STEPS 1000 // 1000 * 0.1 (dt) = 100ms of simulation

#include <math.h>
#include <stdio.h>

#include "gnuplot/gnuplot_i.h"

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
    float tau_r = 2;       // Resting time after spike (in ms)

    float tau_w = 144.f;   // Time for relaxation (in ms)
    float a = 4.f;         // Subthreshold adaptation (nS)
    float b = 80.f;        // Spike of adaption after voltage spike (nA)

    float w = 0.f;         // Adaptaion current (nA)
    float v = e_L;         // Voltage (мВ)
    float i_ext = 500.0f;  // External current, for spikes higher than in EIF (nA)

    int in_rest = -1;
    double x[SIM_STEPS], y[SIM_STEPS];
    for (int i = 0; i < SIM_STEPS; i++){
        float dw_dt = AdEx_dw_dt(
            w, v,
            tau_w, e_L, a
        );

        w += dw_dt * euler_dt;

        if (in_rest > 0) {
            in_rest --;

            x[i] = i * euler_dt;
            y[i] = v_rest;
            continue;
        }

        float dv_dt = AdEx_dv_dt(
            v, i_ext, w,
            c_m, g_L, dt_T, e_L, v_t
        );

        v += dv_dt * euler_dt;

        if (v >= v_peak) {
            y[i] = v_peak;
            v = v_rest;
            w += b;

            in_rest = (int)(tau_r / euler_dt);
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
