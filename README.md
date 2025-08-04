# MeshMatcher

### Dataset (ModelNet10)
* https://modelnet.cs.princeton.edu/

### Setup Instructions
VERSIONS ::
* NODE :: 22.17.0
* NPM :: 10.9.2
* NVM (if using) :: 0.39.7
* Python :: 3.11.0
* Pip :: 

git clone https://github.com/ljgreenwood/proj3.git

before loading the OFF Files in the viewer script, ensure that you have symlink directory called data which points to the onedrive folder

ex: ln -s /home/lucas/OneDrive/Project3_Datatset/ModelNet10/ /home/lucas/proj3/ModelNet10

this command creates a symlink from the left path called ModelNet10 (path to the symlink is on the right)


#### Arch Linux
* install nvm using paru or equivalent
```bash
    paru -S nvm
```
* if this does not work run :: 
```zsh
    curl -o- https://raw.githubusercontent.com/nvm-sh/nvm/v0.39.7/install.sh | bash
```
* check to ensure that the end of the zshrc or bashrc has lines pretaining to initializing nvm for the current session
* install node 22.17.0 using 
```bash
nvm install 22.17.0
```
* (this should also install npm - if it doesnt : exit and reload terminal)
* install npm 10.9.2 using
```bash
npm install -g npm@10.9.2
```
* check that everything has installed properly by running
```bash
npm -v
```

#### WINDOWS






#### SEE README.md in frontend / backend for appropriate instructions