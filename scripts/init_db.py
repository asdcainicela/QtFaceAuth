import sqlite3
import os
import sys

# Configuration
DB_FOLDER = os.path.join(os.path.dirname(os.path.dirname(__file__)), 'db')
DB_PATH = os.path.join(DB_FOLDER, 'faceauth.db')
SCHEMA_PATH = os.path.join(os.path.dirname(__file__), 'schema.sql')

def init_db(reset=False):
    print(f"Initializing Database...")
    
    # 0. Reset if requested
    if reset and os.path.exists(DB_PATH):
        try:
            os.remove(DB_PATH)
            print(f"🗑️ Database Reset: Deleted {DB_PATH}")
        except OSError as e:
            print(f"Error deleting database: {e}")
            sys.exit(1)

    # 1. Create db directory if not exists
    if not os.path.exists(DB_FOLDER):
        os.makedirs(DB_FOLDER)
        print(f"Created directory: {DB_FOLDER}")

    # 2. Connect to SQLite
    try:
        conn = sqlite3.connect(DB_PATH)
        cursor = conn.cursor()
        print(f"Connected to: {DB_PATH}")
    except sqlite3.Error as e:
        print(f"Error connecting to database: {e}")
        sys.exit(1)

    # 3. Read and execute Schema
    try:
        with open(SCHEMA_PATH, 'r') as f:
            schema_sql = f.read()
        
        cursor.executescript(schema_sql)
        print("Schema executed successfully.")
        
        # 4. Seed Default Data (Optional)
        seed_data(cursor)
        
        conn.commit()
        print("Database initialization complete.")
        
    except FileNotFoundError:
        print(f"Error: schema.sql not found at {SCHEMA_PATH}")
    except sqlite3.Error as e:
        print(f"SQL Error: {e}")
    finally:
        conn.close()

def seed_data(cursor):
    """Insert default roles and admin user if empty"""
    
    # Check if roles exist
    cursor.execute("SELECT count(*) FROM roles")
    if cursor.fetchone()[0] == 0:
        print("Seeding default roles...")
        roles = [
            ('admin', '{"all": true}'),
            ('user', '{"access": true}'),
            ('viewer', '{"read_only": true}')
        ]
        cursor.executemany("INSERT INTO roles (name, permissions) VALUES (?, ?)", roles)

    # Seed Default Admin User
    cursor.execute("SELECT count(*) FROM users WHERE username='admin'")
    if cursor.fetchone()[0] == 0:
        print("Seeding default ADMIN user...")
        # Password hardcoded 'admin123' for dev. In prod use bcrypt/argon2.
        # uuid is auto-generated in C++, but here we mock it.
        cursor.execute("""
            INSERT INTO users (uuid, username, full_name, password_hash, is_active)
            VALUES ('00000000-0000-0000-0000-000000000001', 'admin', 'System Administrator', 'admin123', 1)
        """)
        user_id = cursor.lastrowid
        
        # Link to Admin Role
        cursor.execute("SELECT id FROM roles WHERE name='admin'")
        role_id = cursor.fetchone()[0]
        cursor.execute("INSERT INTO user_roles (user_id, role_id) VALUES (?, ?)", (user_id, role_id))
        
    # Check default config
    cursor.execute("SELECT count(*) FROM system_config")
    if cursor.fetchone()[0] == 0:
        print("Seeding default config...")
        configs = [
            ('min_confidence', '0.92', 'security'),
            ('liveness_enabled', 'true', 'security'),
            ('camera_id', '0', 'hardware')
        ]
        cursor.executemany("INSERT INTO system_config (key, value, group_name) VALUES (?, ?, ?)", configs)

if __name__ == "__main__":
    import argparse
    parser = argparse.ArgumentParser()
    parser.add_argument("--reset", action="store_true", help="Delete existing database before initializing")
    args = parser.parse_args()
    
    init_db(reset=args.reset)
