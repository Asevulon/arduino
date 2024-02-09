#include"pch.h"
#include "Global.h"

double rand(double left, double right)
{
    return left + rand() * (right - left) / double(RAND_MAX);
}
