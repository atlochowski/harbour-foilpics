/*
 * Copyright (C) 2017 Jolla Ltd.
 * Copyright (C) 2017 Slava Monich <slava@monich.com>
 *
 * You may use this file under the terms of the BSD license as follows:
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *   1. Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in
 *      the documentation and/or other materials provided with the
 *      distribution.
 *   3. Neither the name of Jolla Ltd nor the names of its contributors
 *      may be used to endorse or promote products derived from this
 *      software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef FOILPICS_MODEL_H
#define FOILPICS_MODEL_H

#include <QtQml>
#include <QImage>
#include <QAbstractListModel>

#include "foil_types.h"

#include "FoilPicsImageRequest.h"

class FoilPicsModel : public QAbstractListModel {
    Q_OBJECT
    Q_ENUMS(FoilState)
    Q_PROPERTY(int count READ count NOTIFY countChanged)
    Q_PROPERTY(bool busy READ busy NOTIFY busyChanged)
    Q_PROPERTY(bool keyAvailable READ keyAvailable NOTIFY keyAvailableChanged)
    Q_PROPERTY(FoilState foilState READ foilState NOTIFY foilStateChanged)
    Q_PROPERTY(QSize thumbnailSize READ thumbnailSize WRITE setThumbnailSize NOTIFY thumbnailSizeChanged)
    Q_PROPERTY(bool mayHaveEncryptedPictures READ mayHaveEncryptedPictures NOTIFY mayHaveEncryptedPicturesChanged)

    class Private;
    class ModelInfo;
    class ModelData;
    class SaveInfoTask;
    class GenerateKeyTask;
    class CheckPicsTask;
    class BaseTask;
    class DecryptTask;
    class EncryptTask;
    class ImageRequestTask;

public:
    class DecryptPicsTask;

    enum FoilState {
        FoilKeyMissing,
        FoilKeyInvalid,
        FoilKeyError,
        FoilKeyNotEncrypted,
        FoilGeneratingKey,
        FoilLocked,
        FoilLockedTimedOut,
        FoilDecrypting,
        FoilPicsReady
    };

    FoilPicsModel(QObject* aParent = NULL);

    int count() const;
    bool busy() const;
    bool keyAvailable() const;
    FoilState foilState() const;
    bool mayHaveEncryptedPictures() const;
    QSize thumbnailSize() const;
    void setThumbnailSize(QSize aSize);

    // QAbstractListModel
    virtual QHash<int,QByteArray> roleNames() const;
    virtual int rowCount(const QModelIndex& aParent) const;
    virtual QVariant data(const QModelIndex& aIndex, int aRole) const;

    Q_INVOKABLE void generateKey(int aBits, QString aPassword);
    Q_INVOKABLE bool checkPassword(QString aPassword);
    Q_INVOKABLE bool changePassword(QString aOld, QString aNew);
    Q_INVOKABLE void lock(bool aTimeout);
    Q_INVOKABLE bool unlock(QString aPassword);
    Q_INVOKABLE bool encryptFile(QUrl aUrl, int aOrientation);
    Q_INVOKABLE void decryptAt(int aIndex);
    Q_INVOKABLE void decryptAll();
    Q_INVOKABLE void removeAt(int aIndex);
    Q_INVOKABLE QVariantMap get(int aIndex) const;

private Q_SLOTS:
    void imageRequest(QString aPath, FoilPicsImageRequest aRequest);

Q_SIGNALS:
    void countChanged();
    void busyChanged();
    void keyAvailableChanged();
    void foilStateChanged();
    void mayHaveEncryptedPicturesChanged();
    void thumbnailSizeChanged();

    void keyGenerated();
    void passwordChanged();

private:
    Private* iPrivate;
};

QML_DECLARE_TYPE(FoilPicsModel)

#endif // FOILPICS_MODEL_H