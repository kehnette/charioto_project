#include "position.hpp"
#include <math.h>
#include "debugger.hpp"

double Position::lat0;
double Position::long0;
bool Position::initialized = false;
float Position::TO_Y;
float Position::TO_X;

Position::Position()
{
    this->latitude = 0;
    this->longitude = 0;
    this->cap = 0;
}

Position::Position(double latitude, double longitude, float cap)
{
    this->latitude = latitude;
    (*this).longitude = longitude;
    (*this).cap = cap;
}

void Position::initPosition0(double latitude, double longitude)
{
    Position::lat0=latitude;
    Position::long0=longitude;
    Position::initialized = true;
    Position::TO_Y = 1852*60;
    Position::TO_X = 1852*60*cos(lat0);
}

double Position::getX()
{
    if (initialized)
    {
        return longitude*Position::TO_X;
    }
    else
    {
        debuggerRed(HIGH);
        return 0;
    }
}

double Position::getY()
{
    if (initialized)
    {
        return latitude*Position::TO_Y;
    }
    else
    {
        debuggerRed(HIGH);
        return 0;
    }
}

Position Position::operator+ (Position const &other)
{
    Position pos = Position();
    pos.latitude = this->latitude + other.latitude;
    pos.longitude = this->longitude + other.longitude;
    pos.cap = this->cap;
    return pos;
}

Position Position::operator- (Position const &other)
{
    Position pos = Position();
    pos.latitude = this->latitude - other.latitude;
    pos.longitude = this->longitude - other.longitude;
    pos.cap = this->cap;
    return pos;
}

Position Position::operator* (float other)
{
    Position pos = Position();
    pos.latitude = this->latitude * other;
    pos.longitude = this->longitude + other;
    pos.cap = this->cap;
    return pos;
}

float Position::operator% (Position &other)
{
    double X1 = this->getX(); //P1.X1
    double X2 = other.getX(); //P2.X2
    double Y1 = this->getY(); //P1.Y1
    double Y2 = other.getY(); //P2.Y2
    double X = X2-X1;
    double Y = Y2-Y1;
    float output = 0; //le cap entre les deux point P1 et P2
    float alpha = 0;
    double H = *this>>other;

    if (X > 0)
    {
        alpha = asin(Y/H);
        if (Y > 0) //On entre dans le cas 1
        {
            output = 90 - alpha;
        }
        else //On entre dans le cas 2
        {
            output = alpha + 90;
        }
    }
    else
    {
        alpha = asin(X/H);
        if (Y > 0) //On entre dans le cas 3
        {
            output = 360 - alpha;
        }
        else // On entre dans le cas 4
        {
            output = 270 - alpha;
        }
    }
}

float Position::operator>> (Position &other)
{
    double deltaX = 0;
    double deltaY = 0;
    deltaX = this->getX() - other.getX();
    deltaY = this->getY() - other.getY();
    return sqrt(deltaX*deltaX + deltaY*deltaY);
}

float Position::moduloFloat360(float x)
{
    while (x>=360)
    {
        x = x - 360;
    }
    while (x<0)
    {
        x = x + 360;
    }
    return x;
}