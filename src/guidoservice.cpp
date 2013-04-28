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

QList<Score> GuidoService::gmnToScores(const QString &gmn) {
    QList<Score> result;

    ARHandler ar;
    GuidoParseString(gmn.toLatin1(), &ar);
    ARMusic *arMusic = ar->armusic;

    GuidoPos pos = arMusic->GetHeadPosition();
    ARMusicalVoice *voice = arMusic->GetNext(pos);
    while (voice) {

        NoteMapper noteMapper(voice);
        voice->browse(noteMapper);

        result = noteMapper.getNotes();

        if (pos)
            voice = arMusic->GetNext(pos);
        else
            break;
    }

    return result;
}
