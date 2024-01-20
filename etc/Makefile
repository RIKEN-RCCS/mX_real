
all: dX_real.hpp tX_real.hpp qX_real.hpp

dX_real.hpp: dX_real.hpp-templ dx_add_sub_mul_div.hpp dx_func.hpp
	python3 mm.py dX_real.hpp-templ > dX_real.hpp
	emacs --script indent.el dX_real.hpp
	cp dX_real.hpp ..
tX_real.hpp: tX_real.hpp-templ tx_add_sub_mul_div.hpp tx_func.hpp
	python3 mm.py tX_real.hpp-templ > tX_real.hpp
	emacs --script indent.el tX_real.hpp
	cp tX_real.hpp ..
qX_real.hpp: qX_real.hpp-templ qx_add_sub_mul_div.hpp qx_func.hpp 
	python3 mm.py qX_real.hpp-templ > qX_real.hpp
	emacs --script indent.el qX_real.hpp
	cp qX_real.hpp ..

dx_add_sub_mul_div.hpp: mx.py my.py
	python3 mx.py 2 0 >  dx_add_sub_mul_div.hpp
	python3 my.py 2 0 >> dx_add_sub_mul_div.hpp
	python3 mx.py 2 1 >> dx_add_sub_mul_div.hpp
	python3 my.py 2 1 >> dx_add_sub_mul_div.hpp
	python3 mx.py 2 2 >> dx_add_sub_mul_div.hpp
	python3 my.py 2 2 >> dx_add_sub_mul_div.hpp
	python3 mx.py 2 3 >> dx_add_sub_mul_div.hpp
	python3 my.py 2 3 >> dx_add_sub_mul_div.hpp
tx_add_sub_mul_div.hpp: mx.py my.py
	python3 mx.py 3 0 >  tx_add_sub_mul_div.hpp
	python3 my.py 3 0 >> tx_add_sub_mul_div.hpp
	python3 mx.py 3 1 >> tx_add_sub_mul_div.hpp
	python3 my.py 3 1 >> tx_add_sub_mul_div.hpp
	python3 mx.py 3 2 >> tx_add_sub_mul_div.hpp
	python3 my.py 3 2 >> tx_add_sub_mul_div.hpp
	python3 mx.py 3 3 >> tx_add_sub_mul_div.hpp
	python3 my.py 3 3 >> tx_add_sub_mul_div.hpp
qx_add_sub_mul_div.hpp: mx.py my.py
	python3 mx.py 4 0 >  qx_add_sub_mul_div.hpp
	python3 my.py 4 0 >> qx_add_sub_mul_div.hpp
	python3 mx.py 4 1 >> qx_add_sub_mul_div.hpp
	python3 my.py 4 1 >> qx_add_sub_mul_div.hpp
	python3 mx.py 4 2 >> qx_add_sub_mul_div.hpp
	python3 my.py 4 2 >> qx_add_sub_mul_div.hpp
	python3 mx.py 4 3 >> qx_add_sub_mul_div.hpp
	python3 my.py 4 3 >> qx_add_sub_mul_div.hpp

dx_func.hpp: mf.py
	python3 mf.py 2 4 >  dx_func.hpp
	python3 mf.py 2 5 >> dx_func.hpp
	python3 mf.py 2 6 >> dx_func.hpp
	python3 mf.py 2 7 >> dx_func.hpp
	python3 mf.py 2 8 >> dx_func.hpp
tx_func.hpp: mf.py
	python3 mf.py 3 4 >  tx_func.hpp
	python3 mf.py 3 5 >> tx_func.hpp
	python3 mf.py 3 6 >> tx_func.hpp
	python3 mf.py 3 7 >> tx_func.hpp
	python3 mf.py 3 8 >> tx_func.hpp
qx_func.hpp: mf.py
	python3 mf.py 4 4 >  qx_func.hpp
	python3 mf.py 4 5 >> qx_func.hpp
	python3 mf.py 4 6 >> qx_func.hpp
	python3 mf.py 4 7 >> qx_func.hpp
	python3 mf.py 4 8 >> qx_func.hpp


clean:
	\rm *.hpp

