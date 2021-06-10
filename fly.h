#ifndef FLY_H
#define FLY_H

#include <QObject>
#include <QThread>
#include <memory>
#include "cell.h"


class Fly : public QThread, public std::enable_shared_from_this<Fly>
{
    Q_OBJECT
public:
    explicit Fly( std::shared_ptr< Cell > cell = std::shared_ptr< Cell >(), unsigned int T = 0, unsigned int M = 1, QObject *parent = nullptr);
    virtual ~Fly();

    float speed() const;
    unsigned int mileage() const;
    unsigned int age()  const;
    bool isDead() const;
    unsigned int stupidity() const;

    const std::weak_ptr<Cell> &cell() const;

    unsigned int num() const;

protected:
    virtual void run() override;

private:
    std::shared_ptr<Cell> directionOfFlight();

private:
    std::weak_ptr< Cell > _cell;
    const unsigned int _stupidity = 0;
    const unsigned int _ageLimit; // = T*M
    unsigned int _age  = 0; //( in amount T )
    unsigned int _mileage = 0;
    unsigned int _num = 0;
};


#endif // FLY_H
