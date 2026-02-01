import db_common

def main():
    print("--- RESETTING DATABASE ---")
    print("This will DELETE all data.")
    confirm = input("Are you sure? (y/N): ")
    if confirm.lower() != 'y':
        print("Cancelled.")
        return

    # Delete
    db_common.delete_db_file()
    
    # Re-Initialize
    print("Re-creating database...")
    db_common.ensure_db_folder()
    conn = db_common.connect_db()
    db_common.execute_schema(conn)
    db_common.seed_data(conn)
    conn.close()
    
    print("Reset complete. Default admin restored.")

if __name__ == "__main__":
    main()
