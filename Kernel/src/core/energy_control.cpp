#include <siberix/core/power.h>

EnergyPolicyEngine::EnergyPolicyEngine()
    : m_sleepAfter(Duration(TimeSpan::Minute, 5)) {}

EnergyPolicyEngine::~EnergyPolicyEngine() {}