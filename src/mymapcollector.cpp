#include "mymapcollector.h"

void MyMapCollector::Graph2TimeMap(const FloatRect &box, const TimeSegment &dates, const GuidoElementInfos &infos) {

    RectInfos rectInfos(dates, infos);
    MapElement mapElement = std::pair<FloatRect, RectInfos>(box, rectInfos);

    mapElements.append(mapElement);
}
