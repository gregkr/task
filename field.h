#ifndef FIELD_H
#define FIELD_H

#include "cell.h"

#include <QObject>
#include <memory>

class Field : public QObject
{
    Q_OBJECT
public:
    explicit Field(QObject *parent = nullptr);

    unsigned int size() const;

    void create( unsigned int M );
    void clear();

    std::shared_ptr< Cell > cellAtPosition( unsigned int row, unsigned int col ) const;
    QVector<int> flyVectorOfCell(unsigned int row, unsigned int col) const;

    void reviveAllFlies();
    void freezeAllFlies();

    QList<FlyStat> statistic() const;

signals:
    void cellStateChanged( int row, int col );
    void reviveFlies();
    void freezeFlies();

public slots:
    void onMaxFliesPerCellChanged();
    void onCellStateChanged(unsigned int row, unsigned int col);

private:
    unsigned int _size = 0;  // M
    std::vector< std::vector< std::shared_ptr< Cell > > > _cellMatrix; // M*M
};

#endif // FIELD_H
