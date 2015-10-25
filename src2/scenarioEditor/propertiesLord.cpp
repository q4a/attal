/****************************************************************
**
** Attal : Lords of Doom
**
** propertiesLord.cpp
** this is a template for all .cpp files
**
** Version : $Id: propertiesLord.cpp,v 1.37 2009/11/11 00:22:58 lusum Exp $
**
** Author(s) : Pascal Audoux - Sardi Carlo
**
** Date : 07/12/2002
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

#include "propertiesLord.h"

// generic include files
// include files for QT
#include <QComboBox>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QLayout>
#include <QListWidget>
#include <QMessageBox>
#include <QPixmap>
#include <QPushButton>
#include <QTabWidget>
#include <QVBoxLayout>

#include <QTableWidget>

// application specific include files
#include "libCommon/artefactManager.h"
#include "libCommon/dataTheme.h"
#include "libCommon/genericLord.h"

#include "libClient/askWidget.h"
#include "libClient/gui.h"
#include "libClient/imageTheme.h"

extern QString IMAGE_PATH;
const int PR_MAX_BOX = 6;

//
// ----- PropertiesLord -----
//

PropertiesLord::PropertiesLord( QList<GenericPlayer *> * players, QWidget * parent )
: QDialog( parent, Qt::Dialog )
{
	setWindowTitle( tr( "Lord properties" ) );
	_players = players;
	_lord = 0;

	_tabWidget = new QTabWidget( this );

	_general = new PropertiesLordGeneral( players, 0 );
	_units = new PropertiesLordUnits( 0 );
	_artefacts = new PropertiesLordArtefacts( 0 );
	_warMachines = new PropertiesLordWarMachines( 0 );

	_tabWidget->addTab( _general, tr( "General" ) );
	_tabWidget->addTab( _units, tr( "Units" ) );
	_tabWidget->addTab( _artefacts, tr( "Artefacts" ) );
	_tabWidget->addTab( _warMachines, tr( "War Machines" ) );
	
	QPushButton * pbOk = new QPushButton( this );
	pbOk->setText( tr( "Ok" ) );
	FIXEDSIZE( pbOk );

	QPushButton * pbCancel = new QPushButton( this );
	pbCancel->setText( tr( "Cancel" ) );
	FIXEDSIZE( pbCancel );

	QHBoxLayout * layH1 = new QHBoxLayout();
	layH1->addStretch( 1 );
	layH1->addWidget( pbOk );
	layH1->addStretch( 1 );
	layH1->addWidget( pbCancel );
	layH1->addStretch( 1 );

	QVBoxLayout * layout = new QVBoxLayout( this );
	layout->setMargin( 5 );
	layout->setSpacing( 5 );
	layout->addWidget( _tabWidget, 1 );
	layout->addLayout( layH1 );
	layout->activate();

	connect( pbOk, SIGNAL( clicked() ), this, SLOT( accept() ) );
	connect( pbCancel, SIGNAL( clicked() ), this, SLOT( reject() ) );
}

void PropertiesLord::accept()
{
	if(_units->hasNoneUnits() ) {
		QMessageBox::warning( this, tr( "Lord's properties error" ),
    		tr( "The lord must have at least 1 unit" ) );
		return;
	}
	if( _general->hasNoOwner() ) {
		QMessageBox::warning( this, tr( "Lord's properties error" ),
    		tr( "The lord must have an owner" ) );
		return;
	}

	_general->accept();
	_units->accept();
	_artefacts->accept();
	_warMachines->accept();

	QDialog::accept();
}

void PropertiesLord::init( GenericLord * lord, uint * ids, int numLords )
{
	_lord = lord;
	_general->init( lord, ids, numLords );
	_units->init( lord );
	_artefacts->init( lord );
	_warMachines->init ( lord );
}

//
// ----- GeneralCharacteristic-----
//

GeneralCharacteristic::GeneralCharacteristic (int number, QWidget* parent  )
    : PixmapSpinBox( parent )
{
	_num = number;

	setFrameStyle( QFrame::Panel | QFrame::Raised );
	setLineWidth ( 1 );

	getPixmap ();

	QString text = getText ();

	PixmapSpinBox::init( & _pix, text, true );
};

void GeneralCharacteristic::init (GenericLord* lord)
{
	LordCharac charac = (LordCharac)_num;

	int value = lord->getBaseCharac ( charac );

	setRange( 0, 10000 );
	setValue( value );
}

void GeneralCharacteristic::save ( GenericLord* lord )
{
	int value = getValue( );

	LordCharac charac = (LordCharac)_num;

	lord->setBaseCharac( charac, value);
}

void GeneralCharacteristic::getPixmap ()
{
	QString path = IMAGE_PATH + "misc/ico_";
	LordCharac charac = (LordCharac)_num;
	
	switch( charac ) {
        case ATTACK:
		path += "attack"; 
		break; 
	case DEFENSE: 
		path += "defense"; 
		break; 
	case POWER: 
		break; 
	case KNOWLEDGE: 
		break; 
	case MAXMOVE: 
		path += "move"; 
		break; 
	case MOVE: 
		break;
	case MAXTECHNICPOINT: 
		break;
	case TECHNICPOINT: 
		break;
	case MORALE: 
		break;
	case LUCK:
		path += "luck"; 
		break;
	case VISION:
		path += "vision"; 
		break;
	case EXPERIENCE:
		path += "experience"; 
		break;
	case CHARISMA:
		break;
	default:
		break;
	}

	path += ".png";

	if( ! _pix.load( path ) ) {
		_pix = QPixmap( 10, 10 );
		_pix.fill ();
	}
}

QString GeneralCharacteristic::getText()
{
	QString text = "";

	LordCharac charac = (LordCharac)_num;

	switch( charac ) {
		case ATTACK:
			text = "Attack";
			break;
		case DEFENSE:
			text = "Defense";
			break;
		case POWER:
			break;
		case KNOWLEDGE:
			break;
		case MOVE:
		break;
	case MAXMOVE:
		text = "Maxmove";
		break;
	case MAXTECHNICPOINT:
		break;
	case TECHNICPOINT:
		break;
	case MORALE:
		break;
	case LUCK:
		text = "Luck";
		break;
	case VISION:
		text = "Vision";
		break;
	case EXPERIENCE:
		text = "Experience";
		break;
	case CHARISMA:
		text = "Charisma";
		break;
	default:
		break;
	}

	return text;
}

//
// ----- PropertiesLordGeneral -----
//

PropertiesLordGeneral::PropertiesLordGeneral( QList<GenericPlayer *> * players, QWidget * parent )
: QWidget( parent )
{
	QLabel* playerLabel = new QLabel( this );
	playerLabel->setText( tr( "Select player:" ) );
	FIXEDSIZE (playerLabel);

	_lord = 0;
	_comboPlayer = new QComboBox( this );
	FIXEDSIZE( _comboPlayer );
	_comboPlayer->setFixedWidth (250);

	_listLords = new QListWidget( this );
	_listLords->setFixedSize( 250, 150 );
	_listLords->setSelectionMode ( QAbstractItemView::SingleSelection );

	_faceLord = new QLabel( this );

	_defaultButton = new QPushButton (this);
	_defaultButton->setText( tr( "Default" ) );
	FIXEDSIZE (_defaultButton);

	connect( _defaultButton, SIGNAL( clicked () ), this, SLOT( slot_defaultButtonPushed() ) );

	QVBoxLayout * layout = new QVBoxLayout;
	layout->setSpacing( 5 );
	layout->addWidget( playerLabel , 1, Qt::AlignHCenter );
	layout->addWidget( _comboPlayer , 1 , Qt::AlignHCenter );
	layout->addWidget( _listLords, 1 );
	layout->addWidget( _faceLord, 1, Qt::AlignHCenter );
	layout->addWidget( _defaultButton, 1, Qt::AlignHCenter );
	layout->activate();

	_players = players;

	QGridLayout* gridLayout = new QGridLayout ();
	gridLayout->setSpacing (2);

	int secondNum = 1;

	for( int num = 0; num < PR_MAX_BOX; ) {
		if( ( (LordCharac)secondNum == MOVE )
		|| ((LordCharac)secondNum == TECHNICPOINT)
		|| ((LordCharac)secondNum == POWER)
		|| ((LordCharac)secondNum == CHARISMA)
		|| ((LordCharac)secondNum == MORALE)
		|| ((LordCharac)secondNum == KNOWLEDGE)
		|| ((LordCharac)secondNum == MAXTECHNICPOINT ) ) {
			secondNum++;
		} else {
			generalChar[num] = new GeneralCharacteristic (secondNum , this);
			secondNum++;

			FIXEDSIZE (generalChar[num]);
			gridLayout->addWidget (generalChar[num],  (num) / 2 , num % 2);

			num++;
		}
       }

       gridLayout->activate ();

       QHBoxLayout* hlayout = new QHBoxLayout (this);
       hlayout->addLayout (layout);
       hlayout->addLayout (gridLayout , 1);
       hlayout->activate ();
}

void PropertiesLordGeneral::init( GenericLord * lord, uint * ids, int numLords )
{
	//was uint num
	int num;

	_lord = lord;
	_comboPlayer->clear();
	_comboPlayer->addItem( QString("No player"), 0 );
	for( int i = 0; i < _players->count(); i++ ) {
		QString title = QString::number( i ) + " - " + _players->at( i )->getName();
		_comboPlayer->addItem( title, i+1 );
	}

	if( lord->getOwner() ) {
		_comboPlayer->setCurrentIndex( lord->getOwner()->getNum()+1 );
	} else {
		_comboPlayer->setCurrentIndex( 0 );
	}

	for( num = 0; num < PR_MAX_BOX; num++ ) {
		generalChar[num]->init(lord);
	}

	uint cnt = 0;
	int nbLords = DataTheme.lords.count();
	for( num = 1; num < nbLords; num++ ) {
		bool isIdUsed = false;
		for( int i = 0; i < numLords; i++ ) {
			//safe: num is always positive
			if( ( uint ) num == ids[i] ) {
				isIdUsed = true;
			}
		}

		if( !isIdUsed ) {
			QString string = QString::number (num) + " - " +
				DataTheme.lords.at(num)->getName() + " (" +
				DataTheme.creatures.getRace( DataTheme.lords.at(num)->getRace () )->getName()  + ")";
			_listLords->addItem( string );
			_ids[cnt] = num;
			cnt++;
		}
	}

	connect( _listLords, SIGNAL( currentItemChanged ( QListWidgetItem *, QListWidgetItem * ) ), this, SLOT( slot_lordChanged() ) );
	  

	int numInList = 0;
	for( uint i = 0; i < cnt; i++ ) {
		if( _ids[i] == lord->getId() ) {
			numInList = i;
		}
	}

	_listLords->setCurrentRow( numInList );
	slot_lordChanged();
}

void PropertiesLordGeneral::slot_lordChanged ()
{
	int index = _listLords->currentRow();
	TRACE("slot_lordChanged index %d", index);

	QPixmap* pix = ImageTheme.getLordPixmap(_ids[index] );

	if (!pix) return;

	_faceLord->setPixmap (*pix);
}

void PropertiesLordGeneral::slot_defaultButtonPushed ()
{
	int index = _listLords->currentRow();

	TRACE("slot_defaultButtonPushed index %d", index);
	GenericLord lord(_ids[index]);

	for (int num = 0; num < PR_MAX_BOX; num++)
	{
		generalChar[num]->init(&lord);
	}

}

void PropertiesLordGeneral::accept()
{
	if( _lord->getId() != _ids[_listLords->currentRow() ] ) {
		_lord->setId( _ids[_listLords->currentRow() ] );
	}

	if( _lord->getOwner() ) {
		_lord->getOwner()->removeLord( _lord );
	}

	int idPlayer = _comboPlayer->currentIndex()-1 ;
	if( idPlayer > -1 ) {
		_lord->setOwner( _players->at( idPlayer ) );
		_players->at( idPlayer )->addLord( _lord );
	}

	for( int num = 0; num < PR_MAX_BOX; num++ ) {
		generalChar[num]->save( _lord );
	}
}

bool PropertiesLordGeneral::hasNoOwner()
{

	int idPlayer = _comboPlayer->currentIndex();
	if ( idPlayer ) {
		return false;
	}

	return true;
}



//
// ----- PropertiesLordUnits -----
//

PropertiesLordUnits::PropertiesLordUnits( QWidget * parent )
: QWidget( parent )
{
	_lord = 0;

	QHBoxLayout * layH1 = new QHBoxLayout();
	QHBoxLayout * layH2 = new QHBoxLayout();

	for( int i = 0; i < MAX_UNIT; i++ ) {
		_troop[i] = new EditTroop( i, this );
		if( i <= (MAX_UNIT / 2 ) ) {
			layH1->addWidget( _troop[i] );
		} else {
			layH2->addWidget( _troop[i] );
		}
	}
	layH1->addStretch( 1 );
	layH2->addStretch( 1 );

	QVBoxLayout * layout = new QVBoxLayout( this );
	layout->setSpacing( 5 );
	layout->addLayout( layH1 );
	layout->addLayout( layH2 );
	layout->addStretch( 1 );
	layout->activate();
}

void PropertiesLordUnits::init( GenericLord * lord )
{
	_lord = lord;

	for( int i = 0; i < MAX_UNIT; i++ ) {
		_troop[i]->init (_lord->getUnit (i));
	}
}

void PropertiesLordUnits::accept()
{
	for( int i = 0; i < MAX_UNIT; i++ ) {
		_troop[i]->save( i, _lord );
	}
}

bool PropertiesLordUnits::hasNoneUnits ()
{
	bool hasUnits = false;

	for( int i = 0; i < MAX_UNIT; i++ ) {
		if (_troop[i]->hasUnits ())
		{
			hasUnits = true;
		}
	}

	return (!hasUnits);
}

//
// ----- EditTroop -----
//

EditTroop::EditTroop( int num, QWidget * parent )
	: QFrame( parent )
{
	_unit = 0;
	setFrameStyle( QFrame::Panel | QFrame::Raised );

	QVBoxLayout * layout = new QVBoxLayout( this );
	layout->setMargin( 5 );
	layout->setSpacing( 5 );

	QLabel * label = new QLabel( this );
	label->setText( "Unit " + QString::number( num ) );
	FIXEDSIZE( label );
	layout->addWidget( label );

	_race = new AskCombo( tr( "Race : " ), this );
	_race->setCurrentItem( 0 );
	layout->addWidget( _race );

	_name = new AskCombo( tr( "Name : " ), this );
	layout->addWidget( _name );

	_number = new AskInt( tr( "Number : " ), this );
	_number->setMaxValue( 65535 );
	layout->addWidget( _number );

	layout->addStretch( 1 );
	layout->activate();

	updateData();

	connect( _race, SIGNAL( sig_activated( int ) ), SLOT( slot_raceChanged( int ) ) );
}

bool EditTroop::hasUnits ()
{
	return (_number->getValue () > 0);
}

void EditTroop::slot_raceChanged( int race )
{
	_name->clear();
	for( int i = 0; i < DataTheme.creatures.getRace( race )->count(); i++ ) {
		_name->insertItem( DataTheme.creatures.getRace( race )->at( i )->getName() );
	}
}

void EditTroop::updateData()
{
	int race = _race->currentItem();

	_race->clear();
	for( uint i = 0; i < DataTheme.countRace(); i++ ) {
		_race->insertItem( DataTheme.creatures.getRace( i )->getName() );
	}
	if( _unit ) {
		_race->setCurrentItem( _unit->getRace() );
	} else {
		_race->setCurrentItem( race );
	}

	_name->clear();
	if(_race->currentItem() >= 0) {
		for( int i = 0; i < DataTheme.creatures.getRace( _race->currentItem() )->count(); i++ ) {
			_name->insertItem( DataTheme.creatures.getRace( _race->currentItem() )->at( i )->getName() );
		}
	}
}

void EditTroop::clear()
{
	_race->setCurrentItem( 0 );
	_name->setCurrentItem( 0 );
	_number->setValue( 0 );
}

void EditTroop::init( GenericFightUnit * unit )
{
	_unit = unit;
	if( unit ) {
		_race->setCurrentItem( unit->getRace() );
		slot_raceChanged( unit->getRace() );
		_name->setCurrentItem( unit->getLevel() );
		_number->setValue( unit->getNumber() );
	} else {
		clear();
	}
}

void EditTroop::save( int i, GenericLord * lord )
{
	GenericFightUnit * unit = lord->getUnit( i );

	if( _number->getValue() > 0 ) {
		if( ! unit ) {
			unit = new GenericFightUnit();
		}
		unit->setCreature( _race->currentItem(), _name->currentItem() );
		unit->setNumber( _number->getValue() );
		lord->setUnit( i, unit );
	} else {
		if( unit ) {
			delete unit;
		}
		lord->setUnit( i, 0 );
	}
}

void EditTroop::save( int i, GenericBase * base )
{
	GenericFightUnit * unit = base->getUnit( i );

	if( _number->getValue() > 0 ) {
		if( ! unit ) {
			unit = new GenericFightUnit();
		}
		unit->setCreature( _race->currentItem(), _name->currentItem() );
		unit->setNumber( _number->getValue() );
		base->setUnit( i, unit );
	} else {
		if( unit ) {
			delete unit;
		}
		base->setUnit( i, 0 );
	}
}


//
// ----- PropertiesLordArtefacts -----
//

PropertiesLordArtefacts::PropertiesLordArtefacts( QWidget * parent )
: DoubleTableWidget( parent )
{
	_leftList->setHorizontalHeaderLabels(  QStringList(tr( "Common artefacts list" )) );
	_rightList->setHorizontalHeaderLabels(  QStringList(tr( "Lord's artefacts list" )) );
}

void PropertiesLordArtefacts::accept()
{
	_lord->getArtefactManager()->clearArtefacts();

	uint nbArtefacts = DataTheme.artefacts.count();
	for( uint i = 0; i < nbArtefacts; i++ ) {
		if( _listItems[i] ) {
			_lord->getArtefactManager()->addArtefact( i );
		}
	}
}

void PropertiesLordArtefacts::fillList()
{
	uint nbArtefacts = DataTheme.artefacts.count();
	setRowTable(nbArtefacts);

	for( uint id = 0; id < nbArtefacts; id++ ) {
		GenericArtefactModel* artefactModel = DataTheme.artefacts.at(id);
		QString artefactName = artefactModel->getName();
		QPixmap* pix = ImageTheme.getArtefactIcon( id );
		
		QTableWidgetItem * item = new QTableWidgetItem(artefactName);
		item->setIcon(QIcon(*pix));
		item->setFlags(Qt::ItemIsSelectable);
		item->setTextAlignment(Qt::AlignVCenter);
		_leftList->setItem(id, 0, item);
		
		QTableWidgetItem * item2 = new QTableWidgetItem(artefactName);
		item2->setIcon(QIcon(*pix));
		item2->setFlags(Qt::ItemIsSelectable);
		item2->setTextAlignment(Qt::AlignVCenter);
		_rightList->setItem(id, 0, item2);
		
		_listItems[id] = false;

		for( uint cnt = 0; cnt < _lord->getArtefactManager()->getArtefactNumber(); cnt++ ) {
			GenericLordArtefact* lordsArtefact = _lord->getArtefactManager()->getArtefact(cnt);
			if( lordsArtefact->getType() == id ) {
				_listItems[id] = true;
			}
		}
	}

	refreshList ();
}

void PropertiesLordArtefacts::refreshList ()
{
	uint nbArtefacts = DataTheme.artefacts.count();

	for( uint i = 0; i < nbArtefacts; i++ ) {
		if( ! _listItems[i] ) {
			_leftList->showRow( i );
			_rightList->hideRow( i );
		} else {
			_leftList->hideRow( i );
			_rightList->showRow( i );
		}
	}

	_leftList->resizeColumnsToContents();
	_rightList->resizeColumnsToContents();

	/// XXX: It's not ideal, I know - may be YOU can do it better :)
}

//
// ----- PropertiesLordWarMachines -----
//

PropertiesLordWarMachines::PropertiesLordWarMachines( QWidget * parent )
: DoubleTableWidget( parent )
{
	_leftList->setHorizontalHeaderLabels( QStringList(tr( "Common war machines list" )) );
	_rightList->setHorizontalHeaderLabels( QStringList(tr( "Lord's war machines list" )) );
}

void PropertiesLordWarMachines::accept()
{
	uint nbMachineLord = _lord->getMachineNumber();

	for( uint num = 0; num < nbMachineLord; num++ ) {
		_lord->removeMachine( 0 );
	}

	uint nbMachine = DataTheme.machines.count();
	for( uint id = 0; id < nbMachine; id++ ) {
		if( _listItems[id] ) {
			_lord->addMachine( id );
		}
	}
}

void PropertiesLordWarMachines::fillList()
{
	uint nbMachines = DataTheme.machines.count();
	setRowTable(nbMachines);

	for( uint id = 0; id < nbMachines; id++ ) {
		QString machineName = DataTheme.machines.at(id)->getName();
		//  XXX: We need it !  QPixmap* pix = ImageTheme.getWarMachineIcon( id );

		QTableWidgetItem * item = new QTableWidgetItem(machineName);
		item->setTextAlignment(Qt::AlignVCenter);
		item->setFlags(Qt::ItemIsSelectable);
		_leftList->setItem(id, 0, item);
		// XXX:         _warMachinesList->setPixmap (id, 0, *pix);

		QTableWidgetItem * item2 = new QTableWidgetItem(machineName);
		item2->setTextAlignment(Qt::AlignVCenter);
		item2->setFlags(Qt::ItemIsSelectable);
		_rightList->setItem(id, 0, item2);
		// XXX:        _lordsWarMachinesList->setPixmap (id, 0, *pix);

		_listItems[id] = false;

		for( uint cnt = 0; cnt < _lord->getMachineNumber(); cnt++) {
			if( _lord->getMachine(cnt) == id ) {
				_listItems[id] = true;
			}
		}
	}

	refreshList();
}

void PropertiesLordWarMachines::refreshList ()
{
	uint nbMachines = DataTheme.machines.count();
    	for( uint id = 0; id < nbMachines; id++ ) {
        	if( ! _listItems[id] ) {
			_leftList->showRow (id);
			_rightList->hideRow (id);
        	} else {
			_leftList->hideRow (id);
			_rightList->showRow (id);
		}
	}

	_leftList->resizeColumnsToContents();
	_rightList->resizeColumnsToContents();
}

DoubleTableWidget::DoubleTableWidget( QWidget * parent )
: QWidget( parent )
{
	_lord = 0;

	// main layout
	QHBoxLayout* mainLayout = new QHBoxLayout (this);

	QPushButton* addButton = new QPushButton (this);
	QPushButton* removeButton = new QPushButton (this);

	addButton->setText ("->");
	removeButton->setText ("<-");

	_leftList = new QTableWidget (this);
	_leftList->setColumnCount(1);
	_leftList->setShowGrid (false);
	_leftList->verticalHeader()->hide();

	_rightList = new QTableWidget (this);
	_rightList->setColumnCount(1);
	_rightList->setShowGrid (false);
	_rightList->verticalHeader()->hide();

	QGridLayout* gridLayout = new QGridLayout ();
	gridLayout->addWidget (addButton,1,0, Qt::AlignCenter);
	gridLayout->addWidget (removeButton,3,0, Qt::AlignCenter);

	mainLayout->setMargin (20);
	mainLayout->addWidget (_leftList, 3);
	mainLayout->addLayout (gridLayout , 2);
	mainLayout->addWidget (_rightList, 3);

	connect (addButton, SIGNAL (clicked ()), this, SLOT (addButtonPushed ()));

	connect (removeButton, SIGNAL (clicked ()), this, SLOT (removeButtonPushed ()));
}

void DoubleTableWidget::init( GenericLord * lord )
{
    _lord = lord;

     fillList ();
}

void DoubleTableWidget::fillList ()
{
}

void DoubleTableWidget::addButtonPushed ()
{
   if (_leftList->selectedRanges().count() <= 0) return;

	 QTableWidgetSelectionRange sel =  _leftList->selectedRanges().at(0);

   int row = sel.topRow ();

   _listItems[row] = true;

   refreshList ();
}

void DoubleTableWidget::removeButtonPushed ()
{
   if (_rightList->selectedRanges().count() <= 0) return;

	 QTableWidgetSelectionRange sel =  _rightList->selectedRanges().at(0);

   int row = sel.topRow ();

	 _listItems[row] = false;

   refreshList ();
}

void DoubleTableWidget::refreshList()
{
}

void DoubleTableWidget::setRowTable(int rows)
{
	_leftList->setRowCount(rows);
	_rightList->setRowCount(rows);
}
