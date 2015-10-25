/****************************************************************
**
** Attal : Lords of Doom
**
** genericBase.cpp
** in order to manage buildings...
**
** Version : $Id: genericBase.cpp,v 1.57 2009/11/19 22:14:10 lusum Exp $
**
** Author(s) : Pascal Audoux - Sardi Carlo
**
** Date : 10/08/2000
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

#include "genericBase.h"

 
// generic include files
// include files for QT
#include <QTextStream>
#include <QStringList>

// application specific include files
#include "libCommon/creature.h"
#include "libCommon/dataTheme.h"
#include "libCommon/genericCell.h"
#include "libCommon/genericInsideBuilding.h"
#include "libCommon/genericPlayer.h"
#include "libCommon/log.h"
#include "libCommon/unit.h"
#include "libCommon/priceMarket.h"

extern QString DATA_PATH;


//
// ----- GenericBase -----
//

GenericBase::GenericBase( int race )
: 	_id( 0 ),
	_name(""),
	_player( 0 )
{
	_race = race;
	for( int i = 0; i < (int)BASE_LAST ; i++ ) {
		_states[(BaseState) i] = true;
	}
	_currentCell = 0;
	_visitorLord = 0;
	_garrisonLord = 0;
	
	_ress = new GenericResourceList();
					
	getInitResources();

	for( int i = 0; i < MAX_UNIT ; i++ ) {
		_units[i] = 0;
	}
}

GenericBase::~GenericBase()
{
	while( ! _buildings.isEmpty() ) {
		delete _buildings.takeFirst();
	}

	for( uint i = 0; i < MAX_UNIT; i++ ) {
		delete _units[ i ];
	}
	
	delete _ress;
}

//XXX workaround for some problems in parser, too late to modify scenarios format, always keep type as first or second field
void GenericBase::setRace( int race ) { 
	_race = race; 
	
	getInitResources();

}

void GenericBase::save( QTextStream * ts, int indent )
{
	indentation( ts, indent );
	*ts << "<base>" << endl;
	
	indentation( ts, indent );
	*ts << "\t<id>" << _id << "</id>" << endl;

	indentation( ts, indent );
	*ts << "\t<type>" << _race << "</type>" << endl;
	
	indentation( ts, indent );
	*ts << "\t<name>" << _name << "</name>" << endl;

	indentation( ts, indent );
	*ts << "\t<col>" << _currentCell->getCol() << "</col>" << endl;

	indentation( ts, indent );
	*ts << "\t<row>" << _currentCell->getRow() << "</row>" << endl;
	
	for( int j = 0; j < BASE_LAST; j++ ) {
		indentation( ts, indent );
		(*ts) << "\t<basestates type=\"" << j << "\">";
		(*ts) << _states[j];
		(*ts) << "</basestates>" << endl;
	}

	for( int j = 0; j < DataTheme.resources.count(); j++ ) {
		indentation( ts, indent );
		(*ts) << "\t<resource type=\"" << j << "\">";
		(*ts) << _ress->getValue( j );
		(*ts) << "</resource>" << endl;
	}

	for( int i = 0; i < _buildings.count(); i++ ) {
		indentation( ts, indent );
		*ts << "\t<building>" << _buildings.at( i )->getLevel() << "</building>" << endl;
	}

	for( int j = 0; j < _forbidden.count(); j++ ) {
		indentation( ts, indent );
		*ts << "\t<forbidden>" << _forbidden.at( j ) << "</forbidden>" << endl;
	}

	for( int i = 0; i < MAX_UNIT; i++ ) {
		if( _units[i] ) {
			indentation( ts, indent );
			*ts << "\t<unit pos=\"" << i << "\">" << endl;
			indentation( ts, indent );
			*ts << "\t\t<race>" << _units[i]->getRace() << "</race>" << endl;
			indentation( ts, indent );
			*ts << "\t\t<level>" << _units[i]->getLevel() << "</level>" << endl;
			indentation( ts, indent );
			*ts << "\t\t<number>" << _units[i]->getNumber() << "</number>" << endl;
			indentation( ts, indent );
			*ts << "\t</unit>" << endl;
		}
	}
	
	int numprod = _counter.getCreatureNumber();
	if( numprod > 0 ) {
		indentation( ts, indent );
		*ts << "\t<available>" << endl;
		for( int i = 0; i < numprod; i++ ) {
			Creature * crea =_counter.getCreature( i );
			if( crea ) {
				indentation( ts, indent );
				*ts << "\t\t<unit>" << endl;
				indentation( ts, indent );
				*ts << "\t\t\t<race>" << crea->getRace() << "</race>" << endl;
				indentation( ts, indent );
				*ts << "\t\t\t<level>" << crea->getLevel() << "</level>" << endl;
				indentation( ts, indent );
				*ts << "\t\t\t<number>" << _counter.getCreatureCount(i) << "</number>" << endl;
				indentation( ts, indent );
				*ts << "\t\t</unit>" << endl;
			}
		}
		indentation( ts, indent );
		*ts << "\t</available>" << endl;
	}

	indentation( ts, indent );
	*ts << "</base>" << endl;
	*ts << flush;
}



void GenericBase::setPosition( GenericCell * cell )
{
	if( _currentCell != 0 ) {
		_currentCell->setBase( 0 );
	}
	_currentCell = cell;
	_currentCell->setBase( this );
}

/*!
  compute the gold earned by the castle...
*/
/// XXX: resource earned now depend on total population 

