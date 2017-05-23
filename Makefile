SOURCES=$(shell find CPPace/ -name *.cpp -print)
HEADERS=$(shell find CPPace/ -name *.h -print)

mfi.tgz: CPPace/* $(SOURCES) $(HEADERS)
	tar czf $@ $(SOURCES) $(HEADERS) -C CPPace CMakeLists.txt

clean:
	rm -f mfi.tgz
