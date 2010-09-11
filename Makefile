include Makefile.rules

OBJ = $(SRC:.cc=.o)

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) -o $@ $^ $(FUCKING_FLAGS) $(LDLIBS) $(LDFLAGS)
	@echo
	@echo -n "[!] Before strip: "
	@ls -F -b -h -l $(EXEC) | sed -e 's/.* \(.*K\).*/\1/'
	@echo -n "[!] After strip: "
	@strip -s -R .comment -R .gnu.version $(EXEC)
	@ls -F -b -h -l $(EXEC) | sed -e 's/.* \(.*K\).*/\1/'
	@zsh -c 'cat **/*.{hh,cc,asm}' | wc

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
	rm -f *.bmp

mrproper: clean
	$(MAKE) -C doc clean
	$(RM) $(EXEC)
	$(RM) $(FINAL)

dist: mrproper
	$(COMP) $(DATE)-D$(TARBALL_NAME) data $(SRC) src/sys/**/*.hh $(TO_COMPRESS)

FINAL = stubbed_demo
$(FINAL): final
final: $(EXEC)
	@cp $(EXEC) $(EXEC)2
#	sstrip $(BIN)
	@lzma -9 $<
	@cat stub $<.lzma > $(FINAL)
	@rm $<.lzma
	@cp $(EXEC)2 $(EXEC) && rm $(EXEC)2
	@chmod a+rx $(FINAL)
	@echo -n "[!] After stub: "
	@ls -F -b -h -l $(FINAL) | sed -e 's/.* \(.*K\).*/\1/'

run: $(FINAL)
	bash $(FINAL)

infos:
	cat **/**.{hh,cc,c,asm} | wc | awk '{print "Lines:", $1, "\nChars:", $3}'

doc::
	$(MAKE) -C doc doc