int GenericBase::collectRessource( int /* num */ )
{
	int ret = 0;
	/*switch( num ) {
	case 0 :
		ret = getPopulation();
		break;
	}*/
	return ret;
}

bool GenericBase::load( QTextStream * )
{
	/// XXX: no args ?
	return true;
}

void GenericBase::exchangeLords()
{
	GenericLord * temp = _visitorLord;
	_visitorLord = _garrisonLord;
	_garrisonLord = temp;
}

uint GenericBase::getDoorRow()
{
	if( _race < DataTheme.bases.count() ) {
		return DataTheme.bases.at( _race )->getDoorRow();
	} else {
		return 0;
	}
}

uint GenericBase::getDoorCol()
{
	if( _race < DataTheme.bases.count() ) {
		return DataTheme.bases.at( _race )->getDoorCol();
	} else {
		return 0;
	}
}

uint GenericBase::getDispoHeight()
{
	if( _race < DataTheme.bases.count() ) {
		return DataTheme.bases.at( _race )->getHeight();
	} else {
		return 0;
	}
}

uint GenericBase::getDispoWidth()
{
	if( _race < DataTheme.bases.count() ) {
		return DataTheme.bases.at( _race )->getWidth();
	} else {
		return 0;
	}
}

void GenericBase::getInitResources()
{
	if( _race < DataTheme.bases.count() ) {
		_ress->setListValues( DataTheme.bases.at( _race )->getResourceList() );
	} else {
	}

}

GenericMapDisposition::DispositionType GenericBase::getDisposition( uint row, uint col )
{
	if( _race < DataTheme.bases.count() ) {
		return DataTheme.bases.at( _race )->getDisposition( row, col );
	} else {
		return GenericMapDisposition::OCCUPIED;
	}
}

QString GenericBase::getModelName()
{
	if( _race < DataTheme.bases.count() ) {
		return DataTheme.bases.at( _race )->getModelName();
	} else {
		return "";
	}			
}

void GenericBase::addBuilding( GenericInsideBuilding * build )
{
	if( ! _buildings.count( build ) ) {
		_buildings.append( build );
	}
}

void GenericBase::addBuilding( int level )
{
	GenericInsideBuilding * building = new GenericInsideBuilding();
	building->setRace( _race );
	building->setLevel( level );
	addBuilding( building );
}

GenericInsideBuilding * GenericBase::getBuilding( uint num )
{
	GenericInsideBuilding * ret = 0;

	if( num < getBuildingCount() ) {
		return _buildings.at( num );
	}

	return ret;
}

GenericInsideBuilding * GenericBase::getBuildingByType( uint type )
{
	GenericInsideBuilding * ret = 0;

	for( int i = 0; i < _buildings.count(); i++ ) {
		if( _buildings.at( i )->getLevel() == type ) {
			ret = _buildings.at( i );
		}
	}

	return ret;
}

bool GenericBase::hasBuildingType( uint type )
{
	bool ret = false;

	for( int i = 0; i < _buildings.count(); i++ ) {
		if( _buildings.at( i )->getLevel() == type ) {
			ret = true;
			i =  _buildings.count();
		}
	}

	return ret;
}

