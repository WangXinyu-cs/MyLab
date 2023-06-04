all:
	@if [ ! -d bin ]; then mkdir bin; fi
	cd src; make all
	cd test; make all

clean:
	cd test; make clean
	cd src; make clean
