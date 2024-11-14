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

testJieba: ./test/02_jiebaSplitCnEn.o
	g++ $^ -o ./bin/$@ $(LIBS) $(addprefix -I, $(INCLUDES)) -g

cleanCn: ./test/03_cleanCnString.o ./src/DictProducer.o ./src/SplitToolCppJieba.o
	g++ $^ -o ./bin/$@ $(LIBS) $(addprefix -I, $(INCLUDES)) -g

opendir: ./test/04_openDir.o
	g++ $^ -o ./bin/$@ $(LIBS) $(addprefix -I, $(INCLUDES)) -g

echo:
	echo $(INCLUDES)
	echo $(SRCS)

clean:
	rm -rf $(OBJS) $(SERVER)
