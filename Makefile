include Makefile.rules

OBJ = $(SRC:.cc=.o)
#src/sys/graphic/import.o

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) -o $@ $^ $(FUCKING_FLAGS) $(LDLIBS) $(LDFLAGS)
	@echo
	@echo -n "[+] Before strip: "
	@ls -F -b -h -l $(EXEC) | sed -e 's/.* \(.*K\).*/\1/'
	@echo -n "[+] After strip: "
	@strip -s -R .comment -R .gnu.version $(EXEC)
	@ls -F -b -h -l $(EXEC) | sed -e 's/.* \(.*K\).*/\1/'
	@echo -n "Char: "
	@cat $(SRC) | perl -pe 's/\s//g' | wc -c

# src/sys/graphic/import.o: src/sys/graphic/import.asm src/sys/includes/graphic.hh
# 	nasm -f elf $<

%.o : %.cc
	$(CC) -o $@ -c $< $(CFLAGS) $(PERSO_FLAGS)

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
	$(COMP) $(TARBALL_NAME) $(SRC) src/sys/**/*.hh $(TO_COMPRESS)

FINAL = final
final: $(EXEC)
#	sstrip $(BIN)
	@lzma -9 $<
	@cat stub $<.lzma > $(FINAL)
	@rm $<.lzma
	@chmod a+rx $(FINAL)
	@echo -n "[+] After stub: "
	@ls -F -b -h -l $(FINAL) | sed -e 's/.* \(.*K\).*/\1/'

run: final
	bash $(FINAL)
