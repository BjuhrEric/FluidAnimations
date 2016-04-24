// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 
 */

class GridTerrain
{
public:
	GridTerrain(unsigned int, unsigned int);
	void dispose();
	inline const int64 getCell(int32 x, int32 y) const;
	inline const int32 getHeight() const;
	inline const int32 getWidth() const;
	inline const void setCell(int32 x, int32 y, int64 ptr) const;

private:
	void init();
	const int32 width;
	const int32 height;
	int64 **cells;
};
