all: qxw.hpp sqrt.hpp

#qxw.hpp: all_functions.c a.awk
#	tr -d \\r < all_functions.c | awk -f a.awk > qxw.hpp
qxw.hpp: gen_code.py
	python3 gen_code.py > qxw.hpp
sqrt.hpp : s.py
	python3 s.py > sqrt.hpp

clean:
	\rm qxw.hpp sqrt.hpp

