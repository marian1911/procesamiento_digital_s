# 🚀 Plan de Acción - Mejoras del Repositorio

**Prioridad Total:** 3 semanas de trabajo

---

## 1️⃣ TAREA 1: Completar Informes LaTeX (Lab 3 y 4)

**Prioridad:** 🔴 CRÍTICA  
**Tiempo Estimado:** 5-7 horas  
**Responsable:** Estudiante/Grupo

### Lab 3 - Transformada Rápida de Fourier

**Archivo:** `PDS_Lab_3/Informe_Latex/pds_lab3.tex`

```latex
% AGREGAR DESPUÉS DEL PREÁMBULO:

\title{Laboratorio 3: Transformada Rápida de Fourier (FFT)}
\author{Grupo DSP}
\date{2026}

\begin{document}
\maketitle

\section{Introducción}
% Explicar qué es FFT, aplicaciones en DSP, objetivo del laboratorio

\section{Marco Teórico}
\subsection{Transformada de Fourier Discreta}
% DFT, propiedades, complejidad computacional

\subsection{Algoritmo FFT}
% Cooley-Tukey, butterfly diagram, ventanas

\section{Implementación}
\subsection{Hardware Utilizado}
% STM32F446RE, ADC, periféricos

\subsection{Software}
% Algoritmo FFT ARM CMSIS-DSP, configuración

\section{Resultados}
\subsection{Análisis de Espectros}
% Incluir gráficos de salidas FFT

\subsection{Características de Rendimiento}
% Tiempo de procesamiento, resolución de frecuencia

\section{Conclusiones}
% Resumen de resultados, limitaciones, aplicaciones futuras

\end{document}
```

**Gráficos necesarios:**
- [ ] Espectro de entrada vs salida
- [ ] Respuesta en frecuencia
- [ ] Resolución espectral para diferentes fs

**Checklist:**
- [ ] Sección de Introducción
- [ ] Marco teórico de FFT
- [ ] Descripción de implementación
- [ ] Resultados con gráficos
- [ ] Análisis de rendimiento
- [ ] Conclusiones

---

### Lab 4 - Proyecto Final

**Archivo:** `PDS_Lab_4/Informe_Latex/pds_lab4.tex`

**Estructura Similar al Lab 3:**
- [ ] Definir objetivo específico del proyecto
- [ ] Integración de técnicas de Lab 1, 2, 3
- [ ] Resultados medibles
- [ ] Gráficos comparativos
- [ ] Análisis de resultados

---

## 2️⃣ TAREA 2: Crear README.md Mejorado

**Prioridad:** 🟡 ALTA  
**Tiempo Estimado:** 2-3 horas  
**Responsable:** Cualquiera

**Ubicación:** Reemplazar `README.txt` por `README.md`

