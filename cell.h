#ifndef CELL_H
#define CELL_H

#include <QObject>
#include <QVector>
#include <QPair>
#include <QMutex>
#include <memory>
#include <array>

class Fly;

struct FlyStat
{
    unsigned int cellRow = 0;
    unsigned int cellCol = 0;
    float speed = .0;
    unsigned int mileage = 0;
    bool isDead = false;
};

class Cell : public QObject, public std::enable_shared_from_this< Cell >
{
    Q_OBJECT
public:
    enum class Direction {
        NORTH,
        EAST,
        SOUTH,
        WEST
    };

    explicit Cell( int row = 0, int col = 0, QObject *parent = nullptr );

    std::shared_ptr<Cell> nearCellInDirection( Direction dir ) const;
    void setNearCellInDirection( std::shared_ptr<Cell> cell, Direction dir );
    QVector< std::shared_ptr<Cell> > nearbyNonzeroCells() const;

    bool flyIn( std::shared_ptr< Fly > fly);
    bool flyOut( std::shared_ptr< Fly > fly);

    std::shared_ptr< Fly > flyAtPosition( unsigned int pos );
    bool setFlyToPosition( unsigned int pos, unsigned int T );
    bool addFly( unsigned int T );

    static unsigned int maxFliesCountPerCell();
    static void setMaxFliesCountPerCell(unsigned int N);

    QVector<int> flyVector() const;

    void resizeMaxFlies();

    void flyDead( Fly* fly);

    QList<FlyStat> statistic() const;

signals:
    void cellStatusChanged( unsigned int row, unsigned int col );

public slots:
    void onReviveFlies();
    void onFreezeFlies();

private:
    static unsigned int _maxFliesCountPerCell;  // N
    const QPair<int, int> _coord;
    QMutex _flyListMutex;
    QVector< std::shared_ptr< Fly > > _flyVec;
    std::array< std::weak_ptr< Cell >, 4> _nearCells;

};
#endif // CELL_H
