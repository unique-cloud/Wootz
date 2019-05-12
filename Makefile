all:
	g++ -g -o main main.cc Grammar.cc Layer.cc Network.cc Parser.cc Scanner.cc TFGenerator.cc TFNode.cc Token.cc Util.cc -I.

clean:
	rm main
