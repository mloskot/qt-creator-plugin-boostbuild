#ifndef BBUTILITY_HPP_INCLUDED
#define BBUTILITY_HPP_INCLUDED
#ifdef _DEBUG

#include "bbprojectmanagerconstants.hpp"
// Qt
#include <QDebug>

#define BBPM_QDEBUG(msg) \
    qDebug().nospace() \
        << "[" << BoostBuildProjectManager::Constants::BOOSTBUILD << "] " \
        << "(" << __PRETTY_FUNCTION__ << ")"; \
    qDebug().nospace() << "\t" << msg

#else

#define BBPM_QDEBUG(msg)

#endif // _DEBUG
#endif // BBUTILITY_HPP_INCLUDED
