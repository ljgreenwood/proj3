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

pip install ipython
pip install ipykernel
pip install jupyter // or just follow whatever install instructions to have these as globals just to make sure everything is there

* using \<insert python/pip version\>, CUDA 12.6, PyTorch 2.7.1, and PyG >= 2.3
`pip3 install torch torchvision torchaudio`
`pip install torch_geometric`

* Optional dependencies:
`pip install pyg_lib torch_scatter torch_sparse torch_cluster torch_spline_conv -f https://data.pyg.org/whl/torch-2.7.0+cu126.html`

python -m ipykernel install --user --name=proj3env
* For viewing OFF Files

pip install open3d