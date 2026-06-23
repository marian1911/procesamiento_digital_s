#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define L 30
#define MAX_PATH_LEN 512

static const double h[L] = {
    0.120, -0.080, 0.055, 0.040, -0.030,
    0.025, 0.020, -0.018, 0.015, 0.012,
    -0.010, 0.009, 0.008, -0.007, 0.006,
    0.005, -0.004, 0.0035, 0.0030, -0.0025,
    0.0020, 0.0018, -0.0015, 0.0012, 0.0010,
    -0.0008, 0.0006, 0.0005, -0.0004, 0.0003
};

static double random_input(void) {
    return 2.0 * ((double)rand() / (double)RAND_MAX) - 1.0;
}

static double coef_rmse(const double w[L]) {
    double acc = 0.0;

    for (int i = 0; i < L; i++) {
        double diff = h[i] - w[i];
        acc += diff * diff;
    }

    return sqrt(acc / L);
}

static double coef_max_abs_error(const double w[L]) {
    double max_error = 0.0;

    for (int i = 0; i < L; i++) {
        double diff = fabs(h[i] - w[i]);
        if (diff > max_error) {
            max_error = diff;
        }
    }

    return max_error;
}

static void build_coef_output_name(char *dst, size_t dst_size, const char *metrics_file) {
    const char *dot = strrchr(metrics_file, '.');

    if (dot != NULL) {
        size_t prefix_len = (size_t)(dot - metrics_file);
        if (prefix_len >= dst_size) {
            prefix_len = dst_size - 1;
        }

        snprintf(dst, dst_size, "%.*s_coeficientes%s", (int)prefix_len, metrics_file, dot);
    } else {
        snprintf(dst, dst_size, "%s_coeficientes.csv", metrics_file);
    }
}

int main(int argc, char *argv[]) {
    double mu = 0.01;
    int samples = 20000;
    int block = 100;
    int seed = 1;
    double constant_value = 0.5;
    char mode[32] = "random";
    char output_file[MAX_PATH_LEN] = "lms_result.csv";
    char coef_output_file[MAX_PATH_LEN];

    if (argc >= 2) mu = atof(argv[1]);
    if (argc >= 3) snprintf(mode, sizeof(mode), "%s", argv[2]);
    if (argc >= 4) samples = atoi(argv[3]);
    if (argc >= 5) snprintf(output_file, sizeof(output_file), "%s", argv[4]);
    if (argc >= 6) constant_value = atof(argv[5]);
    if (argc >= 7) seed = atoi(argv[6]);
    if (argc >= 8) block = atoi(argv[7]);

    if (mu <= 0.0) {
        printf("Error: mu debe ser mayor que cero.\n");
        return 1;
    }

    if (samples <= 0 || block <= 0) {
        printf("Error: samples y block deben ser mayores que cero.\n");
        return 1;
    }

    srand(seed);

    FILE *fp = fopen(output_file, "w");
    if (!fp) {
        printf("Error: no se pudo abrir el archivo de salida.\n");
        return 1;
    }

    build_coef_output_name(coef_output_file, sizeof(coef_output_file), output_file);

    FILE *coef_fp = fopen(coef_output_file, "w");
    if (!coef_fp) {
        printf("Error: no se pudo abrir el archivo de coeficientes.\n");
        fclose(fp);
        return 1;
    }

    double w[L] = {0.0};
    double x_buffer[L] = {0.0};

    double mse_acc = 0.0;
    int mse_count = 0;

    fprintf(fp, "n,mse,coef_rmse,coef_max_abs_error\n");
    fprintf(coef_fp, "n,i,h,w,h_minus_w\n");

    printf("Simulacion LMS\n");
    printf("mu = %.8f | modo = %s | muestras = %d | bloque MSE = %d\n\n",
           mu, mode, samples, block);
    printf("CSV metricas: %s\n", output_file);
    printf("CSV coeficientes: %s\n\n", coef_output_file);

    for (int n = 1; n <= samples; n++) {
        double x;

        if (strcmp(mode, "constant") == 0 || strcmp(mode, "CONSTANT") == 0) {
            x = constant_value;
        } else {
            x = random_input();
        }

        for (int i = L - 1; i > 0; i--) {
            x_buffer[i] = x_buffer[i - 1];
        }
        x_buffer[0] = x;

        double d = 0.0;
        double y = 0.0;

        for (int i = 0; i < L; i++) {
            d += h[i] * x_buffer[i];
            y += w[i] * x_buffer[i];
        }

        double e = d - y;

        for (int i = 0; i < L; i++) {
            w[i] += mu * e * x_buffer[i];
        }

        mse_acc += e * e;
        mse_count++;

        if (n % block == 0) {
            double mse = mse_acc / mse_count;
            double rms = coef_rmse(w);
            double max_err = coef_max_abs_error(w);

            fprintf(fp, "%d,%.15e,%.15e,%.15e\n", n, mse, rms, max_err);

            printf("n=%6d | MSE=% .8e | RMS(h-w)=% .8e | max|h-w|=% .8e\n",
                   n, mse, rms, max_err);

            for (int i = 0; i < L; i++) {
                fprintf(coef_fp, "%d,%d,%.15e,%.15e,%.15e\n",
                        n, i, h[i], w[i], h[i] - w[i]);
            }

            mse_acc = 0.0;
            mse_count = 0;
        }
    }

    printf("\nCoeficientes finales:\n");
    printf(" i         h[i]             w[i]          h[i]-w[i]\n");

    for (int i = 0; i < L; i++) {
        printf("%2d  % .10e  % .10e  % .10e\n",
               i, h[i], w[i], h[i] - w[i]);
    }

    fclose(fp);
    fclose(coef_fp);
    return 0;
}
