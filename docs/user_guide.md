# Manual de Operación y Uso

Guía de referencia para operadores y administradores del sistema **QtFaceAuth**.

## 1. Inicio de Sesión (Operador)

El proceso de autenticación es completamente desasistido ("Hands-free").

### Procedimiento
1.  Sitúese frente al quiosco terminal.
2.  Mantenga una pose neutral (sin gesticulaciones exageradas).
3.  Observe las guías visuales en pantalla:
    *   **Marco Verde**: Posición correcta.
    *   **Marco Amarillo**: Analizando...
    *   **Marco Rojo**: Error (iluminación o distancia).
4.  Si se solicita una **Prueba de Vida**, siga la instrucción en pantalla (ej. "Parpadee").

> **Nota**: No utilice gafas de sol oscuras ni mascarillas que cubran más del 50% del rostro.

## 2. Gestión de Usuarios (Administrador)

El panel de administración permite el enrolamiento y baja de personal.

### 2.1 Enrolamiento de Nuevo Personal
1.  Acceda al menú `Gestión > Nuevo usuario`.
2.  Ingrese los datos demográficos (ID, Nombre, Rol).
3.  Presione `Capturar Biometría`. El sistema tomará 5 muestras de alta calidad.
    *   *El sistema descartará automáticamente muestras borrosas o con ojos cerrados.*
4.  Confirme la operación para encriptar y guardar el vector.

### 2.2 Baja de Personal
1.  Busque al usuario por ID o Nombre.
2.  Seleccione `Revocar Acceso`.
    *   Esto realiza un **Soft Delete** (mantiene log de auditoría pero impide nuevos accesos).

## 3. Solución de Problemas (Troubleshooting)

| Síntoma | Causa Probable | Acción Correctiva |
| :--- | :--- | :--- |
| **"Cámara no detectada"** | Driver o conexión física | Verifique conexión USB y permisos de privacidad en Windows. |
| **Falsos Negativos (No reconoce)** | Iluminación extrema | Evite contraluz fuerte o oscuridad total. Ajuste la iluminación frontal. |
| **Bloqueo "Liveness Fail"** | Movimiento excesivo | Manténgase estático durante el análisis pasivo (1-2 seg). |
