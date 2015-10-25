
#include <QtTest/QtTest>
#include "client/clientInterface.h"
#include "libCommon/attalCommon.h"
#include "libAi/analyst.h"
#include "libClient/imageTheme.h"
#include "libServer/attalServer.h"
#include "libServer/engine.h"
#include "libServer/fightEngine.h"
#include "libCommon/define.h"
#include "conf.h"

#include "fakeLoader.h"

class TestAttal: public QObject
{
	Q_OBJECT
		private slots:
		void testLord();
		void testPlayer();
		void testFightEngine();
		void testEngine();
		void testGameData();
		void testGame();
		void testMap();
		void testGameAi();
};

void TestAttal::testLord()
{

	useTheme();

	DataTheme.init();
	GenericLord lord( 1 );
	GenericLordModel * model = DataTheme.lords.at( 1 );
	lord.reinit();
	QCOMPARE( lord.getCharac( LEVEL ) , 0 );
	QCOMPARE( lord.getOwner() , (GenericPlayer *)0 );
	QCOMPARE( lord.getId() , 1 );
	QCOMPARE( lord.getBaseCharac( MOVE ) , model->getBaseCharac( MAXMOVE ) );
	lord.setBaseCharac( MOVE, lord.getCharac( MAXMOVE ) );
	lord.decreaseBaseCharac( MOVE, 2 );
	QCOMPARE( lord.getBaseCharac( MOVE ) , model->getBaseCharac( MAXMOVE ) -2  );
	lord.endTurn();
	QCOMPARE( lord.getBaseCharac( MOVE ) , model->getBaseCharac( MAXMOVE ) );
	QCOMPARE( ( uint ) lord.getRace() , model->getRace() );
	//int count = lord.countUnits();
	Creature * crea = new Creature();
	lord.addCreatures( crea, 10 );
	lord.clearUnits();
	QCOMPARE( lord.countUnits() , 0 );
	lord.setCost( 0, 100);
	QCOMPARE( ( int ) lord.getCost(0) ,100 );
	lord.setVisible( false );
	QCOMPARE( lord.isVisible() ,false );
	GenericCell * cell = new GenericCell();
	lord.setCell( cell );
	QCOMPARE( lord.getCell() ,cell );
	QCOMPARE( cell->getLord() ,(GenericLord *)&lord );
	lord.removeFromGame();
	QCOMPARE( lord.getCell() , (GenericCell * )0 );
	QCOMPARE( cell->getLord() ,(GenericLord *)0 );
	QCOMPARE( lord.hasMachineType( 0 ) , false );
	
	delete crea;
	delete cell;

}

void TestAttal::testPlayer()
{
	GenericPlayer player;
	QCOMPARE( player.isAlive() , true );

}

void TestAttal::testFightEngine()
{
	AttalServer * server = new AttalServer( 1717 );

	QCOMPARE( server->isListening() , true );
		
	FightEngine * fight = new FightEngine( server );
	
	delete fight;
	delete server;

}

void TestAttal::testEngine()
{
	AttalServer * server = new AttalServer( 1717 );
	
	QCOMPARE( server->isListening() , true );
	Engine * engine = new Engine( server );
	engine->setGameId( 11 );
	engine->start();
	
	QCOMPARE( engine->getGameId() ,11 );

	engine->quit();

	engine->wait();

	delete engine;	
	delete server;
}

void TestAttal::testGameData()
{
	GameData * data = new GameData();
	GenericBase * base = data->getNewBase( 1 );
	base->setId( 1 );
	GameData * data2 = new GameData(*data);
	GenericBase * base2 = data2->getBaseById( 1 );
	QCOMPARE( base, base2 );
	GameData data3(*data);
	GenericBase * base3 = data3.getBaseById( 1 );
	QCOMPARE( base, base3 );
}

void TestAttal::testMap()
{
	GenericMap * map = new GenericMap();
	map->newUnknownMap( 50, 60 );
	QCOMPARE( (int)map->getHeight() , 50 );
	QCOMPARE( (int)map->getWidth() , 60 );
	GenericCell * cell = NULL;

	bool except = false;
	try{ 
		cell = map->at( 500, 650 ); 
	} catch( const char * err ) {
		except = true;
	}
	
	QCOMPARE( except , true );
	
	except = false;
	try{ 
		cell = map->at( -5, -6 ); 
	} catch( const char * err ) {
		except = true;
	}

	QCOMPARE( except , true );

	except = false;
	try{ 
		cell = map->at( 5, 6 ); 
	} catch( const char * err ) {
		except = true;
	}
	
	QCOMPARE( except , false );
	QCOMPARE( cell->getRow() , 5 );
	
	map->newMapType( 50, 60,2 );
	QCOMPARE( (int)map->getHeight() , 50 );
	QCOMPARE( (int)map->getWidth() , 60 );
	QCOMPARE( (int)map->at(1,1)->getType() ,2 );

	delete map;
}

void TestAttal::testGame()
{
	//setLogLevel((LogLevel) 5 );
	FakeLoader *  loader = new FakeLoader();

	Analyst * ai = new Analyst();
	AttalSocket * oldSocket = ai->getSocket();
	ai->start();
	AttalSocket * newSocket = ai->getSocket();
	QCOMPARE( (oldSocket == newSocket), true );
	ai->connectAi( "localhost", loader->getServer()->serverPort()  );

	QString path = THEME_PATH + "scenarii/demo_1player.scn";

	loader->load( path );
	loader->endGame2();
	
	ai->disconnectAi();

	ai->exit(0);
	ai->wait();
	delete ai;

	delete loader;
}

void TestAttal::testGameAi()
{
	/*
	 * test connectAi before and after start in threads
	 * test disconnectAi before the end of thread or thread that end without disconnect
	 * port set to 1718 cause operating system don't release fast enough sockets from test before 
	 */
	//setLogLevel((LogLevel) 5 );
	QString port = "1718";
	FakeLoader *  loader = new FakeLoader(port);
	QCOMPARE( (loader->getServer() != NULL ), true );

	Analyst * ai = new Analyst();
	ai->start();
	
	ai->connectAi( "localhost", loader->getServer()->serverPort() );

	QString path = THEME_PATH + "scenarii/demo_1player.scn";

	loader->load( path );
	loader->endGame2();

	ai->disconnectAi();

	ai->exit(0);
	ai->wait();
	delete ai;

	QCOMPARE( (loader->getServer() != NULL ), true );

	ai = new Analyst();
	ai->connectAi( "localhost", loader->getServer()->serverPort() );
	ai->start();

	path = THEME_PATH + "scenarii/demo_1player.scn";

	loader->load( path );
	loader->endGame2();
	
	ai->exit(0);
	ai->wait();
	delete ai;	
	
	delete loader;
}



QTEST_MAIN(TestAttal)
#include "testattal.moc"
