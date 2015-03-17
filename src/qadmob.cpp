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

#include <QStringList>
#include <QNetworkRequest>
#include <QDebug>
#include <QSize>

#include "qadmob.h"
#include "parser.h"

// CONSTANTS
static const QString kAdMobRequestUrl = "http://r.admob.com/ad_source.php";
static const QString kContentType = "application/x-www-form-urlencoded";
static const QString kClientSDK = "20100322-ANDROID-3312276cc1406347";

static const QString kUserAgent ="Mozilla/5.0 (SymbianOS/9.4; Series60/5.0 NokiaN97-1/12.0.024; Profile/MIDP-2.1 Configuration/CLDC-1.1; en-us) AppleWebKit/525 (KHTML, like Gecko) BrowserNG/7.1.12344";

//
QAdMob::QAdMob(QObject *parent) :
    QObject(parent), iAdTypeHint(AdTypeHintText),
    iTestMode ( false ), iVisitorGender(GenderUnknown), iAdReady ( false )
{
}


void QAdMob::setPublisherId(const QString& aPublisherId)
{
    iPublisherId = aPublisherId;
}

QString QAdMob::publisherId() const
{
    return iPublisherId;
}

void QAdMob::setKeywords(const QString& aKeywords)
{
    iKeywords = aKeywords;
}

QString QAdMob::keywords() const
{
    return iKeywords;
}

void QAdMob::resetKeywords()
{
    iKeywords.clear();
}

void QAdMob::setAdTypeHint( QAdMob::AdTypeHint aTypeHint)
{
    iAdTypeHint = aTypeHint;
}

QAdMob::AdTypeHint QAdMob::adTypeHint() const
{
    return iAdTypeHint;
}

void QAdMob::setTestMode(const QBool& aMode)
{
        iTestMode = aMode;
}

QBool QAdMob::testMode() const
{
    return iTestMode;
}


QString QAdMob::adLanguage() const
{
    return iAdLanguage;
}

void QAdMob::setAdLanguage(const QString& aLanguage)
{
    iAdLanguage = aLanguage;
}

void QAdMob::setVisitorAreaCode( const QString& aCode)
{
    iVisitorAreaCode = aCode;
}

QString QAdMob::visitorAreaCode() const
{
    return iVisitorAreaCode;
}

void QAdMob::resetVisitorAreaCode()
{
    iVisitorAreaCode.clear();
}

void QAdMob::setVisitorPostalCode( const QString& aCode)
{
    iVisitorPostalCode = aCode;
}

QString QAdMob::visitorPostalCode() const
{
    return iVisitorPostalCode;
}

void QAdMob::resetVisitorPostalCode()
{
    iVisitorPostalCode.clear();
}

void QAdMob::setVisitorLocationCoords( const QString& aCoords)
{
    iVisitorLocationCoords=aCoords;
}

QString QAdMob::visitorLocationCoords() const
{
    return iVisitorLocationCoords;
}

void QAdMob::resetVisitorLocationCoors()
{
    iVisitorLocationCoords.clear();
}

void QAdMob::setVisitorDOB( const QDate& aDate)
{
    iVisitorDob = aDate;
}

QDate QAdMob::visitorDOB() const
{
    return iVisitorDob;
}

void QAdMob::resetVisitorDOB()
{
    iVisitorDob = QDate();
}

void QAdMob::setVisitorGender( QAdMob::Gender aGender)
{
    iVisitorGender = aGender;
}

QAdMob::Gender QAdMob::visitorGender() const
{
    return iVisitorGender;
}

void QAdMob::resetVisitorGender()
{
    iVisitorGender = GenderUnknown;
}


QString QAdMob::genDataString() const
{
    QStringList stringList;

    QString timeStamp = "z=" + QString::number( QDateTime::currentDateTime().toTime_t() , 10 );
    stringList.append(timeStamp);

    const QString requestType = "rt=0" ; // request type, 0 is ad request

    stringList.append( requestType.toUtf8() );

    if (iAdLanguage.length())
        stringList.append("l=" + iAdLanguage.toUtf8());
    else
        stringList.append("l=en");

    stringList.append("f=jsonp");
    stringList.append("s=" + iPublisherId.toUtf8());
    stringList.append("i=");    // needed like this !

    switch(iAdTypeHint)
    {
    case AdTypeHintText:
        stringList.append("y=text");
        break;

    case AdTypeHintBanner:
        stringList.append("y=banner");
        break;

    }

    if (iTestMode)
        stringList.append("m=test");

    stringList.append("client_sdk=1");
    stringList.append("v=" + kClientSDK.toUtf8());

    return stringList.join("&");
}