```markdown
# Procesamiento Digital de Señales - Laboratorios STM32F446RE

## 📖 Descripción General

Este repositorio contiene la implementación de laboratorios de Procesamiento Digital de Señales (PDS) 
usando el microcontrolador STM32F446RE (ARM Cortex-M4).

## 📚 Laboratorios Incluidos

### ✅ Laboratorio 1: Adquisición de Señales
- **Objetivo:** Captura y procesamiento de señales analógicas
- **Periféricos:** ADC, DAC, DMA, Timer
- **Estado:** Completo ✓

### ✅ Laboratorio 2: Filtrado Digital
- **Objetivo:** Diseño e implementación de filtros IIR
- **Periféricos:** ADC, DAC, DMA
- **Filtros:** LP, HP, BP, BS (Biquad Cascade)
- **Estado:** Completo ✓

### ⚠️ Laboratorio 3: Transformada de Fourier
- **Objetivo:** Análisis espectral con FFT
- **Periféricos:** ADC, UART, DMA
- **Estado:** Código OK, informe en progreso

### ⚠️ Laboratorio 4: Proyecto Final
- **Objetivo:** Integración de técnicas aprendidas
- **Estado:** Código OK, informe en progreso

## 🛠️ Requisitos del Sistema

### Hardware
- **Placa:** STM32F446RE Nucleo o similar
- **Periféricos:** Entrada analógica, salida de audio/DAC
- **Componentes adicionales:** Fuente de señal, analizador (opcional)

### Software
- **IDE:** STM32CubeIDE o similar
- **Compilador:** arm-none-eabi-gcc (v9.0+)
- **Herramientas:** STM32CubeMX (para reconstruir configuración)
- **Librerías:**
  - ARM CMSIS-DSP 1.13+
  - STM32F4xx HAL Driver 1.27+

## 📥 Instalación

### 1. Clonar Repositorio
\`\`\`bash
git clone https://github.com/marian1911/procesamiento_digital_s.git
cd procesamiento_digital_s
\`\`\`

### 2. Abrir en STM32CubeIDE
- Importar como "Existing Projects into Workspace"
- Seleccionar carpeta del laboratorio deseado (PDS_Lab_N/lab*_dsp)

### 3. Compilar
- Botón derecho en proyecto → Build Project
- O usar línea de comandos:
  \`\`\`bash
  cd PDS_Lab_1/lab1_dsp
  make
  \`\`\`

### 4. Programar Placa
- Conectar via USB
- Click en "Run" o Ctrl+F11 en STM32CubeIDE

## 📊 Estructura del Proyecto

\`\`\`
procesamiento_digital_s/
├── PDS_Lab_1/
│   ├── Informe_Latex/
│   └── lab1_dsp/          # Proyecto STM32CubeIDE
├── PDS_Lab_2/
│   ├── Informe_Latex/
│   └── lab2_dsp/
├── PDS_Lab_3/
│   ├── Informe_Latex/
│   └── lab3_dsp/
└── PDS_Lab_4/
    ├── Informe_Latex/
    └── lab4_dsp/
\`\`\`

## 🚀 Guía Rápida de Uso

### Lab 1: Adquisición
1. Compilar proyecto: `PDS_Lab_1/lab1_dsp`
2. Programar en placa
3. Entrada: Conectar señal a ADC (pin correspondiente)
4. Salida: Observar en DAC

### Lab 2: Filtrado
1. Compilar: `PDS_Lab_2/lab2_dsp`
2. Pulsador para cambiar tipo de filtro (LP/HP/BP/BS)
3. LED RGB indica filtro activo

### Lab 3: FFT
1. Compilar: `PDS_Lab_3/lab3_dsp`
2. UART para recibir datos de espectro
3. Terminal: `putty COM_PORT 115200`

## 🔧 Configuración Personalizada

### Cambiar Tasa de Muestreo
En `main.c`:
\`\`\`c
#define FS_8K   11249   // 8 kHz
#define FS_16K  5624    // 16 kHz
#define FS_48K  1874    // 48 kHz
\`\`\`

### Tamaño de Buffer
\`\`\`c
#define BUFFER_SIZE 512  // Cambiar según necesidad
\`\`\`

## 📝 Documentación

- **Informes:** Ver carpetas `Informe_Latex/` en cada laboratorio
- **Diagramas:** Ver `diagrama de flujo PDS_LAB1.drawio.png`

## 🐛 Resolución de Problemas

### Error de compilación: "arm_math.h no encontrado"
- Verificar inclusión de CMSIS-DSP en el proyecto
- Path: `Project Properties → C/C++ General → Paths and Symbols`

### Placa no responde
- Verificar conexión USB
- Reinstalar drivers ST-LINK

## 📞 Contacto y Soporte

- **Asignatura:** Procesamiento Digital de Señales
- **Institución:** Universidad Nacional de Córdoba

## 📄 Licencia

Este proyecto es solo con fines educativos.

---
**Última actualización:** 11/05/2026
```

**Checklist:**
- [ ] Reemplazar README.txt por README.md
- [ ] Incluir instrucciones de compilación
- [ ] Incluir guía de uso por laboratorio
- [ ] Incluir requisitos del sistema

---

## 3️⃣ TAREA 3: Crear .gitignore

**Prioridad:** 🟡 ALTA  
**Tiempo Estimado:** 0.5 horas  
**Responsable:** Cualquiera

**Archivo:** `.gitignore`

```
# Archivos de compilación
*.o
*.a
*.elf
*.bin
*.hex
*.list
*.map

# IDE STM32CubeIDE
Debug/
Release/
.settings/
.project
.cproject

# Archivos temporales
*.swp
*.swo
*~
.DS_Store

# Directorios de build
build/
dist/
*.d

# Archivos de línker generados
*.ld

# Caché
.vscode/
*.code-workspace

# LaTeX generados
*.pdf
*.aux
*.toc
*.fdb_latexmk
*.fls
*.log
*.out

# Directorios grandes innecesarios
.git/
node_modules/
```

**Checklist:**
- [ ] Crear archivo `.gitignore` en raíz
- [ ] Verificar que compile normal después
- [ ] Hacer commit

---

## 4️⃣ TAREA 4: Reorganizar Directorios

**Prioridad:** 🟢 BAJA  
**Tiempo Estimado:** 2 horas  
**Responsable:** Cualquiera

### Cambios Recomendados

