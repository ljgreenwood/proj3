# 3D Model Similarity Search Backend

FastAPI backend for serving 3D model data and similarity search functionality.

## Setup

1. Create virtual environment (Make sure it's Python 3.11)
```bash
python -m venv backend_env
source backend_env/bin/activate  # On Windows: backend_env\Scripts\activate
```

2. Install dependencies:
```bash
pip install -r requirements.txt
```

3. Run the server:
```bash
python main.py
```

The API will be available at `http://localhost:8000` 
## API Endpoints

- `GET /models/list` - List all available 3D models
- `GET /models/categories` - Get available model categories  

