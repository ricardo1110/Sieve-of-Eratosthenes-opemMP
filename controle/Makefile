CC = gcc
SEQ_S = sequential.c
CTRL_S = control.c
DATA_S = data.c
LIB = -lm -fopenmp

SEQ_EXE = sequential.exe
CTRL_EXE = control.exe
DATA_EXE = data.exe

#Compile everything
all: $(CTRL_EXE) $(SEQ_EXE) #$(DATA_EXE)

$(SEQ_EXE): $(SEQ_S)
	$(CC) $(SEQ_S) -o $@ $(LIB)

$(CTRL_EXE): $(CTRL_S)
	$(CC) $(CTRL_S) -o $@ $(LIB)

# $(DATA_EXE): $(DATA_S)
# 	$(CC) $(DATA_S) -o $@ $(LIB)

.PHONY: clear
clear:
	rm *.exe