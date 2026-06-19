# Revisión técnica del repositorio `procesamiento_digital_s`

**Fecha de revisión:** 19 de junio de 2026

**Alcance:** árbol de trabajo local, fuentes, informes, configuración STM32CubeIDE, artefactos existentes y estado de Git.
**Limitación:** no se compiló ni se ejecutó firmware en placa durante esta revisión. El responsable del repositorio confirmó que Lab 1 y Lab 2 están completos.

## 1. Resumen

La revisión anterior, fechada el 11 de mayo, quedó desactualizada. Desde entonces se completó gran parte de los informes de Lab 3 y Lab 4, se añadieron herramientas de recepción para Lab 3 y apareció `PDS_Lab_Final`, un proyecto distinto de Lab 4 orientado a ECG.

El repositorio contiene trabajo técnico valioso, pero su estado actual no es reproducible desde Git: todo el proyecto final y varios resultados de Lab 3 están sin versionar. Además, el proyecto final conserva configuración heredada de Lab 1 y su último ELF es anterior al `main.c` vigente.

## 2. Instantánea verificable

| Dato | Observación |
|---|---|
| Rama local | `main` |
| Relación mostrada con remoto | `main...origin/main`, sin conteo ahead/behind |
| Commits alcanzables desde HEAD | 20 |
| Archivos seguidos por Git | 1301 |
| Estado del árbol | Sucio: recursos de Lab 3 y todo `PDS_Lab_Final/` sin versionar |
| Documentación raíz | `README.txt`, 5 líneas |
| `.gitignore` | No existe |
| Directorios de entregas | `PDS_Lab_1` a `PDS_Lab_4` y `PDS_Lab_Final` |

El árbol local completo ocupa aproximadamente 541 MiB y contiene unos 9600 archivos, contando `.git`, entornos Python, drivers y salidas de compilación. Esas cifras no representan el tamaño de una distribución limpia.

## 3. Estado por entrega

### Lab 1 — adquisición y reproducción

Rutas principales:

- `PDS_Lab_1/lab1_dsp/Core/Src/main.c`
- `PDS_Lab_1/Informe_Latex/pds_lab1.tex`

Evidencia observada:

- ADC y DAC con DMA circular, disparados por TIM2.
- Buffer de 512 muestras.
- Selección de tasas aproximadas de 8, 16, 22, 44 y 48 kHz.
- Informe de 1015 líneas con introducción, resultados, figuras y conclusiones.
- Artefactos ELF históricos en `Debug/`.

Evaluación: **completo**, según confirmación del responsable del repositorio. La revisión constató código, informe y artefactos históricos coherentes con ese estado.

### Lab 2 — filtrado FIR

Rutas principales:

- `PDS_Lab_2/lab2_dsp/Core/Src/main.c`
- `PDS_Lab_2/lab2_dsp/Drivers/filtros.h`
- `PDS_Lab_2/Informe_Latex/pds_lab2.tex`

Evidencia observada:

- Procesamiento Q15 mediante `arm_fir_q15`, no filtros IIR biquad.
- Cuatro respuestas seleccionables: pasa-bajos, pasa-altos, pasa-banda y rechaza-banda.
- Coeficientes dependientes de 8, 16, 22, 44 y 48 kHz.
- Buffer de 5096 muestras.
- Informe de 1296 líneas con numerosas figuras de resultados y conclusiones.
- Artefacto ELF histórico en `Debug/`.

Evaluación: **completo**, según confirmación del responsable del repositorio. Debe mantenerse la descripción técnica correcta: el fuente actual implementa filtros FIR Q15, no IIR.

### Lab 3 — FFT y transporte UART

Rutas principales:

- `PDS_Lab_3/lab3_dsp/Core/Src/main.c`
- `PDS_Lab_3/Informe_Latex/build/pds_lab3.tex`
- `PDS_Lab_3/Recepcion_FFT.m`
- `PDS_Lab_3/serial_debug.py`, `serial_pattern.py`, `serial_plotter.py`

Evidencia observada:

