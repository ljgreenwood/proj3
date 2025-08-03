import React from 'react';
import {
  List,
  ListItem,
  ListItemButton,
  ListItemText,
  Typography,
  Chip,
  Box,
  Button,
  ButtonGroup,
} from '@mui/material';
import { ModelInfo } from '../types';

interface ModelListProps {
  models: ModelInfo[];
  onModelSelect: (model: ModelInfo, position: 1 | 2) => void;
  selectedModel1: ModelInfo | null;
  selectedModel2: ModelInfo | null;
}

const ModelList: React.FC<ModelListProps> = ({ 
  models, 
  onModelSelect, 
  selectedModel1,
  selectedModel2 
}) => {
  const isSelected = (model: ModelInfo, position: 1 | 2) => {
    const selectedModel = position === 1 ? selectedModel1 : selectedModel2;
    return selectedModel?.filename === model.filename;
  };

  return (
    <Box sx={{ height: '100%', overflow: 'auto' }}>
      <List dense>
        {models.map((model) => (
          <ListItem key={model.filename} disablePadding>
            <Box sx={{ width: '100%' }}>
              <ListItemButton
                sx={{ 
                  mb: 1,
                  borderRadius: 1,
                  border: '1px solid',
                  borderColor: 'grey.300',
                  '&:hover': { borderColor: 'primary.main' }
                }}
              >
                <ListItemText
                  primary={
                    <Typography variant="body2" sx={{ fontWeight: 'medium' }}>
                      {model.filename.split('/').pop()?.replace('.off', '')}
                    </Typography>
                  }
                  secondary={
                    <Box>
                      <Typography variant="caption" color="text.secondary">
                        {model.vertices.toLocaleString()} vertices • {model.faces.toLocaleString()} faces
                      </Typography>
                      <br />
                      <Chip 
                        label={model.category} 
                        size="small" 
                        variant="outlined"
                        sx={{ mt: 0.5 }}
                      />
                    </Box>
                  }
                />
              </ListItemButton>
              
              <ButtonGroup size="small" fullWidth sx={{ mb: 1 }}>
                <Button
                  variant={isSelected(model, 1) ? "contained" : "outlined"}
                  color="success"
                  onClick={() => onModelSelect(model, 1)}
                  sx={{ 
                    fontSize: '0.75rem',
                    bgcolor: isSelected(model, 1) ? 'success.main' : 'transparent'
                  }}
                >
                  View Left
                </Button>
                <Button
                  variant={isSelected(model, 2) ? "contained" : "outlined"}
                  color="primary"
                  onClick={() => onModelSelect(model, 2)}
                  sx={{ 
                    fontSize: '0.75rem',
                    bgcolor: isSelected(model, 2) ? 'primary.main' : 'transparent'
                  }}
                >
                  View Right
                </Button>
              </ButtonGroup>
            </Box>
          </ListItem>
        ))}
      </List>
      
      {models.length === 0 && (
        <Box 
          display="flex" 
          alignItems="center" 
          justifyContent="center" 
          height="200px"
        >
          <Typography color="text.secondary">
            No models found
          </Typography>
        </Box>
      )}
    </Box>
  );
};

export default ModelList;