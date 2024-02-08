all: qxw.hpp 

qxw.hpp: gen_code.py s.py
	python3 gen_code.py > qxw.hpp
	python3 s.py >> qxw.hpp
	emacs --script ../etc/indent.el qxw.hpp

clean:
	\rm qxw.hpp sqrt.hpp

distclean:
	make clean

