#include "sample/wordcount/SplitSentenceBolt.h"
#include "hurricane/util/StringUtil.h"
#include <sys/time.h>

void SplitSentenceBolt::Prepare(std::shared_ptr<hurricane::collector::OutputCollector> outputCollector) {
	_outputCollector = outputCollector;
    _logFile.open("timestamp.txt");
}

void SplitSentenceBolt::Cleanup() {
    _logFile.close();
}

std::vector<std::string> SplitSentenceBolt::DeclareFields() {
	return{ "word" };
}

void SplitSentenceBolt::Execute(const hurricane::base::Tuple& tuple) {
    std::string sentence = tuple[0].GetStringValue();
	std::vector<std::string> words = SplitString(sentence, ' ');

    int64_t sourceMicroseconds = tuple[1].GetInt64Value();

    for ( const std::string& word : words ) {
        std::cout << word << std::endl;
        _outputCollector->Emit({ word });
    }

    timeval currentTime;
    gettimeofday(&currentTime, nullptr);

    int64_t currentMicroseconds = currentTime.tv_sec;
    currentMicroseconds *= 1000000;
    currentMicroseconds += currentTime.tv_usec;

    _logFile << sourceMicroseconds << ' ' << currentMicroseconds << std::endl;
}