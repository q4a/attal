/****************************************************************
**
** Attal : Lords of Doom
**
** sectionBase.cpp
** section specifying bases
**
** Version : $Id: sectionBase.cpp,v 1.35 2009/11/19 22:14:10 lusum Exp $
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

#include "sectionBase.h"
 
// generic include files
#include <algorithm>
// include files for QT
#include <QLayout>
#include <QPushButton>
#include <QVBoxLayout>

// application specific include files
#include "libCommon/dataTheme.h"
#include "libCommon/log.h"
#include "libCommon/priceMarket.h"

#include "libClient/gui.h"

#include "themeEditor/mapDispositionEditor.h"
#include "themeEditor/previewBaseDialog.h"

extern QString IMAGE_PATH;

SectionBase::SectionBase( QWidget * parent, const char * name )
	: GenericSection( parent, name )
{
	setTitle( tr( " Base " ) );
	_idBase = 0;

	_name = new AskString( tr( "Name: " ), _mainWidget );

	_vision = new AskInt( tr( "Vision: " ), _mainWidget );
	_vision->setMaxValue(1000);
	
	_resBase = new AskCost( tr( "Res Base: " ), _mainWidget );

	_resCost = new AskCost( tr( "Res Cost: " ), _mainWidget );

	_inside = new SectionInsideBuilding( _mainWidget );
	
	_dispo = new MapDispositionEditor( _mainWidget );
	
	QPushButton * preview = new QPushButton( _mainWidget );
	preview->setText( tr( "Preview inside" ) );
	FIXEDSIZE( preview );
	
	_buildingAction = new BuildingAction( _mainWidget );

	QVBoxLayout * layout = new QVBoxLayout( _mainWidget );
	layout->setMargin( 5 );
	layout->setSpacing( 5 );
	layout->addWidget( _name );
	layout->addWidget( _vision );
	layout->addWidget( _resBase );
	layout->addWidget( _resCost );
	layout->addWidget( _dispo );
	layout->addWidget( preview );
	layout->addWidget( _buildingAction );
	layout->addWidget( _inside, 1 );
	layout->activate();

	updateData();
	init();
	connect( preview, SIGNAL( clicked() ), SLOT( slot_preview() ) );
}

void SectionBase::slot_preview()
{
	save();
	PreviewBaseDialog preview;
	preview.init( DataTheme.bases.at( _idBase ) );
	preview.exec();
}

void SectionBase::updateData()
{
}

void SectionBase::init()
{
	if( _idBase < DataTheme.bases.count() ) {
		QString text = QString (tr( " Base : %1/%2" ).arg( _idBase + 1 ).arg(DataTheme.bases.count()));
		setTitle( text );

		uint nbRes = DataTheme.resources.count();
		GenericBaseModel * base = DataTheme.bases.at( _idBase );
		_name->setValue( base->getModelName() );
		_vision->setValue( base->getVision() );
		
		for( uint i = 0; i < nbRes; i++ ) {
			if( base->getPriceMarket() ) {
				_resCost->setValue( i ,base->getPriceMarket()->getResourcePrice( i ) );
			}
			_resBase->setValue( i ,base->getResourceList()->getValue( i ) );
		}

		QString dest = IMAGE_PATH + "base/race_" + QString::number( _idBase ) + "/base.png";
		_dispo->init( base, dest );
		_buildingAction->init( base->getActionList() );
		_inside->setBase( _idBase );
		_inside->init();
	} else {
		clear();
	}
}

void SectionBase::clear()
{
	_idBase = 0;
	_name->setValue( "" );
	_vision->setValue( 3 );
	_inside->setBase( 0 );
	_inside->clear();
}

void SectionBase::selectFirst()
{
	save();
	if( _idBase > 0 ) {
		_idBase = 0;
		init();
	}
}

void SectionBase::selectPrevious()
{
	save();
	if( _idBase > 0 ) {
		_idBase--;
	}
	init();
}

void SectionBase::selectNext()
{
	save();
	if( _idBase < DataTheme.bases.count() - 1 ) {
		_idBase++;
	}
	init();
}

void SectionBase::selectLast()
{
	save();
	_idBase = DataTheme.bases.count() - 1;
	init();
}

void SectionBase::selectNew()
{
	save();
	_idBase = DataTheme.bases.count();
	GenericBaseModel * base = new GenericBaseModel();
	DataTheme.bases.append( base );
	init();
}

void SectionBase::selectDel()
{
	clear();
	delete DataTheme.bases.takeAt( _idBase );
	_idBase = std::min( _idBase, DataTheme.bases.count() - 1 );
	init();
}

void SectionBase::save()
{
	if( _idBase < DataTheme.bases.count() ) {
		int nbRes = DataTheme.resources.count();
		GenericBaseModel * base = DataTheme.bases.at( _idBase );
		base->setModelName( _name->getValue() );
		base->setVision( _vision->getValue() );
		base->clearActions();
		QList<Action *>   alist;
		alist = _buildingAction->save();
		Action * action;
		if( alist.count() ) {
			for( int i = 0; i < alist.count(); i++ ) {
				action = alist.at( i );
				if( action ) {
					base->addAction( action );
				}
			}
		}

		_inside->save();
		for( int i = 0; i < nbRes; i++ ) {
			base->getPriceMarket()->setResourcePrice( i, _resCost->getValue( i ) );
			base->getResourceList()->setValue( i, _resBase->getValue( i ) );
		}
	}
}

//
// ----- SectionInsideBuilding -----
//

SectionInsideBuilding::SectionInsideBuilding( QWidget * parent, const char * /* name */ )
: QWidget( parent )
{
	_idBase = 0;
	_idBuild = 0;
	_select = new SelectionWidget( this );
	_select->setTitle( tr( " Building " ) );

	_name = new AskString( tr( "Name: " ), this );
	_description = new AskString( tr( "Description: " ), this );
	_x = new AskInt( "x: ", this );
	_x->setMaxValue( 1000 );
	_y = new AskInt( "y: ", this );
	_y->setMaxValue( 1000 );
	_cost = new AskCost( tr( "Cost: " ), this );
	_maintCost = new AskCost( tr( "Maintenance cost: " ), this );
	
	_comboInAct = new QComboBox( this );
	_comboInAct->addItem( tr( "Not yet used" ) );
	_comboInAct->addItem( tr( "Main Building" ) );
	_comboInAct->addItem( tr( "Castle" ) );
	_comboInAct->addItem( tr( "Prod Creature" ) );
	_comboInAct->addItem( tr( "Market" ) );
	_comboInAct->addItem( tr( "Lord's building" ) );
	FIXEDSIZE( _comboInAct );

	_params = new AskIntList( this );
	_params->setLabel( tr( "Params: " ) );

	_requirements = new AskIntList( this );
	_requirements->setLabel( tr( "Requirements" ) );

	QVBoxLayout * layout = new QVBoxLayout( this );
	layout->setMargin( 5 );
	layout->setSpacing( 5 );
	layout->addWidget( _select );
	layout->addWidget( _name );
	layout->addWidget( _description );
	layout->addWidget( _x );
	layout->addWidget( _y );
	layout->addWidget( _cost );
	layout->addWidget( _maintCost );
	layout->addWidget( _comboInAct );
	layout->addWidget( _params );
	layout->addWidget( _requirements );


	layout->addStretch( 1 );
	layout->activate();

	connect( _select, SIGNAL( sig_first() ), SLOT( selectFirst() ) );
	connect( _select, SIGNAL( sig_previous() ), SLOT( selectPrevious() ) );
	connect( _select, SIGNAL( sig_next() ), SLOT( selectNext() ) );
	connect( _select, SIGNAL( sig_last() ), SLOT( selectLast() ) );
	connect( _select, SIGNAL( sig_new() ), SLOT( selectNew() ) );
	connect( _select, SIGNAL( sig_del() ), SLOT( selectDel() ) );

	init();
}

