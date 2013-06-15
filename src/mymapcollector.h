#ifndef MYMAPCOLLECTOR_H
#define MYMAPCOLLECTOR_H

class MyMapCollector : public MapCollector
{
public:
    virtual void Graph2TimeMap( const FloatRect& box, const TimeSegment& dates, const GuidoElementInfos& infos );
    QList<MapElement> mapElements;
};


#endif // MYMAPCOLLECTOR_H
