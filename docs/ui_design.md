# Especificación de Interfaz de Usuario (UI/UX)

Este documento define la guía visual y el comportamiento de la interfaz, incluyendo prototipos funcionales renderizados directamente en el navegador.

## 1. Filosofía de Diseño
- **Estética**: "Cyber-Security" / Minimalismo Industrial.
- **Paleta de Colores**: Dark Mode obligatoria para reducir fatiga visual en centros de comando.
- **Feedback**: Respuesta visual inmediata (< 16ms) a eventos del sensor.

### 1.1 Identidad de Marca (Brand Concept)
El logotipo evoca precisión biométrica y seguridad perimetral.

<div style="background: #0f0f15; padding: 30px; border-radius: 8px; text-align: center; margin-bottom: 20px;">
    <!-- CSS Logo Construction -->
    <div style="width: 80px; height: 80px; margin: 0 auto; position: relative;">
        <div style="width: 100%; height: 100%; border: 4px solid #00ff00; border-radius: 15px; transform: rotate(45deg);"></div>
        <div style="position: absolute; top: 20px; left: 20px; width: 40px; height: 40px; background: #00ff00; border-radius: 50%; box-shadow: 0 0 15px #00ff00;"></div>
        <div style="position: absolute; bottom: -10px; right: -10px; color: white; font-family: monospace; font-size: 10px;">AUTH</div>
    </div>
    <h1 style="font-family: 'Courier New', monospace; color: white; letter-spacing: 5px; margin-top: 15px; text-transform: uppercase;">QtFace<span style="color: #00ff00;">Auth</span></h1>
    <p style="color: #666; font-size: 12px; font-family: monospace;">SECURE ACCESS TERMINAL v1.0</p>
</div>

## 2. Galería de Estados (Live Mockups)

A continuación se presentan las visualizaciones de los estados críticos del sistema.

### 2.0 Pantalla de Carga (Boot Sequence)
Simulación de inicialización de sistemas críticos (Hardware check).

<div class="mockup-container" style="background-color: #000; padding: 20px; border-radius: 10px; color: #00ff00; font-family: 'Consolas', monospace; font-size: 12px; text-align: left; height: 300px; overflow: hidden; border: 2px solid #333;">
    <p>> SYSTEM_INIT...</p>
    <p>> CHECKING_MEMORY_INTEGRITY... <span style="color: white;">[OK]</span></p>
    <p>> LOADING_BIOMETRIC_ENGINE (DeepFace)... <span style="color: white;">[OK]</span></p>
    <p>> CONNECTING_CAMERA_DEVICE (Index 0)... <span style="color: white;">[OK]</span></p>
    <p>> VERIFYING_ENCRYPTION_KEYS (AES-256)... <span style="color: white;">[LOCKED]</span></p>
    <p>> ESTABLISHING_SECURE_TUNNEL... <span style="color: white;">[DONE]</span></p>
    <br>
    <p style="color: cyan;">> READY_FOR_AUTH.</p>
    <div style="width: 10px; height: 20px; background: #00ff00; animation: blink 1s infinite;"></div>
</div>

### 2.1 Estado: En Espera (Idle)
Pantalla limpia esperando presencia del usuario.

<div class="mockup-container" style="background-color: #1e1e2e; padding: 40px; border-radius: 10px; color: white; text-align: center; margin-bottom: 20px; border: 1px solid #2e2e3e;">
    <div style="font-size: 60px; margin-bottom: 20px;">🛡️</div>
    <h2 style="margin: 0;">SISTEMA DE ACCESO SEGURO</h2>
    <p style="color: #a6accd; margin-bottom: 30px;">Solo Personal Autorizado</p>
    <button style="background: transparent; border: 2px solid #00ff00; color: #00ff00; padding: 10px 30px; font-size: 16px; border-radius: 5px; cursor: pointer; font-weight: bold; text-transform: uppercase;">Iniciar Escaneo</button>
</div>

### 2.2 Estado: Alerta / Guía (Feedback)
El sistema detecta el rostro pero requiere corrección (mala luz, muy lejos).

<div class="mockup-container" style="background-color: #1e1e2e; padding: 20px; border-radius: 10px; position: relative; overflow: hidden; height: 350px;">
    <!-- Red Frame -->
    <div style="width: 200px; height: 200px; border: 3px solid #ff0055; border-radius: 20px; margin: 40px auto; position: relative; display: flex; align-items: center; justify-content: center; box-shadow: 0 0 30px rgba(255, 0, 85, 0.3);">
        <p style="color: rgba(255,255,255,0.3);">VIDEO FEED</p>
    </div>
    
    <!-- Warning Overlay -->
    <div style="position: absolute; bottom: 80px; left: 0; width: 100%; text-align: center;">
        <span style="background-color: rgba(255, 0, 85, 0.2); border: 1px solid #ff0055; color: #ff0055; padding: 8px 16px; border-radius: 4px; font-weight: bold;">
            ⚠️ MUY OSCURO - ACÉRQUESE
        </span>
    </div>
