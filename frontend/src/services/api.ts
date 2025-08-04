import axios from 'axios';
import { ModelInfo, ModelGeometry, SimilarityResponse, ComparisonResponse } from '../types';

const API_BASE_URL = 'http://localhost:8000';

const apiClient = axios.create({
  baseURL: API_BASE_URL,
  headers: {
    'Content-Type': 'application/json',
  },
});

export const apiService = {
  async getModels(): Promise<ModelInfo[]> {
    const response = await apiClient.get('/list');
    return response.data;
  },

  async getCategories(): Promise<{ categories: string[] }> {
    const response = await apiClient.get('/categories');
    return response.data;
  },

  async getModelGeometry(filename: string): Promise<ModelGeometry> {
    // Parse filename to extract category, split, and filename
    const parts = filename.split('/');
    const [category, split, file] = parts;
    const response = await apiClient.get(`/models/geometry/${category}/${split}/${file}`);
    return response.data;
  },

  async findSimilarModels(sourceModel: string, topK: number = 5, algorithm: string = 'kdtree'): Promise<SimilarityResponse> {
    const response = await apiClient.post('/models/similar', {
      source_model: sourceModel,
      top_k: topK,
      algorithm: algorithm,
    });
    return response.data;
  },

  async compareModels(model1Path: string, model2Path: string): Promise<ComparisonResponse> {
    const response = await apiClient.get(`/models/compare/${model1Path}/vs/${model2Path}`);
    return response.data;
  },
};

export default apiService;