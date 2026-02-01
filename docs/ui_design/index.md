# Mapa de Navegación (Prototipos)

Estructura de menús disponible en ambas orientaciones (Vertical/Horizontal).

```mermaid
graph TD
    Login[Pantalla Principal <br> (Face Scan)]
    
    subgraph "Menús de Gestión"
        Login -->|Botón Menú| Dashboard[Panel de Control]
        Dashboard --> Users[👥 Gestión de Usuarios]
        Dashboard --> Logs[📜 Logs de Auditoría]
        Dashboard --> Settings[⚙️ Configuración]
    end
    
    subgraph "Variantes de Tema"
        Settings -->|Toggle| DarkMode[Modo Dark (Industrial)]
        Settings -->|Toggle| LightMode[Modo Light (Clínico)]
    end
```
