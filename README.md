

## QmlSql



#### Desktops


#### mysql
The 1st thing that one must do if not using sqlight is compile the driver for qt. For this this please see the qt documentation [Located Here ](http://doc.qt.io/qt-5/sql-driver.html#building-the-plugins-manually)


After that is built then you can build the plugin with 

````qmake 
qmake 
make 
make install 
````



#### Android 
For building for android you have to copy over the lib and the qmldir file to your qt root folder qml folder after building it. This is what it should look like after copying the libe and qmldir file.

````
<QTBASE DIR>/<QT Version>/Toolkit/qml/QmlSql/
<QTBASE DIR>/<QT Version>/Toolkit/qml/QmlSql/libQmlSql.so
<QTBASE DIR>/<QT Version>/Toolkit/qml/QmlSql/qmldir
 ````

Then you can use with android. 


#### IMPORTANT 
This is not yet ported for iOS


