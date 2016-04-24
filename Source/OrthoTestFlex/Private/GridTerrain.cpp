// Fill out your copyright notice in the Description page of Project Settings.

#include "OrthoTestFlex.h"
#include "GridTerrain.h"
	
	GridTerrain::GridTerrain(unsigned int w, unsigned int h) : width(w), height(h) {
		init();
	}

	void GridTerrain::dispose() {
		for (int32 i = 0; i < width; ++i) {
			delete[] cells[i];
		}
		delete[] cells;
	}

	inline const int64 GridTerrain::getCell(int32 x, int32 y) const {
		return cells[x][y];
	};

	inline const int32 GridTerrain::getHeight() const {
		return height; 
	};

	inline const int32 GridTerrain::getWidth() const {
		return width; 
	};
		
	inline const void GridTerrain::setCell(int32 x, int32 y, int64 ptr) const {
		cells[x][y] = ptr;
	};
		
	void GridTerrain::init() {
		cells = new int64*[(int) width];
		for (int32 i = 0; i < width; i++) {
			cells[i] = new int64[(int) height];
		}
	}