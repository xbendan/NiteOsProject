#include <siberix/core/power.h>

PowerEngine::PowerEngine()
    : m_sleepAfter(Duration(TimeSpan::Minute, 5)) {}

PowerEngine::~PowerEngine() {}