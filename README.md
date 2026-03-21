# Qt Face Authentication System

High-performance biometric authentication system built with C++ and Qt 6, designed for real-time face recognition in edge and industrial environments.

---

## 🚀 Features

* Real-time face detection and recognition
* Modular C++ architecture (Engine, Database, UI)
* Qt/QML-based user interface
* User enrollment and authentication system
* State-driven UI (idle, challenge, success, spoof detection)
* Low-latency and high-performance execution

---

## 🧠 System Architecture

Camera Input → Face Engine → Recognition → Authentication → UI Feedback

### Core Components

* **FaceEngine** → Detection and recognition pipeline
* **UserManager** → User enrollment and management
* **DatabaseManager** → Persistent storage
* **UI (QML)** → Interactive authentication interface

---

## 🖥️ UI States

* Idle state
* Enrollment view
* Authentication view
* Success / Failure feedback
* Spoof detection state

---

## 🛠️ Tech Stack

* C++
* Qt 6 (QML)
* OpenCV
* SQLite
* CMake

---

## 📂 Project Structure

```
src/
 ├── engine/
 ├── auth/
 ├── database/
 ├── ui/

docs/
scripts/
```

---

## 📘 Documentation

This project includes full technical documentation using MkDocs.

### Run locally:

```bash
pip install mkdocs mkdocs-material mkdocs-mermaid2-plugin
python -m mkdocs serve
```

Then open:

http://127.0.0.1:8000

---

## 🎯 Use Cases

* Access control systems
* Industrial operator authentication
* Smart security terminals
* Edge AI authentication devices

---

## 🔮 Future Improvements

* RTSP / IP camera integration
* Edge deployment (Jetson / embedded systems)
* Anti-spoofing improvements
* Multi-camera support
* MQTT / REST integration

---

## ⚠️ Notes

This project is a clean and structured implementation inspired by real-world biometric authentication systems used in industrial and edge AI environments.
