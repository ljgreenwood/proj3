from fastapi import FastAPI, HTTPException
from fastapi.middleware.cors import CORSMiddleware
# from fastapi.staticfiles import StaticFiles  # Not needed - removed static file serving
from pydantic import BaseModel
import json
import os
import subprocess
from typing import List, Dict, Any
import open3d as o3d
import numpy as np

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

class ModelGeometry(BaseModel):
    vertices: List[List[float]]
    faces: List[List[int]]
    normals: List[List[float]]

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
    """Scan the data directory and return model information"""
    models = []
    data_dir = "../data"
    
    print(f"DEBUG: Looking for data directory at: {os.path.abspath(data_dir)}")
    print(f"DEBUG: Data directory exists: {os.path.exists(data_dir)}")
    
    if not os.path.exists(data_dir):
        print(f"DEBUG: Data directory not found at {data_dir}")
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
    return scan_models_directory()

@app.get("/categories")  
async def categories_direct():
    print("DEBUG: Direct /categories endpoint called!")
    data_dir = "../data"
    categories = []
    
    if os.path.exists(data_dir):
        categories = [name for name in os.listdir(data_dir) 
                     if os.path.isdir(os.path.join(data_dir, name))]
    
    return {"categories": categories}

@app.get("/models/list", response_model=List[ModelInfo])
async def list_models():
    """Get list of all available 3D models"""
    print("DEBUG: Correct /models/list endpoint called!")
    return scan_models_directory()

@app.get("/models/categories")
async def get_categories():
    """Get list of available model categories"""
    print("DEBUG: Correct /models/categories endpoint called!")
    data_dir = "../data"
    categories = []
    
    if os.path.exists(data_dir):
        categories = [name for name in os.listdir(data_dir) 
                     if os.path.isdir(os.path.join(data_dir, name))]
    
    return {"categories": categories}

@app.get("/models/geometry/{category}/{split}/{filename}")
async def get_model_geometry(category: str, split: str, filename: str):
    """Get 3D model geometry data for Three.js rendering"""
    off_path = f"../data/{category}/{split}/{filename}"
    
    if not os.path.exists(off_path):
        raise HTTPException(status_code=404, detail="Model file not found")
    
    try:
        geometry_data = off_to_json(off_path)
        return geometry_data
    except Exception as e:
        raise HTTPException(status_code=500, detail=str(e))

@app.post("/models/similar")
async def find_similar_models(request: SimilarityRequest):
    """Find similar models using preprocessing features"""
    # TODO: Integrate with C++ preprocessing code
    # For now, return mock similar models from same category
    
    source_path = f"../data/{request.source_model}"
    if not os.path.exists(source_path):
        raise HTTPException(status_code=404, detail="Source model not found")
    
    # Extract category from source model path
    category = request.source_model.split('/')[0]
    
    # Mock similarity search - return other models from same category
    models = scan_models_directory()
    similar_models = [
        model for model in models 
        if model.category == category and model.filename != request.source_model
    ][:request.top_k]
    
    return {
        "source_model": request.source_model,
        "similar_models": similar_models,
        "method": "mock_category_based"  # Will be replaced with actual feature similarity
    }

@app.get("/models/compare/{model1_path:path}/vs/{model2_path:path}")
async def compare_models(model1_path: str, model2_path: str):
    """Compare two models side by side (similar to Jupyter notebook functionality)"""
    full_path1 = f"../data/{model1_path}"
    full_path2 = f"../data/{model2_path}"
    
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