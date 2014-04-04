/*
 *
 *  Copyright (c) 2012
 *  name : mhogo mchungu
 *  email: mhogomchungu@gmail.com
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QAction>
#include <QMessageBox>
#include <QObject>
#include <QString>
#include <QByteArray>
#include <QFileDialog>
#include <QFile>
#include <QVector>
#include <QDir>
#include <QProcess>
#include <QCloseEvent>
#include "../../zuluCrypt-gui/socketsendkey.h"
#include "../../zuluCrypt-gui/dialogmsg.h"
#include "getKey.h"
#include <functional>

namespace Ui {
class MainWindow ;
}

class MainWindow : public QWidget
{
	Q_OBJECT
public:
	MainWindow( QWidget * parent = 0 ) ;
	void setToken( const QString& ) ;
	void setApplicationName( const QString& ) ;
	void setkeyLabel( const QString& ) ;
	void setkeyFileLabel( const QString& ) ;
	void setButtonIcon( const QString& ) ;
	void setRequireKey( bool k = true ) ;
	void setRequireKeyFile( bool k = true ) ;
	void Show( void ) ;
	void setExe( const QVector<QString>& exe ) ;
	void setKeyFunction( std::function<QByteArray( const QVector<QString>& exe,const QString& keyFile,const QString& password )> ) ;
	void setfindExeFunction( std::function<const QString&( QVector<QString>& )> ) ;
	~MainWindow() ;
signals:
	void cancel( void ) ;
private slots:
	void defaultButton( void ) ;
	void doneReading( bool ) ;
	void bytesRead( int ) ;
	void startingToreadData( void ) ;
	void pbOpen( void ) ;
	void pbCancel( void ) ;
	void pbKeyFile( void ) ;
	void gotConnected( void ) ;
	void doneWritingData( void ) ;
private:
	void closeEvent( QCloseEvent * ) ;
	void disableAll( void ) ;
	void enableAlll( void ) ;
	void SetFocus( void ) ;
	void Exit( int ) ;
	Ui::MainWindow * m_ui ;
	QString m_addr ;
	QString m_keyFile ;
	QString m_appName ;
	void * m_handle ;
	bool m_working ;
	bool m_requireKey ;
	bool m_requireKeyFile ;
	socketSendKey * m_sendKey ;
	QByteArray m_key ;
	QVector<QString> m_exe ;
	std::function<QByteArray( const QVector<QString>& exe,const QString& keyFile,const QString& password )> m_function ;
	std::function<const QString&( QVector<QString>& )> m_findExecutable ;
};

#endif // MAINWINDOW_H
