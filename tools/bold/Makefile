PYTHON_VERSION=2.5

-include ../../Makefile.rules

build:: bold-0.2.1/build/scripts-$(PYTHON_VERSION)/bold

bold-0.2.1.tar.gz:
	wget http://www.alrj.org/projects/bold/bold-0.2.1.tar.gz

bold-0.2.1/build/scripts-$(PYTHON_VERSION)/bold: bold-0.2.1.tar.gz
	tar xf bold-0.2.1.tar.gz
	cd bold-0.2.1/ && python setup.py build
	cd bold-0.2.1/ && python setup.py clean
