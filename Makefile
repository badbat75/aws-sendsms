AWS-SDK-CPP_DIR = /opt/aws-sdk-cpp
AWS-SDK-CPP_MODULES = aws-cpp-sdk-core,aws-cpp-sdk-sns

CXX = g++
CXXFLAGS += $(PKG_CONFIG_CXXFLAGS)
LDFLAGS += $(PKG_CONFIG_LDFLAGS) -L$(AWS-SDK-CPP_DIR)/lib
LD_LIBRARY_PATH := ${LD_LIBRARY_PATH}:$(AWS-SDK-CPP_DIR)/lib
PKG_CONFIG_PATH := ${PKG_CONFIG_PATH}:$(AWS-SDK-CPP_DIR)/lib64/pkgconfig
RM = rm -f


SOURCES = sns_test.cpp
EXECUTABLE = sns_test
OBJS = $(SOURCES:.cpp=.o)

all: dynamic

dynamic: PKG_CONFIG_CXXFLAGS = `PKG_CONFIG_PATH=$(PKG_CONFIG_PATH) pkg-config --cflags $(AWS-SDK-CPP_MODULES)`
dynamic: PKG_CONFIG_LDFLAGS = `PKG_CONFIG_PATH=$(PKG_CONFIG_PATH) pkg-config --libs $(AWS-SDK-CPP_MODULES)`
dynamic: $(EXECUTABLE)

all-static: static $(EXECUTABLE)

static: PKG_CONFIG_CXXFLAGS = `PKG_CONFIG_PATH=$(PKG_CONFIG_PATH) pkg-config --static --cflags $(AWS-SDK-CPP_MODULES)`
static: PKG_CONFIG_LDFLAGS = -static `PKG_CONFIG_PATH=$(PKG_CONFIG_PATH) pkg-config --static --libs $(AWS-SDK-CPP_MODULES)`
static: $(EXECUTABLE)

$(EXECUTABLE): $(OBJS)
	${CXX} ${LDFLAGS} $^ -o $@

$(OBJS): $(SOURCES)
	${CXX} ${CXXFLAGS} -c $^ -o $@

.PHONY: clean
clean:
	$(RM) $(EXECUTABLE) $(OBJS)
