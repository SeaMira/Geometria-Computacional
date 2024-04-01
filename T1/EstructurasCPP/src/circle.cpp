#include<iostream>
#include<cmath>


#include<Circle/circle.hpp>

circle::circle(double radius)
{
    this->radius = radius;
}

circle::~circle()
{
    std::cout<<"Deleted circle"<<std::endl;
}


void circle::set(double x)
{
    this->radius = x;
}

double circle::calculateArea()
{
    return this->radius*this->radius ;
}

double circle::calculateCircumference()
{
    return this->radius*2;
}