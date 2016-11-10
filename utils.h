#pragma once
#include "structs.h"
namespace dae
{
	void DrawRect(Rectf rect, float e = 1);
	void DrawFillRect(Rectf rect, Color4f color);
	void DrawEqualTriangle(float startX, float startY, float side);
	void DrawGradient(Rectf rect, Color4f firstColor, Color4f secondColor);
	void DrawStar(float centerX, float centerY, float radius, Color4f Color);
	void DrawEllipse(Color4f Color, Point2f Center, Point2f radius);
	float GetDistanceCircle(Point2f point, Point2f circleCenter, float circle2Radius);
	float GetDistance2Circles(Point2f circleCenter, float Radius, Point2f circleCenter2, float Radius2);
	bool Is2CirclesOverlapping(Point2f circleCenter, float Radius, Point2f circleCenter2, float Radius2);
	bool Is2RectOverlapping(Rectf rect, Rectf rect2);
	bool IsPointInRect(Rectf rect, Point2f Mouse);
	bool IsXBetween(float rectXRight, float rectXLeft, float MouseX);
	bool IsYBetween(float rectYTop, float rectYBottom, float MouseY);
	int GetArrayIndex(int rowIdx, int colIdx, int cols);
	void InitializeArrayTable(int *array2D, const int cols, const int rows);
	void PrintElements(int* pNumbers, int size);
	void PrintElements(int* pNumbers, int startIdx, int endIdx);
	void Swap(int * array, int firstIdx, int secondIdx);
	void Shuffle(int * array, const int size, int amountOfShuffles);
	void BubbleSort(int * array, const int size);
	void DrawGrid(bool *array, float XPos, float YPos, const int rows, const int coll, float cellWidth, float cellHeight);
	
	
}
