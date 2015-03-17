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

#ifndef QADMOB_H
#define QADMOB_H

#include <QObject>
#include <QString>
#include <QSize>
#include <QBool>
#include <QDate>
#include <QImage>
#include <QNetworkAccessManager>
#include <QNetworkReply>


#if defined(QADMOB_LIBRARY)
#  define QADMOBSHARED_EXPORT Q_DECL_EXPORT
#else
#  define QADMOBSHARED_EXPORT Q_DECL_IMPORT
#endif

// FORWARD DECLARATIONS
class QAdMob;

/**
 * @brief Represents an Ad
 */
class QADMOBSHARED_EXPORT QAdMobAd : QObject
{
    Q_OBJECT

    Q_PROPERTY(QString text READ text)
    Q_PROPERTY(QString url READ url)
    Q_PROPERTY(QSize size READ size)
    Q_PROPERTY(QImage adTitleImage READ adTitleImage)
    Q_PROPERTY(QImage adUrlTypeImage READ adUrlTypeImage)
    Q_PROPERTY(QString adMobStampText READ adMobStampText)

public:

    QString text() const
    {
        return iText;
    }

    QString url() const
    {
        return iUrl;
    }

    QSize size() const
    {
        return iSize;
    }

    QImage adUrlTypeImage() const
    {
        return iAdUrlTypeImage;
    }

    QImage adTitleImage() const
    {
        return iAdTitleImage;
    }

    QString adMobStampText() const
    {
        return iAdMobStampText;
    }

private:
    QString iText;
    QString iUrl;
    QSize   iSize;
    QImage iAdTitleImage;
    QImage iAdUrlTypeImage;
    QString iAdMobStampText;

    friend class QAdMob;
};

/**
  * @brief Used to fetch ads from the admob servers
  */
class QADMOBSHARED_EXPORT QAdMob : public QObject
{
    Q_OBJECT
    Q_ENUMS(Gender)
    Q_ENUMS(AdTypeHint)

    Q_PROPERTY(QString publisherId READ publisherId WRITE setPublisherId)
    Q_PROPERTY(QString keywords READ keywords WRITE setKeywords RESET resetKeywords)
    Q_PROPERTY(AdTypeHint adTypeHint READ adTypeHint WRITE setAdTypeHint)
    Q_PROPERTY(QBool testMode READ testMode WRITE setTestMode)
    Q_PROPERTY(QString adLanguage READ adLanguage WRITE setAdLanguage)
    Q_PROPERTY(QBool adReady READ adReady)

    /* Visitor propertyes */
    /* Read http://developer.admob.com/wiki/Requests for more info */
    Q_PROPERTY(QString visitorPostalCode READ visitorPostalCode WRITE setVisitorPostalCode RESET resetVisitorPostalCode)
    Q_PROPERTY(QString visitorAreaCode READ visitorAreaCode WRITE setVisitorAreaCode RESET resetVisitorAreaCode)
    Q_PROPERTY(QString visitorLocationCoords READ visitorLocationCoords WRITE setVisitorLocationCoords RESET resetVisitorLocationCoors)
    Q_PROPERTY(QDate visitorDOB READ visitorDOB WRITE setVisitorDOB RESET resetVisitorDOB)
    Q_PROPERTY(Gender visitorGender READ visitorGender WRITE setVisitorGender RESET resetVisitorGender)

signals:
    /**
      * @brief this signals is triggered when an ad is received.
      * @param aResult true if ad was received sucessfully, false otherwise.
      */
    void adReceived(bool aResult);

public slots:
    /**
      * @brief starts the ad fetching process, signal adReceived is triggered as a result.
      */
    void fetchAd();

private slots:
    void networkReplyError ( QNetworkReply::NetworkError aCode );
    void networkReplyFinished ();

    void handleAdTitleImageDownload();
    void handleAdTargetImageDownload();

public:
    explicit QAdMob(QObject *parent = 0);

    /**
      * @brief Used to specify the gender of th use. Used to for better ad targetting by AdMob
      */
    enum Gender { GenderUnknown = 0, GenderMale , GenderFemale };

    /**
      * @brief Used to specify the type of ad you want, a banner or a text ad
      */
    enum AdTypeHint { AdTypeHintText = 0, AdTypeHintBanner };

    /**
      * @brief Set the publisher Id, see AdMob pages for your publisher id
      * @param aPublisherId your publisher id
      */
    void setPublisherId(const QString& aPublisherId);

    /**
      * @returns QString, your publisher id
      */
    QString publisherId() const;

    /**
      * @brief Sets the keywords, AdMob uses show targeted ads, keywords are seperated by spaces
      */
    void setKeywords(const QString&);

    /**
      * @returns QString, the keyrds you have set @see setKeywords
      */
    QString keywords() const;

    /**
      * @brief reset keywords
      */
    void resetKeywords();

    /**
      * @brief Used to specify what type of ad you want, a banner or a text ad
      * @param aType @see AdTypeHint
      */
    void setAdTypeHint( QAdMob::AdTypeHint aType);

    /**
      * @brief returns the ad type hint you have set.
      * @returns QAdMob::AdTypeHint
      */
    QAdMob::AdTypeHint adTypeHint() const;

    /**
      * @brief Used to enable test mode, if test mode is enable AdMob will always send you test ad.
      */
    void setTestMode(const QBool& aMode);
    QBool testMode() const;

    QString adLanguage() const;
    void setAdLanguage(const QString& );

    void setVisitorAreaCode( const QString& );
    QString visitorAreaCode() const;
    void resetVisitorAreaCode();

    void setVisitorPostalCode( const QString& );
    QString visitorPostalCode() const;
    void resetVisitorPostalCode();

    void setVisitorLocationCoords( const QString& );
    QString visitorLocationCoords() const;
    void resetVisitorLocationCoors();

    void setVisitorDOB( const QDate& );
    QDate visitorDOB() const;
    void resetVisitorDOB();

    void setVisitorGender( QAdMob::Gender );
    QAdMob::Gender visitorGender() const;
    void resetVisitorGender();

    QBool adReady() const;
    const QAdMobAd& ad() const;

private:
    QString genDataString() const;
    void handleResponseData( const QByteArray& aResponseData );


    void handleAdReady();

    QString iPublisherId;
    QString iKeywords;
    AdTypeHint iAdTypeHint;
    QBool   iTestMode;
    QString iAdLanguage;

    QString iVisitorAreaCode;
    QString iVisitorPostalCode;
    QString iVisitorLocationCoords;
    QDate   iVisitorDob;
    Gender  iVisitorGender;


    QNetworkAccessManager iNam;

    QAdMobAd iAd;
    bool    iAdReady;

};

#endif // QADMOB_H
