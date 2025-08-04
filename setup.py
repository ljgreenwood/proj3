#!/usr/bin/env python3
"""
Setup script for 3D Model Similarity Search

This script helps set up the development environment and can integrate
the existing C++ preprocessing code with the new web application.
"""

import os
import subprocess
import sys
from pathlib import Path

def run_command(cmd, cwd=None):
    """Run a command and handle errors"""
    try:
        result = subprocess.run(cmd, shell=True, cwd=cwd, check=True, 
                              capture_output=True, text=True)
        print(f"Success: {cmd}")
        return result
    except subprocess.CalledProcessError as e:
        print(f"Error: {cmd} failed: {e.stderr}")
        return None

def setup_backend():
    """Set up the FastAPI backend"""
    print("Setting up backend...")
    
    backend_dir = Path("backend")
    if not backend_dir.exists():
        print("Error: Backend directory not found")
        return False
    
    # Create virtual environment
    venv_path = backend_dir / "venv"
    if not venv_path.exists():
        run_command("python -m venv venv", cwd=backend_dir)
    
    # Install dependencies
    if sys.platform == "win32":
        pip_cmd = str(venv_path / "Scripts" / "pip")
    else:
        pip_cmd = str(venv_path / "bin" / "pip")
    
    run_command(f"{pip_cmd} install -r requirements.txt", cwd=backend_dir)
    
    # Create static directories
    static_dir = backend_dir / "static" / "models"
    static_dir.mkdir(parents=True, exist_ok=True)
    
    print("Backend setup complete")
    return True

def setup_frontend():
    """Set up the React frontend"""
    print("Setting up frontend...")
    
    frontend_dir = Path("frontend")
    if not frontend_dir.exists():
        print("Error: Frontend directory not found")
        return False
    
    # Install npm dependencies
    run_command("npm install", cwd=frontend_dir)
    
    print("Frontend setup complete")
    return True

def check_data_directory():
    """Check if data directory is properly set up"""
    print("Checking data directory...")
    
    data_dir = Path("data")
    if not data_dir.exists():
        print("Warning: Data directory not found. Please ensure your ModelNet10 data is available at ./data/")
        print("   You can create a symlink: ln -s /path/to/ModelNet10/ ./data")
        return False
    
    # Check for expected structure
    categories = ['bathtub', 'bed', 'chair', 'desk', 'dresser', 'monitor', 'night_stand', 'sofa', 'table', 'toilet']
    found_categories = [d.name for d in data_dir.iterdir() if d.is_dir() and d.name in categories]
    
    print(f"Found {len(found_categories)} categories: {found_categories}")
    return len(found_categories) > 0

def compile_cpp_code():
    """Compile the existing C++ preprocessing code"""
    print("Compiling C++ preprocessing code...")
    
    cpp_files = ['KDTree.cpp', 'Octree.cpp', 'preprocessing.cpp', 'main.cpp']
    missing_files = [f for f in cpp_files if not Path(f).exists()]
    
    if missing_files:
        print(f"Warning: Missing C++ files: {missing_files}")
        return False
    
    # Basic compilation (you may need to adjust flags for your system)
    compile_cmd = "g++ -std=c++17 -O2 -o preprocessing main.cpp preprocessing.cpp"
    result = run_command(compile_cmd)
    
    if result:
        print("C++ code compiled successfully")
        return True
    else:
        print("Warning: C++ compilation failed. You may need to adjust compiler flags.")
        return False

def create_integration_script():
    """Create a Python script to integrate C++ preprocessing with the web app"""
    integration_script = '''#!/usr/bin/env python3
"""
Integration script to call C++ preprocessing from Python backend
"""

import subprocess
import json
import sys
from pathlib import Path

def preprocess_model(off_file_path: str) -> dict:
    """
    Call C++ preprocessing on an OFF file and return features
    
    Args:
        off_file_path: Path to the .off file
    
    Returns:
        Dictionary with extracted features
    """
    try:
        # Call C++ preprocessing executable
        result = subprocess.run(
            ['./preprocessing', off_file_path], 
            capture_output=True, 
            text=True, 
            check=True
        )
        
        # Parse output (modify based on your C++ output format)
        # This is a placeholder - you'll need to modify based on your actual C++ output
        features = {
            "kdtree_features": [],  # Replace with actual KDTree features
            "octree_features": [],  # Replace with actual Octree features
            "vertex_count": 0,      # Replace with actual vertex count
            "face_count": 0,        # Replace with actual face count
        }
        
        return features
        
    except subprocess.CalledProcessError as e:
        print(f"Error processing {off_file_path}: {e.stderr}")
        return None

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python integrate_cpp.py <off_file_path>")
        sys.exit(1)
    
    features = preprocess_model(sys.argv[1])
    if features:
        print(json.dumps(features, indent=2))
    else:
        sys.exit(1)
'''
    
    with open("integrate_cpp.py", "w") as f:
        f.write(integration_script)
    
    print("Created integration script: integrate_cpp.py")

def main():
    """Main setup function"""
    print("Setting up 3D Model Similarity Search Web Application")
    print("=" * 60)
    
    # Check data directory
    data_ok = check_data_directory()
    
    # Setup backend
    backend_ok = setup_backend()
    
    # Setup frontend  
    frontend_ok = setup_frontend()
    
    # Compile C++ code (optional)
    cpp_ok = compile_cpp_code()
    
    # Create integration script
    create_integration_script()
    
    print("\n" + "=" * 60)
    print("Setup Summary:")
    print(f"   Data Directory: {'OK' if data_ok else 'FAILED'}")
    print(f"   Backend:        {'OK' if backend_ok else 'FAILED'}")
    print(f"   Frontend:       {'OK' if frontend_ok else 'FAILED'}")
    print(f"   C++ Code:       {'OK' if cpp_ok else 'WARNING'}")
    
    if backend_ok and frontend_ok:
        print("\nWeb application is ready!")
        print("\nTo start the application:")
        print("1. Terminal 1: cd backend && python main.py")
        print("2. Terminal 2: cd frontend && npm run dev")
        print("3. Open http://localhost:3000")
        
        if not data_ok:
            print("\nWarning: Don't forget to set up your data directory!")
        
        if not cpp_ok:
            print("\nWarning: C++ preprocessing not compiled - similarity search will use mock data")
    else:
        print("\nSetup incomplete. Please check the errors above.")

if __name__ == "__main__":
    main()