void GenericBase::removeBuildingType( uint type )
{
	GenericInsideBuilding * building = getBuildingByType( type );
	removeBuilding( building );
}

void GenericBase::removeBuilding( GenericInsideBuilding * building )
{
	_buildings.removeAll( building );
	delete building;
}

void GenericBase::removeBuilding( int level )
{
	GenericInsideBuilding * building = getBuildingByType(level);
	removeBuilding( building );
}

void GenericBase::updateBuilding( uchar level, bool create )
{
	if( create ) {
		addBuilding( level );
	} else {
		removeBuildingType( level );
	}

}

uint GenericBase::getForbiddenBuildingsNumber()
{
	return _forbidden.count();
}

int GenericBase::getForbiddenBuilding( uint num )
{
	int ret = 0;

	if( (int)num < _forbidden.count() ) {
		ret = _forbidden.at( num );
	}

	return ret;
}

void GenericBase::addForbiddenBuilding( int num )
{
	_forbidden.append( num );
}

bool GenericBase::isForbidden( int num )
{
	bool ret = false;

	for( int i = 0; i < _forbidden.count(); i++ ) {
		if( _forbidden.at( i ) == num ) {
			ret = true;
		}
	}

	return ret;
}

bool GenericBase::canBuildBuilding( InsideBuildingModel * building )
{
	bool ret = true;

	int nbRequirements = building->getRequirementNumber();

	for( int i = 0; i < nbRequirements; ++i ) {
		if( ! hasBuildingType( building->getRequirement( i ) ) ) {
			ret = false;
			break;
		}
	}

	return ret;
}

int GenericBase::getCreatureProduction( Creature * creature )
{
	return _counter.getCreatureCount( creature );
}

int GenericBase::getCreatureProduction( int num )
{
	return _counter.getCreatureCount( num );
}

void GenericBase::setCreatureProduction( Creature * creature, int value)
{
	_counter.setCreatureCount( creature, value );
}

bool GenericBase::canProduceCreature( Creature * creature )
{
	uint nbBuildings = _buildings.count();
	int creatRace = creature->getRace();
	int creatLevel = creature->getLevel();

	GenericBaseModel * baseModel = DataTheme.bases.at( _race );
	for( uint i = 0; i < nbBuildings; i++ ) {
		GenericInsideBuilding * building = _buildings.at( i );
		InsideBuildingModel * model = baseModel->getBuildingModel( building->getLevel() );
		InsideAction * action = model->getAction();
		if( action ) {
			if( action->getType() == INSIDE_CREA ) {
				if( creatRace == action->getParam( 0 ) && creatLevel == action->getParam( 1 ) ) {
					return true;
				}
			}
		}
	}
	return false;
}

void GenericBase::initCreatureProduction()
{
	uint nbBuildings = _buildings.count();
	_counter.clear();

	GenericBaseModel * baseModel = DataTheme.bases.at( _race );
	for( uint i = 0; i < nbBuildings; i++ ) {
		GenericInsideBuilding * building = _buildings.at( i );
		InsideBuildingModel * model = baseModel->getBuildingModel( building->getLevel() );
		InsideAction * action = model->getAction();
		if( action ) {
			if( action->getType() == INSIDE_CREA ) {
				int race = action->getParam( 0 );
				int level = action->getParam( 1 );
				int number = action->getParam( 2 );
				
				addCreature( race, level, number );
			}
		}
	}
}

void GenericBase::buyCreature( Creature * creature, int number )
{
	_counter.decreaseCreature( creature, number );
}

void GenericBase::addCreature( int race, int level, int number )
{
	TRACE("GenericBase::addCreature race %d, level %d, number %d", race, level, number );
	Creature * creature = DataTheme.creatures.at( race, level );			
	_counter.addCreature( creature, number );
}

int GenericBase::getCreatureNumber()
{
	return _counter.getCreatureNumber();
}

Creature * GenericBase::getCreatureProduced(int num)
{
	return _counter.getCreature(num);
}


