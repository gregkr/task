#include "field.h"

Field::Field(QObject *parent) : QObject(parent)
{

}

unsigned int Field::size() const
{
    return _size;
}

void Field::create( unsigned int M )
{
    clear();
    _size = M;

    for( unsigned int row = 0; row < _size; ++row )
    {
        std::vector< std::shared_ptr< Cell > > rowVec( _size, nullptr);
        std::vector< std::shared_ptr< Cell > >::iterator it = rowVec.begin();
        for( ; it != rowVec.end(); ++it )
        {
            *it = std::make_shared< Cell >( row, it - rowVec.begin(),  this );
        }
        _cellMatrix.push_back( rowVec );
    }

    for( unsigned int row = 0; row < _size; ++row )
    {
        for( unsigned int col = 0; col < _size; ++col )
        {
            if ( row > 0 )
            {
               _cellMatrix[row][col]->setNearCellInDirection( _cellMatrix[row - 1][col], Cell::Direction::NORTH);
            }
            if ( col > 0 )
            {
               _cellMatrix[row][col]->setNearCellInDirection( _cellMatrix[row][col-1], Cell::Direction::WEST);
            }
            if ( row < _size - 1 )
            {
               _cellMatrix[row][col]->setNearCellInDirection( _cellMatrix[row + 1][col], Cell::Direction::SOUTH);
            }
            if ( col < _size - 1 )
            {
               _cellMatrix[row][col]->setNearCellInDirection( _cellMatrix[row][col + 1], Cell::Direction::EAST);
            }
            connect( _cellMatrix[row][col].get(), &Cell::cellStatusChanged, this, &Field::onCellStateChanged );
            connect( this, &Field::reviveFlies, _cellMatrix[row][col].get(), &Cell::onReviveFlies );
            connect( this, &Field::freezeFlies, _cellMatrix[row][col].get(), &Cell::onFreezeFlies );
        }
    }
}

void Field::clear()
{
    for( std::vector< std::shared_ptr< Cell > > row: _cellMatrix)
    {
        row.clear();
    }
    _cellMatrix.clear();
}

std::shared_ptr<Cell> Field::cellAtPosition(unsigned int row, unsigned int col) const
{
    if ( row >= _size || col >= _size )
    {
        return std::shared_ptr<Cell>();
    }
    return _cellMatrix[row][col];
}

QVector<int> Field::flyVectorOfCell(unsigned int row, unsigned int col) const
{
    return cellAtPosition( row, col )->flyVector();
}

void Field::reviveAllFlies()
{
    emit reviveFlies();
}

void Field::freezeAllFlies()
{
    emit freezeFlies();
}

QList<FlyStat> Field::statistic() const
{
    QList<FlyStat> result;
    for( std::vector< std::shared_ptr< Cell > > row :_cellMatrix)
    {
        for( std::shared_ptr< Cell > cell : row )
        {
            result.append( cell->statistic() );
        }
    }
    return result;
}

void Field::onMaxFliesPerCellChanged()
{
    for( std::vector< std::shared_ptr< Cell > >& row :_cellMatrix)
    {
        for( std::shared_ptr< Cell >& cell : row )
        {
            cell->resizeMaxFlies();
        }
    }
}

void Field::onCellStateChanged( unsigned int row, unsigned int col )
{
    emit cellStateChanged( row, col );
}

