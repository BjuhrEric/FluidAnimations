// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 
 */

const int32 TYPE_DESTRUCTIBLE_CELL = 0;
const int32 TYPE_EMPTY_CELL = 1;
const int32 TYPE_INDESTRUCTIBLE_CELL = 2;

class GridTerrain
{
public:
	GridTerrain(unsigned int, unsigned int);
	void dispose();
	inline const int32 getCellType(int32 x, int32 y) const;
	inline const int32 getHeight() const;
	inline const int32 getWidth() const;
	inline const void setCellType(int32 x, int32 y, int32 type) const;

private:
	void init();
	const int32 width;
	const int32 height;
	int32 **cells;
};
