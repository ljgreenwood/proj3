<<<<<<< HEAD
# Relational Search System for <insert file type here> Files
### Resources
* https://pytorch.org/get-started/locally/
* https://pytorch-geometric.readthedocs.io/en/latest/
* https://www.geeksforgeeks.org/deep-learning/graph-neural-networks-with-pytorch/
* https://blogs.nvidia.com/blog/what-are-graph-neural-networks/
* https://modelnet.cs.princeton.edu/

### Setup Instructions

ensure that cuda is installed on your system
* https://developer.nvidia.com/cuda-downloads
* ensure that you have installed the pytorch and pytorch-geometric libraries
* pip install python and pip, then install the following

git clone https://github.com/ljgreenwood/proj3.git
python3 -m venv proj3env

once in the project directory run the below to ensure that the environment is initialized
`source proj3env/bin/activate`

=======
# Relational Search System for OFF files
### Resources
* https://modelnet.cs.princeton.edu/

### Setup Instructions
* pip install python version MAX 3.11.0 and pip, then install the following

git clone https://github.com/ljgreenwood/proj3.git
python -m venv proj3env

* once in the project directory run the below to ensure that the environment is initialized
`source proj3env/bin/activate`

* install the necessary libraries for jupyter 
>>>>>>> main
pip install ipython
pip install ipykernel
pip install jupyter // or just follow whatever install instructions to have these as globals just to make sure everything is there

<<<<<<< HEAD
* using \<insert python/pip version\>, CUDA 12.6, PyTorch 2.7.1, and PyG >= 2.3
`pip3 install torch torchvision torchaudio`
`pip install torch_geometric`

* Optional dependencies:
`pip install pyg_lib torch_scatter torch_sparse torch_cluster torch_spline_conv -f https://data.pyg.org/whl/torch-2.7.0+cu126.html`

python -m ipykernel install --user --name=proj3env
=======
* import project specific libraries
pip install open3d

python -m ipykernel install --user --name=proj3env
* For viewing OFF Files

before loading the OFF Files in the viewer script, ensure that you have symlink directory called data which points to the onedrive folder

ex: ln -s /home/lucas/OneDrive/Project3_Datatset/ModelNet10/ /home/lucas/proj3/ModelNet10

this command creates a symlink from the left path called ModelNet10 (path to the symlink is on the right)
>>>>>>> main
