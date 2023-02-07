CCFLAGS=-g -Wall -Werror -std=c++17 -Iinclude

TARGET=chess

SRC_DIR=src
TEST_DIR=test

SRC_MAIN=$(SRC_DIR)/main.cc
TEST_MAIN=$(TEST_DIR)/main.cc

SRC_CCS=$(filter-out $(SRC_MAIN), $(wildcard $(SRC_DIR)/*.cc))
TEST_CCS=$(wildcard $(TEST_DIR)/*.cc)

.PHONY : all chess test

all : chess test

chess :
	g++ $(CCFLAGS) -o $(TARGET) $(SRC_CCS) $(SRC_MAIN)

test :
	g++ $(CCFLAGS) -o test_$(TARGET) $(SRC_CCS) $(TEST_CCS)

clean :
	rm -rf $(TARGET)
