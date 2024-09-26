# Compiler
CC=g++-11


# Flags for debugging compilation
DEBUG_FLAGS=-D _DEBUG -ggdb3 -std=c++17 -O0 -Wall $\
-Wextra -Weffc++ -Waggressive-loop-optimizations $\
-Wc++14-compat -Wmissing-declarations -Wcast-align $\
-Wcast-qual -Wchar-subscripts -Wconditionally-supported $\
-Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal $\
-Wformat-nonliteral -Wformat-security -Wformat-signedness $\
-Wformat=2 -Winline -Wlogical-op -Wnon-virtual-dtor $\
-Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith $\
-Winit-self -Wredundant-decls -Wshadow -Wsign-conversion $\
-Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=2 $\
-Wsuggest-attribute=noreturn -Wsuggest-final-methods $\
-Wsuggest-final-types -Wsuggest-override -Wswitch-default $\
-Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code $\
-Wunused -Wuseless-cast -Wvariadic-macros -Wno-literal-suffix $\
-Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast $\
-Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation $\
-fstack-protector -fstrict-overflow -flto-odr-type-merging $\
-fno-omit-frame-pointer -Wlarger-than=8192 -Wstack-usage=8192 $\
-pie -fPIE -fsanitize=address,alignment,bool,$\
bounds,enum,float-cast-overflow,float-divide-by-zero,$\
integer-divide-by-zero,leak,nonnull-attribute,null,object-size,$\
return,returns-nonnull-attribute,shift,signed-integer-overflow,$\
undefined,unreachable,vla-bound,vptr


# Flags for release version compilation
RELEASE_FLAGS=-Wmissing-declarations -Wempty-body -DNDEBUG


#-----------------------------------------------------------------------------------------


# Name of executable program
EXECUTABLE=stack


# Needed directories
SOURCES_DIR=sources
HEADERS_DIR=headers
OBJECTS_DIR=objects


# Source files which we use
SOURCE_FILES=main.cpp stack.cpp myRecalloc.cpp

    
# Header files which we use
HEADER_FILES=stack.h myRecalloc.h


# Object file which are obtained by source files compilation
OBJECT_FILES=$(patsubst %.cpp,%.o,$(SOURCE_FILES))


# List of paths to files with dirictory
SOURCES=$(patsubst %.cpp,$(SOURCES_DIR)/%.cpp,$(SOURCE_FILES))
HEADERS=$(patsubst %.h,$(HEADERS_DIR)/%.h,$(HEADER_FILES))
OBJECTS=$(patsubst %.o,$(OBJECTS_DIR)/%.o,$(OBJECT_FILES))


#-----------------------------------------------------------------------------------------


# Make release version of program
all: release


# Run program
run: $(EXECUTABLE) logs
	@./$<


# Make release version
release: objects_dir clean
	@$(CC) $(RELEASE_FLAGS) $(SOURCES) -o $(EXECUTABLE)


# Make debug version
debug: $(OBJECTS)
	@$(CC) $(DEBUG_FLAGS) $(OBJECTS) -o $(EXECUTABLE)


# Compile object files for dubugging
$(OBJECTS_DIR)/%.o: $(SOURCES_DIR)/%.cpp $(HEADERS) objects_dir
	@$(CC) -c $(DEBUG_FLAGS) $< -o $@


# Make OBJECTS_DIR if it doesn't exist
objects_dir:
	@if [ ! -d $(OBJECTS_DIR) ]; \
	then                         \
		mkdir $(OBJECTS_DIR);    \
	fi


# Make logs directory if it doesn't exist
logs: 
	@if [ ! -d $@ ]; \
	then             \
		mkdir $@;    \
	fi


# Clean log files
log_clean:
	@rm -f logs/log.txt logs/emergencyLog.txt


# Clean objects dir
clean:
	@rm -f $(OBJECTS) $(EXECUTABLE)