void QAdMob::fetchAd()
{
    QNetworkRequest request;
    request.setUrl(QUrl(kAdMobRequestUrl));
    request.setRawHeader("User-Agent", kUserAgent.toUtf8());

    request.setAttribute(QNetworkRequest::CacheLoadControlAttribute, QNetworkRequest::AlwaysNetwork);
    request.setAttribute(QNetworkRequest::CacheSaveControlAttribute, false);

    request.setRawHeader("Content-Type", kContentType.toUtf8());

    QByteArray postBody ;
    postBody = genDataString().toUtf8();

    QNetworkReply* reply = iNam.post(request, postBody);

    QObject::connect(reply, SIGNAL(finished()),  this, SLOT(networkReplyFinished()));
    QObject::connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),
                             this, SLOT(networkReplyError(QNetworkReply::NetworkError)));
}

void QAdMob::networkReplyError ( QNetworkReply::NetworkError /*aCode*/ )
{
    //todo: should cancel all outstanding requests !
    emit adReceived(false);
    iAdReady = false;
    sender()->deleteLater();
}

void QAdMob::networkReplyFinished ()
{
    QNetworkReply* reply = static_cast<QNetworkReply*> ( sender() );
    QByteArray responseData = reply->readAll();

    handleResponseData(responseData);

    reply->deleteLater();
}

void QAdMob::handleResponseData( const QByteArray& aResponseData )
{
    QJson::Parser parser;
    bool ok;

    QVariantMap result = parser.parse(aResponseData, &ok).toMap();

    if (!ok)
    {
        emit adReceived(false);
        return;
    }

    QNetworkRequest request;
    request.setRawHeader("User-Agent", kUserAgent.toUtf8());

    request.setAttribute(QNetworkRequest::CacheLoadControlAttribute, QNetworkRequest::AlwaysNetwork);
    request.setAttribute(QNetworkRequest::CacheSaveControlAttribute, false);

    QString text = result["text"].toString();
    iAd.iText = text;

    QString url = result["url"].toString();
    iAd.iUrl = url;

    QVariantList imageDimensionsList = result["d"].toList();
    QSize imageSize (imageDimensionsList[0].toInt(), imageDimensionsList[1].toInt());
    iAd.iSize = imageSize;

    QVariantMap markupMap = result ["markup"].toMap();

    QVariantList adMobStringList = markupMap["v"].toList();
    QString adsByAdmobString = adMobStringList.last().toMap()["x"].toString();

    QString adTitleImageUrl = markupMap["$"].toMap()["t"].toMap()["u"].toString();
    QString adTargetImageUrl = markupMap["$"].toMap()["a"].toMap()["u"].toString();

    request.setUrl(QUrl(adTitleImageUrl));

    QNetworkReply* reply = iNam.get(request);

    QObject::connect(reply, SIGNAL(finished()),  this, SLOT(handleAdTitleImageDownload()));
    QObject::connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),
                             this, SLOT(networkReplyError(QNetworkReply::NetworkError)));

    request.setUrl(QUrl(adTargetImageUrl));

    reply = iNam.get(request);

    QObject::connect(reply, SIGNAL(finished()),  this, SLOT(handleAdTargetImageDownload()));
    QObject::connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),
                             this, SLOT(networkReplyError(QNetworkReply::NetworkError)));

}


void QAdMob::handleAdTitleImageDownload()
{
    QNetworkReply* reply = static_cast<QNetworkReply*> ( sender() );
    QByteArray responseData = reply->readAll();

    iAd.iAdTitleImage = QImage::fromData(responseData);

    if (!iAd.iAdUrlTypeImage.isNull())
        handleAdReady();

    reply->deleteLater();
}

void QAdMob::handleAdTargetImageDownload()
{
    QNetworkReply* reply = static_cast<QNetworkReply*> ( sender() );
    QByteArray responseData = reply->readAll();

    iAd.iAdUrlTypeImage = QImage::fromData(responseData);

    if (!iAd.iAdTitleImage.isNull())
        handleAdReady();

    reply->deleteLater();
}

void QAdMob::handleAdReady()
{
    iAdReady = true;
    emit adReceived(true);
}

const QAdMobAd& QAdMob::ad() const
{
    return iAd;
}

QBool QAdMob::adReady() const
{
    return QBool(iAdReady);
}
