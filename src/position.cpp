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

int Position::operator% (Position const &other)
{
    int output = 0;
    if ((other.cap - this->cap)%360 < 180)
    {
        output = (other.cap - this->cap)%360;
        return output;
    }
    else
    {
        output = -(360 - ((other.cap - this->cap)%360));
        return output;
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