</div>

### 2.3 Estado: Prueba de Vida (Challenge)
Desafío aleatorio para anti-spoofing.

<div class="mockup-container" style="background-color: #1e1e2e; padding: 20px; border-radius: 10px; position: relative; overflow: hidden; height: 350px;">
    <!-- Yellow Frame -->
    <div style="width: 200px; height: 200px; border: 3px dashed #ffaa00; border-radius: 50%; margin: 40px auto; position: relative; display: flex; align-items: center; justify-content: center;">
        <div style="font-size: 40px;">😐 ➔ 😉</div>
    </div>
    
    <div style="text-align: center; color: #ffaa00;">
        <h3 style="margin: 0; text-transform: uppercase;">Prueba de Vida</h3>
        <p style="font-size: 20px; font-weight: bold;">GUIÑE EL OJO DERECHO</p>
        <div style="width: 200px; height: 4px; background: #333; margin: 10px auto; border-radius: 2px;">
            <div style="width: 70%; height: 100%; background: #ffaa00;"></div>
        </div>
    </div>
</div>

### 2.4 Estado: Acceso Concedido (Success)
Validación biométrica exitosa.

<div class="mockup-container" style="background-color: #1e1e2e; padding: 20px; border-radius: 10px; text-align: center; border: 2px solid #00ff00;">
    <div style="width: 100px; height: 100px; background: #00ff00; border-radius: 50%; margin: 40px auto; display: flex; align-items: center; justify-content: center; color: black; font-size: 50px;">
        ✓
    </div>
    <h2 style="color: #00ff00;">ACCESO CONCEDIDO</h2>
    <p style="color: white; font-size: 18px;">Bienvenido, <strong>Oficial Anderson</strong></p>
    <p style="color: #666; font-size: 14px;">Nivel de Seguridad: A1 | 12:45 PM</p>
</div>

### 2.5 Estado: Ataque Detectado (Spoofing Alert)
El sistema identifica un intento de suplantación (foto o máscara).

<div class="mockup-container" style="background-color: #2b0000; padding: 20px; border-radius: 10px; text-align: center; border: 4px solid #ff0000; position: relative;">
    <!-- Siren Animation -->
    <div style="position: absolute; top: 10px; right: 10px; font-size: 20px;">🚨</div>
    <div style="position: absolute; top: 10px; left: 10px; font-size: 20px;">🚨</div>

    <div style="width: 120px; height: 120px; border: 4px solid #ff0000; margin: 30px auto; position: relative; background: url('https://placehold.co/100x100/330000/ff0000?text=FAKE'); background-size: cover; filter: grayscale(100%) contrast(200%);">
        <div style="position: absolute; top: 0; left: 0; width: 100%; height: 100%; background: repeating-linear-gradient(45deg, transparent, transparent 10px, rgba(255,0,0,0.3) 10px, rgba(255,0,0,0.3) 20px);"></div>
    </div>

    <h2 style="color: #ff0000; font-family: 'Impact', sans-serif; letter-spacing: 2px;">AMENAZA DETECTADA</h2>
    <div style="background: #ff0000; color: white; display: inline-block; padding: 5px 15px; font-weight: bold; border-radius: 4px; margin-bottom: 10px;">SPOOFING (PRINT ATTACK)</div>
    <p style="color: #ffaaaa; font-size: 14px;">Protocolo de seguridad activado.<br>Se ha notificado a administración.</p>
    <p style="color: #880000; font-size: 10px; margin-top: 20px;">INCIDENT ID: #SP-99283-X</p>
</div>

## 3. Simulación de Flujo Completo (Demo)

A continuación, una **secuencia automática** que simula el ciclo de vida completo de una autenticación exitosa.

<div class="demo-container">
    <!-- SCANNER EFFECT (The one you liked) -->
    <div class="scan-line"></div>
    <div class="overlay-grid"></div>

    <!-- STAGE 1: BOOT -->
    <div class="stage boot-stage">
        <div class="terminal-text">
            > SYSTEM_BOOT...<br>
            > CHECKING_SENSORS... [OK]<br>
            > LOADING_MODELS... [OK]<br>
            > READY.
        </div>
    </div>

    <!-- STAGE 2: IDLE -->
    <div class="stage idle-stage">
        <div class="logo-box">
            <div class="glitch-logo">QtFaceAuth</div>
            <div class="sub-text">SECURE ACCESS</div>
            <div class="blink-text">ESPERANDO SUJETO...</div>
        </div>
    </div>

    <!-- STAGE 3: SCANNING -->
    <div class="stage scan-stage">
        <div class="face-bracket tl"></div>
        <div class="face-bracket tr"></div>
        <div class="face-bracket bl"></div>
        <div class="face-bracket br"></div>
        
        <div class="scan-status">
            ANALIZANDO BIOMETRÍA...<br>
            <span style="color: cyan;">CONFIDENCE: 98.4%</span>
        </div>
    </div>

    <!-- STAGE 4: SUCCESS -->
    <div class="stage success-stage">
        <div class="success-icon">✓</div>
        <h2 style="margin: 10px 0;">ACCESO AUTORIZADO</h2>
        <p>BIENVENIDO, USUARIO</p>
    </div>
