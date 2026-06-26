close all; clear all; clc;
% -------------------------------------------------------------------------
%% --------------- CARACTERISTICAS DE LOS FILTROS -------------------------

Fs = [8000, 16000, 22000, 44000, 48000]; % Frecuencias de muestreo en Hz


%% ----------------- DISEÑO FILTRO PASA BAJO ------------------------------
% -
% Especificaciones del TP: Pasa Bajos: Fc= 3600Hz Astop=30db
% -
FPB_Fc = 3600;
FPB_Ast = 30;
FPB_Ap = 0.5;
disp('FILTROS PASA BAJOS');

% Filtro Pasa Bajos para 8 KHz:
N_FPB_8K = 79; % 29;
FPB_8K_Specs = fdesign.lowpass('N,Fc,Ap,Ast', N_FPB_8K, FPB_Fc, FPB_Ap, FPB_Ast, Fs(1));
FPB_8K = design(FPB_8K_Specs, 'equiripple');
Fc_corr = fcParaQueF3dBSea(FPB_8K.Numerator, Fs(1), FPB_Fc);
FPB_8K_spec_corr = fdesign.lowpass('N,Fc,Ap,Ast', N_FPB_8K, Fc_corr, FPB_Ap, FPB_Ast, Fs(1));
FPB_8K = design(FPB_8K_spec_corr,'equiripple');
disp('Filtro PB 8K: ');
revisarVectorExtremos(FPB_8K.Numerator);

% Filtro Pasa Bajos para 16 KHz:
N_FPB_16K = 79; % 39;
FPB_16K_Specs = fdesign.lowpass('N,Fc,Ap,Ast', N_FPB_16K, FPB_Fc, FPB_Ap, FPB_Ast, Fs(2));
FPB_16K = design(FPB_16K_Specs, 'equiripple');
Fc_corr = fcParaQueF3dBSea(FPB_16K.Numerator, Fs(2), FPB_Fc);
FPB_16K_spec_corr = fdesign.lowpass('N,Fc,Ap,Ast', N_FPB_16K, Fc_corr, FPB_Ap, FPB_Ast, Fs(2));
FPB_16K = design(FPB_16K_spec_corr,'equiripple');
disp('Filtro PB 16K: ');
revisarVectorExtremos(FPB_16K.Numerator);

% Filtro Pasa Bajos para 22 KHz:
N_FPB_22K = 79; % 49;
FPB_22K_Specs = fdesign.lowpass('N,Fc,Ap,Ast', N_FPB_22K, FPB_Fc, FPB_Ap, FPB_Ast, Fs(3));
FPB_22K = design(FPB_22K_Specs, 'equiripple');
Fc_corr = fcParaQueF3dBSea(FPB_22K.Numerator, Fs(3), FPB_Fc);
FPB_22K_spec_corr = fdesign.lowpass('N,Fc,Ap,Ast', N_FPB_22K, Fc_corr, FPB_Ap, FPB_Ast, Fs(3));
FPB_22K = design(FPB_22K_spec_corr,'equiripple');
disp('Filtro PB 22K: ');
revisarVectorExtremos(FPB_22K.Numerator);

% Filtro Pasa Bajos para 44 KHz:
N_FPB_44K = 79; % 79;
FPB_44K_Specs = fdesign.lowpass('N,Fc,Ap,Ast', N_FPB_44K, FPB_Fc, FPB_Ap, FPB_Ast, Fs(4));
FPB_44K = design(FPB_44K_Specs, 'equiripple');
Fc_corr = fcParaQueF3dBSea(FPB_44K.Numerator, Fs(4), FPB_Fc);
FPB_44K_spec_corr = fdesign.lowpass('N,Fc,Ap,Ast', N_FPB_44K, Fc_corr+30, FPB_Ap, FPB_Ast, Fs(4));
FPB_44K = design(FPB_44K_spec_corr,'equiripple');
disp('Filtro PB 44K: ');
revisarVectorExtremos(FPB_44K.Numerator);

% Filtro Pasa Bajos 48 KHz:
N_FPB_48K = 79; % 89;
FPB_48K_Specs = fdesign.lowpass('N,Fc,Ap,Ast', N_FPB_48K, FPB_Fc, FPB_Ap, FPB_Ast, Fs(5));
FPB_48K = design(FPB_48K_Specs, 'equiripple');
Fc_corr = fcParaQueF3dBSea(FPB_48K.Numerator, Fs(5), FPB_Fc);
FPB_48K_spec_corr = fdesign.lowpass('N,Fc,Ap,Ast', N_FPB_48K, Fc_corr, FPB_Ap, FPB_Ast, Fs(5));
FPB_48K = design(FPB_48K_spec_corr,'equiripple');
disp('Filtro PB 48K: ');
revisarVectorExtremos(FPB_48K.Numerator);



