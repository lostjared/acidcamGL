CXXFLAGS = -std=c++11 `pkg-config rtaudio --cflags`
LDFLAGS = `pkg-config rtaudio --libs`
CPP_FILES := $(wildcard *.cpp)
OBJ_FILES := $(CPP_FILES:.cpp=.o)
OUTPUT_NAME = rt_devices
DEPS := $(wildcard *.hpp)

%.o: %.cpp $(DEPS) 
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OUTPUT_NAME): $(OBJ_FILES)
	$(CXX)  $^ -o $@ $(LDFLAGS)

all: $(OUTPUT_NAME)

install: all
	cp $(OUTPUT_NAME) /usr/local/bin

clean:
	rm -f $(OUTPUT_NAME) $(OBJ_FILES)

.PHONY: all install clean
