#include "imageprovider.h"

#include <QPainter>

ImageProvider::ImageProvider()
    : QQuickImageProvider(QQuickImageProvider::Pixmap)
{
    _img[0] = QImage( ":/assets/no_fly.png" );
    _img[1] = QImage( ":/assets/fly_live.png" );
    _img[2] = QImage( ":/assets/fly_dead.png" );
}

QPixmap ImageProvider::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
{
    QPixmap pix(_img[0].width() * id.size(), _img[0].height() );
    QPainter *paint = new QPainter(&pix);
    QPoint point(0,0);
    for( QChar ch : id )
    {
        int idx = ch.digitValue();
        paint->drawImage(point, _img[idx]);
        point.setX( point.x() + _img[idx].width() );
    }
    delete paint;

    return pix;
}

