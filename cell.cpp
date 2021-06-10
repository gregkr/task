#include "cell.h"
#include "fly.h"
#include "field.h"

unsigned int Cell::_maxFliesCountPerCell = 0;

Cell::Cell( int row, int col, QObject *parent)
    : QObject(parent)
    , _coord(row, col )
    , _flyVec( _maxFliesCountPerCell )
{

}

std::shared_ptr<Cell> Cell::nearCellInDirection( Cell::Direction dir ) const
{
    return _nearCells[ static_cast<unsigned int>(dir)].lock();
}

void Cell::setNearCellInDirection(std::shared_ptr<Cell> cell, Direction dir)
{
    _nearCells[static_cast<unsigned int>(dir)] = cell;
}

QVector< std::shared_ptr<Cell> > Cell::nearbyNonzeroCells() const
{
    QVector< std::shared_ptr<Cell> > result;
    for( std::weak_ptr< Cell > cell: _nearCells)
    {
        if ( cell.lock() )
        {
            result.push_back( cell.lock() );
        }
    }
    return result;
}

bool Cell::flyIn( std::shared_ptr< Fly > fly)
{
    bool result = false;
    if ( _maxFliesCountPerCell )
    {
        if ( _flyListMutex.tryLock( fly->stupidity()*1000 ) )
        {
            int pos = _flyVec.lastIndexOf( nullptr );
            if ( pos > -1 )
            {
                _flyVec[pos] = fly;
                result = true;
            }
            _flyListMutex.unlock();
        }
    }
    if ( result )
    {
        emit cellStatusChanged( _coord.first, _coord.second );
    }
    return result;
}

bool Cell::flyOut( std::shared_ptr< Fly > fly)
{
    bool result = false;
    if ( _maxFliesCountPerCell )
    {
        _flyListMutex.lock();
        int pos = _flyVec.indexOf( fly );
        if ( pos > -1 )
        {
            _flyVec[pos].reset();
        }
        _flyListMutex.unlock();
    }
    emit cellStatusChanged( _coord.first, _coord.second );
    return result;
}

std::shared_ptr<Fly> Cell::flyAtPosition(unsigned int pos)
{
    if ( pos >= _maxFliesCountPerCell )
    {
        return std::shared_ptr<Fly>();
    }
    return _flyVec[pos];
}

bool Cell::setFlyToPosition( unsigned int pos, unsigned int T )
{
    if ( pos >= _flyVec.size() || _flyVec[pos] )
    {
        return false;
    }

    bool result = false;
    Field* field = qobject_cast< Field* >( parent() );
    if ( field )
    {
        _flyVec[pos] = std::make_shared<Fly>( shared_from_this(), T, field->size() );
        result = true;
    }

    return result;
}

bool Cell::addFly(unsigned int T)
{
    bool result = false;
    if ( _maxFliesCountPerCell )
    {
        _flyListMutex.lock();
        int pos = _flyVec.indexOf( nullptr );
        if ( pos > -1 )
        {
            Field* field = qobject_cast< Field* >( parent() );
            if ( field )
            {
                _flyVec[pos] = std::make_shared<Fly>( shared_from_this(), T, field->size() );
                result = true;
                emit cellStatusChanged( _coord.first, _coord.second );
            }
        }
        _flyListMutex.unlock();
    }
    return result;
}

unsigned int Cell::maxFliesCountPerCell()
{
    return _maxFliesCountPerCell;
}

void Cell::setMaxFliesCountPerCell(unsigned int newMaxFliesCountPerCell)
{
    _maxFliesCountPerCell = newMaxFliesCountPerCell;
}

QVector<int> Cell::flyVector() const
{
    QVector<int> result;
    for ( std::shared_ptr< Fly > fly : _flyVec)
    {
        if ( fly )
        {
            result.push_back( fly->isDead() ? 2 : 1 );
        }
        else
        {
            result.push_back(0);
        }
    }
    return result;
}

void Cell::resizeMaxFlies()
{
    QVector< std::shared_ptr< Fly > > newVec( _maxFliesCountPerCell );
    const unsigned int size = ( _flyVec.size() < _maxFliesCountPerCell ) ? _flyVec.size() : _maxFliesCountPerCell;
    for ( unsigned int i = 0; i < size; ++i)
    {
       newVec[i] = _flyVec[i];
    }
    _flyVec.swap( newVec );
}

void Cell::flyDead(Fly *fly)
{
    emit cellStatusChanged( _coord.first, _coord.second );
}

QList<FlyStat> Cell::statistic() const
{
    QList<FlyStat> result;
    for( std::shared_ptr< Fly > fly : _flyVec)
    {
        if ( fly )
        {
            FlyStat s;
            s.cellRow = _coord.first;
            s.cellCol = _coord.second;
            s.speed = fly->speed();
            s.mileage = fly->mileage();
            s.isDead = fly->isDead();
            result.push_back( s );
        }
    }
    return result;
}

void Cell::onReviveFlies()
{
    for( std::shared_ptr< Fly >& fly : _flyVec)
    {
        if ( fly && !fly->isDead() )
        {
            fly->start();
        }
    }
}

void Cell::onFreezeFlies()
{
    for( std::shared_ptr< Fly >& fly : _flyVec)
    {
        if ( fly && !fly->isDead() )
        {
            fly->terminate();
        }
    }
}
