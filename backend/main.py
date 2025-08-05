from fastapi import FastAPI, HTTPException, Response
from fastapi.middleware.cors import CORSMiddleware
# from fastapi.staticfiles import StaticFiles  # Not needed - removed static file serving
from pydantic import BaseModel
import json
import os
import subprocess
import time
from typing import List, Dict, Any
import open3d as o3d
import numpy as np
from pathlib import Path

app = FastAPI(title="3D Model Similarity Search API")

# Enable CORS for React frontend
app.add_middleware(
    CORSMiddleware,
    allow_origins=["http://localhost:3000", "http://localhost:5173"],  # React dev servers
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)

# Static file serving removed - using live conversion instead

class ModelInfo(BaseModel):
    filename: str
    category: str
    vertices: int
    faces: int

class SimilarityRequest(BaseModel):
    source_model: str
    top_k: int = 5
    algorithm: str = "kdtree"  # Algorithm choice: 'kdtree' or 'octree'

class ModelGeometry(BaseModel):
    vertices: List[List[float]]
    faces: List[List[int]]
    normals: List[List[float]]

# Cache storage
_model_cache: List[ModelInfo] = []
_model_cache_timestamp: float = 0
_geometry_cache: Dict[str, Dict[str, Any]] = {}
CACHE_DURATION = 300  # 5 minutes for model list cache

def get_data_dir() -> str:
    """Get the absolute path to the ModelNet10 directory"""
    # Get the directory where this script is located (backend/)
    backend_dir = Path(__file__).parent
    # Go up one level to project root, then into ModelNet10/
    modelnet_dir = backend_dir.parent / "ModelNet10"
    return str(modelnet_dir)

def is_model_cache_expired() -> bool:
    """Check if the model cache has expired"""
    return time.time() - _model_cache_timestamp > CACHE_DURATION

def get_cached_models() -> List[ModelInfo]:
    """Get cached model list or scan directory if cache is expired"""
    global _model_cache, _model_cache_timestamp
    
    if not _model_cache or is_model_cache_expired():
        print("DEBUG: Refreshing model cache...")
        _model_cache = scan_models_directory()
        _model_cache_timestamp = time.time()
        print(f"DEBUG: Cached {len(_model_cache)} models")
    else:
        print(f"DEBUG: Using cached models ({len(_model_cache)} models)")
    
    return _model_cache

def get_cached_geometry(filename: str) -> Dict[str, Any]:
    """Get cached geometry or load from file if not cached"""
    if filename not in _geometry_cache:
        print(f"DEBUG: Loading and caching geometry for {filename}")
        modelnet_dir = get_data_dir()
        off_path = os.path.join(modelnet_dir, filename.replace('/', os.sep))
        _geometry_cache[filename] = off_to_json(off_path)
        print(f"DEBUG: Geometry cached. Cache size: {len(_geometry_cache)}")
    else:
        print(f"DEBUG: Using cached geometry for {filename}")
    
    return _geometry_cache[filename]

def write_geometry_to_off(geometry: Dict[str, Any], filepath: str):
    """Write geometry data back to OFF file format for C++ processing"""
    vertices = geometry['vertices']
    faces = geometry['faces']
    
    with open(filepath, 'w') as f:
        # Write OFF header
        f.write("OFF\n")
        f.write(f"{len(vertices)} {len(faces)} 0\n")
        
        # Write vertices
        for vertex in vertices:
            f.write(f"{vertex[0]} {vertex[1]} {vertex[2]}\n")
        
        # Write faces
        for face in faces:
            f.write(f"{len(face)} {' '.join(map(str, face))}\n")

