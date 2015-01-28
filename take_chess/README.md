take_chess
============

What is take_chess class?
-------------------------

take_chessクラスはあらかじめ指定された数のチェスボードを撮影し、キャリブレーションを行うcalibクラスで使われるxmlファイルを作成するもので、そのxmlファイルの中にはあらかじめ指定したチェスボードの情報と実際に撮影したチェスボードの画像ファイル名が列挙されています。

take_chess class searchs chessboard decided before and takes specified number of chessboard pictures. Then it generate xml file used calib class for camera calibration. 

execution check environment
---------------------------
windows8.1 64bit	
vc12 win32 console application		
opencv2.4.10		

License
-------
Source codes are under GPLv3.