#include "utils.h"
#include "structs.h"
#include <SDL_opengl.h>
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>

namespace dae
{
	void DrawRect(Rectf rect, float e)
	{
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

		glLineWidth(e);

		glBegin(GL_LINE_LOOP);

		glVertex2f(rect.left, rect.bottom);
		glVertex2f(rect.left + rect.width, rect.bottom);
		glVertex2f(rect.left + rect.width, rect.bottom + rect.height);
		glVertex2f(rect.left, rect.bottom + rect.height);

		glEnd();
	}
	void DrawRect(Rectf rect, Color4f color, float e)
	{
		glColor4f(color.r,color.g,color.b,color.a);

		glLineWidth(e);

		glBegin(GL_LINE_LOOP);

		glVertex2f(rect.left, rect.bottom);
		glVertex2f(rect.left + rect.width, rect.bottom);
		glVertex2f(rect.left + rect.width, rect.bottom + rect.height);
		glVertex2f(rect.left, rect.bottom + rect.height);

		glEnd();
	}
	void DrawFillRect(Rectf rect, Color4f color)
	{
		glColor4f(color.r, color.g, color.b, color.a);

		glBegin(GL_QUADS);

		glVertex2f(rect.left, rect.bottom);
		glVertex2f(rect.left + rect.width, rect.bottom);
		glVertex2f(rect.left + rect.width, rect.bottom + rect.height);
		glVertex2f(rect.left, rect.bottom + rect.height);

		glEnd();
	}
	void DrawEqualTriangle(float startX, float startY, float side)
	{
		glBegin(GL_TRIANGLES);

		glVertex2f(startX, startY);
		glVertex2f(startX + side, startY);
		glVertex2f(startX + side / 2, startY + side / 2 * float(sqrt(3)));

		glEnd();
	}
	void DrawGradient(Rectf rect, Color4f firstColor, Color4f secondColor)
	{
		for (int i{}; i < rect.width; i++)
		{
			float ColorMin{ float(i / rect.width) };
			float ColorPlus{ float((rect.width - i) / rect.width) };
			glColor4f(firstColor.r*ColorPlus + secondColor.r*ColorMin, firstColor.g*ColorPlus + secondColor.g*ColorMin,
				firstColor.b*ColorPlus + secondColor.b*ColorMin, firstColor.a*ColorPlus + secondColor.a*ColorMin);

			glBegin(GL_LINES);
			glVertex2f(rect.left + i, rect.bottom);
			glVertex2f(rect.left + i, rect.bottom + rect.height);
			glEnd();
		}
	}
	void DrawStar(float centerX, float centerY, float radius, Color4f Color)
	{
		float angle{};
		float changeX{};
		float changeY{};
		glColor3f(Color.r, Color.g, Color.b);
		glBegin(GL_LINE_LOOP);
		for (int i{}; i < 5; i++)
		{
			changeX = float(cos(angle * 2 * M_PI / 360)*radius);
			changeY = float(sin(angle * 2 * M_PI / 360)*radius);
			glVertex2f(centerX + changeX, centerY + changeY);
			angle += 144;
		}
		glEnd();
	}
	void DrawEllipse(Color4f Color, Point2f Center, Point2f radius)
	{
		int changeX{};
		int changeY{};
		glColor4f(Color.r, Color.g, Color.b, Color.a);
		glBegin(GL_TRIANGLE_FAN);

		glVertex2f(Center.x, Center.y);
		for (int i{}; i <= 90; i++)
		{
			changeX = int(cos(i * 4 * 2 * M_PI / 360)*radius.x);
			changeY = int(sin(i * 4 * 2 * M_PI / 360)*radius.y);
			glVertex2f(Center.x + changeX, Center.y + changeY);
		}
		glEnd();

	}
	float GetDistanceCircle(Point2f point, Point2f circleCenter, float Radius)
	{
		float distance{};
		distance = sqrt(pow((point.x - circleCenter.x), 2) + pow((point.y - circleCenter.y), 2)) - Radius;
		return distance;
	}
	float GetDistance2Circles(Point2f circleCenter, float Radius, Point2f circleCenter2, float Radius2)
	{
		float distance{};
		distance = sqrt(pow((circleCenter.x - circleCenter2.x), 2) + pow((circleCenter.y - circleCenter2.y), 2)) - Radius - Radius2;
		return distance;
	}
	bool Is2CirclesOverlapping(Point2f circleCenter, float Radius, Point2f circleCenter2, float Radius2)
	{
		bool check{ false };
		if (GetDistance2Circles(circleCenter, Radius, circleCenter2, Radius2) < 0)
		{
			check = true;
		}
		return check;
	}
	bool Is2RectOverlapping(Rectf rect, Rectf rect2)
	{
		bool check{ false };
		if (rect.bottom + rect.height > rect2.bottom 
			&& rect.bottom   < rect2.bottom + rect2.height 
			&& rect.left + rect.width > rect2.left
			&&  rect.left < rect2.left  + rect2.width)
		{
			check = true;
		}
		return check;
	}
	bool IsPointInRect(Rectf rect, Point2f Mouse)
	{
		bool check{ false };
		if (IsXBetween(rect.left+rect.width, rect.left, Mouse.x) && IsYBetween(rect.bottom + rect.height, rect.bottom, Mouse.y))
		{
			check = true;
		}
		return check;
	}
	bool IsXBetween(float rectXRight, float rectXLeft, float MouseX)
	{
		bool check{ false };

		if (MouseX<rectXRight && MouseX > rectXLeft)
		{
			check = true;
		}
		return check;
	}
	bool IsYBetween(float rectYTop, float rectYBottom, float MouseY)
	{
		bool check{ false };

		if (MouseY < rectYTop && MouseY > rectYBottom)
		{
			check = true;
		}
		return check;
	}
	int GetArrayIndex(int rowIdx, int colIdx, int cols)
	{
		int idx{};
		idx = rowIdx * cols + colIdx % cols;
		return idx;
	}
	void InitializeArrayTable(int *array2D, const int cols, const int rows)
	{


		for (int rowIdx{}; rowIdx < rows; rowIdx++)
		{
			std::cout << std::endl;
			for (int collIdx{}; collIdx < cols; collIdx++)
			{
				array2D[GetArrayIndex(rowIdx, collIdx, cols)] += 10 * (rowIdx + 1);
				array2D[GetArrayIndex(rowIdx, collIdx, cols)] += 1 * (collIdx + 1);
				std::cout << array2D[GetArrayIndex(rowIdx, collIdx, cols)] << " ";
			}

		}
	}
	void PrintElements(int * pNumbers, int size)
	{
		for (int i{}; i < size; i++)
		{
			std::cout << *(pNumbers + i);
		}
		std::cout << std::endl;
	}
	void PrintElements(int* pNumbers, int startIdx, int endIdx)
	{
		for (startIdx; startIdx <= endIdx; startIdx++)
		{
			std::cout << *(pNumbers + startIdx);
		}
		std::cout << std::endl;
	}
	void Swap(int * array, int firstIdx, int secondIdx)
	{
		int oldNumber{};
		int oldNumber2{};
		oldNumber = array[firstIdx];
		oldNumber2 = array[secondIdx];
		array[firstIdx] = oldNumber2;
		array[secondIdx] = oldNumber;

	}
	void Shuffle(int * array, const int size, int amountOfShuffles)
	{
		int firstIdx{};
		int secondIdx{};
		int amountIndexEqual{};
		for (int i{}; i<amountOfShuffles + amountIndexEqual; i++)
		{
			firstIdx = rand() & (size - 1);
			secondIdx = rand() & (size - 1);
			if (firstIdx == secondIdx)
			{
				amountIndexEqual++;
			}
			Swap(array, firstIdx, secondIdx);
		}
	}
	void BubbleSort(int * array, const int size)
	{
		int passes{};
		while (passes < (size - 1))
		{
			for (int i{}; i < (size - 1 - passes); i++)
			{
				if (array[i] > array[i + 1])
				{
					Swap(array, i, i + 1);
				}
			}
			passes++;
		}
	}
	void DrawGrid(bool *array, float XPos, float YPos, const int rows, const int coll, float cellWidth, float cellHeight)
	{
		for (int i{}; i<coll; i++)
		{

			for (int j{}; j<rows; j++)
			{
				Rectf rect{ XPos ,YPos, cellWidth*(i + 1) ,cellHeight*j };
				dae::DrawRect(rect);
			}
		}

	}
}

