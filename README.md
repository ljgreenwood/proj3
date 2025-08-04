# MeshMatcher

### Setup Instructions
VERSIONS ::
* NODE :: 22.17.0
* NPM :: 10.9.2
* NVM (if using) :: 0.39.7
* Python :: 3.11.0

#### Dataset (ModelNet10)
1. Go to https://modelnet.cs.princeton.edu/
2. Find the "ModelNet10.zip" highlighted in orange. Right click and open in new tab. This will start the download.
3. git clone https://github.com/ljgreenwood/proj3.git
4. move the ModelNet10.zip into the director and unzip. In the unzipped contents move the folder "ModelNet10" into the root directory of proj3
5. you're all set!

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
* Go to https://nodejs.org/en/download and download and run the installer 
* Ensure everything installed properly 
```bash
node -v
npm -v
```




#### SEE README.md in frontend / backend for appropriate instructions