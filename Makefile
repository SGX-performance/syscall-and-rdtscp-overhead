CXXFLAGS =	-O2 -g -Wall -fmessage-length=0

OBJS =		syscall_and_rdtscp_overhead.o

LIBS =

TARGET =	syscall_and_rdtscp_overhead

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
