# Plan de acción del repositorio

**Repositorio:** `procesamiento_digital_s`

**Actualizado:** 19 de junio de 2026
**Criterio:** primero asegurar reproducibilidad y evidencia; después mejorar presentación.

## Objetivo inmediato

Dejar los cuatro laboratorios y el proyecto final identificables, compilables desde una copia limpia del repositorio y respaldados por informes cuyos resultados puedan trazarse a mediciones o artefactos concretos.

**Estado confirmado:** Lab 1 y Lab 2 están completos. No requieren tareas de cierre académico; solo deben preservarse al mejorar la documentación y la reproducibilidad general del repositorio.

## Prioridad 0 — Preservar y hacer reproducible el trabajo actual

### 1. Incorporar el trabajo que todavía no está versionado

El árbol de trabajo no está limpio. Git informa como no versionados:

- todo `PDS_Lab_Final/`, incluido el proyecto STM32CubeIDE, la propuesta y el ejemplo MATLAB;
- las imágenes, scripts Python y `Recepcion_FFT.m` agregados en `PDS_Lab_3/`.

Acciones:

- [ ] Revisar que no haya archivos temporales o binarios de compilación dentro de lo que se agregará.
- [ ] Crear un `.gitignore` específico para STM32CubeIDE, Python y LaTeX.
- [ ] No ignorar `.project`, `.cproject`, `.ioc`, fuentes, informes, imágenes de resultados ni librerías requeridas por los proyectos.
- [ ] Agregar primero fuentes y configuración; agregar los resultados experimentales en un commit separado.
- [ ] Confirmar con `git status --short` que no queden archivos importantes fuera del seguimiento.

Resultado esperado: una clonación nueva contiene el código fuente, la configuración del IDE y los recursos necesarios para reconstruir cada entrega.

### 2. Corregir la configuración de compilación de `labfinal_dsp`

El código actual de `PDS_Lab_Final/labfinal_dsp/Core/Src/main.c` implementa dos ADC, dos DAC, LMS y una cadena parcial de Pan–Tompkins. Sin embargo, la configuración persistida todavía conserva elementos del proyecto base:

- `lab1.ioc` describe ADC1 y DAC1, pero no refleja ADC2 ni DAC2;
- `.cproject` enlaza `arm_cortexM4l_math` y solo declara `Core` y `Drivers` como rutas fuente;
- `Lib/Source` contiene `arm_lms_f32.c` y `arm_lms_init_f32.c`, pero esa carpeta no figura como fuente del proyecto;
- los artefactos `Debug/lab1_dsp.elf` son anteriores al `main.c` actual.

Acciones en STM32CubeIDE:

- [ ] Definir una única estrategia CMSIS-DSP: biblioteca compatible con hard-float o fuentes de `Lib/Source`.
- [ ] Opción recomendada: agregar `Lib/Include` a los includes y compilar `arm_lms_f32.c` y `arm_lms_init_f32.c` con el mismo ABI `fpv4-sp-d16`/hard-float del proyecto.
- [ ] Retirar el enlace a `arm_cortexM4l_math` si se compilan esas funciones desde fuente.
- [ ] Sincronizar el `.ioc` con ADC2/PA1, DMA2 Stream 2, DAC2 y DMA1 Stream 6 antes de volver a generar código.
- [ ] Verificar que una regeneración de CubeMX no elimine las modificaciones de `adc.c`, `dac.c`, `dma.c` o `main.c`.
- [ ] Ejecutar `Project > Clean` y luego `Build Project`.
- [ ] Registrar el log de compilación limpio y el tamaño final de Flash/RAM.

Resultado esperado: el ELF se genera desde el código actual sin referencias indefinidas a `arm_lms_f32` ni incompatibilidades ABI.

### 3. Validar el proyecto final en placa

- [ ] Confirmar la frecuencia real de muestreo producida por TIM2.
- [ ] Verificar que ADC1 y ADC2 avanzan sincronizados en ambos semibuffers DMA.
- [ ] Medir las salidas DAC1 y DAC2 y documentar qué señal expone cada selección.
- [ ] Comprobar convergencia del LMS con ECG y ruido correlacionado.
- [ ] Validar cada etapa Pan–Tompkins: pasa-bajos, pasa-altos, derivada, cuadrado e integración móvil.
- [ ] Aclarar que la implementación actual entrega la señal integrada; todavía no implementa umbral adaptativo ni decisión de picos R.
- [ ] Guardar capturas, parámetros, señal de entrada y criterio de aceptación.

## Prioridad 1 — Cerrar las entregas académicas

### 4. Finalizar el informe de Lab 3

`PDS_Lab_3/Informe_Latex/build/pds_lab3.tex` ya no está vacío: contiene 735 líneas, teoría, implementación y una sección de resultados. Queda pendiente convertirlo en una entrega respaldada por evidencia.

