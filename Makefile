CC = g++
TEST_C_FLAGS = -std=c++11 -Wall -Werror -pthread -fsanitize=address -static-libasan -g -Og -DTEST -MMD
MAIN_C_FLAGS = -std=c++11 -Wall -Werror -pthread -O3 -MMD

BUILD_DIR = build
TEST_BUILD_DIR = $(BUILD_DIR)/test
MAIN_BUILD_DIR = $(BUILD_DIR)/main

TEST_SOURCE_DIR = source/test
TEST_INCLUDE_DIR = source/test
TEST_RESOURCES_DIR = source/test/resources

MAIN_SOURCE_DIR = source/main
MAIN_INCLUDE_DIR = source/main

PKG_JSONCPP_SOURCE_DIR = ../jsoncpp/src/lib_json
PKG_JSONCPP_INCLUDE_DIR = ../jsoncpp/include

PKG_TEST_SOURCE_DIR = ../sr-test-cpp/build/library
PKG_TEST_INCLUDE_DIR = ../sr-test-cpp/build/library

TEST_CPP_FILES = $(notdir $(wildcard $(TEST_SOURCE_DIR)/*.cpp))
MAIN_CPP_FILES = $(notdir $(wildcard $(MAIN_SOURCE_DIR)/*.cpp))
PKG_JSONCPP_CPP_FILES = $(notdir $(wildcard $(PKG_JSONCPP_SOURCE_DIR)/*.cpp))
PKG_TEST_CPP_FILES = $(notdir $(wildcard $(PKG_TEST_SOURCE_DIR)/*.cpp))
TEST_RESOURCE_FILES = $(notdir $(wildcard $(TEST_RESOURCES_DIR)/*))

TEST_O_FILES = $(TEST_CPP_FILES:%.cpp=test_%.o)
MAIN_O_FILES = $(MAIN_CPP_FILES:%.cpp=main_%.o)
PKG_JSONCPP_O_FILES = $(PKG_JSONCPP_CPP_FILES:%.cpp=pkg_jsoncpp_%.o)
PKG_TEST_O_FILES = $(PKG_TEST_CPP_FILES:%.cpp=pkg_test_%.o)

ALL_TEST_O_FILES = $(TEST_O_FILES) $(MAIN_O_FILES) $(PKG_JSONCPP_O_FILES) $(PKG_TEST_O_FILES)
ALL_MAIN_O_FILES = $(MAIN_O_FILES) $(PKG_JSONCPP_O_FILES)

ALL_TEST_INCLUDE_DIRS = $(MAIN_INCLUDE_DIR) $(PKG_JSONCPP_INCLUDE_DIR) $(PKG_TEST_INCLUDE_DIR)
ALL_MAIN_INCLUDE_DIRS = $(MAIN_INCLUDE_DIR) $(PKG_JSONCPP_INCLUDE_DIR)
ALL_PKG_JSONCPP_INCLUDE_DIRS = $(PKG_JSONCPP_INCLUDE_DIR)
ALL_PKG_TEST_INCLUDE_DIRS = $(PKG_TEST_INCLUDE_DIR)

.PHONY: default all test main clean

default : test main

all : default

test : $(TEST_BUILD_DIR)/a.out $(TEST_RESOURCE_FILES:%=$(TEST_BUILD_DIR)/%)
	$<

$(TEST_BUILD_DIR)/a.out : $(ALL_TEST_O_FILES:%=$(TEST_BUILD_DIR)/%)
	$(CC) $(TEST_C_FLAGS) -o $@ $^

$(TEST_BUILD_DIR)/test_%.o : $(TEST_SOURCE_DIR)/%.cpp Makefile | $(TEST_BUILD_DIR)
	$(CC) -c $(TEST_C_FLAGS) $(ALL_TEST_INCLUDE_DIRS:%=-I%) -o $@ $<

$(TEST_BUILD_DIR)/main_%.o : $(MAIN_SOURCE_DIR)/%.cpp Makefile | $(TEST_BUILD_DIR)
	$(CC) -c $(TEST_C_FLAGS) $(ALL_MAIN_INCLUDE_DIRS:%=-I%) -o $@ $<

$(TEST_BUILD_DIR)/pkg_jsoncpp_%.o : $(PKG_JSONCPP_SOURCE_DIR)/%.cpp Makefile | $(TEST_BUILD_DIR)
	$(CC) -c $(TEST_C_FLAGS) $(ALL_PKG_JSONCPP_INCLUDE_DIRS:%=-I%) -o $@ $<

$(TEST_BUILD_DIR)/pkg_test_%.o : $(PKG_TEST_SOURCE_DIR)/%.cpp Makefile | $(TEST_BUILD_DIR)
	$(CC) -c $(TEST_C_FLAGS) $(ALL_PKG_TEST_INCLUDE_DIRS:%=-I%) -o $@ $<
	
$(TEST_BUILD_DIR)/% : $(TEST_RESOURCES_DIR)/% | $(TEST_BUILD_DIR)
	cp $< $@

$(TEST_BUILD_DIR) :
	mkdir -p $@

main : $(MAIN_BUILD_DIR)/a.out

$(MAIN_BUILD_DIR)/a.out : $(ALL_MAIN_O_FILES:%=$(MAIN_BUILD_DIR)/%)
	$(CC) $(MAIN_C_FLAGS) -o $@ $^

$(MAIN_BUILD_DIR)/main_%.o : $(MAIN_SOURCE_DIR)/%.cpp Makefile | $(MAIN_BUILD_DIR)
	$(CC) -c $(MAIN_C_FLAGS) $(ALL_MAIN_INCLUDE_DIRS:%=-I%) -o $@ $<

$(MAIN_BUILD_DIR)/pkg_jsoncpp_%.o : $(PKG_JSONCPP_SOURCE_DIR)/%.cpp Makefile | $(MAIN_BUILD_DIR)
	$(CC) -c $(MAIN_C_FLAGS) $(ALL_PKG_JSONCPP_INCLUDE_DIRS:%=-I%) -o $@ $<

$(MAIN_BUILD_DIR) :
	mkdir -p $@
	
%.hpp : ;

%.h : ;

clean:
	rm -rf $(BUILD_DIR)

-include $(wildcard $(TEST_BUILD_DIR)/*.d)
-include $(wildcard $(MAIN_BUILD_DIR)/*.d)
	
