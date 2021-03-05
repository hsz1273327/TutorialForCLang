DIR = $(shell pwd)
INCLUDE_DIR = $(DIR)/source/inc
SRC_DIR = $(DIR)/source/src
OBJECT_DIR = $(DIR)/source/obj
LIB_DIR = $(DIR)/build/lib
BIN_DIR = $(DIR)/build/bin
TEST_DIR = $(DIR)/test

objects = $(addprefix $(OBJECT_DIR)/,binary_operator.o unary_operator.o struct_operator.o)

all: $(LIB_DIR)/libvector.a
	echo "done!"

$(OBJECT_DIR)/struct_operator.o: $(INCLUDE_DIR)/binary_vector.h
	gcc -c \
-o $(OBJECT_DIR)/struct_operator.o \
-I $(INCLUDE_DIR) \
$(SRC_DIR)/struct_operator.c    

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