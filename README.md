## QmlSql

  QmlSql comes with some helper Objects to connect to Databases Run Queries against SQL.
Make Sqlight Databases . Create Models from SQL Queries. All from Qml. QmlSql source code
is licensed under the GNU LESSER GENERAL PUBLIC LICENSE. While QmlSql documentation is licensed
under the GNU Free Documentation License. QmlSql Comes with two examples and documents for both.
If you run the code below it will install the docs to QT_INSTALL_EXAMPLES QT_INSTALL_QML QT_INSTALL_DOCS. After running the first time restart Qtcreator and you will see the examples on the welcome screen and the docs in the assiatant and also in the help section of Qt creator.


#### Example for mysql
The first thing that one must do if not using sqlight is compile the driver for qt for the database that they wish to use. For this this please see the qt documentation [Located Here ](http://doc.qt.io/qt-5/sql-driver.html#building-the-plugins-manually)

This plugin has only been tested on postgre mysql and sqlight.

After that is built then you can build the plugin and its docs and examples with

````make
  qmake
  make
  make docs
  make install_docs
  make install
````



#### Online Documentation

There are some gh-pages coming real soon in a push or two away. see #6 or more info.

#### Mobile notes

I hve tested this both on android and ios and it works (mysql,sqlight). I added the CONFIG flag of static to get it to compile and work on ios. but this is now wrote into the profile. If one needs help with cross compile for android I am willing to help for a small fee. But it is really not that hard to do.


After building you have to copy over the lib and the qmldir file to your qt root folder qml folder after building it. This is what it should look like after copying the libe and qmldir file.

````
<QTBASE DIR>/<QT Version>/Toolkit/qml/QmlSql/
<QTBASE DIR>/<QT Version>/Toolkit/qml/QmlSql/libQmlSql.a
<QTBASE DIR>/<QT Version>/Toolkit/qml/QmlSql/qmldir
 ````

Then you can use with android and ios.


