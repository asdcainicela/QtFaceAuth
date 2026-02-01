# QtFaceAuth - Arquitectura

Este documento proporciona un desglose técnico y visual del sistema QtFaceAuth.

## 1. System Overview
The system is divided into three main layers: UI, Logic (Auth/Face Engine), and Data.

```mermaid
graph TD
    A[User Interface - Qt 6] --> B[Auth Service]
    B --> C[Face Engine - OpenCV/Dlib]
    B --> D[Database Manager - SQLite]
    C --> E[Webcam / Camera Stream]
```

## 2. Authentication Flow (Face Login)
The following sequence diagram illustrates the real-time face authentication process.

```mermaid
sequenceDiagram
    participant U as User
    participant UI as Qt Interface
    participant CW as Camera Worker
    participant FE as Face Engine
    participant DB as SQLite Database

    U->>UI: Opens Login Screen
    UI->>CW: Start Acquisition
    loop Real-time Stream
        CW->>FE: Send Image Frame
        FE->>FE: Detect Face & Extract Embedding
        alt Face Found
            FE->>DB: Query for Match
            DB-->>FE: Return Result (UserID / Similarity)
            FE->>UI: Signal Match (Success/Fail)
        else No Face
            FE-->>UI: Update View (Scanning...)
        end
    end
    UI->>U: Grant/Deny Access
```

## 3. User Management (CRUD)
The logical flow for registering or deleting a user.

```mermaid
graph LR
    Start([Start]) --> Mode{Select Mode}
    Mode -- Registration --> Capture[Capture Biometric Data]
    Capture --> Valid{Valid?}
    Valid -- Yes --> Save[Save to SQLite]
    Save --> End([End])
    Valid -- No --> Capture
    
    Mode -- Deletion --> Search[Search User]
    Search --> Found{Found?}
    Found -- Yes --> Delete[Remove Record]
    Delete --> End
    Found -- No --> End
```

## 4. Component Responsibilities
| Component | Responsibility |
| :--- | :--- |
| **Qt Interface** | Hardware-accelerated GUI, user feedback, and window management. |
| **Camera Worker** | Asynchronous frame capture in a separate thread to prevent UI freezing. |
| **Face Engine** | Native C++ implementation of Dlib/OpenCV for feature extraction. |
| **Database Manager** | Securely handles user profiles and biometric signatures via QSql. |

---
*Desarrollado por asdcainicela*
