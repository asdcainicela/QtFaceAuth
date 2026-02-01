import sqlite3
import os
import sys

# --- CONFIGURATION ---
DB_FOLDER = os.path.join(os.path.dirname(os.path.dirname(__file__)), 'db')
DB_PATH = os.path.join(DB_FOLDER, 'faceauth.db')
SCHEMA_PATH = os.path.join(os.path.dirname(__file__), 'schema.sql')

def ensure_db_folder():
    if not os.path.exists(DB_FOLDER):
        os.makedirs(DB_FOLDER)
        print(f"Created directory: {DB_FOLDER}")

def delete_db_file():
    if os.path.exists(DB_PATH):
        try:
            os.remove(DB_PATH)
            print(f"🗑️ Database Deleted: {DB_PATH}")
            return True
        except OSError as e:
            print(f"Error deleting database: {e}")
            sys.exit(1)
    else:
        print("Database file does not exist.")
        return False

def connect_db():
    try:
        conn = sqlite3.connect(DB_PATH)
        return conn
    except sqlite3.Error as e:
        print(f"Error connecting to database: {e}")
        sys.exit(1)

def execute_schema(conn):
    try:
        with open(SCHEMA_PATH, 'r') as f:
            schema_sql = f.read()
        conn.cursor().executescript(schema_sql)
        print("Schema executed successfully.")
    except FileNotFoundError:
        print(f"Error: schema.sql not found at {SCHEMA_PATH}")
        sys.exit(1)
    except sqlite3.Error as e:
        print(f"SQL Error executing schema: {e}")
        sys.exit(1)

def seed_data(conn):
    cursor = conn.cursor()
    
    # 1. Roles
    cursor.execute("SELECT count(*) FROM roles")
    if cursor.fetchone()[0] == 0:
        print("Seeding default roles...")
        roles = [
            ('admin', '{"all": true}'),
            ('user', '{"access": true}'),
            ('viewer', '{"read_only": true}')
        ]
        cursor.executemany("INSERT INTO roles (name, permissions) VALUES (?, ?)", roles)

    # 2. Admin User
    cursor.execute("SELECT count(*) FROM users WHERE username='admin'")
    if cursor.fetchone()[0] == 0:
        print("Seeding default ADMIN user...")
        cursor.execute("""
            INSERT INTO users (uuid, username, full_name, password_hash, is_active)
            VALUES ('00000000-0000-0000-0000-000000000001', 'admin', 'System Administrator', 'admin123', 1)
        """)
        user_id = cursor.lastrowid
        
        # Link to Admin Role
        cursor.execute("SELECT id FROM roles WHERE name='admin'")
        role_id = cursor.fetchone()[0]
        cursor.execute("INSERT INTO user_roles (user_id, role_id) VALUES (?, ?)", (user_id, role_id))

    # 3. Config
    cursor.execute("SELECT count(*) FROM system_config")
    if cursor.fetchone()[0] == 0:
        print("Seeding default config...")
        configs = [
            ('min_confidence', '0.92', 'security'),
            ('liveness_enabled', 'true', 'security'),
            ('camera_id', '0', 'hardware')
        ]
        cursor.executemany("INSERT INTO system_config (key, value, group_name) VALUES (?, ?, ?)", configs)
    
    conn.commit()
    print("Seeding complete.")
