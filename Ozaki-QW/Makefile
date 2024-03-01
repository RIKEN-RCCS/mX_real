all: qxw.hpp fp_const.hpp

qxw.hpp fp_const.hpp: gen_code.py s.py
	python3 gen_code.py 1 > fp_const.hpp
	emacs --script ../etc/indent.el fp_const.hpp
	python3 gen_code.py > qxw.hpp
	python3 s.py >> qxw.hpp
	emacs --script ../etc/indent.el qxw.hpp

clean:
	\rm qxw.hpp fp_const.hpp

distclean:
	make clean

