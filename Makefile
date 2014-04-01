all:
	make -C src all
compile:
	make -C src compile
clean:
	make -C src clean
doc:
	make -C src doc
run:
	$(shell pwd)/udrzavoj

