import os
import sys
import subprocess
import argparse
import shutil

# --- CONFIGURATION ---
PROJECT_ROOT = os.path.dirname(os.path.abspath(__file__))
BUILD_DIR = os.path.join(PROJECT_ROOT, "build")
QT_SEARCH_PATHS = [r"C:\Qt", os.path.expanduser("~") + r"\Qt"]

def find_qmlscene():
    """Search for qmlscene.exe in common Qt locations"""
    for base in QT_SEARCH_PATHS:
        if not os.path.exists(base): continue
        for root, dirs, files in os.walk(base):
            if "qmlscene.exe" in files:
                return os.path.join(root, "qmlscene.exe")
    return None

def run_command(cmd, cwd=PROJECT_ROOT):
    """Run a shell command and print output"""
    print(f"-> {cmd}")
    try:
        subprocess.check_call(cmd, shell=True, cwd=cwd)
    except subprocess.CalledProcessError:
        print(f"Error executing command.")
        sys.exit(1)

def do_build(clean=False):
    print("--- BUILDING PROJECT ---")
    
    if clean and os.path.exists(BUILD_DIR):
        print("Cleaning build directory...")
        shutil.rmtree(BUILD_DIR)
    
    if not os.path.exists(BUILD_DIR):
        os.makedirs(BUILD_DIR)
    
    # Configure (Force VS 2022)
    cmake_cache = os.path.join(BUILD_DIR, "CMakeCache.txt")
    if not os.path.exists(cmake_cache):
        print("Configuring with CMake (Visual Studio 17 2022)...")
        
        # Load QT_PATH from .env
        qt_path = None
        env_file = os.path.join(PROJECT_ROOT, ".env")
        if os.path.exists(env_file):
            with open(env_file, "r") as f:
                for line in f:
                    if line.strip().startswith("QT_PATH="):
                        qt_path = line.strip().split("=", 1)[1].strip()
                        break
        
        if not qt_path:
            print("ERROR: QT_PATH not found in .env file.")
            print("Please create a .env file with: QT_PATH=C:\\path\\to\\Qt\\...")
            sys.exit(1)
        
        print(f"Using Qt Path from .env: {qt_path}")

        if not os.path.exists(qt_path):
             print(f"WARNING: Qt path {qt_path} does not exist. Build might fail.")
        
        cmd = f'cmake -G "Visual Studio 17 2022" -A x64 -DCMAKE_PREFIX_PATH="{qt_path}" ..'
        run_command(cmd, cwd=BUILD_DIR)
    
    # Build
    print("Compiling...")
    run_command("cmake --build . --config Debug", cwd=BUILD_DIR)
    print("Build Complete.")

def load_qt_path():
    env_file = os.path.join(PROJECT_ROOT, ".env")
    if os.path.exists(env_file):
        with open(env_file, "r") as f:
            for line in f:
                if line.strip().startswith("QT_PATH="):
                    return line.strip().split("=", 1)[1].strip()
    return None

def do_run():
    print("--- RUNNING APPLICATION ---")
    exe_path = os.path.join(BUILD_DIR, "Debug", "appQtFaceAuth.exe")
    if not os.path.exists(exe_path):
        exe_path = os.path.join(BUILD_DIR, "Release", "appQtFaceAuth.exe")
        
    if os.path.exists(exe_path):
        # Inject Qt bin to PATH
        qt_path = load_qt_path()
        if qt_path:
            qt_bin = os.path.join(qt_path, "bin")
            os.environ["PATH"] = qt_bin + os.pathsep + os.environ["PATH"]
            print(f"Added to PATH: {qt_bin}")
            
        run_command(f'"{exe_path}"', cwd=PROJECT_ROOT)
    else:
        print("Executable not found. Run 'build' first.")

def do_preview():
    print("--- STARTING QML PREVIEW ---")
    qmlscene = find_qmlscene()
    if not qmlscene:
        print("Could not find qmlscene.exe. Make sure Qt is installed.")
        return
        
    qml_file = os.path.join(PROJECT_ROOT, "ui", "main.qml")
    run_command(f'"{qmlscene}" "{qml_file}"', cwd=PROJECT_ROOT)

def do_init_db():
    print("--- INITIALIZING DATABASE ---")
    init_script = os.path.join(PROJECT_ROOT, "scripts", "init_db.py")
    run_command(f"python \"{init_script}\"", cwd=PROJECT_ROOT)

def main():
    parser = argparse.ArgumentParser(description="QtFaceAuth Manager")
    parser.add_argument("command", choices=["build", "run", "preview", "init-db", "clean"], help="Command to execute")
    
def main():
    parser = argparse.ArgumentParser(description="QtFaceAuth Manager")
    parser.add_argument("command", choices=["build", "run", "preview", "clean"], help="Command to execute")
    
    args = parser.parse_args()
    
    if args.command == "build":
        do_build()
    elif args.command == "clean":
        do_build(clean=True)
    elif args.command == "run":
        do_run()
    elif args.command == "preview":
        do_preview()

if __name__ == "__main__":
    main()
