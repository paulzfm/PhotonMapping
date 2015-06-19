SOURCES = $(wildcard *.cpp util/*.cpp util/shape/*.cpp util/light/*.cpp third-party/*.cpp third-party/jsonbox/*.cpp)
HEADERS = $(wildcard *.h util/*.h util/shape/*.h util/light/*.cpp third-party/*.h third-party/jsonbox/*.h)
OBJECTS = $(SOURCES:%.cpp=%.o)
FLAGS   = -std=c++11 -O2 -march=native
TARGET  = rt

all: $(TARGET)

$(TARGET): $(OBJECTS)
	g++ $(OBJECTS) -o $(TARGET)

%.o: %.cpp $(HEADERS)
	g++ -c $(FLAGS) $< -o $@
	
clean:
	rm -rf *.o *~ $(TARGET) util/*.o util/*~ util/shape/*.o util/shape/*~ util/light/*.o util/light/*~ third-party/*.o third-party/*~