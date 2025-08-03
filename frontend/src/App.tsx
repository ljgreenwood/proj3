import React, { useState, useEffect } from 'react';
import {
  Container,
  Grid,
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
} from '@mui/material';
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
  const [selectedModel2, setSelectedModel2] = useState<ModelInfo | null>(null);
  const [modelGeometry1, setModelGeometry1] = useState<ModelGeometry | null>(null);
  const [modelGeometry2, setModelGeometry2] = useState<ModelGeometry | null>(null);
  const [loading, setLoading] = useState(false);

  useEffect(() => {
    loadInitialData();
  }, []);

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
    setSelectedModel2(null);
    setModelGeometry1(null);
    setModelGeometry2(null);
  };

  const handleModelSelect = async (model: ModelInfo, position: 1 | 2) => {
    try {
      setLoading(true);
      const geometry = await apiService.getModelGeometry(model.filename);
      
      if (position === 1) {
        setSelectedModel1(model);
        setModelGeometry1(geometry);
      } else {
        setSelectedModel2(model);
        setModelGeometry2(geometry);
      }
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
      const similarModels = await apiService.findSimilarModels(selectedModel1.filename, 1);
      if (similarModels.similar_models.length > 0) {
        await handleModelSelect(similarModels.similar_models[0], 2);
      }
    } catch (error) {
      console.error('Error finding similar models:', error);
    } finally {
      setLoading(false);
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
        
        <Container maxWidth="xl" sx={{ flexGrow: 1, py: 2 }}>
          <Grid container spacing={2} sx={{ height: '100%' }}>
            {/* Model List */}
            <Grid item xs={12} md={3}>
              <Paper sx={{ height: '100%', p: 2 }}>
                <Typography variant="h6" gutterBottom>
                  Models ({filteredModels.length})
                </Typography>
                {loading ? (
                  <Box display="flex" justifyContent="center" p={2}>
                    <CircularProgress />
                  </Box>
                ) : (
                  <ModelList
                    models={filteredModels}
                    onModelSelect={handleModelSelect}
                    selectedModel1={selectedModel1}
                    selectedModel2={selectedModel2}
                  />
                )}
              </Paper>
            </Grid>

            {/* 3D Viewers */}
            <Grid item xs={12} md={9}>
              <Grid container spacing={2} sx={{ height: '100%' }}>
                <Grid item xs={12} md={6}>
                  <Paper sx={{ height: '100%', p: 2 }}>
                    <Typography variant="h6" gutterBottom>
                      Model 1: {selectedModel1?.filename || 'None selected'}
                    </Typography>
                    <Box sx={{ height: 'calc(100% - 40px)' }}>
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
                </Grid>

                <Grid item xs={12} md={6}>
                  <Paper sx={{ height: '100%', p: 2 }}>
                    <Typography variant="h6" gutterBottom>
                      Model 2: {selectedModel2?.filename || 'None selected'}
                    </Typography>
                    <Box sx={{ height: 'calc(100% - 40px)' }}>
                      {modelGeometry2 ? (
                        <ModelViewer 
                          geometry={modelGeometry2} 
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
                            {selectedModel1 ? 'Click "Find Similar" or select a model' : 'Select first model'}
                          </Typography>
                        </Box>
                      )}
                    </Box>
                  </Paper>
                </Grid>
              </Grid>
            </Grid>
          </Grid>
        </Container>
      </Box>
    </ThemeProvider>
  );
}

export default App;