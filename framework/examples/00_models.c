#include "gnuplot/gnuplot_i.h"

#include "openneu/abstract.h"
#include "openneu/watcher.h"

#include "openneu/lif.h"
#include "openneu/eif.h"
#include "openneu/adex.h"
#include "openneu/izhikevich.h"
#include "openneu/hodjkin_haxly.h"

#define SIM_STEPS 1000

void plot(double x[], double y[], size_t dots);

int lif_test() {
    lif_params params = {
        .tau_m = 20.f,
        .resist_m = 10.f,
        .v_rest = -70.f,
        .v_threshold = -55.f
    };

    neu_vector vec, nvec;
    neu_neuron nr;

    lif_new_vector(&vec);
    lif_new_vector(&nvec);
    lif_new_neuron(&nr, params);

    double x[SIM_STEPS], y[SIM_STEPS];
    for (int i = 0; i < SIM_STEPS; i++) {
        lif_vec_update(&nvec, &vec);
        lif_sim_step(&nr, &vec, &nvec, 2.f, 0.1);

        float v = 0;
        if (lif_is_spiked(&nr, &nvec))
            v = 20;
        else
            v = lif_vec(&nvec)->v;

        x[i] = i * 0.1;
        y[i] = (double)v;
    }

    neu_vector_destroy(&vec);
    neu_vector_destroy(&nvec);
    neu_neuron_destroy(&nr);

    plot(x, y, SIM_STEPS);
    return 0;
}

int eif_test() {
    eif_params params = {
        .c_m = 200.f,
        .g_L = 10.f,
        .dt_T = 2.f,
        .e_L = -70.f,
        .v_t = -50.f,
        .v_rest = -58.f,
        .v_peak = 20.f,
        .tau_r = 2
    };

    neu_vector vec, nvec;
    neu_neuron nr;

    eif_new_vector(&vec, params);
    eif_new_vector(&nvec, params);
    eif_new_neuron(&nr, params);

    double x[SIM_STEPS], y[SIM_STEPS];
    for (int i = 0; i < SIM_STEPS; i++) {
        eif_vec_update(&nvec, &vec);
        eif_sim_step(&nr, &vec, &nvec, 200.f, 0.1);

        float v = 0;
        if (eif_is_spiked(&nr, &nvec))
            v = 20;
        else
            v = eif_vec(&nvec)->v;

        x[i] = i * 0.1;
        y[i] = (double)v;
    }

    neu_vector_destroy(&vec);
    neu_vector_destroy(&nvec);
    neu_neuron_destroy(&nr);

    plot(x, y, SIM_STEPS);

    return 0;
}

int adex_test() {
    adex_params params = {
        .c_m = 200.f,
        .g_L = 10.f,
        .dt_T = 2.f,
        .e_L = -70.f,
        .v_t = -50.f,
        .v_rest = -58.f,
        .v_peak = 20.f,
        .tau_r = 2,
        .tau_w = 144.f,
        .a = 4.f,
        .b = 80.f,
    };

    neu_vector vec, nvec;
    neu_neuron nr;

    adex_new_vector(&vec, params);
    adex_new_vector(&nvec, params);
    adex_new_neuron(&nr, params);

    double x[SIM_STEPS], y[SIM_STEPS];
    for (int i = 0; i < SIM_STEPS; i++) {
        adex_vec_update(&nvec, &vec);
        adex_sim_step(&nr, &vec, &nvec, 500.f, 0.1);

        float val = 0;
        if (adex_is_spiked(&nr, &nvec))
            val = params.v_peak;
        else
            val = adex_vec(&nvec)->v;

        x[i] = i * 0.1;
        y[i] = (double)val;
    }

    neu_vector_destroy(&vec);
    neu_vector_destroy(&nvec);
    neu_neuron_destroy(&nr);

    plot(x, y, SIM_STEPS);

    return 0;
}

int izhikevich_test() {
    izh_params params = {
        .a = 0.02, .b = 0.2, .c = -65, .d = 8
    };

    neu_vector vec, nvec;
    neu_neuron nr;

    izh_new_vector(&vec, params);
    izh_new_vector(&nvec, params);
    izh_new_neuron(&nr, params);

    double x[SIM_STEPS], y[SIM_STEPS];
    for (int i = 0; i < SIM_STEPS; i++) {
        izh_vec_update(&nvec, &vec);
        izh_sim_step(&nr, &vec, &nvec, 10.f, 0.1);

        float v = 0;
        if (izh_is_spiked(&nr, &nvec))
            v = IZHIKEVICH_TRESHOLD;
        else
            v = izh_vec(&nvec)->v;

        x[i] = i * 0.1;
        y[i] = (double)v;
    }

    neu_vector_destroy(&vec);
    neu_vector_destroy(&nvec);
    neu_neuron_destroy(&nr);

    plot(x, y, SIM_STEPS);
    return 0;
}

int hodjkin_haxly_test() {
    hh_biohys bparams = {
        .c_m = 1.0f,
        .g_Na = 120.0f,
        .g_K = 36.0f,
        .g_L = 0.3f,
    };

    hh_params params = {
        .e_Na = 50.0f,
        .e_K = -77.0f,
        .e_L = -54.4f,
    };

    neu_vector vec, nvec;
    neu_neuron nr;
    neu_biophysp bp;

    hh_new_biophys(&bp, bparams);
    hh_new_vector(&vec);
    hh_new_vector(&nvec);
    hh_new_neuron(&nr, params);

    double x[SIM_STEPS], y[SIM_STEPS];
    for (int i = 0; i < SIM_STEPS; i++) {
        hh_vec_update(&nvec, &vec);
        hh_sim_step(&bp, &nr, &vec, &nvec, 10.f, 0.01);

        float v = hh_vec(&nvec)->v;

        x[i] = i * 0.01;
        y[i] = (double)v;
    }

    neu_vector_destroy(&vec);
    neu_vector_destroy(&nvec);
    neu_neuron_destroy(&nr);
    neu_biophysp_destroy(&bp);

    plot(x, y, SIM_STEPS);
    return 0;
}

int main() {
    neu_check(lif_test());
    neu_check(eif_test());
    neu_check(adex_test());
    neu_check(izhikevich_test());
    neu_check(hodjkin_haxly_test());
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
