# Protocolos de Seguridad y Anti-Suplantación

Este documento detalla las medidas de seguridad de **Grado Militar** implementadas en QtFaceAuth para garantizar la integridad y confidencialidad del sistema. Cumple con los estándares **ISO/IEC 30107** (Biometric presentation attack detection).

## 1. Detección de Prueba de Vida (ISO/IEC 30107-3)

La solución implementa mecanismos de defensa contra ataques de presentación (PAD - Presentation Attack Detection) clasificados según el vector de ataque.

### 1.1 Análisis Biométrico Pasivo
El subsistema de visión computacional evalúa la autenticidad del sujeto mediante análisis espectral y de movimiento involuntario:
1.  **Micro-gestos**: Detección de variaciones sutiles en la musculatura facial y frecuencia de parpadeo natural (aprox. 15-20/min).
2.  **Análisis de Textura Cutánea**: Uso de redes neuronales convolucionales (CNN) para identificar patrones de Moiré (pantallas) o falta de dispersión de luz superficial (máscaras/papel).
3.  **Mapa de Profundidad**: Validación volumétrica mediante estereoscopía o sensores ToF (si el hardware lo soporta).

### 1.2 Desafío-Respuesta Activo
Protocolo de interacción aleatoria para mitigar ataques de inyección de video (Replay Attacks):
1.  **Estímulo**: El sistema genera una instrucción criptográficamente segura y efímera (ej. *"Giro cefálico: 90° Izquierda"*).
2.  **Ventana de Tiempo**: El sujeto debe completar la acción en `< 3000ms`.
3.  **Validación**: Se analiza la congruencia entre el flujo óptico y la instrucción solicitada.

---

## 2. Encriptación y Protección de Datos

### 2.1 Almacenamiento de Vectores (AES-256)
Los vectores biométricos (consecuencia matemática del rostro) **NUNCA** se guardan en texto plano.
- **Algoritmo**: AES-256-GCM (Authenticated Encryption).
- **Claves**: Gestión mediante **Key Wrapper** en memoria volátil. Las claves de desencriptado nunca se escriben en disco.
- **Hashing**: Los datos personales (nombre, ID) se hashean con Salt dinámico si no son necesarios para visualización.

### 2.2 Protección de Base de Datos
- **SQLCipher**: La base de datos SQLite está cifrada totalmente a nivel de página.
- **Anti-Tampering**: Checksum de integridad al inicio. Si la BD fue modificada externamente, el sistema entra en **Modo Bloqueo**.

---

## 3. Auditoría y Trazabilidad (Audit Logs)

Registro inmutable de eventos para análisis forense.

| Evento | Datos Registrados | Retención |
| :--- | :--- | :--- |
| **Acceso Exitoso** | UserID, Timestamp, Confianza (%), IP | 1 año |
| **Intento Fallido** | Captura del rostro (Snapshot), Motivo falla | 6 meses |
| **Spoofing Detectado** | **Alerta Crítica**, Snapshot, Vector de ataque | Permanente |
| **Admin Login** | Acciones realizadas, IP de origen | 5 años |

---

## 4. Hardware Soportado

Para alcanzar los niveles de seguridad más altos (EAL4+), se recomienda hardware certificado:
- **Cámara**: Sensor RGB + IR (Infrarrojo) para liveness en oscuridad total.
- **Procesador**: Soporte para instrucciones AVX2/NEON para inferencia local sin latencia.
