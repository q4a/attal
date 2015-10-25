#include "fakeLoader.h"

FakeLoader::FakeLoader(QString port)
{
  _server = new AttalServer( port.toInt(), QHostAddress::LocalHost );
	
	if( _server->isListening() ) {
		newEngine();

	} else {
		delete _server;
		_server = NULL;
	}

}

void FakeLoader::endServer()
{
	if( _server ){
		delete _server;
		_server = NULL;
	}
}

