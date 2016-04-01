// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 
 */

const unsigned int TYPE_DESTRUCTIBLE_CELL = 0;
const unsigned int TYPE_EMPTY_CELL = 1;
const unsigned int TYPE_INDESTRUCTIBLE_CELL = 2;

class GridTerrain
{
public:
	GridTerrain(unsigned int, unsigned int);
	void dispose();
	inline const int getCellType(unsigned int x, unsigned int y) const;
	inline const unsigned int getHeight() const;
	inline const unsigned int getWidth() const;
	inline const void setCellType(unsigned int x, unsigned int y, unsigned int type) const;

private:
	void init();
	const unsigned int width;
	const unsigned int height;
	unsigned int **cells;
};
