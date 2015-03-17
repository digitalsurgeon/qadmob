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

#ifndef QADMOBWIDGET_H
#define QADMOBWIDGET_H

#include <QWidget>
#include "qadmob.h"

class QADMOBSHARED_EXPORT QAdMobWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QAdMobWidget(QWidget *parent = 0);

    QAdMob& ad();

    /* from QWidget */
    QSize sizeHint() const;

protected:
    void mousePressEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);

signals:
    void adFetchResult(bool);

public slots:
    void fetchAd();
    void adReceived(bool);

private:
    QAdMob*  iAdMob;
};

#endif // QADMOBWIDGET_H