%% ------------------- DISEÑO FILTRO PASA ALTO ----------------------------
% -
% Especificaciones del TP: Pasa Altos: Fc=400 Hz Astop=30db
% -
FPA_Fc  = 400;
FPA_Ast = 30;
FPA_Ap  = 1.5;
disp('   ');
disp('FILTROS PASA ALTOS');

% Filtro Pasa Altos 8 KHz:
N_FPA_8K = 79;
FPA_8K_Specs = fdesign.highpass('N,Fc,Ast,Ap', N_FPA_8K, FPA_Fc, FPA_Ast, FPA_Ap, Fs(1));
FPA_8K = design(FPA_8K_Specs, 'fircls');
Fc_corr = fcParaQueF3dBSea_HP(FPA_8K.Numerator, Fs(1), FPA_Fc);
FPA_8K_spec_corr = fdesign.highpass('N,Fc,Ast,Ap', N_FPA_8K, Fc_corr, FPA_Ast, FPA_Ap, Fs(1));
FPA_8K = design(FPA_8K_spec_corr, 'fircls');
disp('Filtro PA 8K: ');
revisarVectorExtremos(FPA_8K.Numerator);

% Filtro Pasa Altos 16 KHz:
N_FPA_16K = 79;
FPA_16K_Specs = fdesign.highpass('N,Fc,Ast,Ap', N_FPA_16K, FPA_Fc, FPA_Ast, FPA_Ap, Fs(2));
FPA_16K = design(FPA_16K_Specs, 'fircls');
Fc_corr = fcParaQueF3dBSea_HP(FPA_16K.Numerator, Fs(2), FPA_Fc);
FPA_16K_spec_corr = fdesign.highpass('N,Fc,Ast,Ap', N_FPA_16K, Fc_corr, FPA_Ast, FPA_Ap, Fs(2));
FPA_16K = design(FPA_16K_spec_corr, 'fircls');
disp('Filtro PA 16K: ');
revisarVectorExtremos(FPA_16K.Numerator);

% Filtro Pasa Altos 22 KHz:
N_FPA_22K = 79;
FPA_22K_Specs = fdesign.highpass('N,Fc,Ast,Ap', N_FPA_22K, FPA_Fc, FPA_Ast, FPA_Ap, Fs(3));
FPA_22K = design(FPA_22K_Specs, 'fircls');
Fc_corr = fcParaQueF3dBSea_HP(FPA_22K.Numerator, Fs(3), FPA_Fc);
FPA_22K_spec_corr = fdesign.highpass('N,Fc,Ast,Ap', N_FPA_22K, Fc_corr, FPA_Ast, FPA_Ap, Fs(3));
FPA_22K = design(FPA_22K_spec_corr, 'fircls');
disp('Filtro PA 22K: ');
revisarVectorExtremos(FPA_22K.Numerator);

% Filtro Pasa Altos 44 KHz:
N_FPA_44K = 79;
FPA_44K_Specs = fdesign.highpass('N,Fc,Ast,Ap', N_FPA_44K, FPA_Fc, FPA_Ast, FPA_Ap, Fs(4));
FPA_44K = design(FPA_44K_Specs, 'fircls');
Fc_corr = fcParaQueF3dBSea_HP(FPA_44K.Numerator, Fs(4), FPA_Fc);
FPA_44K_spec_corr = fdesign.highpass('N,Fc,Ast,Ap', N_FPA_44K, Fc_corr, FPA_Ast, FPA_Ap+1, Fs(4));
FPA_44K = design(FPA_44K_spec_corr, 'fircls');
disp('Filtro PA 44K: ');
revisarVectorExtremos(FPA_44K.Numerator);

% Filtro Pasa Altos 48 KHz:
N_FPA_48K = 79;
FPA_48K_Specs = fdesign.highpass('N,Fc,Ast,Ap', N_FPA_48K, FPA_Fc, FPA_Ast, FPA_Ap, Fs(5));
FPA_48K = design(FPA_48K_Specs, 'fircls');
Fc_corr = fcParaQueF3dBSea_HP(FPA_48K.Numerator, Fs(5), FPA_Fc);
FPA_48K_spec_corr = fdesign.highpass('N,Fc,Ast,Ap', N_FPA_48K, Fc_corr, FPA_Ast, FPA_Ap+1, Fs(5));
FPA_48K = design(FPA_48K_spec_corr, 'fircls');
disp('Filtro PA 48K: ');
revisarVectorExtremos(FPA_48K.Numerator);

%% -------------------- DISEÑO FILTRO PASA BANDA --------------------------
% -
% Especificaciones del TP: Pasa Banda Fc1=35Hz, Fc2=3500Hz Astop=30db.
% -
FPBD_Fc1 = 400; % 110;
FPBD_Fc2 = 3600; % 3700;
FPBD_Ast1 = 30;
FPBD_Ast2 = 30;
FPBD_Ap = 1;
disp('   ');
disp('FILTROS PASA BANDA');