bool GenericBase::canAddGarrison( Creature * creature )
{
	bool ret = false;

	for( int i = 0; i < MAX_UNIT; i++ ) {
		if( isGarrisonLord() ) {
			GenericFightUnit * tempUnit = getGarrisonLord()->getUnit( i );
			if( ( tempUnit == 0 ) || ( tempUnit->getCreature() == creature ) ) {
				ret = true;
			}
		} else {
			if( ( getUnit( i ) == 0 ) || ( getUnit( i )->getCreature() == creature ) ) {
				ret = true;
			}
		}
	}

	return ret;
}

void GenericBase::addGarrison( Creature * creature, int number )
{
	if( isGarrisonLord() ) {
		getGarrisonLord()->addCreatures( creature, number );
	} else {
		for( int i = 0; i < MAX_UNIT; i++ ) {
			if( getUnit( i ) == 0 ) {
				GenericFightUnit * unit = new GenericFightUnit( creature );
				unit->setNumber( number );
				setUnit( i, unit );
				break;
			} else if( getUnit( i )->getCreature() == creature ) {
				GenericFightUnit * unit = getUnit( i );
				unit->addNumber( number );
				break;
			}
		}
	}
}

void GenericBase::addUnit( int pos , int race , int level , int nb )
{
	GenericFightUnit * uni =getUnit( pos );
		
		if( nb != 0 ){
			if( !uni ){
				uni = new GenericFightUnit();
			}
      //XXX bugged code?
			uni->setCreature( race, level );
			uni->setNumber( nb );
		}	else {
			if( uni ) {
				uni->setNumber( nb );
				delete uni;
				uni = NULL;
			}
		}
		setUnit( pos, uni );
}

int GenericBase::getVision()
{
	int ret = 3;

	if( _race < DataTheme.bases.count() ) {
		ret = DataTheme.bases.at( _race )->getVision();
	}

	return ret;
}

QString GenericBase::getBaseDescription()
{
	QString msg;
	msg = QObject::tr( "Base " ) + getName(); 
	msg += _ress->getAutoDescription();
	return msg;
}

QList<Action *> GenericBase::getActionList( Action::ActionType type )
{
	QList<Action *> ret;

	GenericBaseModel * _model = DataTheme.bases.at( _race );
	for( int i = 0; i < _model->getActionList()->count(); i++ ) {
		if( _model->getActionList()->at( i )->getType() == type ) {
			ret.append( _model->getActionList()->at( i ) );
		}
	}
	return ret;
}


//
// ----- GenericBaseModel -----
//

GenericBaseModel::GenericBaseModel()
: _race( 0 ), _vision( 3 )
{
	_actionList = new QList<Action *>;

	_ress = new GenericResourceList();

	GenericMapDisposition::init( 5, 5 );
	setDisposition( 4, 0, DOOR );
	_price = new PriceMarket();
}

GenericBaseModel::~GenericBaseModel()
{
	while( ! _buildings.isEmpty() ) {
		delete _buildings.takeFirst();
	}

	if(_price) {	
		delete _price;
		_price = 0;

	}
	delete _ress;

	while( ! _actionList->isEmpty() ) {
		delete _actionList->takeFirst();
	}
	delete _actionList;

}

void GenericBaseModel::addBuilding( InsideBuildingModel * building )
{
	_buildings.append( building );
}


InsideBuildingModel * GenericBaseModel::getBuildingModel( int num )
{
	return _buildings.at( num );
}

void GenericBaseModel::removeBuilding( uint num )
{
	if( (int)num < _buildings.count() ) {
		delete _buildings.takeAt( num );
	}
}
	
void GenericBaseModel::setPriceMarket( PriceMarket * price ) 
{
	if(_price) {
		delete _price;
		_price = 0;
	}
 	_price = price;
}


