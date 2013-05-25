#include "statisticsservice.h"

#include <cmath>

#include "kiss_fft.h"
#include "tools/kiss_fftr.h"

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
    return result;
}
