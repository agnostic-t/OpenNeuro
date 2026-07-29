/*
 * Illustrates Hodjkin-Haxly model
 * on single neuron
 */

#define SIM_STEPS 2000 // 2000 * 0.1 (dt) = 200ms of simulation
#define HH_BIO_SHIFT 65.f

#include <math.h>
#include <stdio.h>
#include "gnuplot/gnuplot_i.h"

float alpha_m(float v) { return 0.1f * (v + 40.0f) / (1.0f - expf(-(v + 40.0f) / 10.0f)); }
float beta_m(float v)  { return 4.0f * expf(-(v + HH_BIO_SHIFT) / 18.0f); }

float alpha_h(float v) { return 0.07f * expf(-(v + HH_BIO_SHIFT) / 20.0f); }
float beta_h(float v)  { return 1.0f / (1.0f + expf(-(v + 35.0f) / 10.0f)); }

float alpha_n(float v) { return 0.01f * (v + 55.0f) / (1.0f - expf(-(v + 55.0f) / 10.0f)); }
float beta_n(float v)  { return 0.125f * expf(-(v + HH_BIO_SHIFT) / 80.0f); }

float HH_dv_dt(float v, float m, float h, float n, float i_ext,
               float c_m, float g_Na, float g_K, float g_L,
               float e_Na, float e_K, float e_L) {
    float i_Na = g_Na * m * m * m * h * (v - e_Na);
    float i_K  = g_K * n * n * n * n * (v - e_K);
    float i_L  = g_L * (v - e_L);
    return (i_ext - i_Na - i_K - i_L) / c_m;
}

float HH_dm_dt(float m, float v) { return alpha_m(v) * (1.0f - m) - beta_m(v) * m; }
float HH_dh_dt(float h, float v) { return alpha_h(v) * (1.0f - h) - beta_h(v) * h; }
float HH_dn_dt(float n, float v) { return alpha_n(v) * (1.0f - n) - beta_n(v) * n; }

void plot(double x[], double y[], size_t dots);

int main() {
    float euler_dt = 0.01f;

    // Constants of biophysical environment
    float c_m = 1.0f;
    float g_Na = 120.0f;
    float g_K = 36.0f;
    float g_L = 0.3f;

    // Stable potentials (unique to unique types of neurons)
    float e_Na = 50.0f;
    float e_K = -77.0f;
    float e_L = -54.4f;

    // Start state (rest)
    float v = -65.0f;
    float m = 0.05f;
    float h = 0.6f;
    float n = 0.32f;

    float i_ext = 10.0f;

    double x[SIM_STEPS], y[SIM_STEPS];

    for (int i = 0; i < SIM_STEPS; i++) {
        float dv_dt = HH_dv_dt(v, m, h, n, i_ext, c_m, g_Na, g_K, g_L, e_Na, e_K, e_L);
        float dm_dt = HH_dm_dt(m, v);
        float dh_dt = HH_dh_dt(h, v);
        float dn_dt = HH_dn_dt(n, v);

        v += dv_dt * euler_dt;
        m += dm_dt * euler_dt;
        h += dh_dt * euler_dt;
        n += dn_dt * euler_dt;

        y[i] = (double)v;
        x[i] = i * euler_dt;
    }

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
