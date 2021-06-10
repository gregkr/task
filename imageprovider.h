#ifndef IMAGEPROVIDER_H
#define IMAGEPROVIDER_H

#include <QQuickImageProvider>

class ImageProvider : public QQuickImageProvider
 {
 public:
     ImageProvider();

     QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize) override;

private:
     QImage _img[3];
};

#endif // IMAGEPROVIDER_H
