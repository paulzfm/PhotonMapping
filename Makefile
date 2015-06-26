CXX = clang-omp++
SOURCES = $(wildcard *.cpp util/*.cpp util/shape/*.cpp third-party/*.cpp third-party/jsonbox/*.cpp third-party/objparser/*.cpp)
HEADERS = $(wildcard *.h util/*.h util/shape/*.h third-party/*.h third-party/jsonbox/*.h third-party/objparser*.h)
OBJECTS = $(SOURCES:%.cpp=%.o)
CFLAGS  = -std=c++11 -O2 -march=native -fopenmp
LFLAGS  = -O2 -march=native -fopenmp
TARGET  = rt

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(LFLAGS) $(OBJECTS) -o $(TARGET)

%.o: %.cpp $(HEADERS)
	$(CXX) -c $(CFLAGS) $< -o $@
	
clean:
	rm -rf *.o *~ $(TARGET) util/*.o util/*~ util/shape/*.o util/shape/*~ third-party/*.o third-party/*~ third-party/jsonbox/*~ third-party/jsonbox/*.o