void GenericBaseModel::save( QTextStream * ts, int indent )
{
	uint nbRes = DataTheme.resources.count();

	indentation( ts, indent );
	*ts << "<base>" << endl;

	indentation( ts, indent );
	*ts << "\t<name>" << _modelName << "</name>" << endl;
	
	indentation( ts, indent );
	*ts << "\t<!--race>" << _race << "</race-->" << endl;
	indentation( ts, indent );
	*ts << "\t<vision>" << _vision << "</vision>" << endl;

	GenericMapDisposition::save( ts, indent+1 );

	for( int j = 0; j < DataTheme.resources.count(); j++ ) {
		indentation( ts, indent );
		*ts << "\t<resource type=\"" << j << "\">";
		*ts << _ress->getValue( j );
		*ts << "\t</resource>" << endl;
	}
	indentation( ts, indent+1 );
	*ts << "<rescost>" << endl;
	for( uint k = 0; k < nbRes; k++ ) {
		if( _price->getResourcePrice(k) > 0 ) {
			indentation( ts, indent+1 );
			*ts << "\t<cost ressource=\"" << k << "\">";
			*ts << _price->getResourcePrice( k ) << "</cost>" << endl;
		}
	}
	indentation( ts, indent+1 );
	*ts << "</rescost>" << endl;

	for( int i = 0; i < _actionList->count(); ++i ) {
		_actionList->at( i )->save( ts, indent );
	}

	//indentation( ts, indent );

	for( int i = 0; i < _buildings.count(); ++i ) {
		InsideBuildingModel * build = _buildings.at(i);
		build->save( ts, indent + 1 );
	}

	indentation( ts, indent );
	*ts << "</base>" << endl;
	*ts << flush;
}


//
// ----- BaseList -----
//

BaseList::BaseList()
{
}

BaseList::~BaseList()
{
	clear();
	/*while( ! _type.isEmpty() ) {
		delete _type.takeFirst();
	}*/
}

void BaseList::clear()
{
	_type.clear();
	while( ! isEmpty() ) {
		delete takeFirst();
	}
}

bool BaseList::init()
{
	clear();
	BaseHandler handler( this );
	QFile file( DATA_PATH + "bases.dat" );
	QXmlInputSource source( &file );
	QXmlSimpleReader reader;
	reader.setContentHandler( &handler );
	reader.setErrorHandler( &handler );
	bool ok = reader.parse( source );
	file.close();
	if ( !ok ) {
		logEE( "Parse Error (%s) : %s", qPrintable( QString( DATA_PATH + "bases.dat" ) ), qPrintable( handler.errorProtocol() ) );
		return false;
	}

	return true;
}

bool BaseList::save()
{
	
	QString filename = DATA_PATH + "bases.dat";
	QFile file( filename );

	if (! file.open( QIODevice::WriteOnly ) ) {
		logEE( "Could not open file %s for writing\n", qPrintable( filename ) );
		return false;
	}

	QTextStream ts( &file );

	ts << "<?xml version=\"1.0\" encoding=\"UTF-8\"?><!DOCTYPE bases>" << endl;
	ts << "<bases>" << endl;

	GenericBaseModel * base;
	for( int i = 0; i < count(); i++ ) {
		base = at( i );
		if( base ) {
			base->save( &ts, 1 );
		}
	}

	ts << "</bases>" << endl;

	file.close();

	return true;
	
}


GenericBaseModel * BaseList::at( QString typeName )
{
	GenericBaseModel * ret = first();
	GenericBaseModel * current;
	
	QListIterator<GenericBaseModel *> ite( *this );
	
	while( ite.hasNext() ) {
		current = ite.next();
		if( current->getModelName() == typeName ) {
			ret = current;
		}
	}	

	return ret;
}

// void BaseList::clear()
// {
// 	QPtrList<GenericBaseModel>::clear();
// 	_type.clear();
// }

// int BaseList::addType( QString typeName )
// {
// 	QPtrListIterator<QString> ite( _type );
// 	int cpt = 0, ret = -1;
	
// 	for( ite.toFirst(); ite.current(); ++ite ) {
// 		if( *(ite.current()) == typeName ) {
// 			ret = cpt;
// 			break;
// 		}
// 		cpt++;
// 	}
// 	if( ret == -1 ) {
// 		ret = cpt;
// 		_type.append( new QString( typeName ) );
// 	}
	
// 	return ret;	
// }


//
// ----- BaseHandler -----
//


BaseHandler::BaseHandler( BaseList * list )
{
	_list = list;
}

bool BaseHandler::startDocument()
{
	// at the beginning of parsing: do some initialization
	_errorProt = "";
	_list->clear();
	_race = 0;
	_level = 0;

	_state = StateInit;
	return true;
}

