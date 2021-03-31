CC = gcc
DIR = $(shell pwd)
INCLUDE_DIR = $(DIR)/inc
SRC_DIR = $(DIR)/src
LIB_DIR = $(DIR)/lib
TEST_BIN_DIR = $(DIR)/build/test
TEST_DIR = $(DIR)/test

objects = $(patsubst %.c, %.o, $(wildcard $(SRC_DIR)/*.c))

all: $(LIB_DIR)/libvector.a 
	@echo "done!"

$(LIB_DIR)/libvector.a: ${objects}
	$(AR) crvu $@ $^

$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) -c -I $(INCLUDE_DIR) $^ -o $@

$(TEST_BIN_DIR)/vectorTest: $(LIB_DIR)/libvector.a $(TEST_DIR)/test.c
	$(CC) -static -I${INCLUDE_DIR} -L${LIB_DIR}  $(TEST_DIR)/test.c -lvector -o $@

test: $(TEST_BIN_DIR)/vectorTest
	$(TEST_BIN_DIR)/vectorTest
    
clean: $(TEST_BIN_DIR)/vectorTest $(objects) $(LIB_DIR)/libvector.a
	rm $(TEST_BIN_DIR)/vectorTest $(objects) $(LIB_DIR)/libvector.a

echo:
	@echo "make echo"
	@echo "objects ${objects}"