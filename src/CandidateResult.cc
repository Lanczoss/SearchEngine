#include "CandidateResult.h"

CandidateResult::CandidateResult(const string& word, int frequency = 0,
                                 int distance = 0)
    : _word(word), _freq(frequency), _dist(distance) {}