bool BaseHandler::startElement( const QString &, const QString &, const QString & qName, const QXmlAttributes & atts )
{
	if( qName == "bases" && _state == StateInit ) {
		_state = StateDocument;
	} else if( qName == "base" && _state == StateDocument ) {
		_state = StateBase;
		_base = new GenericBaseModel();
		_base->setRace( _race );
	} else if( qName == "name" && _state == StateBase ) {
		_state = StateName;
	} else if( qName == "vision" && _state == StateBase ) {
		_state = StateVision;
	} else if( qName == "disposition" && _state == StateBase ) {
		_state = StateDisposition;
		_height = atts.value( "height" ).toUInt();
		_width = atts.value( "width" ).toUInt();
		_base->GenericMapDisposition::init( _height, _width );
	} else if( qName == "population" && _state == StateBase ) {
		//population, value, growth and loss mantain compatibility with old scenarios, but do nothing
		_state = StatePopulation;
	} else if( qName == "value" && _state == StatePopulation ) {
		_state = StatePopulationValue;
	} else if( qName == "growth" && _state == StatePopulation ) {
		_state = StatePopulationGrowth;
	} else if( qName == "loss" && _state == StatePopulation ) {
		_state = StatePopulationLoss;
	} else if( qName == "resource" && _state == StateBase ) {
		_state = StateResource;
		_res = atts.value( "type" ).toInt();
	} else if( qName == "rescost" && _state == StateBase ) {
		_price = new PriceMarket();
		_state = StateRessPrice;
	} else if( qName == "cost" && _state == StateRessPrice ) {
		if( atts.index( "ressource" ) != -1 ) {
			_csress = atts.value( "ressource" ).toInt();
		} else {
			_csress = 0;
		}
		_state = StateRessCost;
	} else if( qName == "action" && _state == StateBase ) {
		_state = StateAction;
		_baseAction = new Action();
		_baseAction->setType( (Action::ActionType)( atts.value( "type" ).toInt() ) );
		_baseAction->setCoeff( ( atts.value( "coeff" ).toInt() ) );
	} else if( qName == "elementary" && _state == StateAction ) {
		_state = StateElementary;
		_elementary = new ElementaryAction();
		_elementary->setType( (ElementaryAction::ElementaryType)( atts.value( "type" ).toInt() ) );
		_elementary->setArg( atts.value( "arg" ).toInt() );
		_elementary->setArg1( atts.value( "arg1" ).toInt() );
	} else if( qName == "building" && _state == StateBase ) {
		_building = new InsideBuildingModel();
		_building->setRace( _race );
		_building->setLevel( _level );
		_level++;
		_state = StateBuilding;
	} else if( qName == "name" && _state == StateBuilding ) {
		_state = StateBuildingName;
	} else if( qName == "description" && _state == StateBuilding ) {
		_state = StateBuildingDescription;
	} else if( qName == "require" && _state == StateBuilding ) {
		_state = StateBuildingRequire;
	} else if( qName == "replace" && _state == StateBuilding ) {
		_state = StateBuildingReplace;
	} else if( qName == "cost" && _state == StateBuilding ) {
		if( atts.index( "ressource" ) != -1 ) {
			_ress = atts.value( "ressource" ).toInt();
		} else {
			_ress = 0;
		}
		_state = StateBuildingCost;
	} else if( qName == "mcost" && _state == StateBuilding ) {
		if( atts.index( "ressource" ) != -1 ) {
			_mress = atts.value( "ressource" ).toInt();
		} else {
			_mress = 0;
		}
		_state = StateBuildingMantCost;
	} else if( qName == "x" && _state == StateBuilding ) {
		_state = StateBuildingX;
	} else if( qName == "y" && _state == StateBuilding ) {
		_state = StateBuildingY;
	} else if( qName == "action" && _state == StateBuilding ) {
		_action = new InsideAction();
		if( atts.index( "type" ) != -1 ) {
			_action->setType( atts.value( "type" ).toInt() );
		} else {
			_action->setType( 0 );
		}
		_state = StateBuildingAction;
	} else if( qName == "param" && _state == StateBuildingAction ) {
		_state = StateBuildingActionParam;
	} else {
		// error
		return false;
	}
	return true;
}

