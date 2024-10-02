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
RELEASE_FLAGS=-Wmissing-declarations -Wempty-body -DNDEBUG -DLOG_SWITCH_OFF


#-----------------------------------------------------------------------------------------


# Name of executable program
EXECUTABLE=stack


# Needed directories
SOURCES_DIR=sources
HEADERS_DIR=headers
OBJECTS_DIR=objects

LOG_SUBDIR=logPrinter


# Source files which we use
SOURCE_FILES=main.cpp stack.cpp myRecalloc.cpp canary.cpp murmurHash.cpp
LOG_SOURCE_FILES=logPrinter.cpp

    
# Header files which we use
HEADER_FILES=stack.h myRecalloc.h canary.h stackConfigs.h murmurHash.h
LOG_HEADER_FILES=logPrinter.h logPrinterConfigs.h


# Object file which are obtained by source files compilation
OBJECT_FILES=$(patsubst %.cpp,%.o,$(SOURCE_FILES)) 
LOG_OBJECT_FILES=$(patsubst %.cpp,%.o,$(LOG_SOURCE_FILES))


# List of paths to files with dirictory
SOURCES=$(patsubst %.cpp,$(SOURCES_DIR)/%.cpp,$(SOURCE_FILES))
LOG_SOURCES=$(patsubst %.cpp,$(LOG_SUBDIR)/%.cpp,$(LOG_SOURCE_FILES))

HEADERS=$(patsubst %.h,$(HEADERS_DIR)/%.h,$(HEADER_FILES))
LOG_HEADERS=$(patsubst %.h,$(LOG_SUBDIR)/%.h,$(LOG_HEADER_FILES))

OBJECTS=$(patsubst %.o,$(OBJECTS_DIR)/%.o,$(OBJECT_FILES))
LOG_OBJECTS=$(patsubst %.o,$(OBJECTS_DIR)/%.o,$(LOG_OBJECT_FILES))


#-----------------------------------------------------------------------------------------


# Printing lists of files for debugging
print_sources: 
	echo $(SOURCES) $(LOG_SOURCES)


print_headers: 
	echo $(HEADERS) $(LOG_HEADERS)


print_objects:
	echo $(OBJECTS)


#-----------------------------------------------------------------------------------------


# Make release version of program
all: release


# Run program
run: $(EXECUTABLE)
	@./$<


# Make release version
release: objects_dir clean
	@$(CC) $(RELEASE_FLAGS) $(SOURCES) $(LOG_SOURCES) -o $(EXECUTABLE)


# Make debug version
debug: $(OBJECTS) $(LOG_OBJECTS)
	@$(CC) $(DEBUG_FLAGS) $(OBJECTS) $(LOG_OBJECTS) -o $(EXECUTABLE)


# Compile object files for dubugging
$(OBJECTS_DIR)/%.o: $(SOURCES_DIR)/%.cpp $(HEADERS) $(LOG_HEADERS) objects_dir
	@$(CC) -c $(DEBUG_FLAGS) $< -o $@

$(OBJECTS_DIR)/%.o: $(LOG_SUBDIR)/%.cpp $(HEADERS) $(LOG_HEADERS) objects_dir
	@$(CC) -c $(DEBUG_FLAGS) $< -o $@


# Make OBJECTS_DIR if it doesn't exist
objects_dir:
	@if [ ! -d $(OBJECTS_DIR) ]; \
	then                         \
		mkdir $(OBJECTS_DIR);    \
	fi


# Clean log files
log_clean:
	@rm -f logs/log.txt logs/emergencyLog.txt


# Clean objects dir
clean:
	@rm -f $(OBJECTS) $(LOG_OBJECTS) $(EXECUTABLE)


#-----------------------------------------------------------------------------------------


PUSH_POP_TEST_NAME=pushPopTest

PUSH_POP_TEST_SOURCE_FILES=pushPopTest.cpp stack.cpp myRecalloc.cpp canary.cpp murmurHash.cpp
PUSH_POP_TEST_SOURCES=$(patsubst %.cpp,$(SOURCES_DIR)/%.cpp,$(PUSH_POP_TEST_SOURCE_FILES))
PUSH_POP_TEST_OBJECTS=$(patsubst %.cpp,$(OBJECTS_DIR)/%.o,$(PUSH_POP_TEST_SOURCE_FILES))


# Test for StackPush() and StackPop()
$(PUSH_POP_TEST_NAME): $(PUSH_POP_TEST_OBJECTS) $(LOG_OBJECTS) $(HEADERS) $(LOG_HEADERS)
	@$(CC) $(DEBUG_FLAGS) $(PUSH_POP_TEST_OBJECTS) $(LOG_OBJECTS) -o $(PUSH_POP_TEST_NAME)

# Test for StackPush() and StackPop() with RELEASE_FLAGS
$(PUSH_POP_TEST_NAME)_release: objects_dir $(PUSH_POP_TEST_NAME)_clean
	@$(CC) $(RELEASE_FLAGS) $(PUSH_POP_TEST_SOURCES) $(LOG_SOURCES) -o $(PUSH_POP_TEST_NAME)
	@./$(PUSH_POP_TEST_NAME)


# Run test for StackPush() and StacPop()
$(PUSH_POP_TEST_NAME)_run: $(PUSH_POP_TEST_NAME)
	@./$<

$(PUSH_POP_TEST_NAME)_clean:
	@rm -f $(PUSH_POP_TEST_OBJECTS) $(LOG_OBJECTS) $(PUSH_POP_TEST_NAME)


#-----------------------------------------------------------------------------------------


CANARY_TEST_SOURCE_FILES=stack.cpp myRecalloc.cpp canary.cpp canaryTest.cpp
CANARY_TEST_SOURCES=$(patsubst %.cpp,$(SOURCES_DIR)/%.cpp,$(CANARY_TEST_SOURCE_FILES))

CANARY_TEST_NAME=canaryTest

$(CANARY_TEST_NAME): objects_dir
	@$(CC) $(DEBUG_FLAGS) -DHASH_SWITCH_OFF $(CANARY_TEST_SOURCES) $(LOG_SOURCES) -o $(CANARY_TEST_NAME)

$(CANARY_TEST_NAME)_run: $(CANARY_TEST_NAME)
	@./$<