- [ ] Completar las tres subsecciones vacías de conclusiones.
- [ ] Insertar y explicar `Imagen1_lab3_220hz.png`, `Imagen1_lab3_920hz.png` e `Imagen2_lab3_920hz.png`.
- [ ] Relacionar cada figura con `fs`, tamaño FFT, resolución espectral y señal aplicada.
- [ ] Verificar experimentalmente los tiempos de 2,5/5,2/11,3 ms escritos en el informe; corregirlos o indicar el método DWT usado.
- [ ] Explicar el protocolo UART implementado y vincularlo con `Recepcion_FFT.m` o los scripts Python.
- [ ] Compilar el documento desde una ruta fuente inequívoca y conservar el PDF entregable.

### 5. Revisar el informe de Lab 4

`PDS_Lab_4/Informe_Latex/build/pds_lab4.tex` contiene 537 líneas y documenta el cancelador LMS. No debe seguir figurando como “informe vacío”.

- [ ] Incorporar figuras de convergencia, señal contaminada y error del LMS; hoy solo se incluye el logo.
- [ ] Sustituir estimaciones de ciclos, uso de CPU y tiempo de convergencia por mediciones reproducibles, o marcarlas explícitamente como valores teóricos.
- [ ] Verificar que nombres, tamaños y parámetros coincidan con el `main.c` actual: buffer 512, FIR de contaminación de 30 taps y LMS de 60 taps.
- [ ] Diferenciar claramente este laboratorio —ruido sintético y LMS— del nuevo proyecto final —dos ADC, LMS y Pan–Tompkins para ECG—.

### 6. Crear la documentación del proyecto final

- [ ] Crear un informe o memoria técnica para `PDS_Lab_Final`.
- [ ] Documentar el diagrama de señales: ADC1 = ECG contaminado `d(n)`, ADC2 = referencia `x(n)`, LMS, error `e(n)` y etapas Pan–Tompkins.
- [ ] Especificar `BLOCK_SIZE = 32`, `LMS_ORDER = 32`, `MWI_SIZE = 30`, tasa de muestreo y factor de adaptación `mu`.
- [ ] Registrar el pinout efectivo desde los fuentes y desde el `.ioc` ya sincronizado.
- [ ] Comparar el firmware con `Procesaciento_ECG_ejemplo.m` e indicar qué etapas del algoritmo completo aún no fueron portadas.
- [ ] Añadir resultados de placa, limitaciones y trabajo futuro.

## Prioridad 2 — Mejorar mantenimiento y acceso

### 7. Reemplazar el README raíz

`README.txt` tiene solo cinco líneas y ya está desactualizado.

- [ ] Crear `README.md` con propósito, estructura y estado verificable de Lab 1–4 y `PDS_Lab_Final`.
- [ ] Incluir instrucciones de importación y compilación en STM32CubeIDE.
- [ ] Documentar hardware, herramientas y dependencias sin inventar versiones no verificadas.
- [ ] Incluir una tabla de entradas, salidas y funciones de cada proyecto.
- [ ] Eliminar la referencia a “Lab 5” salvo que exista una consigna que lo requiera.

### 8. Limpiar configuraciones heredadas

- [ ] Probar cada `.launch` antes de borrarlo.
- [ ] En Lab 4, retirar configuraciones `lab2*` solo después de conservar una configuración funcional de `lab4_dsp`.
- [ ] En el proyecto final, renombrar proyecto, `.ioc`, `.launch` y artefactos que aún dicen `lab1`, manteniendo consistencia interna.
- [ ] Eliminar de Git los directorios `Debug/` generados después de confirmar que una compilación limpia los reconstruye.
- [ ] No reorganizar masivamente `Informe_Latex` o `lab*_dsp`: el costo de romper rutas supera el beneficio actual.

### 9. Documentar dependencias reales

- [ ] Registrar versión de STM32CubeIDE y toolchain usada por el grupo.
- [ ] Identificar la versión de CMSIS-DSP incluida en cada laboratorio.
- [ ] Documentar el ABI de punto flotante de cada proyecto.
- [ ] Explicar qué laboratorios usan bibliotecas precompiladas y cuáles compilan fuentes CMSIS-DSP.

## Criterio de cierre

El repositorio puede considerarse listo cuando:

- [ ] `git status` no muestra fuentes o resultados importantes sin versionar;
- [ ] cada proyecto compila desde una clonación limpia;
- [ ] el proyecto final compila el `main.c` vigente y su `.ioc` coincide con los periféricos usados;
- [ ] las mediciones citadas en los informes tienen figura, configuración y método de obtención;
- [ ] README y documentación distinguen Lab 4 de `PDS_Lab_Final`;
- [ ] la validación en placa está registrada y no se infiere solo por la presencia de un ELF.
