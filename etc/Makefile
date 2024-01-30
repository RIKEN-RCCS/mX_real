
all: dX_real.hpp tX_real.hpp qX_real.hpp

dX_real.hpp: mX_real.hpp-templ dx_add_sub_mul_div.hpp dx_func.hpp dx_bits.hpp dx_members.hpp dx_comp.hpp dx_uni.hpp
	python3 mm.py mX_real.hpp-templ 2 > dX_real.hpp
	emacs --script indent.el dX_real.hpp
	awk '/_LV_/{ gsub(/)_LV_/,")\\&");}{ print }' dX_real.hpp > dX_real.hpp-
	mv dX_real.hpp- dX_real.hpp
	cp dX_real.hpp ..
tX_real.hpp: mX_real.hpp-templ tx_add_sub_mul_div.hpp tx_func.hpp tx_bits.hpp tx_members.hpp tx_comp.hpp tx_uni.hpp
	python3 mm.py mX_real.hpp-templ 3 > tX_real.hpp
	emacs --script indent.el tX_real.hpp
	awk '/_LV_/{ gsub(/)_LV_/,")\\&");}{ print }' tX_real.hpp > tX_real.hpp-
	mv tX_real.hpp- tX_real.hpp
	cp tX_real.hpp ..
qX_real.hpp: mX_real.hpp-templ qx_add_sub_mul_div.hpp qx_func.hpp qx_bits.hpp qx_members.hpp qx_comp.hpp qx_uni.hpp
	python3 mm.py mX_real.hpp-templ 4 > qX_real.hpp
	emacs --script indent.el qX_real.hpp
	awk '/_LV_/{ gsub(/)_LV_/,")\\&");}{ print }' qX_real.hpp > qX_real.hpp-
	mv qX_real.hpp- qX_real.hpp
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

dx_bits.hpp: mf.py
	python3 mf.py 2 9 > dx_bits.hpp
tx_bits.hpp: mf.py
	python3 mf.py 3 9 > tx_bits.hpp
qx_bits.hpp: mf.py
	python3 mf.py 4 9 > qx_bits.hpp

dx_members.hpp: mf.py
	python3 mf.py 2 10 > dx_members.hpp
tx_members.hpp: mf.py
	python3 mf.py 3 10 > tx_members.hpp
qx_members.hpp: mf.py
	python3 mf.py 4 10 > qx_members.hpp

dx_comp.hpp: mc.py
	python3 mc.py 2 > dx_comp.hpp
tx_comp.hpp: mc.py
	python3 mc.py 3 > tx_comp.hpp
qx_comp.hpp: mc.py
	python3 mc.py 4 > qx_comp.hpp

dx_uni.hpp: mx.py
	python3 mx.py 2 5 > dx_uni.hpp
tx_uni.hpp: mx.py
	python3 mx.py 3 5 > tx_uni.hpp
qx_uni.hpp: mx.py
	python3 mx.py 4 5 > qx_uni.hpp


clean:
	-\rm *.hpp
distclean:
	-make clean
	-\rm -rf mpreal
	-\rm qd-2.3.24.tar.gz
	-\rm -rf qd-2.3.24
	-\rm qd_real

