PROJ_OUT_DIR = ./bin
PROJ_NAME=sqlgen
PROJ_TYPE=exe

VPATH = ./:./src


PROJ_OBJS =  GenerateSQLFile.o \
	Logger.o \
	SQLCodeInfo.o \
	SQLDataProcessor.o \
	SQLItem.o \
	SQLType.o \
	TableInfo.o \
	UtilityFun.o \
	main.o


$(PROJ_NAME) : $(PROJ_OBJS)
	g++ -o $(PROJ_NAME) $(PROJ_OBJS)
	-rm -rf $(PROJ_OUT_DIR)
	-mkdir $(PROJ_OUT_DIR)
	cp $(PROJ_NAME) $(PROJ_OUT_DIR)

.PHONY : install clean
install :
	-rm -rf $(PROJ_OUT_DIR)
	-mkdir $(PROJ_OUT_DIR)
	cp $(PROJ_NAME) $(PROJ_OUT_DIR)	
clean :
	-rm $(PROJ_NAME)
	-rm $(PROJ_OBJS)













