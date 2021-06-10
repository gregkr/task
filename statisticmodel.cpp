#include "statisticmodel.h"

Statistic::Statistic(const QString &cell, const QString &speed, const QString &milage, const QString& isAlive)
    : _cell( cell ), _speed( speed ), _milage( milage ), _isAlive( isAlive )
{
}

QString Statistic::cell() const
{
    return _cell;
}

QString Statistic::speed() const
{
    return _speed;
}

QString Statistic::milage() const
{
    return _milage;
}

QString Statistic::isAlive() const
{
    return _isAlive;
}


StatisticModel::StatisticModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

void StatisticModel::addStatistic(const Statistic &s)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    _statList << s;
    endInsertRows();
}

void StatisticModel::clear()
{
    beginRemoveRows(QModelIndex(), rowCount(), rowCount());
    _statList.clear();
    endRemoveRows();
}

int StatisticModel::rowCount(const QModelIndex & parent) const
{
    Q_UNUSED(parent);
    return _statList.count();
}

QVariant StatisticModel::data(const QModelIndex & index, int role) const
{
    if (index.row() < 0 || index.row() >= _statList.count())
        return QVariant();

    const Statistic &stat = _statList[index.row()];

    if (role == CellRole)
        return stat.cell();
    else if (role == SpeedRole)
        return stat.speed();
    else if (role == MilageRole)
        return stat.milage();
    else if (role == IsAliveRole)
        return stat.isAlive();
    return QVariant();
}

QHash<int, QByteArray> StatisticModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[CellRole] = "cell";
    roles[SpeedRole] = "speed";
    roles[MilageRole] = "milage";
    roles[IsAliveRole] = "isAlive";

    return roles;
}

