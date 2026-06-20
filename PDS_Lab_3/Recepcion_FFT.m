% Configuración del puerto serie
clear s;
puerto   = 'COM3';     % Ajustá según corresponda
baudrate = 115200;
Vref = 3.3;

s = serialport(puerto, baudrate, 'Timeout', 5);

hPlot_fft  = [];
hPlot_time = [];
titulo_prev_fft  = '';
titulo_prev_time = '';

SYNC_BYTE  = uint8(15);     % Byte de sincronismo en header[1]

while true
    %% ---- ESPERAR BYTE DE SINCRONISMO (15) ----
    try
        %% ---- SINCRONIZACIÓN ROBUSTA ----
        sync = read(s, 1, 'uint8');
        while sync ~= SYNC_BYTE
            sync = read(s, 1, 'uint8');
        end

    %% ---- LEER RESTO DEL HEADER (fs_mode, window_size, bypass) ----
     header      = read(s, 3, 'uint8');
     fs_mode     = header(1);
     window_size = header(2);
     bypass      = header(3);

    %% ---- Determinar tamaño de ventana según window_size ----
    switch window_size
        case 0
            data_size = 512;
        case 1
            data_size = 1024;
        case 2
            data_size = 2048;
        otherwise
            warning('window_size desconocido: %d', window_size);
            % descarto este frame
            continue;
    end

    %% ---- Mapear fs_mode -> frecuencia de muestreo aproximada ----
    switch fs_mode
        case 0
            fs = 8000;     % ~8 kHz
        case 1
            fs = 16000;    % ~16 kHz
        case 2
            fs = 22000;    % ~22 kHz aprox.
        case 3
            fs = 44000;    % ~44 kHz
        case 4
            fs = 48000;    % ~48 kHz
        otherwise
            fs = NaN;      % Modo stop u otro
    end

    %% =========================
    %   MODO FFT (bypass == 0)
    %% =========================
    if bypass == 0
        % Número de magnitudes enviados por la placa: N = data_size/2 + 1
        num_magnitudes = data_size/2 + 1;

        % ---- LECTURA DE DATOS ----
        magnitudes_q15 = read(s, num_magnitudes, 'int16');

        % ---- FOOTER ----
        footer = read(s, 2, 'uint8');
        if ~isequal(footer, uint8([10 10]))   % 10 = '\n'
            warning('Footer inválido (FFT), posible desalineación. Re-sincronizando...');
            flush(s);
            continue;
        end

        % ---- EJE DE FRECUENCIAS ----
        f = (0:num_magnitudes-1) * (fs / data_size);   % de 0 a fs/2

        % ---- convertir Q15 a dB ----
        mags_lin = double(magnitudes_q15) / 32768;        % [0,1)
        mags_db  = 20*log10(max(mags_lin, 1e-6));         % dB

        % ---- GRÁFICA EN TIEMPO REAL (FFT) ----
        if isempty(hPlot_fft) || ~isvalid(hPlot_fft)
            figure(1);
            hPlot_fft = plot(f, mags_db);
            xlabel('Frecuencia [Hz]');
            ylabel('Magnitud [dB]');
            grid on;
        else
            set(hPlot_fft, 'XData', f, 'YData', mags_db);
        end

        % Título dinámico con info de modo
        titulo_fft = sprintf('FFT %d pts | fs_mode=%d (fs≈%.0f Hz)', ...
                             data_size, fs_mode, fs);
        if ~strcmp(titulo_fft, titulo_prev_fft)
            title(titulo_fft);
            titulo_prev_fft = titulo_fft;
        end

    %% =============================
    %   MODO BYPASS (bypass != 0)
    %% =============================
    else
        % En bypass la placa manda data_size muestras uint16 (ADC crudo)
        num_samples = data_size;

        % ---- LECTURA DE DATOS (uint16) ----
        samples_adc = read(s, num_samples, 'uint16');

        % ---- FOOTER (2 bytes '\n''\n') ----
        footer = read(s, 2, 'uint8');
        if ~isequal(footer, uint8([10 10]))   % 10 = '\n'
            warning('Footer inválido (bypass), posible desalineación. Re-sincronizando...');
            flush(s);
            continue;
        end

        % ---- Convertir a Volts ----
        x_volt = double(samples_adc) * Vref / 4095;   % 12 bits

        % ---- EJE TIEMPO (lo que llamás "período" de la señal) ----
        t = (0:num_samples-1) / fs;   % segundos

        % ---- GRÁFICA EN TIEMPO REAL (señal senoidal) ----
        if isempty(hPlot_time) || ~isvalid(hPlot_time)
            figure(2);
            hPlot_time = plot(t, x_volt);
            xlabel('Tiempo [s]');
            ylabel('Señal [V]');
            grid on;
        else
            set(hPlot_time, 'XData', t, 'YData', x_volt);
        end

        % Título dinámico
        titulo_time = sprintf('Señal cruda (bypass) | %d muestras | fs_mode=%d (fs≈%.0f Hz)', ...
                              data_size, fs_mode, fs);
        if ~strcmp(titulo_time, titulo_prev_time)
            title(titulo_time);
            titulo_prev_time = titulo_time;
        end
    end

    drawnow;

    catch e
        warning('Error: %s — reintentando sync...', e.message);
        flush(s);
    end
end