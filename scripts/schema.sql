-- QtFaceAuth SQLite Schema
-- Enable Foreign Keys constraints
PRAGMA foreign_keys = ON;

-- 1. Roles (RBAC)
CREATE TABLE IF NOT EXISTS roles (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    name TEXT NOT NULL UNIQUE,
    permissions TEXT NOT NULL -- JSON
);

-- 2. Users (Identity)
CREATE TABLE IF NOT EXISTS users (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    uuid TEXT NOT NULL UNIQUE, -- UUID v4 for sync
    username TEXT NOT NULL UNIQUE,
    full_name TEXT NOT NULL,
    password_hash TEXT, -- Nullable (Face-only users)
    is_active INTEGER DEFAULT 1,
    created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    deleted_at DATETIME -- Soft Delete
);

-- 3. User Roles (Many-to-Many)
CREATE TABLE IF NOT EXISTS user_roles (
    user_id INTEGER NOT NULL,
    role_id INTEGER NOT NULL,
    PRIMARY KEY (user_id, role_id),
    FOREIGN KEY (user_id) REFERENCES users(id) ON DELETE CASCADE,
    FOREIGN KEY (role_id) REFERENCES roles(id) ON DELETE CASCADE
);

-- 4. Biometrics (Face Vectors)
CREATE TABLE IF NOT EXISTS biometrics (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    user_id INTEGER NOT NULL,
    face_vector BLOB NOT NULL,
    model_version TEXT DEFAULT 'v1',
    created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (user_id) REFERENCES users(id) ON DELETE CASCADE
);

-- 5. Sessions (Audit Logs)
CREATE TABLE IF NOT EXISTS sessions (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    user_id INTEGER, -- Nullable if unknown user
    login_method TEXT NOT NULL, -- 'FACE', 'PIN'
    confidence REAL,
    event_type TEXT NOT NULL, -- 'SUCCESS', 'FAILED', 'SPOOF'
    snapshot_path TEXT,
    created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (user_id) REFERENCES users(id) ON DELETE SET NULL
);

-- 6. System Configuration (Kiosk Settings)
CREATE TABLE IF NOT EXISTS system_config (
    key TEXT PRIMARY KEY,
    value TEXT NOT NULL,
    group_name TEXT DEFAULT 'general'
);

-- Indexes for performance
CREATE INDEX IF NOT EXISTS idx_users_uuid ON users(uuid);
CREATE INDEX IF NOT EXISTS idx_sessions_created ON sessions(created_at);
