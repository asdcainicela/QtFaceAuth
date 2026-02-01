# Plan de Pruebas y Aseguramiento de Calidad (QA)

Guía para la validación funcional y de seguridad (Pen-Testing) del sistema QtFaceAuth antes del despliegue.

## 1. Matriz de Pruebas Unitarias

| ID | Componente | Prueba | Entrada Esperada | Resultado Esperado |
| :--- | :--- | :--- | :--- | :--- |
| `UT-01` | **Cifrado** | Encriptar vector con AES-256 | `std::vector<float>` | `QByteArray` cifrado (diferente al original). |
| `UT-02` | **Base de Datos** | Soft Delete | User ID válido | `is_deleted = 1`, el registro persiste en disco. |
| `UT-03` | **Cámara** | Desconexión súbita | Retirar USB | Señal `feedbackUpdated("Error Hardware")` en < 500ms. |

## 2. Pruebas de Penetración (Anti-Spoofing Performance)

Estas pruebas deben realizarse en un entorno controlado con "Red Team" (atacantes simulados).

### 2.1 Ataque de Presentación Nivel A (Foto Impresa)
*   **Vector**: Foto de alta resolución impresa en papel mate y brillante.
*   **Procedimiento**: Colocar foto frente a cámara a distancias variables (30cm - 1m).
*   **Criterio de Éxito**: El sistema debe clasificar como `SPOOF_DETECTED` en el 100% de casos mediante análisis de textura.

### 2.2 Ataque de Presentación Nivel B (Replay Video)
*   **Vector**: Tablet/Smartphone reproduciendo video del usuario parpadeando.
*   **Procedimiento**: Reproducir video HD.
*   **Criterio de Éxito**: Detección de patrones de refresco (Moiré) o fallo en desafío activo aleatorio (ej. sistema pide "Gire Izquierda", video sigue de frente).

### 2.3 Ataque de Inyección de Señal
*   **Vector**: Cámara virtual (OBS) inyectando feed directo al driver.
*   **Defensa**: El módulo de hardware verificará la firma digital del dispositivo USB (VID/PID estricto).

## 3. Criterios de Aceptación (Go/No-Go)

Para liberar la versión 1.0, el sistema debe cumplir:

*   [ ] **Falsos Positivos (FAR)**: < 0.001% (1 en 100,000).
*   [ ] **Falsos Negativos (FRR)**: < 2% (Rechazo a usuario legítimo).
*   [ ] **Tiempo de Respuesta**: < 1.5 segundos desde detección hasta apertura.
*   [ ] **Uptime**: Capacidad de funcionar 24/7 sin fugas de memoria (Memory Leaks).

---

## 4. Herramientas de Testing Recomendadas
*   **Catch2**: Para unit testing en C++.
*   **Valgrind**: Para detección de fugas de memoria.
*   **OBS Studio**: Para simulación de ataques de inyección de video.
