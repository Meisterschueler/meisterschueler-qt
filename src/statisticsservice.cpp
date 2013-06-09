#include "statisticsservice.h"

#include <cmath>

#include "kiss_fft.h"
#include "tools/kiss_fftr.h"

#include "score.h"
#include "util.h"

StatisticsService::StatisticsService()
{
}

StatisticItem StatisticsService::getStatisticItem(const QVector<double>& values) {
    StatisticItem result;

    double min = values.at(0);
    double max = values.at(0);
    double sum = 0.0;
    double squaresum = 0.0;

    for (double value : values) {
        min = MIN2(min, value);
        max = MAX2(max, value);
        sum += value;
        squaresum += value*value;
    }

    int N = values.size();
    double mean = sum/N;
    double variance = squaresum/N - mean*mean;

    int nfft=N;
    kiss_fftr_cfg  kiss_fftr_state;
    kiss_fft_scalar rin[nfft+2];
    kiss_fft_cpx sout[nfft];

    for (int i = 0; i < nfft; ++i) {
        rin[i] = values.at(i);
    }

    kiss_fftr_state = kiss_fftr_alloc(nfft,0,0,0);
    kiss_fftr(kiss_fftr_state,rin,sout);

    QVector<double> spectrum(nfft);
    for (int i = 0; i < nfft; ++i) {
        spectrum[i]= sqrt(sout[i].r*sout[i].r +sout[i].i*sout[i].i);
    }

    result.min = min;
    result.max = max;
    result.mean = mean;
    result.variance = variance;
    result.standarddeviation = sqrt(variance);
    result.spectrum = spectrum;

    return result;
}

StatisticCluster StatisticsService::getStatisticCluster(const QList<Score>& scores) {
    StatisticCluster result;

    QVector<double> velocities;
    for (Score score : scores) {
        velocities.append((*score.midiPair.noteOn).getVelocity());
    }
    result.velocity = getStatisticItem(velocities);

    QVector<double> speeds;
    if (scores.size() >= 2) {
        double delta_t = (*scores.at(1).midiPair.noteOn).getTime()-(*scores.at(0).midiPair.noteOn).getTime();
        double speed_bpm = (double)scores.at(0).duration/delta_t * 4 * 1000 * 60;
        speeds.append(speed_bpm);

        for (int i = 1; i < scores.size()-1; ++i) {
            delta_t = (*scores.at(i+1).midiPair.noteOn).getTime()-(*scores.at(i).midiPair.noteOn).getTime();
            speed_bpm = (double)scores.at(i).duration/delta_t * 4 * 1000 * 60;
            speeds.append(speed_bpm);
        }

        delta_t = (*scores.last().midiPair.noteOff).getTime()-(*scores.last().midiPair.noteOn).getTime();
        speed_bpm = (double)scores.last().duration/delta_t * 4 * 1000 * 60;
        speeds.append(speed_bpm);

        result.speed = getStatisticItem(speeds);
    }

    return result;
}
