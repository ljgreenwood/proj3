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
} from '@mui/material';
import { ModelInfo } from '../types';

interface ModelListProps {
  models: ModelInfo[];
  onModelSelect: (model: ModelInfo) => void;
  selectedModel1: ModelInfo | null;
}

const ModelList: React.FC<ModelListProps> = ({ 
  models, 
  onModelSelect, 
  selectedModel1
}) => {
  const isSelected = (model: ModelInfo) => {
    return selectedModel1?.filename === model.filename;
  };

  return (
    <Box sx={{ height: '100%', overflow: 'auto', pr: 1 }}>
      {models.length === 0 ? (
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
      ) : (
        <List dense sx={{ py: 0 }}>
          {models.map((model) => (
            <ListItem key={model.filename} disablePadding sx={{ mb: 2 }}>
              <Box sx={{ width: '100%' }}>
                <ListItemButton
                  sx={{ 
                    mb: 1,
                    borderRadius: 1,
                    border: '1px solid',
                    borderColor: 'grey.300',
                    '&:hover': { borderColor: 'primary.main' },
                    bgcolor: 'background.paper'
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
                
                <Button
                  size="small"
                  fullWidth
                  variant={isSelected(model) ? "contained" : "outlined"}
                  color="primary"
                  onClick={() => onModelSelect(model)}
                  sx={{ 
                    fontSize: '0.75rem',
                    bgcolor: isSelected(model) ? 'primary.main' : 'transparent',
                    '&:hover': {
                      bgcolor: isSelected(model) ? 'primary.dark' : 'primary.light'
                    }
                  }}
                >
                  {isSelected(model) ? 'Selected' : 'Select Model'}
                </Button>
              </Box>
            </ListItem>
          ))}
        </List>
      )}
    </Box>
  );
};

export default ModelList;