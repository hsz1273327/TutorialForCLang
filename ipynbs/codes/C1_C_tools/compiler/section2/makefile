DIR = $(shell pwd)
INCLUDE_DIR = $(DIR)/include
SRC_DIR = $(DIR)/src
OBJECT_DIR = $(DIR)/objects
LIB_DIR = $(DIR)/lib
BIN_DIR = $(DIR)/bin
TEST_DIR = $(DIR)/test

objects = $(addprefix $(OBJECT_DIR)/,binary_operator.o unary_operator.o)

all: $(LIB_DIR)/libvector.a
	echo “done!”

$(OBJECT_DIR)/binary_operator.o: $(INCLUDE_DIR)/binary_vector.h
	gcc -c \
-o $(OBJECT_DIR)/binary_operator.o \
-I $(INCLUDE_DIR) \
$(SRC_DIR)/binary_operator.c
    
$(OBJECT_DIR)/unary_operator.o: $(INCLUDE_DIR)/binary_vector.h
	gcc -c \
-o $(OBJECT_DIR)/unary_operator.o \
-I $(INCLUDE_DIR) \
$(SRC_DIR)/unary_operator.c
    
$(LIB_DIR)/libvector.a: $(objects)
	ar crv $(LIB_DIR)/libvector.a \
$(objects)

$(BIN_DIR)/vectorTest: $(LIB_DIR)/libvector.a $(INCLUDE_DIR)/binary_vector.h $(TEST_DIR)/test.c
	gcc \
-o $(BIN_DIR)/vectorTest \
-I $(INCLUDE_DIR) \
-L $(LIB_DIR) \
-l vector \
$(TEST_DIR)/test.c

test: $(BIN_DIR)/vectorTest
	$(BIN_DIR)/vectorTest
    
clean: $(BIN_DIR)/vectorTest $(objects) $(LIB_DIR)/libvector.a
	rm $(BIN_DIR)/vectorTest $(objects) $(LIB_DIR)/libvector.a