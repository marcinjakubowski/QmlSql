#include "qmlsqlcreatedatabase.h"



/*!
   \qmltype QmlSqlCreateDatabase
   \inqmlmodule QmlSql 1.0
   \ingroup QmlSql
   \inherits QObject
   \brief  Creates a local database for \b{sqlight only}. One must set the databaseName and also the
    filePath in which they would like to save the database file. One can use a md5Sum that is randomly
    generated to create the databaseName. Or if you like you can also  name the database fileName via databaseName.
    The method of \c exec is what is used to create the database.

    Example:

   \code
    QmlSqlCreateDatabase{
            id: mainCreateDatabase
            fileName: "SomeFileName"
            databaseName: "ANewDatabase"
            filePath:  "/Some/Path/To/Save/To"
            useMd5: false
            onCreated:{
                //do something
            }
     }

   \endcode
 */





QmlSqlCreateDatabase::QmlSqlCreateDatabase(QObject *parent) :
    QObject(parent),
    m_useMd5(true),
    m_databaseName("NULL")
{
    connect(this, SIGNAL(error(QString)), this, SLOT(handleError(QString)));
}


/*!
 \qmlproperty string QmlSqlCreateDatabase::filePath
 Sets the path to where one would like to create the sqlight database

*/
QString QmlSqlCreateDatabase::filePath() const {
    return m_filePath;
}


void QmlSqlCreateDatabase::setFilePath(const QString& filePath) {
    if (m_filePath == filePath)
        return;
    m_filePath = filePath;
    emit filePathChanged();
}
/*!
 \qmlproperty string QmlSqlCreateDatabase::fileName
  Sets the file name of the sqlight database to fileName which will be saved on the users system.

  \b{Note:} One must set the filePath first before this. \c fileName is just the name and not the full path that will be saved to the users harddrive.


  \sa useMd5 , filePath
 */
QString QmlSqlCreateDatabase::fileName() const {
    return m_fileName;
}

/*!
 \brief void QmlSqlCreateDatabase::setFileName(const QString& fileName)
 Setter that is used from Qml to set the fileName of the sqlight database. This emits a signal if the fileNmae has changed.
*/
void QmlSqlCreateDatabase::setFileName(const QString& fileName) {
    if (m_fileName == fileName)
        return;
    m_fileName = fileName;
    emit fileNameChanged();
}

/*!
 \qmlproperty bool QmlSqlCreateDatabase::useMd5
    Sets this to true if you would like the fileName of the sqlight database to be a randomly generated
 \c md5Sum.

    \b{Note:} One can not use both fileName and use this feature.

    \sa fileName, filePath
 */
bool QmlSqlCreateDatabase::useMd5() const {
    return m_useMd5;
}

/*!
  \brief void QmlSqlCreateDatabase::setUseMd5(bool useMd5)
   This is used to to tell the method of exec to name the file of the sqlight database as a radom md5sum .
*/
void QmlSqlCreateDatabase::setUseMd5(bool useMd5) {
    if (m_useMd5 == useMd5)
        return;
    m_useMd5 = useMd5;
    emit useMd5Changed();
}


/*!
  \qmlproperty string QmlSqlCreateDatabase::databaseName
    Sets the name of the sqlight database to \c databaseName.

    \b{Note} This is not the name of the file in which sqlight is saved to see fileName.
    This is the database's name that is created.

    \sa fileName, useMd5
 */
QString QmlSqlCreateDatabase::databaseName() const {
    return m_databaseName;
}

/*!
 \brief void QmlSqlCreateDatabase::setDatabaseName(const QString& databaseName)
 Sets the sqlight database name to what the user passed in from qml.  Emits a signal if the databaseName changes.
 */
void QmlSqlCreateDatabase::setDatabaseName(const QString& databaseName) {
    if (m_databaseName == databaseName)
        return;
    m_databaseName = databaseName;
    emit databaseNameChanged();
}


/*!
  \qmlproperty string QmlSqlCreateDatabase::errorString
    Returns a text error if there is a error in the creation of the database via exec().
 */
QString QmlSqlCreateDatabase::errorString() {
    return m_errorString;
}

/*!
  \qmlproperty string QmlSqlCreateDatabase::lastCreatedDatabaseFile
  Returns a string of the fullPath and name (string or md5) of last sqlight database that was created.


  \sa useMd5
 */
QString QmlSqlCreateDatabase::lastCreatedDatabaseFile() const {
    return m_lastCreatedDatabaseFile;
}

/*!
 \brief void QmlSqlCreateDatabase::setLastCreatedDatabaseFile(const QString& lastCreatedDatabaseFile)
   Sets the last created datbase file name.  This is handy if you are using this class to create multi[ple sqlight database and would like to check the last database that was created.
*/
void QmlSqlCreateDatabase::setLastCreatedDatabaseFile(const QString& lastCreatedDatabaseFile) {
    if (m_lastCreatedDatabaseFile == lastCreatedDatabaseFile)
        return;
    m_lastCreatedDatabaseFile = lastCreatedDatabaseFile;
    emit lastCreatedDatabaseFileChanged();
}

