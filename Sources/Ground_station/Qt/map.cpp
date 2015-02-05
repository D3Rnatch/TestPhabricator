#include "libraries.h"
#include <QGridLayout>
#include <QScrollBar>
#include <QScrollArea>
//#include <QWidget>

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
            Coord[i][j]->setFixedSize(200,200);
            a->addWidget(Coord[i][j],i,j);
        }
    }


    /*QScrollArea *area = new QScrollArea(this);
    area->setWidget(this);

    area->horizontalScrollBar(50);
    area->verticalScrollBar(20);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(new QLabel("something else",this));
    layout->addWidget(area);

    setLayout(layout);*/

}
