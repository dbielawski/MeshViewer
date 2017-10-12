==============================================================================
 * Introduction

Educational project made by four students following an Acquisition, Reconstruction
and Visualization module.
The program allows 3D visualization of Obj files and PG3M, rotation around objects,
selection of drawing mode, transparency.

Authors:
Alaric Braillon - Nicolas Palard - Jimmy Gouraud - Damien Bielawski

Version 1.0.0 15/10/2017

==============================================================================
 * Requirements
Qt 5.6 or higher
OpenGL 3.3 or higher
GCC ???

==============================================================================
 * Compilation and Execution

in MeshViewer repository:
	mkdir build && cd mkdir
	qmake ../MeshViewer.pro
	make -j
	./MeshViewer

If you have multiple versions of Qt
you can select qmake version as follow:
	qmake -qt=qt5 ../MeshViewer.pro

==============================================================================
 * Issue
X axis camera rotation (Gimbal lock)

