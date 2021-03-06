#include <QPainter>

#include "ImageSectionItem.h"
#include "ViewportInterface.h"

ImageSectionItem::ImageSectionItem(ViewportInterface* viewport, QRectF const& boundingRect, std::mutex& mutex)
    : QGraphicsItem(), _viewport(viewport), _boundingRect(boundingRect), _mutex(mutex)
{
    auto const viewportRect = _viewport->getRect();
    _imageOfVisibleRect = boost::make_shared<QImage>(viewportRect.width(), viewportRect.height(), QImage::Format_RGB32);
    _imageOfVisibleRect->fill(QColor(0, 0, 0));
}

ImageSectionItem::~ImageSectionItem()
{
}

QImagePtr ImageSectionItem::getImageOfVisibleRect()
{
    //resize image?
    IntVector2D viewportSize{ static_cast<int>(_viewport->getRect().width()), static_cast<int>(_viewport->getRect().height()) };
    viewportSize.x = std::min(static_cast<int>(_boundingRect.width()), viewportSize.x);
    viewportSize.y = std::min(static_cast<int>(_boundingRect.height()), viewportSize.y);
    if (_imageOfVisibleRect->width() != viewportSize.x || _imageOfVisibleRect->height() != viewportSize.y) {
        _imageOfVisibleRect = boost::make_shared<QImage>(viewportSize.x, viewportSize.y, QImage::Format_ARGB32);
        _imageOfVisibleRect->fill(QColor(0, 0, 0));
    }

    return _imageOfVisibleRect;
}

QRectF ImageSectionItem::boundingRect() const
{
    return _boundingRect;
}

void ImageSectionItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /*= Q_NULLPTR*/)
{
    auto const viewportRect = _viewport->getRect();

    std::lock_guard<std::mutex> lock(_mutex);

    painter->drawImage(
        std::max(0.0f, static_cast<float>(viewportRect.x())),
        std::max(0.0f, static_cast<float>(viewportRect.y())),
        *_imageOfVisibleRect);
}

