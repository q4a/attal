/****************************************************************
**
** Attal : Lords of Doom
**
** previewBaseDialog.cpp
** dialog for preview of a base with buildings
**
** Version : $Id: previewBaseDialog.cpp,v 1.10 2008/02/03 17:09:23 lusum Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 28/06/2003
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

#include "previewBaseDialog.h"
 
// generic include files
// include files for QT
#include <QLayout>
#include <QTreeWidget>
#include <QHBoxLayout>
// application specific include files
#include "libCommon/dataTheme.h"
#include "libCommon/genericBase.h"
#include "libCommon/genericInsideBuilding.h"

#include "libClient/gui.h"
#include "libClient/insideBase.h"

PreviewBaseDialog::PreviewBaseDialog( QWidget * parent, const char * name )
: QDialog( parent, Qt::Dialog )
{
	setWindowTitle( QString ( name ) );
	_base = 0;

	_inside = new InsideBase( this );
	InsideBaseView * view = new InsideBaseView( _inside, this );

	_listBuilding = new QTreeWidget( this );
	_listBuilding->setColumnCount( 1 );
	QStringList labels;
	labels << tr( "Buildings" );
	_listBuilding->setHeaderLabels( labels );


	QHBoxLayout * layout = new QHBoxLayout( this );
	layout->addWidget( view, 1 );
	layout->addWidget( _listBuilding );
	layout->activate();

	connect( _listBuilding, SIGNAL( itemClicked( QTreeWidgetItem *, int ) ), SLOT( slot_clickBuilding( QTreeWidgetItem * , int) ) );
}

PreviewBaseDialog::~PreviewBaseDialog()
{
	delete _base;
}

void PreviewBaseDialog::init( GenericBaseModel * model )
{
	delete _base;
	QTreeWidgetItem * item;

	_base = new GenericBase( model->getRace() );
	_inside->setBase( _base );

	_listBuilding->clear();
	for( int i = 0 ; i <= (int)DataTheme.bases.at( _base->getRace() )->getBuildingCount() - 1; i++ ) {
		InsideBuildingModel * building = DataTheme.bases.at( _base->getRace() )->getBuildingModel( i );
		item = new QTreeWidgetItem( _listBuilding, QStringList(building->getName()) );
		item->setCheckState(0, Qt::Unchecked );
	}
	FIXEDSIZE( _listBuilding );
}

void PreviewBaseDialog::slot_clickBuilding(  QTreeWidgetItem * check, int /*column*/ )
{
	QTreeWidgetItem * item;

	int i = 0;
	while( i < _listBuilding->topLevelItemCount() ) {
		item = _listBuilding->topLevelItem( i );
		if( item == check ) {
			break;
		}
		i++;
	}

	if( check->checkState(0) == Qt::Checked ) {
		_base->addBuilding( i );
		GenericInsideBuilding * build = _base->getBuildingByType( i );
		_inside->addBuilding( build );
	} else {
		GenericInsideBuilding * build = _base->getBuildingByType( i );
		_inside->delBuilding( build );
		_base->removeBuildingType( i );
	}
	_inside->update();
}



