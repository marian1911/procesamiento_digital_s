%% ------------------------------------------------------------
% 2) PIPELINE DSP
% ------------------------------------------------------------

% --- LPF ---
% y(nT) = 2y(nT-T) - y(nT-2T) + x(nT) - 2x(nT-6T) + x(nT-12T)
y_lpf = zeros(N,1);
for n = 13:N
    y_lpf(n) = 2*y_lpf(n-1) - y_lpf(n-2) + x(n) - 2*x(n-6) + x(n-12);
end

% --- HPF ---
% y(nT) = y(nT-T) - x(nT)/32 + x(nT-16T) - x(nT-17T) + x(nT-32T)/32
y_hpf = zeros(N,1);
for n = 33:N
    y_hpf(n) = y_hpf(n-1) - y_lpf(n)   /32 + y_lpf(n-16) - y_lpf(n-17) + y_lpf(n-32)/32;
end

% --- Derivador ---
% y(nT) = (2x(nT) + x(nT-T) - x(nT-3T) - 2x(nT-4T)) / 8
y_der = zeros(N,1);
for n = 5:N
    y_der(n) = (1/8)*( 2*y_hpf(n) +  y_hpf(n-1) -  y_hpf(n-3) - 2*y_hpf(n-4) );
end

% --- Cuadrado ---
y_sq = y_der .^ 2;

% --- MWI: ventana 150 ms = 30 muestras ---
MW    = round(0.150*fs);
y_mwi = zeros(N,1);
acc   = 0;
for n = 1:N
    acc = acc + y_sq(n);
    if n > MW,  acc = acc - y_sq(n-MW);  end
    if n >= MW, y_mwi(n) = acc / MW;     end
end