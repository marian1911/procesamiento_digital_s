import csv
import os
import subprocess
import sys
from pathlib import Path
import matplotlib.pyplot as plt

BASE_DIR = Path(__file__).resolve().parent
C_FILE = BASE_DIR / "lms_planta.c"
EXE_FILE = BASE_DIR / "lms_planta.exe"
RESULTS_DIR = BASE_DIR / "resultados_lms"

MUS = [0.001, 0.003, 0.01, 0.03, 0.1]
SAMPLES = 20000
BLOCK = 100
CONSTANT_VALUE = 0.5
SEED = 7

MODES = ["random", "constant"]


def compile_c():
    cmd = ["gcc", C_FILE.name, "-o", EXE_FILE.name, "-lm"]
    print("Compilando C:")
    print(" ".join(cmd))
    subprocess.run(cmd, check=True, cwd=BASE_DIR)


def run_simulation(mu, mode):
    RESULTS_DIR.mkdir(exist_ok=True)

    csv_file = RESULTS_DIR / f"lms_{mode}_mu_{mu}.csv"
    csv_file_arg = Path("resultados_lms") / csv_file.name

    cmd = [
        f".\\{EXE_FILE.name}",
        str(mu),
        mode,
        str(SAMPLES),
        str(csv_file_arg),
        str(CONSTANT_VALUE),
        str(SEED),
        str(BLOCK),
    ]

    print("\nEjecutando:")
    print(" ".join(cmd))

    subprocess.run(cmd, check=True, cwd=BASE_DIR)

    return csv_file


def read_csv(path):
    n = []
    mse = []
    coef_rmse = []

    with open(path, "r", newline="") as f:
        reader = csv.DictReader(f)

        for row in reader:
            n.append(int(row["n"]))
            mse.append(float(row["mse"]))
            coef_rmse.append(float(row["coef_rmse"]))

    return n, mse, coef_rmse


def plot_mse(results, mode):
    plt.figure(figsize=(10, 7))

    for mu, csv_file in results:
        n, mse, _ = read_csv(csv_file)
        plt.semilogy(n, mse, label=f"mu={mu}")

    title_mode = f"CONSTANT (x={CONSTANT_VALUE})" if mode == "constant" else "RANDOM"

    plt.title(f"Convergencia LMS - MSE | modo={title_mode}", fontsize=18)
    plt.xlabel("n (muestras)", fontsize=14)
    plt.ylabel(f"MSE (promedio cada {BLOCK})", fontsize=14)
    plt.grid(True, which="both")
    plt.legend(fontsize=12)
    plt.tight_layout()

    out = RESULTS_DIR / f"convergencia_mse_{mode}.png"
    plt.savefig(out, dpi=150)
    print(f"Grafica guardada: {out}")


def plot_coef_error(results, mode):
    plt.figure(figsize=(10, 7))

    for mu, csv_file in results:
        n, _, coef_rmse = read_csv(csv_file)
        plt.semilogy(n, coef_rmse, label=f"mu={mu}")

    title_mode = f"CONSTANT (x={CONSTANT_VALUE})" if mode == "constant" else "RANDOM"

    plt.title(f"Convergencia LMS - Error de coeficientes | modo={title_mode}", fontsize=18)
    plt.xlabel("n (muestras)", fontsize=14)
    plt.ylabel("RMS(h - w)", fontsize=14)
    plt.grid(True, which="both")
    plt.legend(fontsize=12)
    plt.tight_layout()

    out = RESULTS_DIR / f"convergencia_coeficientes_{mode}.png"
    plt.savefig(out, dpi=150)
    print(f"Grafica guardada: {out}")


def main():
    compile_c()

    for mode in MODES:
        results = []

        for mu in MUS:
            csv_file = run_simulation(mu, mode)
            results.append((mu, csv_file))

        plot_mse(results, mode)
        plot_coef_error(results, mode)

    plt.show()


if __name__ == "__main__":
    try:
        main()
    except FileNotFoundError as e:
        print("\nError: falta gcc o no se encontro algun archivo.")
        print(e)
        sys.exit(1)
    except subprocess.CalledProcessError as e:
        print("\nError: fallo la compilacion o la ejecucion del programa en C.")
        print(e)
        sys.exit(1)
