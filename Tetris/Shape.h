#pragma once

#include <vector>
#include <stdlib.h>

#define STEPS_TO_MOVE 100

using namespace std;


struct Point
{
    int x;
    int y;

    Point(int x, int y)
    {
        this->x = x;
        this->y = y;
    }
};


struct Matrix
{
    vector<vector<bool>> field;
    int width;
    int height;

    Matrix(int width, int height)
    {
        this->width = width;
        this->height = height;

        reset();
    }

    void reset()
    {
        field.clear();

        for (int y = 0; y < height; y++)
        {
            field.push_back(vector<bool>());

            for (int x = 0; x < width; x++)
            {
                field[y].push_back(false);
            }
        }
    }
};


class Shape
{
private:
    Matrix* matrix;
    int moveCounter;

public:
    int x;
    int y;
    int speed;
    vector<Point>* blocks;
    Point center;

    Shape(Matrix* matrix, int speed, int type = -1);
    bool checkForCollision(int x, int y, vector<Point>* blocks = nullptr);
    bool update();
    bool rotateLeft();
    bool rotateRight();
    bool moveLeft();
    bool moveRight();
    bool moveDown();
    ~Shape();
};