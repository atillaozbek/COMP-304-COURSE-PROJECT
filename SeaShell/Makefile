CPP = g++
CPPFLAGS = -std=c++17
LINKER = -o
CFLAGS = -Wall

SHELLPATH = /ShellFiles/
£EXE_DEPS = $(wildcard $(SHELLPATH)/*.h)
OBJDEPS = $(wildcard $(SHELLPATH)/*.cpp)
OBJS := $(wildcard $(SHELLPATH)/*.o)
EXE = Application
S_EXE = Application.cpp

#OBJDEPS = ShellFiles/Command.h
#OBJ


SHELLPATH = /ShellFiles/

.PHONY = clean
.PHONY = all
.PHONY = first
.PHONY = second
.PHONY = run

#$(SHELLPATH)/%.o : $(SHELLPATH)/%.cpp
#	$(CPP) $(CFLAGS) -MD -MP $< -c -o $@


#$(EXE): $(OBJS) $(OBJDEPS $(EXE_DEPS))
#	echo $(OBJDEPS)
#	$(CPP) $(S_EXE) $(OBJS) $^ -o $@

#first: $(OBJDEPS)
#	$(CPP) $(CFLAGS) $(OBJDEPS)

all: first second

first:
	$(CPP) $(CPPFLAGS) -c ./ShellFiles/Command.cpp ./ShellFiles/SeaShell.cpp ./ShellFiles/SystemHandler.cpp ./ShellFiles/SeaShellCore.cpp

second: 
	$(CPP) $(CPPFLAGS) $(CFLAGS) -Wall Application.cpp Command.o SeaShell.o SystemHandler.o SeaShellCore.o -o Application

binaries: 
	$(CPP) $(CPPFLAGS)  -c bin/Comparison.cpp -o bin/Comparison.o
	$(CPP) $(CPPFLAGS) $(CFLAGS) bin/highlight.cpp -o bin/highlight
	$(CPP) $(CPPFLAGS) $(CFLAGS) bin/kdiff.cpp bin/Comparison.o -o bin/kdiff

run: Application
	./Application

clean:
	@rm -rf *.o
	@rm -rf Application

cleanall:
	@rm -rf bin/*.o
	@rm -rf bin/kdiff
	@rm -rf bin/highlight

-include $(OBJS:.o=.d)
