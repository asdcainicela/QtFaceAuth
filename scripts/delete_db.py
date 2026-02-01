import db_common

def main():
    print("--- DELETING DATABASE ---")
    confirm = input("This will PERMANENTLY DELETE the database file. Continue? (y/N): ")
    if confirm.lower() == 'y':
        db_common.delete_db_file()
    else:
        print("Cancelled.")

if __name__ == "__main__":
    main()
