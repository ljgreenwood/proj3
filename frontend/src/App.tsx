import { useState, useEffect, useCallback } from 'react'; // eslint-disable-line
import {
  Paper,
  Typography,
  AppBar,
  Toolbar,
  Select,
  MenuItem,
  FormControl,
  InputLabel,
  Button,
  Box,
  CircularProgress,
  SelectChangeEvent,
  IconButton,
} from '@mui/material';
import { ArrowBackIos, ArrowForwardIos } from '@mui/icons-material';
import { ThemeProvider, createTheme } from '@mui/material/styles';
import CssBaseline from '@mui/material/CssBaseline';
import ModelViewer from './components/ModelViewer';
import ModelList from './components/ModelList';
import { ModelInfo, ModelGeometry } from './types';
import { apiService } from './services/api';

const theme = createTheme({
  palette: {
    mode: 'light',
    primary: {
      main: '#1976d2',
    },
    secondary: {
      main: '#dc004e',
    },
  },
});

function App() {
  const [models, setModels] = useState<ModelInfo[]>([]);
  const [categories, setCategories] = useState<string[]>([]);
  const [selectedCategory, setSelectedCategory] = useState<string>('');
  const [selectedModel1, setSelectedModel1] = useState<ModelInfo | null>(null);
  const [modelGeometry1, setModelGeometry1] = useState<ModelGeometry | null>(null);
  const [similarModels, setSimilarModels] = useState<ModelInfo[]>([]);
  const [currentSimilarIndex, setCurrentSimilarIndex] = useState<number>(0);
  const [currentSimilarGeometry, setCurrentSimilarGeometry] = useState<ModelGeometry | null>(null);
  const [loading, setLoading] = useState(false);
  
  // Geometry cache to avoid re-fetching
  const [geometryCache, setGeometryCache] = useState<Map<string, ModelGeometry>>(new Map());

  useEffect(() => {
    loadInitialData();
  }, []);

  const loadGeometryWithCache = useCallback(async (filename: string): Promise<ModelGeometry> => {
    // Check if geometry is already cached
    if (geometryCache.has(filename)) {
      console.log(`Using cached geometry for ${filename}`);
      return geometryCache.get(filename)!;
    }

    // Load geometry from API
    console.log(`Loading geometry for ${filename}`);
    const geometry = await apiService.getModelGeometry(filename);
    
    // Cache the result
    setGeometryCache(prev => new Map(prev).set(filename, geometry));
    console.log(`Cached geometry for ${filename}. Cache size: ${geometryCache.size + 1}`);
    
    return geometry;
  }, [geometryCache]);

  const loadInitialData = async () => {
    try {
      setLoading(true);
      const [modelsData, categoriesData] = await Promise.all([
        apiService.getModels(),
        apiService.getCategories()
      ]);
      setModels(modelsData);
      setCategories(categoriesData.categories);
    } catch (error) {
      console.error('Error loading initial data:', error);
    } finally {
      setLoading(false);
    }
  };

  const handleCategoryChange = (event: SelectChangeEvent<string>) => {
    setSelectedCategory(event.target.value);
    // Clear selected models when changing category
    setSelectedModel1(null);
    setModelGeometry1(null);
    setSimilarModels([]);
    setCurrentSimilarIndex(0);
    setCurrentSimilarGeometry(null);
  };

  const handleModelSelect = async (model: ModelInfo) => {
    try {
      setLoading(true);
      setSelectedModel1(model);
      // Clear similar models when selecting a new primary model
      setSimilarModels([]);
      setCurrentSimilarIndex(0);
      setCurrentSimilarGeometry(null);
      
      const geometry = await loadGeometryWithCache(model.filename);
      setModelGeometry1(geometry);
    } catch (error) {
      console.error('Error loading model geometry:', error);
    } finally {
      setLoading(false);
    }
  };

  const handleFindSimilar = async () => {
    if (!selectedModel1) return;
    
    try {
      setLoading(true);
      const response = await apiService.findSimilarModels(selectedModel1.filename, 5);
      
      if (response.similar_models.length > 0) {
        setSimilarModels(response.similar_models);
        setCurrentSimilarIndex(0);
        
        // Load geometry for the first similar model immediately
        const firstGeometry = await loadGeometryWithCache(response.similar_models[0].filename);
        setCurrentSimilarGeometry(firstGeometry);
        
        // Preload all other similar model geometries in the background
        console.log('Preloading similar model geometries...');
        response.similar_models.slice(1).forEach(async (model, index) => {
          try {
            await loadGeometryWithCache(model.filename);
            console.log(`Preloaded geometry ${index + 2}/${response.similar_models.length}: ${model.filename}`);
          } catch (error) {
            console.error(`Failed to preload geometry for ${model.filename}:`, error);
          }
        });
      }
    } catch (error) {
      console.error('Error finding similar models:', error);
    } finally {
      setLoading(false);
    }
  };

  const handlePreviousSimilar = async () => {
    if (similarModels.length === 0) return;
    
    const newIndex = currentSimilarIndex > 0 ? currentSimilarIndex - 1 : similarModels.length - 1;
    setCurrentSimilarIndex(newIndex);
    
    try {
      // Since we preloaded geometries, this should be instant from cache
      const geometry = await loadGeometryWithCache(similarModels[newIndex].filename);
      setCurrentSimilarGeometry(geometry);
    } catch (error) {
      console.error('Error loading similar model geometry:', error);
    }
  };

  const handleNextSimilar = async () => {
    if (similarModels.length === 0) return;
    
    const newIndex = currentSimilarIndex < similarModels.length - 1 ? currentSimilarIndex + 1 : 0;
    setCurrentSimilarIndex(newIndex);
    
    try {
      // Since we preloaded geometries, this should be instant from cache
      const geometry = await loadGeometryWithCache(similarModels[newIndex].filename);
      setCurrentSimilarGeometry(geometry);
    } catch (error) {
      console.error('Error loading similar model geometry:', error);
    }
  };

  const filteredModels = selectedCategory 
    ? models.filter(model => model.category === selectedCategory)
    : models;

  return (
    <ThemeProvider theme={theme}>
      <CssBaseline />
      <Box sx={{ flexGrow: 1, height: '100vh', display: 'flex', flexDirection: 'column' }}>
        <AppBar position="static">
          <Toolbar>
            <Typography variant="h6" component="div" sx={{ flexGrow: 1 }}>
              3D Model Similarity Search
            </Typography>
            <FormControl sx={{ minWidth: 120, mr: 2 }}>
              <InputLabel sx={{ color: 'white' }}>Category</InputLabel>
              <Select
                value={selectedCategory}
                onChange={handleCategoryChange}
                size="small"
                sx={{ color: 'white', '.MuiOutlinedInput-notchedOutline': { borderColor: 'white' } }}
              >
                <MenuItem value="">All</MenuItem>
                {categories.map(category => (
                  <MenuItem key={category} value={category}>
                    {category}
                  </MenuItem>
                ))}
              </Select>
            </FormControl>
            <Button 
              color="inherit" 
              onClick={handleFindSimilar}
              disabled={!selectedModel1 || loading}
            >
              Find Similar
            </Button>
          </Toolbar>
        </AppBar>
        
        <Box sx={{ display: 'flex', height: 'calc(100vh - 64px)' }}>
          {/* Fixed Selection Panel */}
          <Box 
            sx={{ 
              width: '320px',
              minWidth: '320px',
              height: '100%',
              borderRight: '1px solid #e0e0e0',
              backgroundColor: '#fafafa'
            }}
          >
            <Paper sx={{ height: '100%', borderRadius: 0, p: 2 }}>
              <Typography variant="h6" gutterBottom>
                Models ({filteredModels.length})
              </Typography>
              <Box sx={{ height: 'calc(100% - 40px)', overflow: 'hidden' }}>
                {loading ? (
                  <Box display="flex" justifyContent="center" alignItems="center" height="200px">
                    <CircularProgress />
                  </Box>
                ) : (
                  <ModelList
                    models={filteredModels}
                    onModelSelect={handleModelSelect}
                    selectedModel1={selectedModel1}
                  />
                )}
              </Box>
            </Paper>
          </Box>

          {/* Fixed 3D Viewers Area */}
          <Box sx={{ flexGrow: 1, height: '100%', p: 2 }}>
            <Box sx={{ display: 'flex', gap: 2, height: '100%' }}>
              {/* Model 1 Viewer - Fixed Size */}
              <Paper sx={{ flex: 1, height: '100%', p: 2 }}>
                <Box sx={{ height: '50px', display: 'flex', alignItems: 'center', mb: 1 }}>
                  <Typography variant="h6">
                    Model 1: {selectedModel1?.filename?.split('/').pop()?.replace('.off', '') || 'None selected'}
                  </Typography>
                </Box>
                <Box sx={{ height: 'calc(100% - 66px)' }}>
                  {modelGeometry1 ? (
                    <ModelViewer 
                      geometry={modelGeometry1} 
                      position="left"
                    />
                  ) : (
                    <Box 
                      display="flex" 
                      alignItems="center" 
                      justifyContent="center" 
                      height="100%"
                      sx={{ backgroundColor: '#f0f0f0', borderRadius: 1 }}
                    >
                      <Typography color="textSecondary">
                        Select a model to view
                      </Typography>
                    </Box>
                  )}
                </Box>
              </Paper>

              {/* Similar Models Viewer - Fixed Size */}
              <Paper sx={{ flex: 1, height: '100%', p: 2 }}>
                <Box sx={{ height: '50px', display: 'flex', alignItems: 'center', justifyContent: 'space-between', mb: 1 }}>
                  <Typography variant="h6">
                    Similar Models
                    {similarModels.length > 0 && (
                      <Typography component="span" variant="body2" color="textSecondary" sx={{ ml: 1 }}>
                        ({currentSimilarIndex + 1} of {similarModels.length})
                      </Typography>
                    )}
                  </Typography>
                  {similarModels.length > 0 && (
                    <Box>
                      <IconButton 
                        onClick={handlePreviousSimilar}
                        disabled={loading}
                        size="small"
                      >
                        <ArrowBackIos />
                      </IconButton>
                      <IconButton 
                        onClick={handleNextSimilar}
                        disabled={loading}
                        size="small"
                      >
                        <ArrowForwardIos />
                      </IconButton>
                    </Box>
                  )}
                </Box>
                <Box sx={{ height: 'calc(100% - 66px)' }}>
                  {currentSimilarGeometry && similarModels.length > 0 ? (
                    <ModelViewer 
                      geometry={currentSimilarGeometry} 
                      position="right"
                    />
                  ) : (
                    <Box 
                      display="flex" 
                      alignItems="center" 
                      justifyContent="center" 
                      height="100%"
                      sx={{ backgroundColor: '#f0f0f0', borderRadius: 1 }}
                    >
                      <Typography color="textSecondary">
                        {selectedModel1 ? 'Click "Find Similar" to see similar models' : 'Select a model first'}
                      </Typography>
                    </Box>
                  )}
                </Box>
              </Paper>
            </Box>
          </Box>
        </Box>
      </Box>
    </ThemeProvider>
  );
}

export default App;