- Adquisición ADC por DMA y procesamiento fuera de las callbacks.
- FFT compleja Q15 de CMSIS-DSP con tamaños 512, 1024 y 2048.
- Cálculo de magnitud y modo bypass.
- Envío binario por UART4 con trama propia.
- Selección de frecuencia de muestreo y tamaño mediante GPIO.
- Informe de 735 líneas con teoría, implementación y resultados redactados.

Pendientes:

- Las subsecciones “Logros alcanzados”, “Limitaciones identificadas” y “Reflexión final” están vacías.
- El informe solo inserta el logo y una imagen de la placa; no integra las capturas espectrales disponibles.
- Los tiempos declarados para las FFT no están acompañados por tabla de medición, captura DWT o procedimiento reproducible.
- Tres PNG, el receptor MATLAB y tres scripts Python figuran sin versionar.
- No se localizó un ELF de Lab 3 en la búsqueda realizada.

Evaluación: el informe no está vacío; está avanzado, pero le falta cierre y evidencia experimental trazable.

### Lab 4 — cancelación adaptiva LMS

Rutas principales:

- `PDS_Lab_4/lab4_dsp/Core/Src/main.c`
- `PDS_Lab_4/Informe_Latex/build/pds_lab4.tex`

Evidencia observada:

- Entrada `s(n)` desde ADC.
- Referencia de ruido pseudoaleatoria generada por LCG.
- FIR de 30 taps que modela la trayectoria de contaminación.
- LMS float32 de 60 taps con `mu = 0.005`.
- Buffer de 512 muestras y procesamiento por semibuffer.
- DAC1 expone el error estimado `e(n)` y DAC2 la señal contaminada `d(n)`.
- Informe de 537 líneas con arquitectura, código, resultados redactados y conclusiones.

Pendientes:

- El informe no contiene figuras experimentales aparte del logo.
- Las cifras de convergencia, ciclos y utilización de CPU deben respaldarse con mediciones o rotularse como estimaciones.
- Existen configuraciones `.launch` y un `.ioc` heredados de Lab 2; no deben borrarse sin comprobar primero cuál configuración abre y depura correctamente.
- No se localizó un ELF de Lab 4 en la búsqueda realizada.

Evaluación: código e informe sustanciales, con validación experimental y limpieza de proyecto pendientes.

### Proyecto final — ECG, LMS y Pan–Tompkins parcial

Rutas principales:

- `PDS_Lab_Final/labfinal_dsp/Core/Src/main.c`
- `PDS_Lab_Final/labfinal_dsp/Core/Src/adc.c`
- `PDS_Lab_Final/labfinal_dsp/Core/Src/dac.c`
- `PDS_Lab_Final/Procesaciento_ECG_ejemplo.m`
- `PDS_Lab_Final/Propuesta ECG.pdf`

Arquitectura observada en los fuentes:

1. ADC1 captura el ECG contaminado `d(n)` por PA0.
2. ADC2 captura la referencia de ruido `x(n)` por PA1.
3. `arm_lms_f32` estima el ruido `y(n)` y produce `e(n) = d(n) - y(n)`.
4. `e(n)` pasa por pasa-bajos, pasa-altos, derivada, cuadrado e integración por ventana móvil.
5. DAC1 entrega la salida MWI y DAC2 permite seleccionar una señal interna.
6. DMA circular y flags de mitad/completo implementan procesamiento ping-pong fuera de las ISR.

Parámetros principales:

| Parámetro | Valor |
|---|---:|
| `BLOCK_SIZE` | 32 |
| `BUFFER_SIZE` | 64 |
| `LMS_ORDER` | 32 |
| `MWI_SIZE` | 30 |
| Representación DSP | `float32_t` |

Alcance algorítmico: se implementa el preprocesamiento característico de Pan–Tompkins hasta la integración móvil. No se observó lógica completa de umbral adaptativo, clasificación de picos, período refractario ni cálculo de frecuencia cardíaca. Debe describirse como cadena parcial y no como detector QRS completo.

Problemas de reproducibilidad:

- Todo `PDS_Lab_Final/` está sin seguimiento de Git.
- `lab1.ioc` solo declara ADC1 y DAC1; no representa ADC2, DAC2 ni todos sus DMA usados por los fuentes.
- `.cproject` incluye `Core/Inc` pero no `Lib/Include`, declara solo `Core` y `Drivers` como rutas fuente y mantiene `-larm_cortexM4l_math`.
- `Lib/Source` contiene `arm_lms_f32.c` y `arm_lms_init_f32.c`, pero esa carpeta no forma parte de las entradas fuente persistidas.
- El proyecto compila para FPU `fpv4-sp-d16`; una biblioteca estática debe coincidir con su ABI hard-float.
- El ELF existente se llama `lab1_dsp.elf`, fue generado el 10 de junio y es anterior al `main.c` modificado el 19 de junio.

Evaluación: la implementación fuente está avanzada, pero no hay evidencia suficiente de que la revisión actual compile o funcione en placa. Este es el mayor riesgo técnico del repositorio.

## 4. Problemas transversales

### Críticos

1. **Trabajo importante sin versionar.** Una limpieza de carpeta, cambio de equipo o fallo del disco puede perder el proyecto final y resultados de Lab 3.
2. **Build final no trazable.** Configuración, `.ioc`, fuentes CMSIS-DSP y artefactos no describen una misma revisión coherente.
3. **Resultados sin evidencia suficiente.** Varias cifras de los informes aparecen como experimentales sin figura, log o método asociado.

### Altos

4. **README obsoleto.** No menciona el proyecto final y sigue marcando Lab 3/Lab 4 como faltantes.
5. **Ausencia de `.gitignore`.** Se mezclan fuentes con `Debug/`, objetos, cachés y entornos locales.
6. **Identidad heredada.** Lab 4 conserva archivos `lab2*`; el proyecto final conserva `lab1*`. Esto aumenta el riesgo de importar, regenerar o depurar el proyecto equivocado.

### Medios

7. **Dependencias no documentadas.** Conviven librerías estáticas y fuentes CMSIS-DSP sin una política común.
8. **Fuente canónica de informes poco clara.** Los `.tex` de Lab 3 y Lab 4 viven dentro de `Informe_Latex/build/`, nombre que normalmente identifica salidas generadas.
9. **Sin automatización de verificación.** No hay un procedimiento raíz que reconstruya informes o confirme compilaciones.

## 5. Correcciones respecto de la revisión anterior

| Afirmación anterior | Estado actual verificado |
|---|---|
| “Lab 3 sin informe” | Incorrecto: existe un informe sustancial de 735 líneas; faltan conclusiones y evidencia gráfica. |
| “Lab 4 sin informe” | Incorrecto: existe un informe sustancial de 537 líneas; falta trazabilidad experimental. |
| “Lab 2 usa IIR biquad” | Incorrecto para el fuente actual: usa `arm_fir_q15`. |
| “Repositorio limpio” | Incorrecto: hay múltiples archivos y un directorio completo sin versionar. |
| “30+ commits” | El conteo local actual de `HEAD` es 20. |
| “Falta Lab 5” como defecto | No hay evidencia local de que Lab 5 sea una entrega requerida; no debe tratarse como problema sin consigna. |
| “Lab 4 es el proyecto final” | Ya existe `PDS_Lab_Final`, distinto de `PDS_Lab_4`. |

## 6. Recomendación

No conviene comenzar por renombrar carpetas ni por CI/CD. El orden técnico correcto es:

1. preservar en Git las fuentes y resultados actuales;
2. hacer reproducible el build de `labfinal_dsp` y alinear su `.ioc`;
3. validar el firmware vigente en placa;
4. completar evidencia y conclusiones de los informes;
5. actualizar README, dependencias y nombres heredados;
6. recién después evaluar automatización o reorganización estructural.

El detalle operativo y los criterios de cierre están en `PLAN_DE_ACCION.md`; la síntesis está en `RESUMEN_EXPRESS.md`.