def off_to_json(off_path: str) -> Dict[str, Any]:
    """Convert OFF file to JSON format compatible with Three.js"""
    try:
        mesh = o3d.io.read_triangle_mesh(off_path)
        mesh.compute_vertex_normals()
        
        vertices = np.asarray(mesh.vertices).tolist()
        faces = np.asarray(mesh.triangles).tolist()
        normals = np.asarray(mesh.vertex_normals).tolist()
        
        return {
            "vertices": vertices,
            "faces": faces,
            "normals": normals,
            "metadata": {
                "vertex_count": len(vertices),
                "face_count": len(faces)
            }
        }
    except Exception as e:
        raise HTTPException(status_code=500, detail=f"Error converting OFF file: {str(e)}")

def scan_models_directory() -> List[ModelInfo]:
    """Scan the ModelNet10 directory and return model information"""
    models = []
    data_dir = get_data_dir()
    
    print(f"DEBUG: Looking for ModelNet10 directory at: {data_dir}")
    print(f"DEBUG: ModelNet10 directory exists: {os.path.exists(data_dir)}")
    
    if not os.path.exists(data_dir):
        print(f"DEBUG: ModelNet10 directory not found at {data_dir}")
        return models
    
    categories = os.listdir(data_dir)
    print(f"DEBUG: Found categories: {categories}")
    
    for category in categories:
        category_path = os.path.join(data_dir, category)
        if not os.path.isdir(category_path):
            print(f"DEBUG: Skipping non-directory: {category}")
            continue
        
        print(f"DEBUG: Processing category: {category}")
        
        for split in ["train", "test"]:
            split_path = os.path.join(category_path, split)
            if not os.path.exists(split_path):
                print(f"DEBUG: Split not found: {split_path}")
                continue
            
            files = [f for f in os.listdir(split_path) if f.endswith(".off")]
            print(f"DEBUG: Found {len(files)} .off files in {category}/{split}")
            
            for filename in files[:5]:  # Limit to first 5 files for debugging
                try:
                    off_path = os.path.join(split_path, filename)
                    print(f"DEBUG: Loading {off_path}")
                    mesh = o3d.io.read_triangle_mesh(off_path)
                    
                    if len(mesh.vertices) == 0:
                        print(f"DEBUG: Empty mesh in {filename}")
                        continue
                    
                    models.append(ModelInfo(
                        filename=f"{category}/{split}/{filename}",
                        category=category,
                        vertices=len(mesh.vertices),
                        faces=len(mesh.triangles)
                    ))
                    print(f"DEBUG: Successfully loaded {filename}: {len(mesh.vertices)} vertices")
                except Exception as e:
                    print(f"DEBUG: Error loading {filename}: {e}")
                    continue
    
    return models

@app.get("/")
async def root():
    return {"message": "3D Model Similarity Search API"}

@app.get("/test")
async def test():
    print("DEBUG: Test endpoint called!")
    return {"message": "Test endpoint working - server reloaded!"}

@app.get("/list")
async def list_direct():
    print("DEBUG: Direct /list endpoint called!")
    return get_cached_models()

@app.get("/categories")  
async def categories_direct():
    print("DEBUG: Direct /categories endpoint called!")
    modelnet_dir = get_data_dir()
    categories = []
    
    if os.path.exists(modelnet_dir):
        categories = [name for name in os.listdir(modelnet_dir) 
                     if os.path.isdir(os.path.join(modelnet_dir, name))]
    
    return {"categories": categories}

@app.get("/models/list", response_model=List[ModelInfo])
async def list_models():
    """Get list of all available 3D models"""
    print("DEBUG: Correct /models/list endpoint called!")
    return get_cached_models()

@app.get("/models/categories")
async def get_categories():
    """Get list of available model categories"""
    print("DEBUG: Correct /models/categories endpoint called!")
    modelnet_dir = get_data_dir()
    categories = []
    
    if os.path.exists(modelnet_dir):
        categories = [name for name in os.listdir(modelnet_dir) 
                     if os.path.isdir(os.path.join(modelnet_dir, name))]
    
    return {"categories": categories}

