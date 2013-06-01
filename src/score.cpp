#include "score.h"

#include "events.h"

Score::Score(char pitch, Status status) : pitch(pitch), status(status), finger(UNKNOWN_FINGER), hand(UNKNOWN_HAND)
{
}
