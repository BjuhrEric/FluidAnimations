// Fill out your copyright notice in the Description page of Project Settings.

#include "OrthoTestFlex.h"
#include "GridTerrain.h"
	
	GridTerrain::GridTerrain(unsigned int w, unsigned int h) : width(w), height(h) {
		init();
	}

	void GridTerrain::dispose() {
		for (unsigned int i = 0; i < width; ++i) {
			delete[] cells[i];
		}
		delete[] cells;
	}

	inline const int GridTerrain::getCellType(unsigned int x, unsigned int y) const {
		return cells[x][y];
	};

	inline const unsigned int GridTerrain::getHeight() const {
		return height; 
	};

	inline const unsigned int GridTerrain::getWidth() const {
		return width; 
	};
		
	inline const void GridTerrain::setCellType(unsigned int x, unsigned int y, unsigned int type) const {
		cells[x][y] = type;
	};
		
	void GridTerrain::init() {
		cells = new unsigned int*[width];
		for (unsigned int i = 0; i < width; i++) {
			cells[i] = new unsigned int[height];
		}
	}