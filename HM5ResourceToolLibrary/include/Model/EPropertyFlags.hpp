#pragma once

enum class EPropertyFlags
{
	xAxisLocked = 1,
	yAxisLocked = 2,
	zAxisLocked = 4,
	hiResPositions = 8,
	ps3Edge = 16,
	color1 = 32,
	isNoPhysicsProp = 64
};

inline EPropertyFlags operator &(EPropertyFlags left, EPropertyFlags right)
{
	return static_cast<EPropertyFlags>(static_cast<int>(left) & static_cast<int>(right));
}
