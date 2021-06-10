#include "fieldmodel.h"
#include "statisticmodel.h"

FieldModel::FieldModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    clear();
    Cell::setMaxFliesCountPerCell(2);
    connect( this, &FieldModel::maxFliesPerCellChanged, &_field, &Field::onMaxFliesPerCellChanged );
    connect( &_field, &Field::cellStateChanged, this, &FieldModel::onCellStateChanged );
}

int FieldModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return _field.size();
}

int FieldModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return _field.size();
}

QVariant FieldModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() )
    {
        return QVariant();
    }
    else if ( role == CellRole )
    {
        return QVariant( true );
    }
    else if ( role == CellFlies )
    {
        QString str;
        for ( int n : _field.cellAtPosition( index.row(), index.column() )->flyVector() )
        {
            str += QString::number(n);
        }
        return QVariant( str );
    }
    return QVariant();
}

bool FieldModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (( role != CellRole && role != CellFlies ) || data(index, role) == value)
        return false;

    emit dataChanged(index, index, {role});

    return true;
}

Qt::ItemFlags FieldModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEditable;
}

void FieldModel::createField( unsigned int M )
{
    beginResetModel();
    _field.create( M );
    endResetModel();
    emit dataChanged(index(0, 0), index( _field.size() - 1, _field.size() - 1), {CellRole});
}

void FieldModel::clear()
{
    beginResetModel();
    _field.clear();
    endResetModel();
    emit dataChanged(index(0, 0), index( _field.size() - 1, _field.size() - 1), {CellRole});
}

void FieldModel::load()
{
    createField( 2 );
}

void FieldModel::addFly(unsigned int cellRow, unsigned int cellCol, unsigned int T)
{
    if ( cellRow < _field.size()
         && cellCol < _field.size()
         && T > 0 )

    {
        _field.cellAtPosition( cellRow, cellCol )->addFly(T);
        setData( index(cellRow, cellCol), QVariant(), CellFlies );
    }
}

void FieldModel::setStatModel( StatisticModel* statModel)
{
    if ( statModel )
    {
        statModel->clear();
        for ( FlyStat fs: _field.statistic() )
        {
            statModel->addStatistic( Statistic( QString("(%1,%2)").arg( fs.cellRow).arg( fs.cellCol )
                                                , QString::number( fs.speed, 'f', 2)
                                                , QString::number( fs.mileage )
                                                , (fs.isDead) ? "Сдохла" : "Жива"));
        }
    }
}

void FieldModel::onCellStateChanged(int row, int col)
{
    setData(index( row, col), QVariant(), {CellFlies});
}

void FieldModel::setRunning(int val)
{
    Q_UNUSED(val)
    if ( _state == GameState::NotStarted )
    {
        _field.reviveAllFlies();
        _state = GameState::Running;
        emit runningChanged();
    }
    else if ( _state == GameState::Running)
    {
        _field.freezeAllFlies();
        _state = GameState::Stopped;
        emit runningChanged();
    }
}
