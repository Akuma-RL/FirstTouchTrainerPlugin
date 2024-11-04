#pragma once
#include "bakkesmod/wrappers/wrapperstructs.h"

class CanvasWrapper;

namespace RT
{
    class Circle2D
    {
    public:
        Vector2F location;
        Vector2 location2;
        float radius;
        int steps;
        float lineThickness;

        // CONSTRUCTORS
        Circle2D();
        Circle2D(Vector2F loc, float rad);
        Circle2D(Vector2F loc, float rad, int step);
        Circle2D(Vector2F loc, float rad, int step, float lineThicc);


        // FUNCTIONS
		void Draw(CanvasWrapper canvas) const;
    };

    class Circle2D2
    {
    public:
        Vector2 location;
        float radius;
        int steps;
        float lineThickness;

        void Draw(CanvasWrapper canvas) const;

        Circle2D2(Vector2 loc, float rad, int set, float lineThicc);
    };
}