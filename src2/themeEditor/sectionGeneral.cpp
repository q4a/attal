/****************************************************************
**
** Attal : Lords of Doom
**
** sectionGeneral.cpp
** section with general info about theme
**
** Version : $Id: sectionGeneral.cpp,v 1.9 2009/11/04 22:34:29 lusum Exp $
**
** Author(s) : Pascal Audoux - Sardi Carlo
**
** Date : 02/06/2001
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

#include "sectionGeneral.h"
 
// generic include files
// include files for QT
#include <QString>
#include <QLabel>
#include <QLayout>
#include <QPushButton>
#include <QVBoxLayout>
// application specific include files
#include "libCommon/dataTheme.h"
#include "libCommon/generalOptions.h"
#include "libCommon/log.h"

#include "libClient/gui.h"

extern QString IMAGE_PATH;

SectionGeneral::SectionGeneral( QWidget * parent, const char * name )
	: Section( parent, name )
{
	_dayN = 1;
	_weekN = 1;
	_monthN = 1;
	_vision = new AskCombo( tr( "Vision management: " ), this );
	_vision->insertItem( GeneralOptions::getVisionManagementName( GeneralOptions::VISION_ONCE ) );
	_vision->insertItem( GeneralOptions::getVisionManagementName( GeneralOptions::VISION_REAL ) );

	_nbUnit = new AskInt( tr( "Number of unit/lord" ), this );
	_nbUnit->setMinValue( 1 );
	_nbUnit->setMaxValue( 10 );

	_logo = new AskPixmap( false, IMAGE_PATH + QString( "misc/logo.png" ), tr( "Logo (200x180): " ), this );

	_day = new SelectionDate( this );
	_day->setTitle( tr( "Day " ) );
	_dayName = new AskString( tr( "Name: " ), this );
	_week = new SelectionDate( this );
	_week->setTitle( tr( "Week " ) );
	_weekName = new AskString( tr( "Name: " ), this );
	_month = new SelectionDate( this );
	_month->setTitle( tr( "Month" ) );
	_monthName = new AskString( tr( "Name: " ), this );
	
	QVBoxLayout * layout = new QVBoxLayout( this );
	layout->setMargin( 5 );
	layout->setSpacing( 5 );
	layout->addWidget( _vision );
	layout->addWidget( _nbUnit );
	layout->addWidget( _logo );
	layout->addWidget( _day );
	layout->addWidget( _dayName );
	layout->addWidget( _week );
	layout->addWidget( _weekName );
	layout->addWidget( _month );
	layout->addWidget( _monthName );
	layout->addStretch( 1 );
	layout->activate();

	connect( _day, SIGNAL( sig_first() ), SLOT( slot_firstDay() ) );
	connect( _day, SIGNAL( sig_previous() ), SLOT( slot_previousDay() ) );
	connect( _day, SIGNAL( sig_next() ), SLOT( slot_nextDay() ) );
	connect( _day, SIGNAL( sig_last() ), SLOT( slot_lastDay() ) );

	connect( _week, SIGNAL( sig_first() ), SLOT( slot_firstWeek() ) );
	connect( _week, SIGNAL( sig_previous() ), SLOT( slot_previousWeek() ) );
	connect( _week, SIGNAL( sig_next() ), SLOT( slot_nextWeek() ) );
	connect( _week, SIGNAL( sig_last() ), SLOT( slot_lastWeek() ) );

	connect( _month, SIGNAL( sig_first() ), SLOT( slot_firstMonth() ) );
	connect( _month, SIGNAL( sig_previous() ), SLOT( slot_previousMonth() ) );
	connect( _month, SIGNAL( sig_next() ), SLOT( slot_nextMonth() ) );
	connect( _month, SIGNAL( sig_last() ), SLOT( slot_lastMonth() ) );
	
	init();
}

void SectionGeneral::init()
{
	_vision->setCurrentItem( (uint)DataTheme.general.getVisionManagement() );
	QString text =  QString (tr(" Day  : %1/%2").arg(_dayN).arg(FIRST_LEVEL) );
	_day->setTitle( text );
	_dayName->setValue(DataTheme.general.getCalendar()->getLevelName( 0 , _dayN - 1));
	text = QString (tr(" Week  : %1/%2").arg(_weekN).arg(SECOND_LEVEL) );
	_week->setTitle( text );
	_weekName->setValue(DataTheme.general.getCalendar()->getLevelName( 1 , _weekN - 1));
	text = QString (tr(" Month : %1/%2").arg(_monthN).arg(THIRD_LEVEL) );
	_month->setTitle( text );
	_monthName->setValue(DataTheme.general.getCalendar()->getLevelName( 2 , _monthN - 1));

}

void SectionGeneral::save()
{
	DataTheme.general.setVisionManagement( ( GeneralOptions::VISION_MANAGEMENT )_vision->currentItem() );
	_logo->save();
	DataTheme.general.getCalendar()->setLevelName( 0 , _dayN - 1, _dayName->getValue());
	DataTheme.general.getCalendar()->setLevelName( 1 , _weekN - 1, _weekName->getValue());
	DataTheme.general.getCalendar()->setLevelName( 2 , _monthN - 1, _monthName->getValue());
	emit sig_changed();
}

void SectionGeneral::slot_firstDay()
{
	save();
	_dayN = 1;
	init();
}

void SectionGeneral::slot_previousDay()
{
	
	save();
	_dayN = (uint)std::max( 1, (int)_dayN-1 );
	init();
}

void SectionGeneral::slot_nextDay()
{
	save();
	_dayN = std::min( FIRST_LEVEL, (int)_dayN + 1 );
	init();
}

void SectionGeneral::slot_lastDay()
{
	save();
	_dayN = FIRST_LEVEL;
	init();
}

void SectionGeneral::slot_firstWeek()
{
	save();
	_weekN = 1;
	init();
}

void SectionGeneral::slot_previousWeek()
{
	
	save();
	_weekN = (uint)std::max( 1, (int)_weekN-1 );
	init();
}

void SectionGeneral::slot_nextWeek()
{
	save();
	_weekN = std::min( SECOND_LEVEL, (int)_weekN + 1 );
	init();
}

void SectionGeneral::slot_lastWeek()
{
	save();
	_weekN = SECOND_LEVEL;
	init();
}

void SectionGeneral::slot_firstMonth()
{
	save();
	_monthN = 1;
	init();
}

void SectionGeneral::slot_previousMonth()
{
	save();
	_monthN = (uint)std::max( 1, (int)_monthN-1 );
	init();
}

void SectionGeneral::slot_nextMonth()
{
	save();
	_monthN = std::min( THIRD_LEVEL, (int)_monthN + 1 );
	init();
}

void SectionGeneral::slot_lastMonth()
{
	save();
	_monthN = THIRD_LEVEL;
	init();
}





SelectionDate::SelectionDate( QWidget * parent, const char * name )
	: QFrame( parent )
{	
	setWindowTitle( QString ( name ));

	QHBoxLayout * layout = new QHBoxLayout( this );
	setFrameStyle( QFrame::Box | QFrame::Raised );
	
	layout->addSpacing( 5 );
	
	QPushButton * butFirst = new QPushButton( "|<", this );
	FIXEDSIZE( butFirst );
	layout->addWidget( butFirst );
	
	QPushButton * butPrevious = new QPushButton( "<", this );
	FIXEDSIZE( butPrevious );
	layout->addWidget( butPrevious );
	layout->addSpacing( 5 );
	
	_labTitle = new QLabel( tr( "None" ), this );
	FIXEDSIZE( _labTitle );
	layout->addWidget( _labTitle );
	layout->addSpacing( 5 );
	
	QPushButton * butNext = new QPushButton( ">", this );
	FIXEDSIZE( butNext );
	layout->addWidget( butNext );

	QPushButton * butLast = new QPushButton( ">|", this );
	FIXEDSIZE( butLast );
	layout->addWidget( butLast );
	
	layout->addSpacing( 5 );
	layout->addStretch( 1 );
	
	layout->addSpacing( 5 );
	layout->activate();
	
	setMinimumHeight( 40 );
	
	connect( butFirst, SIGNAL( clicked() ), SIGNAL( sig_first() ) );
	connect( butPrevious, SIGNAL( clicked() ), SIGNAL( sig_previous() ) );
	connect( butNext, SIGNAL( clicked() ), SIGNAL( sig_next() ) );
	connect( butLast, SIGNAL( clicked() ), SIGNAL( sig_last() ) );
}

void SelectionDate::setTitle( QString title )
{
	_labTitle->setText( title );
	FIXEDSIZE( _labTitle );
}

