#ifndef FAKELADER_H
#define FAKELOADER_H

#include "libServer/loadGame.h"

extern QString THEME;
extern QString SCENARIO_PATH;
extern QString CAMPAIGN_PATH;
extern QString SAVE_PATH;
extern QString THEME_PATH;
extern QString PORT;

class FakeLoader : public LoadGame 
{
public:

	FakeLoader( QString port = "1717" );

	void endGame2() { endGame(); }
	
	AttalServer * getServer() { return _server; }

private:
  void disconnectAI() {}

	void addInternalAI() {}
	
	void endServer();
	
	bool killAI() { return true; }
				
	void adjustWidgetLoading( StatusWidget /* type */ )  {}

};

#endif // FAKELOADER_H