@app.get("/models/geometry/{category}/{split}/{filename}")
async def get_model_geometry(category: str, split: str, filename: str, response: Response):
    """Get 3D model geometry data for Three.js rendering"""
    model_path = f"{category}/{split}/{filename}"
    modelnet_dir = get_data_dir()
    off_path = os.path.join(modelnet_dir, category, split, filename)
    
    if not os.path.exists(off_path):
        raise HTTPException(status_code=404, detail="Model file not found")
    
    try:
        geometry_data = get_cached_geometry(model_path)
        
        # Add cache headers for browser-level caching
        response.headers["Cache-Control"] = "public, max-age=3600"  # Cache for 1 hour
        response.headers["ETag"] = f'"{hash(model_path)}"'  # Simple ETag based on filename
        
        return geometry_data
    except Exception as e:
        raise HTTPException(status_code=500, detail=str(e))

@app.post("/models/similar")
async def find_similar_models(request: SimilarityRequest):
    """Find similar models using C++ algorithms with geometry caching"""
    
    # Load and cache source model geometry once
    print(f"DEBUG: Starting similarity search for {request.source_model}")
    print(f"DEBUG: Using {request.algorithm.upper()} algorithm")
    
    try:
        source_geometry = get_cached_geometry(request.source_model)
        print(f"DEBUG: Source geometry loaded/cached - {len(source_geometry['vertices'])} vertices")
    except Exception as e:
        raise HTTPException(status_code=404, detail=f"Source model not found: {e}")
    
    # Get all available models to compare against
    all_models = get_cached_models()
    model_scores = []  # List of (model, similarity_score) tuples
    
    search_start_time = time.time()
    print(f"DEBUG: Comparing against {len(all_models)} total models")
    
    # Create temporary files for C++ processing (reuse source file)
    temp_dir = "temp_similarity"
    os.makedirs(temp_dir, exist_ok=True)
    source_temp_path = os.path.join(temp_dir, "source.off")
    compare_temp_path = os.path.join(temp_dir, "compare.off")
    
    # Write source geometry to temp file once
    write_geometry_to_off(source_geometry, source_temp_path)
    print(f"DEBUG: Source geometry written to temp file")
    
    # Compare against each model 
    for model in all_models:
        if model.filename == request.source_model:
            continue  # Skip comparing with itself
            
        try:
            start_time = time.time()
            
            if request.algorithm == "kdtree":
                # KDTree: Use direct file access with proper working directory
                modelnet_dir = get_data_dir()
                source_direct_path = os.path.join(modelnet_dir, request.source_model.replace('/', os.sep))
                compare_direct_path = os.path.join(modelnet_dir, model.filename.replace('/', os.sep))
                
                if not os.path.exists(compare_direct_path):
                    continue
                
                # Convert to relative paths from project root
                project_root = os.path.dirname(os.getcwd())  # Parent of backend/
                source_rel = os.path.relpath(source_direct_path, project_root)
                compare_rel = os.path.relpath(compare_direct_path, project_root)
                
                # Use full path to executable to avoid "file not found" issues
                exe_path = os.path.join(project_root, "similarity_search")
                cmd = [exe_path, source_rel, compare_rel, request.algorithm]
                
                # DEBUG: Print exact command and working directory
                print(f"DEBUG: Running command: {' '.join(cmd)}")
                print(f"DEBUG: Working directory: {project_root}")
                print(f"DEBUG: Executable exists: {os.path.exists(exe_path)}")
                print(f"DEBUG: Source path exists: {os.path.exists(source_direct_path)}")
                print(f"DEBUG: Compare path exists: {os.path.exists(compare_direct_path)}")
                    
                result = subprocess.run(cmd, capture_output=True, text=True, timeout=15, cwd=project_root)
                
            else:
                # Octree: Use cached system (works fine)
                compare_geometry = get_cached_geometry(model.filename)
                write_geometry_to_off(compare_geometry, compare_temp_path)
                
                result = subprocess.run([
                    "../similarity_search",
                    source_temp_path,
                    compare_temp_path,
                    request.algorithm
                ], capture_output=True, text=True, timeout=15)
            
            duration = time.time() - start_time
            
            if result.returncode == 0:
                # DEBUG: Print raw output to diagnose parsing issues
                raw_stdout = result.stdout.strip()
                raw_stderr = result.stderr.strip() if result.stderr else "No stderr"
                print(f"DEBUG: Raw stdout: '{raw_stdout}'")
                print(f"DEBUG: Raw stderr: '{raw_stderr}'")
                
                try:
                    similarity_score = float(raw_stdout)
                    model_scores.append((model, similarity_score))
                    print(f"DEBUG: {model.filename} - Similarity: {similarity_score:.3f}% ({duration:.2f}s)")
                except ValueError as e:
                    print(f"DEBUG: Failed to parse similarity score for {model.filename}: {e}")
                    print(f"DEBUG: Stdout content: '{raw_stdout}'")
            else:
                print(f"DEBUG: Error with {model.filename} - Return code: {result.returncode} ({duration:.2f}s)")
                print(f"DEBUG: Error stdout: '{result.stdout}'")
                print(f"DEBUG: Error stderr: '{result.stderr}'")
                
        except subprocess.TimeoutExpired:
            print(f"DEBUG: Timeout (>15s) comparing {model.filename}")
            continue
        except Exception as e:
            print(f"DEBUG: Error comparing {model.filename}: {e}")
            continue
    
    # Cleanup temp files
    try:
        os.remove(source_temp_path)
        os.remove(compare_temp_path)
        os.rmdir(temp_dir)
    except:
        pass  # Ignore cleanup errors
    
    # Sort by similarity score (highest first) and take top-k
    model_scores.sort(key=lambda x: x[1], reverse=True)
    similar_models = [model for model, score in model_scores[:request.top_k]]
    
    total_duration = time.time() - search_start_time
    print(f"DEBUG: Found {len(similar_models)} similar models in {total_duration:.1f} seconds")
    print(f"DEBUG: Top scores: {[(model.filename, score) for model, score in model_scores[:request.top_k]]}")
    
    # Avoid division by zero if no models were successfully processed
    if len(model_scores) > 0:
        print(f"DEBUG: Average time per comparison: {total_duration/len(model_scores):.2f}s")
    else:
        print(f"DEBUG: No models were successfully compared")
    print(f"DEBUG: Geometry cache size: {len(_geometry_cache)}")
    
    return {
        "source_model": request.source_model,
        "similar_models": similar_models,
        "similarity_scores": [score for model, score in model_scores[:request.top_k]],
        "method": f"{'direct' if request.algorithm == 'kdtree' else 'cached'}_{request.algorithm}_similarity_scores"
    }

@app.get("/models/compare/{model1_path:path}/vs/{model2_path:path}")
async def compare_models(model1_path: str, model2_path: str):
    """Compare two models side by side (similar to Jupyter notebook functionality)"""
    modelnet_dir = get_data_dir()
    full_path1 = os.path.join(modelnet_dir, model1_path.replace('/', os.sep))
    full_path2 = os.path.join(modelnet_dir, model2_path.replace('/', os.sep))
    
    if not os.path.exists(full_path1) or not os.path.exists(full_path2):
        raise HTTPException(status_code=404, detail="One or both model files not found")
    
    try:
        geometry1 = off_to_json(full_path1)
        geometry2 = off_to_json(full_path2)
        
        return {
            "model1": {
                "path": model1_path,
                "geometry": geometry1
            },
            "model2": {
                "path": model2_path,
                "geometry": geometry2
            }
        }
    except Exception as e:
        raise HTTPException(status_code=500, detail=str(e))

if __name__ == "__main__":
    import uvicorn
    uvicorn.run("main:app", host="0.0.0.0", port=8000, reload=True)