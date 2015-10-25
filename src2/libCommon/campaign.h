/****************************************************************
**
** Attal : Lords of Doom
**
** campaign.h
** manage campaign
**
** Version : $Id: campaign.h,v 1.4 2009/12/27 23:36:50 lusum Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 21/08/2004
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

 
#ifndef CAMPAIGN_H
#define CAMPAIGN_H
 
 
// generic include files
// include files for QT
#include <QList>
#include <QString>
#include <QXmlDefaultHandler>
// application specific include files
 

class QTextStream;
 
/*              ------------------------------
 *                         Campaign
 *              ------------------------------ */


class Campaign
{
 
public:
	/** Constructor */
	Campaign();
	
	/** Destructor */
	virtual ~Campaign();

	bool load( const QString & filename );
	
	void setName( const QString & name ) { _name = name; }
	
	const QString & getName() { return _name; }
	
	const QString & getFileName() { return _filename; }
	
	void setDescription( const QString & desc ) { _description = desc; }
	
	const QString & getDescription() { return _description; }
	
	void setTheme( const QString & theme ) { _theme = theme; }
	
	const QString & getTheme() { return _theme; }
	
	int getScenarioNumber();
	
	int getCurrentScenario() { return _current; }
	
	int getNextScenario();

	void setCurrentScenario( int current) {  _current = current; }
	
	void addScenario( const QString & scenarioName );

	void replaceScenario( int i, const QString & scenarioName );
	
	QString getScenario( int num );

	void clearScenarii();
	
	void save( QTextStream * ts );
	
protected:
	QString _name, _description, _theme, _filename;
	QList<QString> _listScenarii;
	int _current;
};
 
/*              ------------------------------
 *                       CampaignParser
 *              ------------------------------ */


class CampaignParser : public QXmlDefaultHandler
{
public:
	/** Constructor */
	CampaignParser( Campaign * campaign );
	
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
	Campaign * _campaign;
	QString _errorProt;
	
	enum State {
		StateInit,
		StateDocument,
		StateName,
		StateDescription,
		StateCurrent,
		StateTheme,
		StateScenario
	};
	
	State _state;
};

#endif // CAMPAIGN_H   
