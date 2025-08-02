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
pip install ipython
pip install ipykernel
pip install jupyter // or just follow whatever install instructions to have these as globals just to make sure everything is there

* import project specific libraries
pip install open3d

python -m ipykernel install --user --name=proj3env
* For viewing OFF Files

before loading the OFF Files in the viewer script, ensure that you have symlink directory called data which points to the onedrive folder

ex: ln -s /home/lucas/OneDrive/Project3_Datatset/ModelNet10/ /home/lucas/proj3/ModelNet10

this command creates a symlink from the left path called ModelNet10 (path to the symlink is on the right)

