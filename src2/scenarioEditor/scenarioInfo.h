/****************************************************************
**
** Attal : Lords of Doom
**
** scenarioInfo.h
** Display/modif scenario info
**
** Version : $Id: scenarioInfo.h,v 1.6 2006/10/06 21:27:36 lusum Exp $
**
** Author(s) : Pascal Audoux - Cyrille Verrier - Sardi Carlo
**
** Date : 08/02/2001
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

 
#ifndef SCENARIOINFO_H
#define SCENARIOINFO_H
 
 
// generic include files
// include files for QT
#include <QComboBox>
#include <QDialog>
#include <QLineEdit>
#include <QSpinBox>
#include <QString>
#include <QTextEdit>

// application specific include files
 

/*              ------------------------------
 *                         ScenarioInfo
 *              ------------------------------ */


class ScenarioInfo : public QDialog
{
	Q_OBJECT
public:
	/** Constructor */
	ScenarioInfo( QWidget * parent = 0, const char * name = 0 );

	/** Clear the infos */
	void clear();

	/** Set description of the scenario */
	void setDescription( const QString & desc ) { _desc->setText( desc ); }

	/** Return description of the scenario */
	QString getDescription() { return _desc->toPlainText(); }

	/** Set name of the scenario */
	void setName( QString name ) { _name->setText( name ); }

	/** Return name of the scenario */
	QString getName() { return _name->text(); }

	/** Set theme required by scenario */
	void setTheme( const QString & theme );

	/** Return theme required by scenario */
	QString getTheme() { return _theme->currentText(); }

	/** Set nb of players required by the scenario */
	void setNbPlayer( int nb ) { _nbPlayers->setValue( nb ); }

	/** Return nb of players required by the scenario */
	uint getNbPlayer() { return _nbPlayers->value(); }

	/** Set height of the map */
	void setMapHeight( int height ) { _editHeight->setText( QString::number( height ) ); }

	/** Return height of the map */
	int getMapHeight() { return _editHeight->text().toInt(); }

	/** Set width of the map */
	void setMapWidth( int width ) { _editWidth->setText( QString::number( width ) ); }

	/** Return width of the map */
	int getMapWidth() { return _editWidth->text().toInt(); }

	void setDay( int nb ) { _day->setValue( nb ); }

	uint getDay() { return  _day->value(); }
	
	void setWeek( int nb ) { _week->setValue( nb ); }

	uint getWeek() { return  _week->value(); }

	void setMonth( int nb ) { _month->setValue( nb ); }

	uint getMonth() { return  _month->value(); }

	void setYear( int nb ) { _year->setValue( nb ); }

	uint getYear() { return  _year->value(); }



public slots:
	/** Slot for 'Ok' button */
	void slot_ok();

private:
	void searchThemes( QComboBox * combo );

	QLineEdit * _name;
	QTextEdit * _desc;
	QComboBox * _theme;
	QSpinBox * _nbPlayers;
	QSpinBox * _day , * _month, * _week , * _year;
	QLineEdit * _editWidth, * _editHeight;
	static int _defaultVal;
};

#endif // SCENARIOINFO_H   
