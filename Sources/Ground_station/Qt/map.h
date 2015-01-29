#ifndef MAP
#define MAP

//#include "libraries.h"
#include <QLabel>

class Map : public QWidget
{
    Q_OBJECT

    public:
    Map(QWidget *parent);
    QLabel* Coord[20][50];

};

#endif // MAP

