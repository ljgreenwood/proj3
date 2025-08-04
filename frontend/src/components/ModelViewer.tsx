import React, { useRef, useEffect } from 'react';
import * as THREE from 'three';
import { ModelGeometry } from '../types';

interface ModelViewerProps {
  geometry: ModelGeometry;
  position: 'left' | 'right';
}

const ModelViewer: React.FC<ModelViewerProps> = ({ geometry, position }) => {
  const containerRef = useRef<HTMLDivElement>(null);
  const sceneRef = useRef<THREE.Scene>();
  const rendererRef = useRef<THREE.WebGLRenderer>();
  const cameraRef = useRef<THREE.PerspectiveCamera>();
  const meshRef = useRef<THREE.Mesh>();
  const frameRef = useRef<number>();

  useEffect(() => {
    if (!containerRef.current) return;

    const initializeScene = () => {
      if (!containerRef.current) return;
      
      // Wait for container to have stable dimensions
      const width = containerRef.current.clientWidth;
      const height = containerRef.current.clientHeight;
      
      if (width <= 0 || height <= 0) {
        // Retry after a short delay if dimensions aren't ready
        setTimeout(initializeScene, 50);
        return;
      }

      // Initialize Three.js scene
      const scene = new THREE.Scene();
      scene.background = new THREE.Color(0xf0f0f0);
      sceneRef.current = scene;

      // Initialize camera with stable dimensions
      const camera = new THREE.PerspectiveCamera(75, width / height, 0.1, 1000);
      camera.position.set(0, 0, 3);
      cameraRef.current = camera;

      // Initialize renderer with stable dimensions
      const renderer = new THREE.WebGLRenderer({ antialias: true });
      renderer.setSize(width, height);
      renderer.shadowMap.enabled = true;
      renderer.shadowMap.type = THREE.PCFSoftShadowMap;
      rendererRef.current = renderer;

      containerRef.current.appendChild(renderer.domElement);

      // Add lights
      const ambientLight = new THREE.AmbientLight(0xffffff, 0.6);
      scene.add(ambientLight);

      const directionalLight = new THREE.DirectionalLight(0xffffff, 0.8);
      directionalLight.position.set(10, 10, 5);
      directionalLight.castShadow = true;
      scene.add(directionalLight);

      // Handle resize with ResizeObserver for better container size detection
      const handleResize = () => {
        if (containerRef.current && cameraRef.current && rendererRef.current) {
          const width = containerRef.current.clientWidth;
          const height = containerRef.current.clientHeight;
          
          // Only resize if we have valid dimensions
          if (width > 0 && height > 0) {
            cameraRef.current.aspect = width / height;
            cameraRef.current.updateProjectionMatrix();
            rendererRef.current.setSize(width, height);
          }
        }
      };

      // Use both window resize and ResizeObserver for comprehensive size detection
      window.addEventListener('resize', handleResize);
      
      let resizeObserver: ResizeObserver | null = null;
      if (containerRef.current) {
        resizeObserver = new ResizeObserver(() => {
          // Small delay to ensure layout is stable
          setTimeout(handleResize, 10);
        });
        resizeObserver.observe(containerRef.current);
      }

      // Mouse controls for rotation
      let isDragging = false;
      let previousMousePosition = { x: 0, y: 0 };

      const handleMouseDown = (event: MouseEvent) => {
        isDragging = true;
        previousMousePosition = { x: event.clientX, y: event.clientY };
      };

      const handleMouseMove = (event: MouseEvent) => {
        if (!isDragging || !meshRef.current) return;

        const deltaMove = {
          x: event.clientX - previousMousePosition.x,
          y: event.clientY - previousMousePosition.y,
        };

        meshRef.current.rotation.y += deltaMove.x * 0.01;
        meshRef.current.rotation.x += deltaMove.y * 0.01;

        previousMousePosition = { x: event.clientX, y: event.clientY };
      };

      const handleMouseUp = () => {
        isDragging = false;
      };

      renderer.domElement.addEventListener('mousedown', handleMouseDown);
      window.addEventListener('mousemove', handleMouseMove);
      window.addEventListener('mouseup', handleMouseUp);

      // Animation loop
      const animate = () => {
        frameRef.current = requestAnimationFrame(animate);
        
        if (rendererRef.current && sceneRef.current && cameraRef.current) {
          rendererRef.current.render(sceneRef.current, cameraRef.current);
        }
      };
      animate();

      // Return cleanup function
      return () => {
        if (frameRef.current) {
          cancelAnimationFrame(frameRef.current);
        }
        
        window.removeEventListener('resize', handleResize);
        renderer.domElement.removeEventListener('mousedown', handleMouseDown);
        window.removeEventListener('mousemove', handleMouseMove);
        window.removeEventListener('mouseup', handleMouseUp);
        
        if (resizeObserver) {
          resizeObserver.disconnect();
        }
        
        if (containerRef.current && renderer.domElement) {
          containerRef.current.removeChild(renderer.domElement);
        }
        
        renderer.dispose();
      };
    };

    // Start initialization with a small delay to ensure container is rendered
    const cleanup = initializeScene();
    
    return cleanup;
  }, []);

  useEffect(() => {
    if (!geometry || !sceneRef.current) return;

    // Remove existing mesh
    if (meshRef.current) {
      sceneRef.current.remove(meshRef.current);
      meshRef.current.geometry.dispose();
      if (Array.isArray(meshRef.current.material)) {
        meshRef.current.material.forEach(material => material.dispose());
      } else {
        meshRef.current.material.dispose();
      }
    }

    // Create Three.js geometry from our data
    const threeGeometry = new THREE.BufferGeometry();

    // Convert vertices array to Float32Array
    const vertices = new Float32Array(geometry.vertices.flat());
    threeGeometry.setAttribute('position', new THREE.BufferAttribute(vertices, 3));

    // Convert faces to indices
    const indices = new Uint32Array(geometry.faces.flat());
    threeGeometry.setIndex(new THREE.BufferAttribute(indices, 1));

    // Add normals if available
    if (geometry.normals && geometry.normals.length > 0) {
      const normals = new Float32Array(geometry.normals.flat());
      threeGeometry.setAttribute('normal', new THREE.BufferAttribute(normals, 3));
    } else {
      threeGeometry.computeVertexNormals();
    }

    // Center and scale the geometry
    threeGeometry.computeBoundingBox();
    const boundingBox = threeGeometry.boundingBox!;
    const center = new THREE.Vector3();
    boundingBox.getCenter(center);
    
    const size = new THREE.Vector3();
    boundingBox.getSize(size);
    const maxDimension = Math.max(size.x, size.y, size.z);
    const scale = 2 / maxDimension; // Scale to fit in a 2x2x2 box

    // Apply centering and scaling
    threeGeometry.translate(-center.x, -center.y, -center.z);
    threeGeometry.scale(scale, scale, scale);

    // Create material with different colors for left/right
    const material = new THREE.MeshPhongMaterial({
      color: position === 'left' ? 0x4CAF50 : 0x2196F3, // Green for left, blue for right
      shininess: 100,
      side: THREE.DoubleSide,
    });

    // Create and add mesh
    const mesh = new THREE.Mesh(threeGeometry, material);
    mesh.castShadow = true;
    mesh.receiveShadow = true;
    meshRef.current = mesh;

    sceneRef.current.add(mesh);
  }, [geometry, position]);

  return (
    <div 
      ref={containerRef} 
      style={{ 
        width: '100%', 
        height: '100%', 
        cursor: 'grab',
        borderRadius: '4px',
        overflow: 'hidden'
      }}
    />
  );
};

export default ModelViewer;