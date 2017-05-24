SOURCES=$(shell find CPPace/ -name *.cpp -print)
HEADERS=$(shell find CPPace/ -name *.h -print)

mfi.tgz: CPPace/* $(SOURCES) $(HEADERS)
	tar czf $@ $(SOURCES) $(HEADERS) -C CPPace CMakeLists.txt

build: mfi.tgz
	rm -rf mfi/
	mkdir mfi
	tar -xf mfi.tgz -C mfi/
	cd mfi; cmake CMakeLists.txt; make

clean:
	rm -f mfi.tgz
	rm -rf mfi/
