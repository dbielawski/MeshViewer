# MeshViewer

MeshViewer is an educational project made by four students following an Acquisition, Reconstruction
and Visualization module.
The program contains the following features:
* 3D visualization of OBJ and PGM3D files
* 3D movements around the objects
* Different drawing mode
* Bounding box visualization
* Opacity managment
* Exporting as an OBJ file

# Getting Started

## Prerequisites
```
Qt 5.6 or higher
OpenGL 3.3 or higher
A fairly recent C++ compiler
```
## Installation
```
mkdir build ; cd build
qmake ../MeshViewer.pro
make -j
```
**Note**: If you have multiple Qt version installed you can tell qmake with which version it should
compile, for example:
```
qmake -qt=qt5 ../MeshViewer.pro
```
This will create a **MeshViewer** executable. Use the following command to run it:
```
./MeshViewer
```

## Authors
* [Braillon Alaric](https://github.com/AlaricBr)
* [Palard Nicolas](https://github.com/nicpalard)
* [Gouraud Jimmy](https://github.com/krazyxx)
* [Bielawski Damien](https://github.com/dbielawski)

## Version
1.0.0 - 15/10/2017

## Known Issues
* X axis camera rotation (Gimbal lock)

