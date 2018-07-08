CXX = g++ -std=c++14 -g


menu: menu.cpp
	$(CXX) $^ -o $@

%.o: %.cpp
	$(CXX) -c $<


clean:
	$(RM) menu
	$(RM) *.o
