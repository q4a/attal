TEMPLATE = subdirs

SUBDIRS += libCommon
SUBDIRS += libServer
SUBDIRS += libAi
SUBDIRS += libClient
SUBDIRS += libFight
SUBDIRS += ai
SUBDIRS += client
SUBDIRS += server
SUBDIRS += scenarioEditor
SUBDIRS += campaignEditor
SUBDIRS += themeEditor
SUBDIRS += themes
SUBDIRS += i18n
!isEmpty( ATT_DEBUG ) {
		SUBDIRS += test
		test.depends = libCommon libServer libAi libFight libClient
}

ai.depends = libServer libAi
campaignEditor.depends = libClient libFight
client.depends = libClient libFight libServer libAi
libAi.depends = libCommon libServer
libClient.depends = libCommon
libFight.depends = libCommon libClient
libServer.depends = libCommon
scenarioEditor.depends = libClient
server.depends = libServer
themeEditor.depends = libClient
