INCLUDES:=./  ./include
SRCS:=$(wildcard ./src/*.cc) 
LIBS:=  -lpthread
OBJS:=$(patsubst %.cc, %.o, $(SRCS))
SERVER:= SearchEngine

# 目标 : 依赖
$(SERVER): &(OBJS)
	g++ $^ -o $@ $(LIBS) $(addprefix -I, $(INCLUDES)) -g

%.o:%.cc
	g++ -c $^ -o $@  $(addprefix -I, $(INCLUDES)) -g

testReadFiles: ./test/01_testReadFiles.o ./src/DictProducer.o
	g++ $^ -o ./bin/$@ $(LIBS) $(addprefix -I, $(INCLUDES)) -g

echo:
	echo $(INCLUDES)
	echo $(SRCS)

clean:
	rm -rf $(OBJS) $(SERVER)
