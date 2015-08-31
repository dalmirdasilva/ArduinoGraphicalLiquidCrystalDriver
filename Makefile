ARDUINO_LIB_PATH=/opt/arduino-1.6.5/libraries
LIB_LIST=GraphicalLiquidCrystal \
	GraphicalLiquidCrystalKs0108\
	GraphicalLiquidCrystalNokia5110
SOURCE_PATH=`pwd`

all: 
	@echo "Use [install], [unistall] or [doc]"

install:
	@echo "Instaling all libraries..."
	@for lib in $(LIB_LIST) ; do \
		ln -s  $(SOURCE_PATH)/$$lib $(ARDUINO_LIB_PATH); \
	done
	@echo "done."
	
uninstall:
	@echo "Uninstaling all libraries..."
	@for lib in $(LIB_LIST) ; do \
		rm -r $(ARDUINO_LIB_PATH)/$$lib ; \
	done
	@echo "done."
		
doc:
	@echo "Running doxygen..."
	@rm -rf doc
	@mkdir doc
	doxygen doxygen.conf
	@cd doc/latex; make pdf; cp refman.pdf ../../Documentation.pdf 
	@cd ../..
	@rm -rf doc
	@echo "done."
	
	
	