% Filtro Pasa Banda a 8 KHz:
N_FPBD_8K = 79; % 39;
FPBD_8K_Specs = fdesign.bandpass('N,Fc1,Fc2,Ast1,Ap,Ast2', N_FPBD_8K, FPBD_Fc1, FPBD_Fc2+100, FPBD_Ast1, FPBD_Ap, FPBD_Ast2, Fs(1));
FPBD_8K = design(FPBD_8K_Specs, 'fircls');
disp('Filtro PBD 8K: ');
revisarVectorExtremos(FPBD_8K.Numerator);

% Filtro Pasa Banda a 16 KHz:
N_FPBD_16K = 79; % 49;
FPBD_16K_Specs = fdesign.bandpass('N,Fc1,Fc2,Ast1,Ap,Ast2', N_FPBD_16K, FPBD_Fc1, FPBD_Fc2, FPBD_Ast1, FPBD_Ap, FPBD_Ast2, Fs(2));
FPBD_16K = design(FPBD_16K_Specs, 'fircls');
disp('Filtro PBD 16K: ');
revisarVectorExtremos(FPBD_16K.Numerator);

% Filtro Pasa Banda a 22 KHz:
N_FPBD_22K = 79; % 89;
FPBD_22K_Specs = fdesign.bandpass('N,Fc1,Fc2,Ast1,Ap,Ast2', N_FPBD_22K, FPBD_Fc1, FPBD_Fc2, FPBD_Ast1, FPBD_Ap, FPBD_Ast2, Fs(3));
FPBD_22K = design(FPBD_22K_Specs, 'fircls');
disp('Filtro PBD 22K: ');
revisarVectorExtremos(FPBD_22K.Numerator);

% Filtro Pasa Banda a 44 KHz:
N_FPBD_44K = 79; % 99;
FPBD_44K_Specs = fdesign.bandpass('N,Fc1,Fc2,Ast1,Ap,Ast2', N_FPBD_44K, FPBD_Fc1, FPBD_Fc2, FPBD_Ast1, FPBD_Ap, FPBD_Ast2, Fs(4));
FPBD_44K = design(FPBD_44K_Specs, 'fircls');
disp('Filtro PBD 44K: ');
revisarVectorExtremos(FPBD_44K.Numerator);

% Filtro Pasa Banda a 48 KHz:
N_FPBD_48K = 79; % 109;
FPBD_48K_Specs = fdesign.bandpass('N,Fc1,Fc2,Ast1,Ap,Ast2', N_FPBD_48K, FPBD_Fc1, FPBD_Fc2, FPBD_Ast1, FPBD_Ap, FPBD_Ast2, Fs(5));
FPBD_48K = design(FPBD_48K_Specs, 'fircls');
disp('Filtro PBD 48K: ');
revisarVectorExtremos(FPBD_48K.Numerator);

%% -------------------- DISEÑO FILTRO RECHAZA BANDA -----------------------
% -
% Especificaciones del TP: Elimina Banda Fr= 50 Hz Bw=15 Hz Astop=25db.
% -
FRBD_Fc1 = 400; % 50;
FRBD_Fc2 = 700; % 150;
FRBD_Ap1 = 2;
FRBD_Ast = 30;
FRBD_Ap2 = 2;
disp('   ');
disp('FILTROS RECHAZA BANDA');

% Filtros rechaza banda 8 KHz:
N_FRBD_8K = 79; % 99;
FRBD_8K_Specs = fdesign.bandstop('N,Fc1,Fc2,Ap1,Ast,Ap2', N_FRBD_8K, FRBD_Fc1, FRBD_Fc2, FRBD_Ap1, FRBD_Ast, FRBD_Ap2, Fs(1));
FRBD_8K = design(FRBD_8K_Specs, 'fircls');
disp('Filtro RBD 8K: ');
revisarVectorExtremos(FRBD_8K.Numerator);

% Filtros rechaza banda 16 KHz:
N_FRBD_16K = 79; % 179;
FRBD_16K_Specs = fdesign.bandstop('N,Fc1,Fc2,Ap1,Ast,Ap2', N_FRBD_16K, FRBD_Fc1, FRBD_Fc2, FRBD_Ap1, FRBD_Ast, FRBD_Ap2, Fs(2));
FRBD_16K = design(FRBD_16K_Specs, 'fircls');
disp('Filtro RBD 16K: ');
revisarVectorExtremos(FRBD_16K.Numerator);

% Filtros rechaza banda 22 KHz:
N_FRBD_22K = 79; % 259;
FRBD_22K_Specs = fdesign.bandstop('N,Fc1,Fc2,Ap1,Ast,Ap2', N_FRBD_22K, FRBD_Fc1-30, FRBD_Fc2+30, FRBD_Ap1, FRBD_Ast, FRBD_Ap2, Fs(3));
FRBD_22K = design(FRBD_22K_Specs, 'fircls');
disp('Filtro RBD 22K: ');
revisarVectorExtremos(FRBD_22K.Numerator);

