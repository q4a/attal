TEMPLATE = lib

include( ../config.pro )
CONFIG +=thread

unix {
	target.path = $${ATT_PREFIX}/share/games/attal/translations/
	target.files = de/*.qm
	target.files += fr/*.qm
	target.files += it/*.qm
	target.files += ru/*.qm
	INSTALLS += target
}

