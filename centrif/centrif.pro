#
CONFIG += debug_and_release
TEMPLATE = subdirs
CONFIG   += ordered
SUBDIRS  = ioserv/ioserv.pro \
	    scada/scada.pro 

