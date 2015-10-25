TEMPLATE = lib

include( ../config.pro )
CONFIG +=special

unix {
	target.path = $${ATT_TEMP_THEMES_PREFIX}
	target.files = medieval
	target.files += cyberpunk
	INSTALLS += target
}
