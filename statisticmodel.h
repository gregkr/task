#ifndef STATISTICMODEL_H
#define STATISTICMODEL_H


#include <QAbstractListModel>
#include <QStringList>

class Statistic
{
public:
    Statistic(const QString &cell, const QString &speed, const QString &milage, const QString& isAlive);

    QString cell() const;
    QString speed() const;
    QString milage() const;
    QString isAlive() const;

private:// средняя скорость и пробег, сдохла или жива
    QString _cell;
    QString _speed;
    QString _milage;
    QString _isAlive;
};

class StatisticModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum StatisticRoles {
        CellRole = Qt::UserRole + 1,
        SpeedRole,
        MilageRole,
        IsAliveRole
    };

    StatisticModel(QObject *parent = 0);


    void addStatistic(const Statistic &s);
    void clear();

    int rowCount(const QModelIndex & parent = QModelIndex()) const;

    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

protected:
    QHash<int, QByteArray> roleNames() const;

private:
    QList<Statistic> _statList;
};

#endif
