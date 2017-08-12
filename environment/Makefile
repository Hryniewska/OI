# Kacper Kołodziej, 214919
CXX=g++
CXXFLAGS=-std=c++14 -O2 -Wall -Wextra -pedantic \
		 -include cstdint \
		 -Irun/include \
		 -Irun/include/ImageMagick/ \
		 -fopenmp -DMAGICKCORE_HDRI_ENABLE=1 -DMAGICKCORE_QUANTUM_DEPTH=16

LDFLAGS=-Lrun/lib/ \
		-Lrun/lib/ImageMagick/ \
		-lboost_program_options \
		-lMagick++ -lMagickWand -lMagickCore \
		-fopenmp -DMAGICKCORE_HDRI_ENABLE=1 -DMAGICKCORE_QUANTUM_DEPTH=16  \
		-pthread

OBJS=src/check.o \
	 src/test_file.o \
	 src/parse_error.o

TARGET=bin/check

all: $(TARGET)

$(TARGET): $(OBJS)
	@echo -e "[LINK]\t $@"
	@$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET) $(LDFLAGS)

$(OBJS): %.o: %.cpp
	@echo -e "[C++]\t $<"
	@$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	@echo -e "[RM]\t $(OBJS)"
	@rm -f $(OBJS)
	@echo -e "[RM]\t $(TARGET)"
	@rm -f $(TARGET)