void SectionInsideBuilding::setBase( uint base )
{
	if( (int)base != _idBase ) {
		save();
		clear();
		_idBase = base;
		_idBuild = 0;
		init();

	}
}
void SectionInsideBuilding::selectFirst()
{
	save();
	if( _idBuild > 0 ) {
		_idBuild = 0;
		init();
	}
}

void SectionInsideBuilding::selectPrevious()
{
	save();
	if( _idBuild > 0 ) {
		_idBuild--;
	}
	init();
}

void SectionInsideBuilding::selectNext()
{
	save();
	if( ( _idBase < DataTheme.bases.count() )
		&& ( _idBuild < (int)DataTheme.bases.at( _idBase )->getBuildingCount() - 1 ) ) {
		_idBuild++;
	}
	init();
}

void SectionInsideBuilding::selectLast()
{
	save();
	if( _idBase < DataTheme.bases.count() ) {
		_idBuild = DataTheme.bases.at( _idBase )->getBuildingCount() - 1;
	}
	init();
}

void SectionInsideBuilding::selectNew()
{
	save();
	if( _idBase < DataTheme.bases.count() ) {
		_idBuild = DataTheme.bases.at( _idBase )->getBuildingCount();
		InsideBuildingModel * build = new InsideBuildingModel();
		DataTheme.bases.at( _idBase )->addBuilding( build );
		/// XXX: setRace/setLevel
	}
	init();
}

