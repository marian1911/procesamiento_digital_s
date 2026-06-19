"""
Serial plotter - Equivalente Python del script MATLAB.
Requiere: pip install pyserial matplotlib numpy
"""

import serial
import serial.tools.list_ports
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import warnings
import sys

# ── Configuración ──────────────────────────────────────────────────────────────
PUERTO    = 'COM7'      # Ajustá según corresponda (Linux: '/dev/ttyUSB0', etc.)
BAUDRATE  = 115200
TIMEOUT   = 5
VREF      = 5.0
SYNC_BYTE = 15          # Byte de sincronismo en header[1]

# ── Modos de protocolo ─────────────────────────────────────────────────────────
PROTOCOL_MODE = "LEGACY"  # Cambia a "DEBUG" para ver datos sin procesar

# ── Mapas de configuración ─────────────────────────────────────────────────────
WINDOW_SIZE_MAP = {
    0: 512,
    1: 1024,
    2: 2048,
}

FS_MAP = {
    0: 8_000,
    1: 16_000,
    2: 22_000,
    3: 44_000,
    4: 48_000,
}


def leer_byte(ser: serial.Serial) -> int:
    """Lee un único byte como entero sin signo."""
    return int.from_bytes(ser.read(1), byteorder='little', signed=False)


def leer_bytes(ser: serial.Serial, n: int) -> bytes:
    return ser.read(n)


def sincronizar(ser: serial.Serial) -> None:
    """Espera hasta encontrar el byte de sincronismo. Ignora bytes nulos iniciales."""
    # Descartar bytes nulos al inicio (ruido)
    byte = leer_byte(ser)
    while byte == 0:
        byte = leer_byte(ser)
    
    # Buscar el byte de sincronismo o el primer byte no nulo
    while byte != SYNC_BYTE:
        # Si encontramos un byte que parece ser parte de un header válido, volver atrás
        if byte != 0:
            # Asumir que este es el primer byte válido
            break
        byte = leer_byte(ser)


def leer_header(ser: serial.Serial) -> tuple[int, int, int]:
    """Lee los 3 bytes restantes del header: fs_mode, window_size, bypass."""
    data = ser.read(3)
    fs_mode, window_size, bypass = data[0], data[1], data[2]
    return fs_mode, window_size, bypass


def leer_footer(ser: serial.Serial) -> bool:
    """Lee 2 bytes de footer y verifica que sean [10, 10]."""
    footer = ser.read(2)
    return footer == bytes([10, 10])


def leer_int16(ser: serial.Serial, n: int) -> np.ndarray:
    """Lee n valores int16 (little-endian)."""
    raw = ser.read(n * 2)
    return np.frombuffer(raw, dtype='<i2')


def leer_uint16(ser: serial.Serial, n: int) -> np.ndarray:
    """Lee n valores uint16 (little-endian)."""
    raw = ser.read(n * 2)
    return np.frombuffer(raw, dtype='<u2')


def listar_puertos_disponibles():
    """Lista todos los puertos seriales disponibles."""
    puertos = serial.tools.list_ports.comports()
    if not puertos:
        print("⚠️  No hay puertos seriales disponibles.")
        return []
    
    print("Puertos seriales disponibles:")
    for puerto in puertos:
        print(f"  - {puerto.device}: {puerto.description}")
    return [p.device for p in puertos]


# ── Figuras ────────────────────────────────────────────────────────────────────
fig_fft,  ax_fft  = None, None
fig_time, ax_time = None, None
line_fft  = None
line_time = None
titulo_prev_fft  = ''
titulo_prev_time = ''


def init_fig_fft():
    global fig_fft, ax_fft, line_fft
    fig_fft, ax_fft = plt.subplots(num='FFT')
    line_fft, = ax_fft.plot([], [])
    ax_fft.set_xlabel('Frecuencia [Hz]')
    ax_fft.set_ylabel('Magnitud [dB]')
    ax_fft.grid(True)
    fig_fft.show()


def init_fig_time():
    global fig_time, ax_time, line_time
    fig_time, ax_time = plt.subplots(num='Señal cruda')
    line_time, = ax_time.plot([], [])
    ax_time.set_xlabel('Tiempo [s]')
    ax_time.set_ylabel('Señal [V]')
    ax_time.grid(True)
    fig_time.show()


def actualizar_fft(f: np.ndarray, mags_db: np.ndarray,
                   data_size: int, fs_mode: int, fs: float) -> None:
    global titulo_prev_fft, line_fft

    if fig_fft is None or not plt.fignum_exists(fig_fft.number):
        init_fig_fft()

    line_fft.set_data(f, mags_db)
    ax_fft.relim()
    ax_fft.autoscale_view()

    titulo = f'FFT {data_size} pts | fs_mode={fs_mode} (fs≈{fs:.0f} Hz)'
    if titulo != titulo_prev_fft:
        ax_fft.set_title(titulo)
        titulo_prev_fft = titulo

    fig_fft.canvas.draw()
    fig_fft.canvas.flush_events()


