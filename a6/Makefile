OPT:=-O2

CXX = u++					# compiler
CXXFLAGS = -g -Wall ${OPT} -multi -MMD -std=c++11 # compiler flags
MAKEFILE_NAME = ${firstword ${MAKEFILE_LIST}}	# makefile name

OBJECTS = main.o config.o student.o watCard.o watCardOffice.o bank.o parent.o groupoff.o vendingMachine.o nameServer.o bottlingPlant.o truck.o printer.o # list of object files for question 1 prefixed with "q3"
EXEC = soda

DEPENDS = ${OBJECTS:.o=.d}			# substitute ".o" with ".d"
EXECS = ${EXEC}				# all executables

#############################################################

.PHONY : all clean

all : ${EXECS}					# build all executables

${EXEC} : ${OBJECTS}
	${CXX} ${CXXFLAGS} $^ -o $@

#############################################################

${OBJECTS} : ${MAKEFILE_NAME}			# OPTIONAL : changes to this file => recompile

-include ${DEPENDS}		# include *.d files containing program dependences

clean :						# remove files that can be regenerated
	rm -f *.d *.o ${EXECS}
