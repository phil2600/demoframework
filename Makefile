include Makefile.rules

OBJ = $(SRC:.cc=.o)

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) -o $@ $^ $(FUCKING_FLAGS) $(LDLIBS) $(LDFLAGS) $(PERSO_FLAGS)
	@echo
	@echo -n "Before strip: "
	@ls -F -b -h -l cube | sed -e 's/.* \(.*K\).*/\1/'
	@echo -n "After strip: "
	@strip cube
	@ls -F -b -h -l cube | sed -e 's/.* \(.*K\).*/\1/'
	cat $(SRC) | perl -pe 's/\s//g' | wc -c

%.o : %.cc
	$(CC) -o $@ -c $< $(CFLAGS) #$(PERSO_FLAGS)

clean:
	find . -iname "*~"	-delete
	find . -iname "*.o"	-delete
	find . -iname "*.a"     -delete
	find . -iname "*\#*"    -delete
	find . -iname "*.so"    -delete
	find . -iname "*.out"   -delete
	find . -iname "*.core"  -delete
	find . -iname "*.rules" -delete

mrproper: clean
	$(RM) $(EXEC)

dist: mrproper
	$(COMP) $(TARBALL_NAME) $(SRC) $(TO_COMPRESS)