% Filtros rechaza banda 44 KHz:
N_FRBD_44K = 79; % 299;
FRBD_44K_Specs = fdesign.bandstop('N,Fc1,Fc2,Ap1,Ast,Ap2', N_FRBD_44K, FRBD_Fc1-120, FRBD_Fc2+150, FRBD_Ap1+5, FRBD_Ast, FRBD_Ap2+5, Fs(4));
FRBD_44K = design(FRBD_44K_Specs, 'fircls');
disp('Filtro RBD 44K: ');
revisarVectorExtremos(FRBD_44K.Numerator);

% Filtros rechaza banda 48 KHz:
N_FRBD_48K = 79; % 299;
FRBD_48K_Specs = fdesign.bandstop('N,Fc1,Fc2,Ap1,Ast,Ap2', N_FRBD_48K, FRBD_Fc1-120, FRBD_Fc2+200, FRBD_Ap1+5, FRBD_Ast, FRBD_Ap2+5, Fs(5));
FRBD_48K = design(FRBD_48K_Specs, 'fircls');
disp('Filtro RBD 48K: ');
revisarVectorExtremos(FRBD_48K.Numerator);



%% -------------------------- GRAFICOS -----------------------------------------------

% Q = 15;
% S = 2^Q;
% INT16_MIN = -32768;
% INT16_MAX =  32767;
% 
% NFFT = 4096;
% 
% Fs_list = [8000 16000 22000 44000 48000];
% Fs_labels = {'8 kHz','16 kHz','22 kHz','44 kHz','48 kHz'};
% 
% % Helper: cuantiza vector b a int16 Q15 y vuelve a double escalado
% q15 = @(b) (double( int16( min(max(round(b*S),INT16_MIN),INT16_MAX) ) ) / S);
% 
% % ------------------ PASA BAJOS ------------------
% figure('Name','Pasa Bajos (Q15)','Color','w'); hold on; grid on;
% filtersPB = {FPB_8K, FPB_16K, FPB_22K, FPB_44K, FPB_48K};
% for i = 1:5
%     bq = q15(filtersPB{i}.Numerator(:).');
%     [H,w] = freqz(bq, 1, NFFT, Fs_list(i));
%     plot(w, 20*log10(abs(H)+1e-12), 'DisplayName', Fs_labels{i});
% end
% title('Respuesta en frecuencia - Pasa Bajos (coeficientes Q15)');
% xlabel('Hz'); ylabel('|H| (dB)');
% xlim([0 max(Fs_list)/2]);
% ylim([-120 10]);
% legend('Location','southwest');
% 
% % ------------------ PASA ALTOS ------------------
% figure('Name','Pasa Altos (Q15)','Color','w'); hold on; grid on;
% filtersPA = {FPA_8K, FPA_16K, FPA_22K, FPA_44K, FPA_48K};
% for i = 1:5
%     bq = q15(filtersPA{i}.Numerator(:).');
%     [H,w] = freqz(bq, 1, NFFT, Fs_list(i));
%     plot(w, 20*log10(abs(H)+1e-12), 'DisplayName', Fs_labels{i});
% end
% title('Respuesta en frecuencia - Pasa Altos (coeficientes Q15)');
% xlabel('Hz'); ylabel('|H| (dB)');
% xlim([0 max(Fs_list)/2]);
% ylim([-120 10]);
% legend('Location','southwest');
% 
% % ------------------ PASA BANDA ------------------
% figure('Name','Pasa Banda (Q15)','Color','w'); hold on; grid on;
% filtersPBD = {FPBD_8K, FPBD_16K, FPBD_22K, FPBD_44K, FPBD_48K};
% for i = 1:5
%     bq = q15(filtersPBD{i}.Numerator(:).');
%     [H,w] = freqz(bq, 1, NFFT, Fs_list(i));
%     plot(w, 20*log10(abs(H)+1e-12), 'DisplayName', Fs_labels{i});
% end
% title('Respuesta en frecuencia - Pasa Banda (coeficientes Q15)');
% xlabel('Hz'); ylabel('|H| (dB)');
% xlim([0 max(Fs_list)/2]);
% ylim([-120 10]);
% legend('Location','southwest');
% 
% % ------------------ RECHAZA BANDA ------------------
% figure('Name','Rechaza Banda (Q15)','Color','w'); hold on; grid on;
% filtersRBD = {FRBD_8K, FRBD_16K, FRBD_22K, FRBD_44K, FRBD_48K};
% for i = 1:5
%     bq = q15(filtersRBD{i}.Numerator(:).');
%     [H,w] = freqz(bq, 1, NFFT, Fs_list(i));
%     plot(w, 20*log10(abs(H)+1e-12), 'DisplayName', Fs_labels{i});
% end
% title('Respuesta en frecuencia - Rechaza Banda (coeficientes Q15)');
% xlabel('Hz'); ylabel('|H| (dB)');
% xlim([0 max(Fs_list)/2]);
% ylim([-120 10]);
% legend('Location','southwest');

Q = 15;
S = 2^Q;
INT16_MIN = -32768;
INT16_MAX =  32767;

NFFT = 4096;

Fs_list   = [8000 16000 22000 44000 48000];
Fs_labels = {'8 kHz','16 kHz','22 kHz','44 kHz','48 kHz'};

q15 = @(b) (double( int16( min(max(round(b*S),INT16_MIN),INT16_MAX) ) ) / S);

plotTipo = @(titulo, filtersCell, ylimMag) ...
    plot_mag_fase_q15(titulo, filtersCell, Fs_list, Fs_labels, NFFT, q15, ylimMag);

% ---- PASA BAJOS ----
plotTipo('Pasa Bajos (Q15)',  {FPB_8K,  FPB_16K,  FPB_22K,  FPB_44K,  FPB_48K},  [-120 10]);

% ---- PASA ALTOS ----
plotTipo('Pasa Altos (Q15)',  {FPA_8K,  FPA_16K,  FPA_22K,  FPA_44K,  FPA_48K},  [-120 10]);

% ---- PASA BANDA ----
plotTipo('Pasa Banda (Q15)',  {FPBD_8K, FPBD_16K, FPBD_22K, FPBD_44K, FPBD_48K}, [-120 10]);

% ---- RECHAZA BANDA ----
plotTipo('Rechaza Banda (Q15)',{FRBD_8K, FRBD_16K, FRBD_22K, FRBD_44K, FRBD_48K},[-120 10]);




%% -------------------------- SIMULACIONES DE COSTOS DE RECURSOS --------------------------------
% % --- PASO 1: Tu filtro flotante ---
% % FRBD_8K (Asumo que esta es tu variable)
% 
% % --- PASO 2: Copias ---
% FRBD_8K_q15 = copy(FRBD_8K);
% FRBD_8K_q15_hard = copy(FRBD_8K);
% 
% % --- Configuración Q15 (Full Precision) ---
% set(FRBD_8K_q15, 'Arithmetic', 'fixed');
% % Define la entrada
% set(FRBD_8K_q15, 'InputWordLength', 16);
% set(FRBD_8K_q15, 'InputFracLength', 15);
% % Define los coeficientes
% set(FRBD_8K_q15, 'CoeffWordLength', 16);
% set(FRBD_8K_q15, 'CoeffAutoScale', false); 
% set(FRBD_8K_q15, 'NumFracLength', 15);     
% % NO se toca 'FilterInternals' (queda en 'FullPrecision' por defecto)
% 
% % --- Configuración Q15 (Hardware Sim) ---
% set(FRBD_8K_q15_hard, 'Arithmetic', 'fixed');
% set(FRBD_8K_q15_hard, 'FilterInternals', 'SpecifyPrecision');
% 
% set(FRBD_8K_q15_hard, 'InputWordLength', 16);
% set(FRBD_8K_q15_hard, 'InputFracLength', 15);
% set(FRBD_8K_q15_hard, 'CoeffWordLength', 16);
% set(FRBD_8K_q15_hard, 'CoeffAutoScale', false);
% set(FRBD_8K_q15_hard, 'NumFracLength', 15);
% set(FRBD_8K_q15_hard, 'ProductWordLength', 32);
% set(FRBD_8K_q15_hard, 'ProductFracLength', 30);
% set(FRBD_8K_q15_hard, 'AccumWordLength', 64);
% set(FRBD_8K_q15_hard, 'AccumFracLength', 30);
% set(FRBD_8K_q15_hard, 'OutputWordLength', 16);
% set(FRBD_8K_q15_hard, 'OutputFracLength', 15);
% set(FRBD_8K_q15_hard, 'OverflowMode', 'saturate'); 
% set(FRBD_8K_q15_hard, 'RoundMode', 'floor'); 
% 
% % % --- PASO 3: Analizar ---
% % fvtool(FRBD_8K, FRBD_8K_q15, FRBD_8K_q15_hard, ...
% %        'Analysis', 'magnitude', 'Fs', 8000) % Añade tu Fs
% % 
% % legend('Filtro Flotante (Ideal)', ...
% %        'Q15 (Coefs. cuantizados)', ...
% %        'Q15 (Sim. Hardware)')
% 
% % --- 1. Definir Fs (¡Usa la tuya!) ---
% Fs = 8000; % Asegúrate de que esta Fs sea la correcta
% 
% % --- 2. Crear Señal de Prueba (Barrido de Frecuencia) ---
% % 1 segundo de barrido, de 0 Hz a Fs/2 (Nyquist)
% t = 0:1/Fs:1-(1/Fs);
% % Amplitud 0.95 (casi a tope, para tener un pequeño margen)
% x_float = 0.95 * chirp(t, 0, t(end), Fs/2); 
% 
% % --- 3. Crear Señal de Prueba Q15 ---
% % Convertimos la señal de entrada flotante a Q15
% T_q15_in = numerictype(1, 16, 15); % Signado, 16-bit, 15 frac
% x_q15 = fi(x_float, T_q15_in);
% 
% % --- 4. Filtrar con los 3 Filtros ---
% % Nota: El filtro flotante usa la entrada flotante.
% % Los filtros fijos usan la entrada fija.
% y_float = filter(FRBD_8K, x_float);
% 
% % Filtro Q15 "Ideal" (Precisión Total)
% y_q15_ideal = filter(FRBD_8K_q15, x_q15);
% 
% % Filtro Q15 "Hardware" (Saturación)
% y_q15_hardware = filter(FRBD_8K_q15_hard, x_q15);
% 
% 
% % --- 5. Graficar y Comparar ---
% figure;
% subplot(3,1,1);
% plot(t, y_float);
% title('Salida: Filtro Flotante (Ideal)');
% grid on;
% ylim([-1.5, 1.5]); % Amplía el eje Y para ver sobreimpulsos
% 
% subplot(3,1,2);
% plot(t, y_q15_ideal);
% title('Salida: Q15 "Full Precision" (Resultado Matemático)');
% grid on;
% ylim([-1.5, 1.5]);
% 
% subplot(3,1,3);
% plot(t, y_q15_hardware);
% title('Salida: Q15 "Simulación Hardware" (¡Saturación!)');
% grid on;
% ylim([-1.5, 1.5]);
% 
% xlabel('Tiempo (s)');

%% ------------------------- IMPRESION EN FILT.H --------------------------

% disp('   ');
% disp('-----------------------------------------------');
% disp('Diseño de filtros completado.');
% disp('Iniciando exportación a filt.h...');
% 
% % --- 1. Crear un Mapa (diccionario) para (nombre, objeto_filtro) ---
% % Esto nos permite iterar y usar los nombres de variable como texto
% filter_map = containers.Map;
% 
% % Llenar el mapa con tus 20 filtros
% filter_map('FPB_8K') = FPB_8K;
% filter_map('FPB_16K') = FPB_16K;
% filter_map('FPB_22K') = FPB_22K;
% filter_map('FPB_44K') = FPB_44K;
% filter_map('FPB_48K') = FPB_48K;
% 
% filter_map('FPA_8K') = FPA_8K;
% filter_map('FPA_16K') = FPA_16K;
% filter_map('FPA_22K') = FPA_22K;
% filter_map('FPA_44K') = FPA_44K;
% filter_map('FPA_48K') = FPA_48K;
% 
% filter_map('FPBD_8K') = FPBD_8K;
% filter_map('FPBD_16K') = FPBD_16K;
% filter_map('FPBD_22K') = FPBD_22K;
% filter_map('FPBD_44K') = FPBD_44K;
% filter_map('FPBD_48K') = FPBD_48K;
% 
% filter_map('FRBD_8K') = FRBD_8K;
% filter_map('FRBD_16K') = FRBD_16K;
% filter_map('FRBD_22K') = FRBD_22K;
% filter_map('FRBD_44K') = FRBD_44K;
% filter_map('FRBD_48K') = FRBD_48K;
% 
% % --- 2. Llamar a la función de exportación ---
% output_filename = 'filtros.h';
% write_header_file(filter_map, output_filename);
% 
% fprintf('¡Éxito! Archivo "%s" generado en el directorio actual.\n', output_filename);




%% ------------------------- FUNCIONES EXTRAS -----------------------------

% -------------------------------------------------------------------------
% --- SUB-FUNCIÓN: EXPORTADOR A .H ---
% -------------------------------------------------------------------------
function write_header_file(filter_map, filename)
    % Abre el archivo para escritura (w)
    fid = fopen(filename, 'w');
    if fid == -1
        error('No se pudo abrir el archivo: %s', filename);
    end
    
    % --- Escribir la cabecera del .h ---
    fprintf(fid, '#ifndef FILTROS_H_\n');
    fprintf(fid, '#define FILTROS_H_\n\n');
    fprintf(fid, '/*\n');
    fprintf(fid, ' *******************************************************************\n');
    fprintf(fid, ' * Archivo de coeficientes de filtro autogenerado por MATLAB\n');
    fprintf(fid, ' * Fecha: %s\n', datestr(now));
    fprintf(fid, ' *******************************************************************\n');
    fprintf(fid, ' */\n\n');
    fprintf(fid, '#include "arm_math.h" // Requerido para el tipo q15_t\n\n');
    
    % --- Definir el formato Q15 ---
    T_q15 = numerictype(1, 16, 15); % Signado, 16-bit, 15-frac
    % Usar 'floor' y 'saturate' para coincidir con la mayoría de los DSP
    F_math = fimath('OverflowMode', 'saturate', 'RoundMode', 'floor');
    
    % --- Iterar sobre todos los filtros en el mapa ---
    % ===== ORDEN DESEADO DE EXPORTACIÓN =====
    ordered_filter_names = {
        'FPB_8K'
        'FPB_16K'
        'FPB_22K'
        'FPB_44K'
        'FPB_48K'
    
        'FPA_8K'
        'FPA_16K'
        'FPA_22K'
        'FPA_44K'
        'FPA_48K'
    
        'FPBD_8K'
        'FPBD_16K'
        'FPBD_22K'
        'FPBD_44K'
        'FPBD_48K'
    
        'FRBD_8K'
        'FRBD_16K'
        'FRBD_22K'
        'FRBD_44K'
        'FRBD_48K'
    };
    
    % ===== EXPORTACIÓN ORDENADA =====
    for k = 1:length(ordered_filter_names)
        filter_name = ordered_filter_names{k};
    
        % (si falta un filtro por error, simplemente lo saltea)
        if ~isKey(filter_map, filter_name)
            warning('Filtro "%s" no está en filter_map, se omite.', filter_name);
            continue;
        end
    
        filter_obj = filter_map(filter_name);
        
        % Obtener coeficientes flotantes
        h_float = filter_obj.Numerator;
        N_taps = length(h_float);

        % --- Forzar cantidad de taps PAR ---
        % (si es impar, agrego un coeficiente 0 al final)
        if mod(N_taps, 2) ~= 0
            resto = abs(h_float(end-1) - h_float(end));
            if h_float(end-1) < h_float(end) 
                h_float(end+1) = h_float(end) + resto;
            else
                h_float(end+1) = h_float(end) - resto;
            end
            N_taps = N_taps + 1;
        end
        
        % --- Convertir coeficientes a Q15 (int16) ---
        h_q15_fi = fi(h_float, T_q15, F_math);
        h_int16_values = int16(storedInteger(h_q15_fi));

        % --- Escribir las definiciones de C en el archivo ---
        
        fprintf(fid, '// --- Coeficientes para el filtro: %s ---\n', filter_name);
        
        % 1. Definir el ORDEN (N_TAPS)
        % Esto es lo que usas en C para declarar tu vector de estado:
        % q15_t %s_STATE[%s_NUM_TAPS + BLOCK_SIZE - 1];
        tap_define = sprintf('%s_NUM_TAPS', upper(filter_name));
        fprintf(fid, '#define %-30s %d\n', tap_define, N_taps);
        
        % 2. Definir el array de COEFICIENTES
        coeffs_name = sprintf('%s_COEFFS', upper(filter_name));
        fprintf(fid, 'const q15_t %s[%s] = {\n', coeffs_name, tap_define);
        
        % Bucle para imprimir los valores, 8 por línea para legibilidad
        for i = 1:N_taps
            fprintf(fid, '  %d', h_int16_values(i));
            
            % Añade coma si no es el último elemento
            if i < N_taps
                fprintf(fid, ',');
            end
            
            % Salto de línea cada 8 valores o al final
            if mod(i, 8) == 0 || i == N_taps
                fprintf(fid, '\n');
            end
        end
        
        fprintf(fid, '};\n\n'); % Cierre del array
    end
    
    % --- Escribir el final del .h ---
    fprintf(fid, '#endif // FILTROS_H_\n');
    
    % Cerrar el archivo
    fclose(fid);
end

% -------------------------------------------------------------------------
% --- SUB-FUNCIÓN: revision de rango ---
% -------------------------------------------------------------------------


function revisarVectorExtremos(vectorEntrada)
    % --- Definir umbrales ---
    % Umbral inferior (números muy cercanos a cero)
    umbral_pequeno = 1 / (2^15);
    % Umbral superior (saturación)
    umbral_grande = 1.0;
    
    % Obtener los valores absolutos una sola vez
    valores_abs = abs(vectorEntrada);
    
    % --- Realizar las comprobaciones ---
    
    % 1. Comprueba si hay números MUY PEQUEÑOS (underflow)
    hay_muy_pequenos = any(valores_abs < umbral_pequeno);
    
    % 2. Comprueba si hay números MUY GRANDES (overflow/saturación)
    %    Es decir, si alguno es MAYOR O IGUAL a 1
    hay_muy_grandes = any(valores_abs >= umbral_grande);
    
    % --- Imprimir resultados ---
    
    % Mensaje para números pequeños
    if hay_muy_pequenos
        disp('AVISO: Se encontró un número cuyo valor absoluto es MENOR al umbral (1/2^15).');
    end
    
    % Mensaje para números grandes
    if hay_muy_grandes
        disp('AVISO: Se encontró un número cuyo valor absoluto es MAYOR O IGUAL a 1.');
    end
    
    % Mensaje si todos los números están bien
    if ~hay_muy_pequenos && ~hay_muy_grandes
         disp('Todos los números están dentro del rango operativo.');
    end
end

% -------------------------------------------------------------------------
% --- SUB-FUNCIÓN: revision de Fc -3dB ---
% -------------------------------------------------------------------------

function Fc_corr = fcParaQueF3dBSea(b, Fs, Fc_obj, NFFT)
% fcParaQueF3dBSea
% Dado un FIR ya diseñado (numerator b), devuelve la Fc corregida
% para que el punto de -3 dB quede en Fc_obj.
%
% Entradas:
%   b      : numerator (vector FIR)
%   Fs     : frecuencia de muestreo [Hz]
%   Fc_obj : frecuencia deseada donde querés -3 dB [Hz]
%   NFFT   : (opcional) puntos para freqz (default 16384)
%
% Salida:
%   Fc_corr: Fc corregida [Hz] para rediseñar el filtro

    if nargin < 4 || isempty(NFFT)
        NFFT = 16384;
    end

    b = b(:).';  % asegurar fila

    % Respuesta en frecuencia
    [H,w] = freqz(b, 1, NFFT, Fs);
    magdB = 20*log10(abs(H) + 1e-12);

    % Normalizar a 0 dB en DC
    magdB = magdB - magdB(1);

    % Buscar cruce a -3 dB
    idx = find(magdB <= -3, 1, 'first');
    if isempty(idx) || idx == 1
        error('No se encontró cruce a -3 dB. Aumentá NFFT o revisá el filtro.');
    end

    % Interpolación lineal
    w1 = w(idx-1); w2 = w(idx);
    y1 = magdB(idx-1); y2 = magdB(idx);

    if y2 == y1
        F3_real = w2;
    else
        F3_real = w1 + ((-3 - y1) * (w2 - w1)) / (y2 - y1);
    end

    % Como Fc_obj es también la Fc inicial usada en el diseño:
    Fc_corr = (Fc_obj^2) / F3_real;
end


function Fc_corr = fcParaQueF3dBSea_HP(b, Fs, Fc_obj, NFFT)
% fcParaQueF3dBSea_HP
% Dado un FIR pasa-altos ya diseñado (numerator b), devuelve la Fc corregida
% para que el punto de -3 dB quede en Fc_obj.
%
% Entradas:
%   b      : numerator (vector FIR)
%   Fs     : frecuencia de muestreo [Hz]
%   Fc_obj : frecuencia deseada donde querés -3 dB [Hz]
%   NFFT   : (opcional) puntos para freqz (default 16384)
%
% Salida:
%   Fc_corr: Fc corregida [Hz] para rediseñar el filtro

    if nargin < 4 || isempty(NFFT)
        NFFT = 16384;
    end

    b = b(:).'; % fila

    [H,w] = freqz(b, 1, NFFT, Fs);
    magdB = 20*log10(abs(H) + 1e-12);

    % Normalizar a 0 dB en altas (cerca de Nyquist) para HP
    magdB = magdB - magdB(end);

    % Buscar el primer cruce >= -3 dB (subiendo desde DC)
    idx = find(magdB >= -3, 1, 'first');
    if isempty(idx) || idx == 1
        error('No se encontró cruce a -3 dB en [0, Fs/2]. Aumentá NFFT o revisá el filtro.');
    end

    % Interpolación lineal entre (idx-1) y (idx)
    w1 = w(idx-1); w2 = w(idx);
    y1 = magdB(idx-1); y2 = magdB(idx);

    if y2 == y1
        F3_real = w2;
    else
        F3_real = w1 + ((-3 - y1) * (w2 - w1)) / (y2 - y1);
    end

    % Misma idea que en PB: si diseñaste con Fc_obj como "Fc inicial"
    Fc_corr = (Fc_obj^2) / F3_real;
end

% ------------------------------------------------------------
% Función local (al final del script) para dibujar una figura
% ------------------------------------------------------------
function plot_mag_fase_q15(titulo, filtersCell, Fs_list, Fs_labels, NFFT, q15, ylimMag)

    figure('Name',titulo,'Color','w');

    % --- Magnitud ---
    subplot(2,1,1); hold on; grid on;
    for i = 1:5
        bq = q15(filtersCell{i}.Numerator(:).');
        [H,w] = freqz(bq, 1, NFFT, Fs_list(i));
        plot(w, 20*log10(abs(H)+1e-12), 'DisplayName', Fs_labels{i});
    end
    title(['Respuesta en frecuencia - ' titulo ' - Magnitud']);
    xlabel('Hz'); ylabel('|H| (dB)');
    xlim([0 max(Fs_list)/2]);
    ylim(ylimMag);
    legend('Location','southwest');

    % --- Fase ---
    subplot(2,1,2); hold on; grid on;
    for i = 1:5
        bq = q15(filtersCell{i}.Numerator(:).');
        [H,w] = freqz(bq, 1, NFFT, Fs_list(i));
        fase = unwrap(angle(H));          % rad
        plot(w, fase, 'DisplayName', Fs_labels{i});
    end
    title(['Respuesta en frecuencia - ' titulo ' - Fase (unwrap)']);
    xlabel('Hz'); ylabel('Fase (rad)');
    xlim([0 max(Fs_list)/2]);
    legend('Location','southwest');

end
