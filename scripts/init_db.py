import db_common

def main():
    print("--- INITIALIZING DATABASE ---")
    db_common.ensure_db_folder()
    
    conn = db_common.connect_db()
    db_common.execute_schema(conn)
    db_common.seed_data(conn)
    
    conn.close()
    print("Database ready.")

if __name__ == "__main__":
    main()
