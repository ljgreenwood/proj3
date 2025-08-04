export interface ModelInfo {
  filename: string;
  category: string;
  vertices: number;
  faces: number;
}

export interface ModelGeometry {
  vertices: number[][];
  faces: number[][];
  normals: number[][];
  metadata: {
    vertex_count: number;
    face_count: number;
  };
}

export interface SimilarityResponse {
  source_model: string;
  similar_models: ModelInfo[];
  similarity_scores?: number[];
  method: string;
}

export interface ComparisonResponse {
  model1: {
    path: string;
    geometry: ModelGeometry;
  };
  model2: {
    path: string;
    geometry: ModelGeometry;
  };
}