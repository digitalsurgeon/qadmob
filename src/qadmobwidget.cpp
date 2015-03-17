/*
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0

 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
*/

#include "qadmobwidget.h"
#include "qadmob.h"

#include <QDesktopServices>
#include <QPainter>
#include <QMouseEvent>
#include <QPaintEvent>


static const QSize kDefaultWidgetSize = QSize (200,50);

QAdMobWidget::QAdMobWidget(QWidget *parent) :
    QWidget(parent),
    iAdMob ( new QAdMob(this))
{
     setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

     connect(iAdMob, SIGNAL(adReceived(bool)), this, SLOT(adReceived(bool)));
}

 QAdMob& QAdMobWidget::ad()
 {
     return *iAdMob;
 }

QSize QAdMobWidget::sizeHint() const
{
    if (iAdMob->adReady())
    {
        return iAdMob->ad().size();
    }
    else
    {
        return kDefaultWidgetSize;
    }
}

void QAdMobWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    if (iAdMob->adReady())
    {
        QImage adTitleImage = iAdMob->ad().adTitleImage();
        QImage adUrlTypeImage = iAdMob->ad().adUrlTypeImage();
        QSize adSize = iAdMob->ad().size();

        QPoint point(0,0);
        painter.drawImage(point, adTitleImage);

        painter.drawText( QPoint ( adTitleImage.size().width() + 5, adTitleImage.size().height() / 3 ),
                          iAdMob->ad().text());

        painter.drawText( QPoint ( adTitleImage.size().width() + 5 , adTitleImage.size().height()),
                         iAdMob->ad().adMobStampText());

        point.setX( adSize.width() - adUrlTypeImage.size().width() );
        point.setY( (adSize.height() - adUrlTypeImage.size().height() ) / 2 );
        painter.drawImage(point, adUrlTypeImage);
    }
}

void QAdMobWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && iAdMob->adReady())
    {
        QDesktopServices::openUrl(iAdMob->ad().url());
    }
}

void QAdMobWidget::fetchAd()
{
    iAdMob->fetchAd();
}

void QAdMobWidget::adReceived(bool aResult)
{
    if (aResult)
    {
        setFixedSize(iAdMob->ad().size());
        setCursor(Qt::PointingHandCursor);
    }

}
