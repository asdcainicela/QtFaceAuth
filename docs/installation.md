# Guía de Instalación y Despliegue

Este procedimiento técnico detalla los pasos para compilar, configurar y desplegar la solución **QtFaceAuth** en entornos de producción Windows (x64).

## 1. Requisitos del Sistema

### Hardware Mínimo
*   **CPU**: Intel Core i5 (8va Gen) o equivalente (soporte AVX2 requerido para DeepFace).
*   **RAM**: 8 GB DDR4.
*   **Cámara**: Dispositivo compatible con UVC (Resolución mín. 720p).

### Software Base
*   **OS**: Windows 10/11 Pro (Build 2004+).
*   **Runtime**: Microsoft Visual C++ Redistributable 2019+.
*   **Drivers**: Controladores de cámara actualizados (evitar drivers genéricos de Microsoft si es posible).

## 2. Preparación del Entorno de Desarrollo

### 2.1 Herramientas de Compilación
Se requiere la instalación de las siguientes herramientas en el nodo de compilación:

1.  **Qt 6.4+ SDK**: Componentes `Qt Quick`, `Qt Multimedia`, `Qt SQL`.
2.  **CMake 3.20+**: Sistema de meta-compilación.
3.  **Compilador MSVC v142/v143**: Incluido en Visual Studio 2019/2022 o Build Tools.

```powershell
# Verificar versiones instaladas
cmake --version
cl.exe
```

## 3. Compilación del Código Fuente

Siga estos pasos para generar el binario ejecutable.

### 3.1 Configuración
Generar los archivos de proyecto Makefiles/Ninja desde la raíz del repositorio:

```bash
mkdir build
cd build
cmake -G "Ninja" -DCMAKE_BUILD_TYPE=Release ..
```

### 3.2 Construcción
Compilar el proyecto utilizando el compilador nativo configurado:

```bash
cmake --build . --config Release
```

## 4. Despliegue (Deployment)

Para distribuir la aplicación a máquinas cliente sin Qt instalado, utilice la herramienta `windeployqt`.

```powershell
# Ejecutar desde la consola de Qt
cd build/Release
windeployqt --qmldir ../../src/qml appQtFaceAuth.exe
```

Esto copiará todas las DLLs (`Qt6Core.dll`, `Qt6Gui.dll`, plugins de imagen, sql drivers) en la carpeta del ejecutable.
