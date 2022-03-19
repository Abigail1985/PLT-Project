VPATH=src include

ifeq ($(debug),yes)
DEBUG := -D debug
endif

OBJ = testlex testsyn testsem compile main

CFLAGS = -Wall -o   #编译标志位


all: testlex testsyn testsem compile main

testlex: testlex.c analyse_lexical.h mystack.h transition.h
		gcc $(CFLAGS) bin/testlex src/testlex.c -I./include 
testsyn: testsyn.c analyse_syntax.h analyse_lexical.h mystack.h transition.h
		gcc $(CFLAGS) bin/testsyn src/testsyn.c -I./include
testsem: testsem.c  analyse_semantic.h analyse_syntax.h analyse_lexical.h mystack.h transition.h
		gcc $(CFLAGS) bin/testsem src/testsem.c -I./include
compile: compile.c compile_automate.h analyse_semantic.h analyse_syntax.h analyse_lexical.h mystack.h transition.h
		gcc $(CFLAGS) bin/compile src/compile.c -I./include
main: main.c executeur.h compile_automate.h analyse_semantic.h analyse_syntax.h analyse_lexical.h mystack.h transition.h
		gcc $(DEBUG) $(CFLAGS) bin/main src/main.c -I./include 
clean:
		rm -rf $(OBJ)
        
.PHONY: clean
