#ifndef _POSITION_HPP
#define _POSITION_HPP

/* Cette classe stocke latitude longitude
    Elle définit aussi un repère (x, y).
    où y positif pointe vers le nord, le
    x positif pointe vers l'est et la position
    (0, 0) est définie comme la position
    initiale de Charioto(R) */

class Position
{

public:
    Position();
    Position(double latitude, double longitude, float cap);
    double latitude;
    double longitude;
    int cap;
    double getX();
    double getY();
    static void initPosition0(double latitude, double longitude);
    Position operator+ (Position const &other);
    Position operator- (Position const &other);
    Position operator* (float other);
    int operator% (Position const &other);
    //- - -> Donne le delta entre deux caps
    float operator>> (Position &other);
    //- - -> Donne la distance entre deux Positions

private:
    static double lat0;
    static double long0;
    static bool initialized;
    static float TO_X;
    static float TO_Y;
};

#endif

