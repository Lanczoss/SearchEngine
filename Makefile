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

01_testReadFiles.exe: ./test/01_testReadFiles.o ./src/DictProducer.o
	g++ $^ -o ./bin/$@ $(LIBS) $(addprefix -I, $(INCLUDES)) -g

02_testJieba.exe: ./test/02_jiebaSplitCnEn.o
	g++ $^ -o ./bin/$@ $(LIBS) $(addprefix -I, $(INCLUDES)) -g

03_cleanCn.exe: ./test/03_cleanCnString.o ./src/DictProducer.o ./src/SplitToolCppJieba.o ./src/Configuration.o ./src/SplitTool.cc
	g++ $^ -o ./bin/$@ $(LIBS) $(addprefix -I, $(INCLUDES)) -g

04_opendir.exe: ./test/04_openDir.o
	g++ $^ -o ./bin/$@ $(LIBS) $(addprefix -I, $(INCLUDES)) -g

05_config.exe: ./test/05_config.o ./src/Configuration.o
	g++ $^ -o ./bin/$@ $(LIBS) $(addprefix -I, $(INCLUDES)) -g

07_readDictAndIndex.exe: ./test/07_readDictAndIndex.o ./src/Configuration.o ./src/DictProducer.o ./src/Dictionary.o ./src/SplitToolCppJieba.o ./src/SplitTool.o
	g++ $^ -o ./bin/$@ $(LIBS) $(addprefix -I, $(INCLUDES)) -g

08_unionSet.exe: ./test/08_unionSet.o ./src/Configuration.o ./src/DictProducer.o ./src/Dictionary.o ./src/SplitToolCppJieba.o ./src/SplitTool.o ./src/CandidateResult.o ./src/KeyRecommander.o
	g++ $^ -o ./bin/$@ $(LIBS) $(addprefix -I, $(INCLUDES)) -g

09_minEditDist.exe: ./test/09_minEditDist.o
	g++ $^ -o ./bin/$@ $(LIBS) $(addprefix -I, $(INCLUDES)) -g

echo:
	echo $(INCLUDES)
	echo $(SRCS)

clean:
	rm -rf $(OBJS) $(SERVER)