void SectionInsideBuilding::selectDel()
{
	clear();
	if( _idBase < DataTheme.bases.count() ) {
		DataTheme.bases.at( _idBase )->removeBuilding( _idBuild );
		_idBuild = std::min( _idBase, (int)DataTheme.bases.at( _idBase )->getBuildingCount() - 1 );
	}
	init();
}

void SectionInsideBuilding::save()
{
	if( ( _idBase < DataTheme.bases.count() )
	  && ( _idBuild < (int)DataTheme.bases.at( _idBase )->getBuildingCount() ) ) {
		InsideBuildingModel * build = DataTheme.bases.at( _idBase )->getBuildingModel( _idBuild );
		build->setName( _name->getValue() );
		build->setDescription( _description->getValue() );
		build->setX( _x->getValue() );
		build->setY( _y->getValue() );
		uint nbRes = DataTheme.resources.count();
		for( uint i = 0; i < nbRes; i++ ) {
			build->setCost( i, _cost->getValue( i ) );
			build->setMantCost( i, _maintCost->getValue( i ) );
		}
		
		InsideAction * action = new InsideAction();
		action->setType((uint) _comboInAct->currentIndex());
		for( uint i = 0; i < _params->count(); i++ ) {
			action->addParam( _params->getValue(i) );
		}
		build->setAction( action );

		build->clearRequirements();
		for( uint i = 0; i < _requirements->count(); i++ ) {
			build->addRequirement( _requirements->getValue( i ) - 1 );
		}
	}
}

void SectionInsideBuilding::init()
{
	if( ( _idBase < DataTheme.bases.count() )
	  && ( _idBuild < (int)DataTheme.bases.at( _idBase )->getBuildingCount() ) ) {
		QString text = QString (tr( " Building : %1/%2" ).arg(_idBuild + 1).arg(DataTheme.bases.at( _idBase )->getBuildingCount()));
		_select->setTitle( text );

		InsideBuildingModel * build = DataTheme.bases.at( _idBase )->getBuildingModel( _idBuild );
		_name->setValue( build->getName() );
		_description->setValue( build->getDescription() );
		_x->setValue( build->getX() );
		_y->setValue( build->getY() );
		_params->clear();
		_requirements->clear();
		InsideAction * action = build->getAction();
		if( action ){
			_comboInAct->setCurrentIndex((uint) action->getType());
			for( uint i = 0; i < action->getParamNumber(); i++ ) {
				_params->addValue( action->getParam(i) );
			}
		} else {
			_comboInAct->setCurrentIndex( 0 );
		}
		uint nbRes = DataTheme.resources.count();
		for( uint i = 0; i < nbRes; i++ ) {
			_cost->setValue( i, build->getCost( i ) );
			_maintCost->setValue( i, build->getMantCost( i ) );
		}
		for( int i = 0; i < build->getRequirementNumber(); i++ ) {
			_requirements->addValue( build->getRequirement( i ) + 1 );
		}
	}
}

void SectionInsideBuilding::clear()
{
	_idBuild = 0;
	_name->setValue( "" );
	_description->setValue( "" );
	_x->setValue( 0 );
	_y->setValue( 0 );
	uint nbRes = DataTheme.resources.count();
	for( uint i = 0; i < nbRes; i++ ) {
		_cost->setValue( i,  0  );
		_maintCost->setValue( i,  0  );
	}
	_comboInAct->setCurrentIndex( 0 );
	_params->clear();
	_requirements->clear();
}

