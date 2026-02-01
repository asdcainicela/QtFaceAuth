import sqlite3
import os
import sys

# Configuration
DB_FOLDER = os.path.join(os.path.dirname(os.path.dirname(__file__)), 'db')
DB_PATH = os.path.join(DB_FOLDER, 'faceauth.db')
SCHEMA_PATH = os.path.join(os.path.dirname(__file__), 'schema.sql')

def init_db():
    print(f"Initializing Database...")
    
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
    init_db()