/*!
  \qmlmethod void QmlSqlCreateDatabase::exec()
  A method that is run to create the database. Returns a \c errorString if it can not complete the method.
  If one is using a md5 it creates the fileName based on the random md5 that is generated in this method.
  Else it uses the fileName that you have set.

  \b{Note} If you do not set the fileName and also do not set the md5Sum to \c true. This will return a \c errorString


  \sa useMd5, fileName
*/
void QmlSqlCreateDatabase::exec() {
    QString dataDir = QStandardPaths::standardLocations(QStandardPaths::DataLocation).first();
    qDebug() << "Making the database ";
    QString finalName;
    //just in case
    finalName.clear();

    if(m_fileName.length() < 1) {
        error("You forgot to set your databases Name");
        return;
    }

    // Deal with the path length
    if (m_filePath.length() < 1) {
        if (m_useMd5 == true){
            QString fName =  generateMd5Sum(m_databaseName);
            finalName = QString("%1/%2%3").arg(dataDir).arg(fName).arg(".sqlight");
            QFile file(finalName);
            if(!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                QString err = QString("Could not open the file %1 for writing to ").arg(finalName);
                error(err);
            }
            //ok we got the file open now lets make sure that we make into a db
            else {
                file.close();
                QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
                db.setDatabaseName(m_databaseName);

                if (db.open()) {
                    qDebug()<< "Sweet we created the database " << finalName;
                    setLastCreatedDatabaseFile(finalName);
                    created();
                    db.close();
                }
                else {
                    error("Error in opening the init database");
                }
            }
        }
        // NO MD5Sum no Path
        else {
            finalName = QString("%1/%2%3").arg(dataDir).arg(m_fileName).arg(".sqlight");
            QFile file(finalName);
            if(!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                QString err = QString("Could not open the file %1 for writing to ").arg(finalName);
                error(err);
            }
            //ok we got the file open now lets make sure that we make into a db
            else {
                file.close();
                QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
                db.setDatabaseName(m_databaseName);
                if(db.open()) {
                    qDebug()<< "Sweet we created the database " << finalName;
                    setLastCreatedDatabaseFile(finalName);
                    created();
                    db.close();
                }
                else {
                    error("Error in opening the init database");
                }
            }
        }
    }// END NO PATH NAME SET
    // Ok so the file Path is set now deal with that
    else {
        if (m_useMd5 == true) {
            QString fName =  generateMd5Sum(m_databaseName);
            finalName = QString("%1/%2%3").arg(m_filePath).arg(fName).arg(".sqlight");
            QFile file(finalName);
            if(!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                QString err = QString("Could not open the file %1 for writing to ").arg(finalName);
                error(err);
            }
            //ok we got the file open now lets make sure that we make into a db
            else {
                file.close();
                QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
                db.setDatabaseName(m_databaseName);

                if(db.open()) {
                    qDebug()<< "Sweet we created the database " << finalName;
                    setLastCreatedDatabaseFile(finalName);
                    created();
                    db.close();
                }
                else {
                    error("Error in opening the init database");
                }
            }
        }
        //not using md5Sum
        else {
            finalName = QString("%1/%2%3").arg(m_filePath).arg(m_databaseName).arg(".sqlight");
            QFile file(finalName);
            if(!file.open(QIODevice::WriteOnly | QIODevice::Text))             {
                QString err = QString("Could not open the file %1 for writing to ").arg(finalName);
                error(err);
            }
            //ok we got the file open now lets make sure that we make into a db
            else {
                file.close();
                QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
                db.setDatabaseName(m_databaseName);

                if(db.open()) {
                    qDebug()<< "Database created " << finalName;
                    setLastCreatedDatabaseFile(finalName);
                    created();
                    db.close();
                }
                else {
                    error("Error in opening the init database");
                }
            }
        }
    }
}




/*!
 \brief QString QmlSqlCreateDatabase::generateMd5Sum(const QString& databaseName)
  Returns a string md5sum that is used in the exec method if the property of use useMd5 is set to true in the QML code.
 */
QString QmlSqlCreateDatabase::generateMd5Sum(const QString& databaseName) {
    QCryptographicHash md5(QCryptographicHash::Md5);
    // make the md5sum
    md5.addData(databaseName.toUtf8());
    // now that we have the name and the user did not enter in a path we can create a db
    QString fName(QLatin1String(md5.result().toHex()));
    return fName;
}



/*!
 \brief QString QmlSqlCreateDatabase::getRandomString()
 returns a randomly genorated QString from 12 charecters. The possible charecters are a-z upper and lower case and also 0-9
 */
QString QmlSqlCreateDatabase::getRandomString() {
    QString possibleCharacters = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    int randomStringLength = 12;

    qsrand(QTime::currentTime().msec());

    QString rString;
    for(int i=0; i<randomStringLength; ++i)     {
        int index = qrand() % possibleCharacters.length();
        QChar nextChar = possibleCharacters.at(index);
        rString.append(nextChar);
    }
    return rString;
}

/*!
 \brief void QmlSqlCreateDatabase::handelError(const QString& er)
 slot that is used to handel the error string when there is a error in this class.
 */
void QmlSqlCreateDatabase::handleError(const QString& er) {
    qDebug() << er;
    if (m_errorString == er)
        return;
    m_errorString = er;
    emit errorStringChanged();
}