def actualizar_time(t: np.ndarray, x_volt: np.ndarray,
                    data_size: int, fs_mode: int, fs: float) -> None:
    global titulo_prev_time, line_time

    if fig_time is None or not plt.fignum_exists(fig_time.number):
        init_fig_time()

    line_time.set_data(t, x_volt)
    ax_time.relim()
    ax_time.autoscale_view()

    titulo = (f'Señal cruda (bypass) | {data_size} muestras '
              f'| fs_mode={fs_mode} (fs≈{fs:.0f} Hz)')
    if titulo != titulo_prev_time:
        ax_time.set_title(titulo)
        titulo_prev_time = titulo

    fig_time.canvas.draw()
    fig_time.canvas.flush_events()


# ── Loop principal ─────────────────────────────────────────────────────────────
def main():
    plt.ion()   # modo interactivo
    
    # ── Seleccionar puerto ─────────────────────────────────────────────────────
    puerto = PUERTO
    if len(sys.argv) > 1:
        puerto = sys.argv[1]
    
    print(f"\n{'='*60}")
    print(f"Serial Plotter - FFT Visualization")
    print(f"{'='*60}")
    print(f"Configuración: {BAUDRATE} bps, timeout {TIMEOUT}s")
    print(f"Intento de conexión al puerto: {puerto}\n")
    
    # ── Listar puertos disponibles ──────────────────────────────────────────────
    puertos_disponibles = listar_puertos_disponibles()
    
    if puerto not in puertos_disponibles and puertos_disponibles:
        print(f"\n⚠️  El puerto '{puerto}' no está disponible.")
        print(f"Usa: python serial_plotter.py <puerto>")
        print(f"Ejemplo: python serial_plotter.py {puertos_disponibles[0]}")
        sys.exit(1)
    
    # ── Conectar al puerto ─────────────────────────────────────────────────────
    try:
        ser = serial.Serial(puerto, BAUDRATE, timeout=TIMEOUT)
        print(f"✓ Conectado a {puerto} a {BAUDRATE} bps\n")
    except serial.SerialException as e:
        print(f"✗ Error al abrir {puerto}: {e}")
        print(f"\nPuertos disponibles: {puertos_disponibles if puertos_disponibles else 'Ninguno'}")
        sys.exit(1)
    
    contador_frames = 0
    
    with ser:
        while True:
            try:
                # ── Sincronización ─────────────────────────────────────────
                sincronizar(ser)

                # ── Header ─────────────────────────────────────────────────
                fs_mode, window_size, bypass = leer_header(ser)

                # ── Tamaño de ventana ───────────────────────────────────────
                data_size = WINDOW_SIZE_MAP.get(window_size)
                if data_size is None:
                    warnings.warn(f'window_size desconocido: {window_size}')
                    continue

                # ── Frecuencia de muestreo ──────────────────────────────────
                fs = FS_MAP.get(fs_mode, float('nan'))
                
                contador_frames += 1

                # ══════════════════════════════════════════════════════════
                #   MODO FFT (bypass == 0)
                # ══════════════════════════════════════════════════════════
                if bypass == 0:
                    num_magnitudes = data_size // 2 + 1

                    magnitudes_q15 = leer_int16(ser, num_magnitudes)

                    if not leer_footer(ser):
                        warnings.warn('Footer inválido (FFT). Re-sincronizando...')
                        ser.reset_input_buffer()
                        continue

                    f = np.arange(num_magnitudes) * (fs / data_size)   # 0…fs/2

                    mags_lin = magnitudes_q15.astype(np.float64) / 32768.0
                    mags_db  = 20 * np.log10(np.maximum(mags_lin, 1e-6))

                    actualizar_fft(f, mags_db, data_size, fs_mode, fs)
                    print(f"[Frame {contador_frames}] FFT: {data_size} pts, fs={fs:.0f} Hz, max_mag={np.max(mags_db):.1f} dB")

                # ══════════════════════════════════════════════════════════
                #   MODO BYPASS (bypass != 0)
                # ══════════════════════════════════════════════════════════
                else:
                    samples_adc = leer_uint16(ser, data_size)

                    if not leer_footer(ser):
                        warnings.warn('Footer inválido (bypass). Re-sincronizando...')
                        ser.reset_input_buffer()
                        continue

                    x_volt = samples_adc.astype(np.float64) * VREF / 4095.0
                    t      = np.arange(data_size) / fs

                    actualizar_time(t, x_volt, data_size, fs_mode, fs)
                    print(f"[Frame {contador_frames}] BYPASS: {data_size} muestras, fs={fs:.0f} Hz, V_min={np.min(x_volt):.2f}V, V_max={np.max(x_volt):.2f}V")

            except serial.SerialTimeoutException:
                print(f"⚠️  Timeout serial (frame {contador_frames}) — reintentando sync...")
                ser.reset_input_buffer()
            except serial.SerialException as e:
                print(f"✗ Error serial: {e}")
                break
            except Exception as e:
                print(f"✗ Error: {e}")
                ser.reset_input_buffer()


if __name__ == '__main__':
    try:
        main()
    except KeyboardInterrupt:
        print("\n\n✓ Programa interrumpido por el usuario.")
        sys.exit(0)
