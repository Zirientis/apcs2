#pragma once

enum ActionCode
{
    AC_NONE,
    AC_MOVE_FAIL,
    AC_PLACE_FAIL,
    AC_STAIR_TRIGGERED,
    AC_MAX
};

inline bool ActionCodeSucceeded(ActionCode a)
{
    if (a == AC_MOVE_FAIL || a == AC_PLACE_FAIL)
        return false;
    return true;
}