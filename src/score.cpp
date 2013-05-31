#include "score.h"

#include "events.h"

Score::Score(char pitch) : pitch(pitch), finger(UNKNOWN_FINGER), hand(UNKNOWN_HAND), status(UNKNOWN_STATUS)
{
}
