#ifndef FIELDMODEL_H
#define FIELDMODEL_H

#include <QAbstractTableModel>
#include <QPoint>

#include "field.h"
#include "cell.h"

class StatisticModel;

class FieldModel : public QAbstractTableModel
{
    Q_OBJECT

    Q_ENUMS(Roles)

    Q_PROPERTY(int running READ running WRITE setRunning NOTIFY runningChanged)
    Q_PROPERTY(unsigned int maxFliesPerCell READ maxFliesPerCell WRITE setMaxFliesPerCell NOTIFY maxFliesPerCellChanged );

public:
    enum Roles {
        CellRole,
        CellFlies
    };

    QHash<int, QByteArray> roleNames() const override {
        return {
            { CellRole, "value" },
            { CellFlies, "flies" }
        };
    }

    explicit FieldModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;

    Q_INVOKABLE void createField( unsigned int M );
    Q_INVOKABLE void clear();
    Q_INVOKABLE void load();
    Q_INVOKABLE void addFly( unsigned int cellRow, unsigned int cellCol, unsigned int T);
    Q_INVOKABLE void setStatModel(StatisticModel* v);

    int running() const { return _state; }
    void setRunning( int val );

    unsigned int maxFliesPerCell() const { return Cell::maxFliesCountPerCell(); }
    void setMaxFliesPerCell( int val )
    {
        Cell::setMaxFliesCountPerCell( val );
        emit maxFliesPerCellChanged();
    }

signals:
    void runningChanged();
    void cellFliesChanged();
    void maxFliesPerCellChanged();

private slots:
    void onCellStateChanged( int row, int col );

private:
    enum GameState {
        NotStarted,
        Running,
        Stopped
    };
    GameState _state = GameState::NotStarted;
    Field _field;
};



#endif // FIELDMODEL_H
