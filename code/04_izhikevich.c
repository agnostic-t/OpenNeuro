/*
 * Illustrates Izhekevich algorithm
 * on single neuron
 */

#define SIM_STEPS 1000 // 1000 * 0.1 (dt) = 100ms of simulation

#include <stdio.h>
#include "gnuplot/gnuplot_i.h"

float Izhekevich_dv_dt(
    float v, float u, float I
) {
    return 0.04 * v * v + 5 * v + 140 - u + I;
}

float Izhekevich_du_dt(
    float v, float u, float a, float b
) {
    return a * (b * v - u);
}

void plot(double x[], double y[], size_t dots);
int main() {
    float euler_dt = 0.1;          // Step of the Euler's ODE method (ms)

    const float v_treshold = 30.f; // Specific for Izhekevich model

    // Setup for RS
    float a = 0.02;  // Speed of `u` going down
    float b = 0.2;   // How sensitive `u` is to subthreshold values
    float c = -65;   // Voltage after spike
    float d = 8;     // Spike of adaptation

    float v = c;
    float u = b * v;

    float i_ext = 10.;   // Input voltage (abstract)

    double x[SIM_STEPS], y[SIM_STEPS];
    for (int i = 0; i < SIM_STEPS; i++){
        float dv_dt = Izhekevich_dv_dt(v, u, i_ext);
        v += dv_dt * euler_dt;

        float du_dt = Izhekevich_du_dt(v, u, a, b);
        u += du_dt * euler_dt;

        if (v >= v_treshold) {
            y[i] = v_treshold;

            v = c;
            u += d;
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