```
Antes:
PDS_Lab_2/
├── Informe_Latex/
│   ├── Graficos/
│   └── build/

Después:
PDS_Lab_2/
├── reports/
│   ├── images/
│   ├── graphics/
│   └── build/
└── src/
    └── (código)
```

**Pasos:**
1. Renombrar `Informe_Latex` → `reports`
2. Mover `Graficos` → `reports/graphics`
3. Renombrar `lab*_dsp` → `src` (opcional)
4. Actualizar paths en `.gitignore` y documentación

**Beneficios:**
- Más legible en sistemas no-Windows
- Más profesional
- Compatible con CI/CD

---

## 5️⃣ TAREA 5: Documentación de Dependencias

**Prioridad:** 🟡 MEDIA  
**Tiempo Estimado:** 1 hora  
**Responsable:** Cualquiera

**Archivo:** `DEPENDENCIES.md`

```markdown
# Dependencias del Proyecto

## Versiones Testadas

| Software | Versión | Notas |
|----------|---------|-------|
| STM32CubeIDE | 1.12.0+ | O usar línea de comandos |
| arm-none-eabi-gcc | 10.3 | Incluido en STM32CubeIDE |
| ARM CMSIS-DSP | 1.13.0 | Incluido en repo |
| STM32F4xx HAL | 1.27.0 | Generado por CubeMX |

## Instalación de Herramientas

### Windows
1. Descargar STM32CubeIDE desde st.com
2. Instalar STM32CubeMX para reconstruir configuración

### Linux/Mac
\`\`\`bash
# Instalar compilador
sudo apt-get install arm-none-eabi-gcc
sudo apt-get install build-essential

# Instalar STM32CubeIDE (si disponible en tu región)
\`\`\`

## Instrucciones de Compilación

### STM32CubeIDE (GUI)
1. Abrir proyecto
2. Project → Clean
3. Project → Build Project

### Línea de comandos
\`\`\`bash
cd PDS_Lab_1/lab1_dsp
arm-none-eabi-gcc -c src/main.c -o main.o
# (Usar makefile generado)
make
\`\`\`

## Librerías Incluidas

- **CMSIS-DSP:** Procesamiento digital de señales (ARM)
- **STM32F4xx HAL:** Hardware Abstraction Layer
- **CMSIS Core:** CPU core

Todas incluidas en el repo bajo `Drivers/` y `Lib/`.
```

---

## 6️⃣ TAREA 6: Limpiar Archivos Duplicados

**Prioridad:** 🟢 MEDIA  
**Tiempo Estimado:** 1 hora  
**Responsable:** Cualquiera

**Archivos a eliminar:**

```
PDS_Lab_4/lab4_dsp/:
- lab2 Debug (1).launch    ❌ Eliminar
- lab2 Debug.launch         ❌ Eliminar
- lab2_dsp Debug.launch     ❌ Eliminar
- lab2.ioc                  ❌ Eliminar
✓ Mantener: lab4_dsp Debug.launch

PDS_Lab_2/lab2_dsp/:
- lab2 Debug (1).launch    ❌ Eliminar si hay duplicados
✓ Mantener: lab2_dsp Debug.launch
```

**Checklist:**
- [ ] Identificar y eliminar archivos obsoletos
- [ ] Verificar compilación después
- [ ] Hacer commit con mensaje "clean: remove duplicate debug configs"

---

## 📅 Cronograma Sugerido

```
Semana 1:
  Día 1-2: TAREA 1 (Completar informes)
  Día 3-4: TAREA 2 (README.md)
  Día 5: TAREA 3 (.gitignore)

Semana 2:
  Día 1-2: TAREA 4 (Reorganizar directorios)
  Día 3: TAREA 5 (Dependencias)
  Día 4-5: TAREA 6 (Limpiar duplicados)

Semana 3:
  Revisión final
  Testing completo
  Merge a main branch
```

---

## ✨ Beneficios Esperados

| Tarea | Beneficio |
|-------|-----------|
| Completar Informes | Documentación académica completa |
| README.md | Facilita onboarding de nuevos colaboradores |
| .gitignore | Repo más limpio, menos "ruido" |
| Reorganizar dirs | Mejor estructura, compatible con CI/CD |
| Dependencias | Reproducibilidad garantizada |
| Limpiar duplicados | Repo más mantenible |

---

## 🎯 Resultado Final Esperado

Después de completar estos pasos:

✅ Repositorio profesional y completo  
✅ Documentación académica exhaustiva  
✅ Fácil onboarding para nuevos desarrolladores  
✅ Listo para integración CI/CD  
✅ Puntuación estimada: **90/100**

---

**Generado automáticamente por GitHub Copilot**  
**Versión:** 1.0  
**Fecha:** 11/05/2026
