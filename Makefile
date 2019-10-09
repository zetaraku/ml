CC = gcc
CFLAGS = -std=gnu99 -I ./src	# -DDEBUG
LDFLAGS = -L ./libs
LIBS = -lm

LEX = flex
LFLAGS =
YACC = bison
YFLAGS = --debug --verbose --token-table

RM = rm -f

BINDIR = bin
SRCDIR = src
GENDIR = gen
OBJDIR = obj

TARGET = ml
SRC_OBJS = ml.o Value.o SymbolTable.o FuncExecContext.o eval.o utils.o
GEN_OBJS = lex.yy.o ml.tab.o
SRC_OBJECTS = $(addprefix $(OBJDIR)/, $(SRC_OBJS))
GEN_OBJECTS = $(addprefix $(OBJDIR)/, $(GEN_OBJS))
OBJECTS = $(SRC_OBJECTS) $(GEN_OBJECTS)

.PHONY: all clean
.SUFFIXES: # Delete the default suffixes

all: $(BINDIR)/$(TARGET)
$(BINDIR)/$(TARGET): $(OBJECTS) | $(BINDIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJECTS) $(LIBS) -o $@
$(SRC_OBJECTS): $(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) -c -Wall $(CFLAGS) $(LDFLAGS) -o $@ $<
$(GEN_OBJECTS): $(OBJDIR)/%.o: $(GENDIR)/%.c | $(OBJDIR)
	$(CC) -c -w $(CFLAGS) $(LDFLAGS) -o $@ $<
$(GENDIR)/lex.yy.c: $(SRCDIR)/ml.l $(GENDIR)/ml.tab.c | $(GENDIR)
	$(LEX) $(LFLAGS) -o $@ $<
$(GENDIR)/ml.tab.c: $(SRCDIR)/ml.y | $(GENDIR)
	$(YACC) $(YFLAGS) --defines=$(GENDIR)/ml.tab.h -o $(GENDIR)/ml.tab.c $<
$(BINDIR) $(GENDIR) $(OBJDIR):
	mkdir $@
clean:
	$(RM) $(BINDIR)/* $(GENDIR)/* $(OBJDIR)/*
