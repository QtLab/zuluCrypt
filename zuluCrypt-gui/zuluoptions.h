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

#ifndef ZULUOPTIONS_H
#define ZULUOPTIONS_H

#include <QString>
#include <QDir>
#include <QTime>

class zuluOptions{
public:
	static QString wallet( void )  { return QString( "zuluCrypt")  ; }
	static QString key( void )     { return QString( "LUKS" )      ; }
	static QString formData( void ){ return QString( "Form Data" ) ; }
	static QString getSocketPath( void )
	{
		QTime T = QTime::currentTime() ;
		QString h  = QString::number( T.hour() )   ;
		QString m  = QString::number( T.minute() ) ;
		QString s  = QString::number( T.second() ) ;
		QString ms = QString::number( T.msec() )   ;
		return QString( QDir::homePath() + QString( "/.zuluCrypt-socket/" ) + h + m + s + ms  ) ;
	}
};

#endif