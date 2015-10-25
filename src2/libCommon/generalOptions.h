/****************************************************************
**
** Attal : Lords of Doom
**
** generalOptions.h
** general options of the theme
**
** Version : $Id: generalOptions.h,v 1.5 2005/12/26 23:31:51 lusum Exp $
**
** Author(s) : Pascal Audoux - Sardi Carlo
**
** Date : 06/08/2004
**
** Licence :    
**	This program is free software; you can redistribute it and/or modify
**   	it under the terms of the GNU General Public License as published by
**     	the Free Software Foundation; either version 2, or (at your option)
**      any later version.
**
**	This program is distributed in the hope that it will be useful,
** 	but WITHOUT ANY WARRANTY; without even the implied warranty of
**	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**	GNU General Public License for more details.
**
****************************************************************/

 
#ifndef GENERALOPTIONS_H
#define GENERALOPTIONS_H
 
 
// generic include files
// include files for QT
#include <QString>
#include <QXmlDefaultHandler>
// application specific include files
#include "libCommon/calendar.h"
 

/*              ------------------------------
 *                       GeneralOptions
 *              ------------------------------ */


class GeneralOptions
{
	
public:
	enum VISION_MANAGEMENT {
		VISION_ONCE,
		VISION_REAL
	};
	
	/** Constructor */
	GeneralOptions();
	
	~GeneralOptions();
	
	/** Clears data */
	void clear();
	
	/** save into data file */
	bool save();
	
	/** Init options from data file */
	bool init();
	
	VISION_MANAGEMENT getVisionManagement() { return _visionManagement; }
	
	void setVisionManagement( VISION_MANAGEMENT visionManagement ) {
		_visionManagement = visionManagement;
	}
	
	static QString getVisionManagementName( VISION_MANAGEMENT type );

	CalendarModel * getCalendar() {return _calendar;}
	
	void setCalendar(CalendarModel * calendar);

protected:
	VISION_MANAGEMENT _visionManagement;
	CalendarModel * _calendar;
};


/*              ------------------------------
 *                    GeneralOptionsHandler
 *              ------------------------------ */

class GeneralOptionsHandler : public QXmlDefaultHandler
{

public:
	/** Construtor */
	GeneralOptionsHandler( GeneralOptions * general );

	/** Return the error protocol if parsing failed */
	QString errorProtocol() { return _errorProt; }

	/** Before starting parsing */
	bool startDocument();

	/** Define Start elements and associated actions */
	bool startElement( const QString& namespaceURI, const QString& localName, const QString& qName, const QXmlAttributes& atts );

	/** Define End elements and associated actions */
	bool endElement( const QString& namespaceURI, const QString& localName, const QString& qName );

	/** Define what to do of characters */
	bool characters( const QString& ch );

	/** Error function */
	bool fatalError( const QXmlParseException& exception );

private:
	QString _errorProt;
	GeneralOptions * _general;
	CalendarModel * _calendar;
	int _num;
	int _levelC,_value;
	
	enum State {
		StateInit,
		StateDocument,
		StateVision,
		StateCalendar,
		StateCalendarLevel,
		StateCalendarName
	};
	State _state;
};
 
#endif // GENERALOPTIONS_H   
