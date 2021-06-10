#include "fly.h"

#include <QRandomGenerator>

Fly::Fly( std::shared_ptr< Cell > cell, unsigned int T,  unsigned int M, QObject *parent)
    : QThread(parent)
    , _cell( cell )
    , _stupidity( T )
    , _ageLimit( T * M )
{
    static unsigned int num = 1;
    _num = num++;
}

Fly::~Fly()
{
}

float Fly::speed() const
{
    return _age ? static_cast< float >(_mileage)/ _age : .0 ;
}

unsigned int Fly::mileage() const
{
    return _mileage;
}

unsigned int Fly::age() const
{
    return _age;
}

bool Fly::isDead() const
{
    return _age >= _ageLimit;
}

unsigned int Fly::stupidity() const
{
    return _stupidity;
}


void Fly::run()
{
    for ( ; _age < _ageLimit; ++_age )
    {
        msleep( _stupidity * 1000 );
        std::shared_ptr<Cell> nextCell = directionOfFlight();
        if ( nextCell && nextCell->flyIn( shared_from_this() ) )
        {
            _cell.lock()->flyOut( shared_from_this() );
            _cell = nextCell;
            _mileage++;
        }
    }
    _cell.lock()->flyDead( this );
}

std::shared_ptr<Cell> Fly::directionOfFlight()
{
    QVector< std::shared_ptr<Cell> > nearCells = _cell.lock()->nearbyNonzeroCells();
    if ( nearCells.isEmpty() )
    {
        return std::shared_ptr<Cell>();
    }
    int cellNum = QRandomGenerator::global()->generate() % nearCells.size();
    return nearCells[cellNum];
}

unsigned int Fly::num() const
{
    return _num;
}

const std::weak_ptr<Cell> &Fly::cell() const
{
    return _cell;
}
