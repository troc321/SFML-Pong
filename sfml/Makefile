INCLUDE_SFML = -lsfml-graphics -lsfml-window -lsfml-system -I/usr/include/SFML/
INCLUDE_SFML_EXTRAS = -lsfml-audio -lsfml-network
INCLUDE_GL = -lGL -lGLU -lglut -I/usr/include/GL/

.PHONY: all clean

p3ng: p3ng.cpp
	g++ -o p3ng p3ng.cpp $(INCLUDE_SFML) $(INCLUDE_SFML_EXTRAS)

clean :
	rm -rf p3ng *.o
