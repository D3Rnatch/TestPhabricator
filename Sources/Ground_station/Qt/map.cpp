#include "libraries.h"
#include <QGridLayout>

#include "map.h"

using namespace std;

Map::Map(QWidget *parent)
{
    this->setParent(parent);
    QGridLayout *a = new QGridLayout(this);


    for(int i=0; i< 20; i++)
    {
        for(int j=0; j< 50; j++)
        {
           Coord[i][j]= new QLabel(this);
            Coord[i][j]->setText("-");
            a->addWidget(Coord[i][j],i,j);
        }
    }
}