bool BaseHandler::endElement( const QString &, const QString &, const QString & )
{
	switch ( _state ) {
	case StateBase:
		_list->append( _base );
		_race++;
		_level = 0;
		_state = StateDocument;
		break;
	case StateName:
	case StateVision:
		_state = StateBase;
		break;
	case StateDisposition:
		_state = StateBase;
		break;
	case StatePopulation:
		_state = StateBase;
		break;
	case StatePopulationValue:
		_state = StatePopulation;
		break;
	case StatePopulationGrowth:
		_state = StatePopulation;
		break;
	case StatePopulationLoss:
		_state = StatePopulation;
		break;
	case StateResource:
		_state = StateBase;
		break;
	case StateRessPrice:
		_base->setPriceMarket( _price );
		_state = StateBase;
		break;
	case StateRessCost:
		_state = StateRessPrice;
		break;
	case StateAction:
		_state = StateBase;
		_base->addAction( _baseAction );
		break;
	case StateElementary:
		_state = StateAction;
		_baseAction->addElementaryAction( _elementary );
		break;
	case StateBuilding:
		_base->addBuilding( _building );
		_state = StateBase;
		break;
	case StateBuildingName:
		_state = StateBuilding;
		break;
	case StateBuildingDescription:
		_state = StateBuilding;
		break;
	case StateBuildingRequire:
		_state = StateBuilding;
		break;
	case StateBuildingReplace:
		_state = StateBuilding;
		break;
	case StateBuildingCost:
		_state = StateBuilding;
		break;
	case StateBuildingMantCost:
		_state = StateBuilding;
		break;
	case StateBuildingX:
		_state = StateBuilding;
		break;
	case StateBuildingY:
		_state = StateBuilding;
		break;
	case StateBuildingAction:
		_state = StateBuilding;
		_building->setAction( _action );
		break;
	case StateBuildingActionParam:
		_state = StateBuildingAction;
		break;
	default:
	    // do nothing
	    break;
    }
    return true;	
}

bool BaseHandler::characters( const QString & ch )
{
	QString ch_simplified = ch.simplified();
	if( ch_simplified.isEmpty() ) {
		return true;
	}

	switch( _state ) {
	case StateName:
		_base->setModelName( ch_simplified );
		break;
	case StateVision:
		_base->setVision( ch_simplified.toInt() );
		break;
	case StateDisposition: {
		QStringList li = ch_simplified.split( " " );
		GenericMapDisposition::DispositionType type;
		for( uint i = 0; i < _height; i++ ) {
			for( uint j = 0; j < _width; j++ ) {
				type = GenericMapDisposition::DispositionType( li[ i * _width + j ].toInt() );
				_base->setDisposition( i, j, type );
			}
		}
		}
		break;
	case StatePopulationValue:
		break;
	case StatePopulationGrowth:
		break;
	case StatePopulationLoss:
		break;
	case StateResource:
		_base->getResourceList()->setValue( _res, ch_simplified.toInt() );
		break;
	case StateRessCost:{
		int cost = ch_simplified.toInt();
		 cost = cost < 10000 ?  cost : 100;
		 cost = cost > 0 ?  cost : 100;
		_price->setResourcePrice( _csress,cost );
	 }
		break;
	case StateElementary:
		_elementary->setCoeff( ch_simplified.toInt() );
		break;
	case StateBuildingName:
		_building->setName( ch_simplified );
		break;
	case StateBuildingDescription:
		_building->setDescription( ch_simplified );
		break;
	case StateBuildingRequire:
		_building->addRequirement( ch_simplified.toInt() );
		break;
	case StateBuildingReplace:
		_building->setReplacement( ch_simplified.toInt() );
		break;
	case StateBuildingCost:
		_building->setCost( _ress, ch_simplified.toInt() );
		break;
	case StateBuildingMantCost:
		_building->setMantCost( _mress, ch_simplified.toInt() );
		break;
	case StateBuildingX:
		_building->setX( ch_simplified.toInt() );
		break;
	case StateBuildingY:
		_building->setY( ch_simplified.toInt() );
		break;
	case StateBuildingActionParam:
		_action->addParam( ch_simplified.toInt() );
		break;
	default:
	    return false;
    }

    return true;
}


bool BaseHandler::fatalError( const QXmlParseException& exception )
{
    _errorProt += QString( "fatal parsing error: %1 in line %2, column %3\n" )
	.arg( exception.message() )
	.arg( exception.lineNumber() )
	.arg( exception.columnNumber() );

    return QXmlDefaultHandler::fatalError( exception );
}


