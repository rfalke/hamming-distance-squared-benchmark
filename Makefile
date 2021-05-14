all:
	cd c-metric-tree && make
	cd c-simple && make
	cd c-unroll && make
	cd c-unroll2 && make

clean:
	cd c-metric-tree && make clean
	cd c-simple && make clean
	cd c-unroll && make clean
	cd c-unroll2 && make clean
