#include "guidoservice.h"

#include "GUIDOEngine.h"
#include "GUIDOInternal.h"

#include "ARMusic.h"
#include "ARMusicalVoice.h"

#include "notemapper.h"
#include "score.h"

GuidoService::GuidoService()
{
}

QList<Score> GuidoService::gmnToScores5(const QString &gmn) {
    QList<Score> result;

    ARHandler ar;
    GuidoParseString(gmn.toLatin1(), &ar);
    ARMusic *arMusic = ar->armusic;

    GuidoPos pos = arMusic->GetHeadPosition();
    ARMusicalVoice *voice = arMusic->GetNext(pos);
    while (voice) {

        KF_IPointerList<ARMusicalObject> *objekte = voice->getCopy();

        for (int i=1; i<=objekte->GetCount(); i++) {
            ARMusicalObject *o = objekte->Get(i);
            ARMusicalEvent *e = ARMusicalEvent::cast(o);
            if (e) {
            }
        }

        if (pos)
            voice = arMusic->GetNext(pos);
        else
            break;
    }

    return result;
}

QList<Score> GuidoService::gmnToScores(const QString &gmn) {
    QList<Score> result;

    ARHandler ar;
    GuidoErrCode err = GuidoParseString(gmn.toLatin1(), &ar);
    if (err != guidoNoErr) {
        return result;
    }

    ARMusic *arMusic = ar->armusic;

    GuidoPos pos = arMusic->GetHeadPosition();
    ARMusicalVoice *voice = arMusic->GetNext(pos);
    while (voice) {

        NoteMapper noteMapper(voice);
        voice->browse(noteMapper);

        result.append(noteMapper.getNotes());

        if (pos)
            voice = arMusic->GetNext(pos);
        else
            break;
    }

    return result;
}

Score GuidoService::getScore(const QList<Score>& scores, const MapElement& mapElement) {
    TimeSegment timeSegment = mapElement.second.time();
    Fraction position = Fraction(timeSegment.first.num, timeSegment.first.denom);

    for (Score score : scores) {
        if (score.position == position && score.voice == mapElement.second.infos().voiceNum) {
            return score;
        }
    }

    return Score();
}