</div>

<style>
.demo-container {
    width: 600px;
    height: 400px;
    background: #000;
    margin: 0 auto;
    border: 4px solid #333;
    border-radius: 10px;
    position: relative;
    overflow: hidden;
    font-family: 'Consolas', monospace;
    color: #00ff00;
    box-shadow: 0 0 50px rgba(0, 255, 0, 0.1);
}

/* SCANNER & GRID FX */
.scan-line {
    position: absolute;
    top: 0; left: 0; width: 100%; height: 4px;
    background: cyan;
    box-shadow: 0 0 15px cyan;
    animation: scanMove 3s infinite linear;
    z-index: 10;
    opacity: 0.5;
}
.overlay-grid {
    position: absolute;
    top: 0; left: 0; width: 100%; height: 100%;
    background: repeating-linear-gradient(0deg, transparent, transparent 1px, rgba(0, 50, 0, 0.3) 2px);
    z-index: 9;
    pointer-events: none;
}

/* STAGES ANIMATION CONTROLLER */
.stage {
    position: absolute;
    top: 0; left: 0; width: 100%; height: 100%;
    display: flex;
    flex-direction: column;
    align-items: center;
    justify-content: center;
    background: black;
    opacity: 0;
    animation: sequence 12s infinite;
}

/* TIMELINE: 
   0-3s: BOOT
   3-6s: IDLE
   6-9s: SCAN
   9-12s: SUCCESS
*/

.boot-stage { animation-delay: 0s; }
.idle-stage { animation-delay: 3s; }
.scan-stage { animation-delay: 6s; }
.success-stage { animation-delay: 9s; border: 2px solid #00ff00; background: #001100; }

@keyframes sequence {
    0% { opacity: 0; z-index: 0; }
    5% { opacity: 1; z-index: 5; }
    25% { opacity: 1; z-index: 5; }
    30% { opacity: 0; z-index: 0; }
    100% { opacity: 0; z-index: 0; }
}

@keyframes scanMove {
    0% { top: -10%; }
    100% { top: 110%; }
}

/* STAGE SPECIFIC STYLES */
.face-bracket {
    position: absolute;
    width: 30px; height: 30px;
    border: 3px solid cyan;
}
.tl { top: 100px; left: 200px; border-right: 0; border-bottom: 0; }
.tr { top: 100px; right: 200px; border-left: 0; border-bottom: 0; }
.bl { bottom: 100px; left: 200px; border-right: 0; border-top: 0; }
.br { bottom: 100px; right: 200px; border-left: 0; border-top: 0; }

.success-icon {
    font-size: 80px;
    color: #00ff00;
    border: 4px solid #00ff00;
    border-radius: 50%;
    width: 120px; height: 120px;
    display: flex; align-items: center; justify-content: center;
}
</style>

## 4. Elementos de Interfaz (UI Kit)

Componentes reutilizables diseñados para alta visibilidad.

| Componente | Estado Normal | Estado Alerta | Estado Éxito |
| :--- | :--- | :--- | :--- |
| **Marco Facial** | Color `Cyan` (Buscando) | Color `Rojo` (Error) | Color `Verde` (OK) |
| **Mensajes** | Texto Blanco | Fondo Rojo/Texto Rojo | Fondo Verde/Texto Negro |
| **Botones** | Borde `Cyan` | Deshabilitado (Gris) | Borde `Verde` |

## 4. Diagrama de Flujo de Interacción
Flujo lógico de los estados visuales que experimentará el usuario.

```mermaid
stateDiagram-v2
    [*] --> Idle: Inicio del Sistema
    Idle --> Scanning: Detección de Rostro
    
    state Scanning {
        [*] --> Analyzing
        Analyzing --> QualityCheck: Verificar Luz/Posición
        QualityCheck --> Analyzing: OK
        QualityCheck --> Warning: Mala Iluminación
        Warning --> Analyzing: Corrección Usuario
    }
    
    Scanning --> Success: Similitud > 85%
    Scanning --> LivenessFail: Fallo Prueba Vida
    Scanning --> AuthFail: Usuario Desconocido
    
    Success --> [*]: Acceso Concedido
    LivenessFail --> Locked: Bloqueo de Seguridad
    AuthFail --> Idle: Reintento (Max 3)
```

## 4. Componentes Visuales (Design Tokens)

| Token | Valor HEX | Uso |
| :--- | :--- | :--- |
| `Primary` | `#00FF00` (Neon Green) | Éxito, Liveness OK, Bordes Activos |
| `Error` | `#FF0055` (Neon Red) | Fallo, Alerta de Spoofing |
| `Background` | `#1E1E2E` | Fondo Principal |
| `Surface` | `#2E2E3E` | Paneles, Tarjetas |
| `TextPrimary` | `#FFFFFF` | Títulos, Texto Principal |
