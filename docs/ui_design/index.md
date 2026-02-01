# Prototipo Interactivo (Master)

Este es el **Master Prototype**, una simulación viva de la interfaz final. Hemos unificado todas las variantes en una sola experiencia interactiva para que puedas probar la adaptabilidad del sistema en tiempo real.

!!! tip "Cómo interactuar con el demo"
    El prototipo superior es 100% funcional. Si lo abres, podrás probar:
    
    *   **🔄 Cambio de Diseño**: Alterna entre la vista de **Escritorio** (Horizontal) y **Kiosco** (Vertical).
    *   **🎨 Temas Dinámicos**: Prueba cómo se ve la interfaz en entornos oscuros (**Industrial**) vs claros (**Clínico**).
    *   **🖱️ Navegación Real**: Haz clic en el menú lateral para moverte entre el Escáner, la Base de Datos y los Logs.

<div style="background: linear-gradient(135deg, #1a1a1a 0%, #0d0d0d 100%); color: white; padding: 50px; border-radius: 12px; text-align: center; border: 1px solid #333; margin: 20px 0; box-shadow: 0 10px 30px rgba(0,0,0,0.5);">
    <div style="font-size: 40px; margin-bottom: 10px;">📱 🖥️</div>
    <h2 style="margin-top: 0; color: #fff;">Simulador de Interfaz HMI</h2>
    <p style="color: #bbb; max-width: 600px; margin: 0 auto 30px auto;">
        Experimente la interfaz real con transiciones fluidas entre modo <strong>Kiosco (Vertical)</strong> y <strong>Centro de Comando (Horizontal)</strong>. Incluye simulación de temas (Dark/Light) y navegación funcional.
    </p>
    <a href="../../prototypes/master.html" target="_blank" style="background: #00ff00; color: #000; padding: 15px 40px; border-radius: 30px; text-decoration: none; font-weight: 900; font-size: 16px; display: inline-block; transition: transform 0.2s; border: 2px solid #00ff00; text-transform: uppercase; letter-spacing: 1px;">
        ▶ Abrir Demo Interactivo
    </a>
    <p style="font-size: 12px; color: #666; margin-top: 15px;">Se abrirá en una nueva pestaña a pantalla completa.</p>
</div>

---

## Mapa de Navegación

El siguiente diagrama muestra la estructura lógica que se está ejecutando en el prototipo superior.

```mermaid
graph TD
    A[Pantalla Principal] -->|Menu| B[Panel Control]
    
    subgraph Gestion
        B --> C[Usuarios]
        B --> D[Logs]
        B --> E[Configuracion]
    end
    
    subgraph Opciones
        E --> F[Toggle Layout]
        E --> G[Toggle Tema]
    end
```
