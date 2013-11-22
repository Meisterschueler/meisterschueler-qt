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

    int nfft = qMax(2.0, pow(2, ceil(log(N)/log(2))));

    kiss_fftr_cfg  kiss_fftr_state;
    kiss_fft_scalar rin[nfft+2];
    kiss_fft_cpx sout[nfft];

    for (int i = 0; i < N; ++i) {
        rin[i] = values.at(i);
    }

    kiss_fftr_state = kiss_fftr_alloc(nfft,0,0,0);
    kiss_fftr(kiss_fftr_state,rin,sout);

    QVector<double> spectrum(nfft);
    for (int i = 0; i < nfft; ++i) {
        spectrum[i]= sqrt(sout[i].r*sout[i].r +sout[i].i*sout[i].i);
    }

    result.values = values;
    result.min = min;
    result.max = max;
    result.mean = mean;
    result.variance = variance;
    result.standarddeviation = sqrt(variance);
    result.spectrum = spectrum;

    return result;
}

StatisticCluster StatisticsService::getStatisticCluster(const QList<MidiPair>& midiPairs, const Fraction &fraction) {
    QVector<double> velocities;
    QVector<double> speeds;
    QVector<double> overlap;
    QVector<double> acceleration;

    for (MidiPair midiPair : midiPairs ) {
        velocities.append(midiPair.noteOn.getVelocity());
    }

    // forward looking
    for (int i = 1; i < midiPairs.count(); ++i) {
        speeds.append((60.0 * 1000.0)*(((double)Frac_1_4)/((double)fraction))/(midiPairs.at(i).noteOn.getTime()-midiPairs.at(i-1).noteOn.getTime()));
    }
    if (midiPairs.count() > 1) {
        speeds.append(speeds.last());
    } else {
        speeds.append(60.0);
    }

    // backward looking
    for (int i = 0; i < midiPairs.count()-1; ++i) {
        if (midiPairs.at(i).noteOff != emptyNoteOffEvent && midiPairs.at(i+1).noteOn != emptyNoteOnEvent) {
            overlap.append(midiPairs.at(i+1).noteOn.getTime()-midiPairs.at(i).noteOff.getTime());
        } else {
            overlap.append(0.0);
        }
    }
    if (midiPairs.count() > 1) {
        overlap.insert(0, overlap.first());
    } else {
        overlap.insert(0, 0.0);
    }

    // forward and backward looking
    for (int i = 1; i < midiPairs.count()-1; ++i) {
        acceleration.append(2.0*(60.0 * 1000.0)*(((double)Frac_1_4)/((double)fraction))/((midiPairs.at(i).noteOn.getTime()-midiPairs.at(i-1).noteOn.getTime())*(midiPairs.at(i+1).noteOn.getTime()-midiPairs.at(i).noteOn.getTime())));
    }
    if (midiPairs.count() > 2) {
        acceleration.append(acceleration.last());
        acceleration.insert(0, acceleration.first());
    } else {
        acceleration.append(0.0);
        acceleration.append(0.0);
    }

    StatisticCluster result;
    result.speed = getStatisticItem(speeds);
    result.velocity = getStatisticItem(velocities);
    result.overlap = getStatisticItem(overlap);
    result.acceleration = getStatisticItem(acceleration);

    QVector<double> velocitiesLastMeasure = velocities;
    QVector<double> speedsLastMeasure = speeds;
    QVector<double> overlapLastMeasure = speeds;
    QVector<double> accelerationLastMeasure = acceleration;

    int eventsPerMeasure = 1.0/((double)fraction);
    if (velocitiesLastMeasure.count() > eventsPerMeasure) {
        velocitiesLastMeasure = velocitiesLastMeasure.mid(velocitiesLastMeasure.count()-(eventsPerMeasure+1));
        speedsLastMeasure = speedsLastMeasure.mid(speedsLastMeasure.count()-(eventsPerMeasure+1));
        overlapLastMeasure = overlapLastMeasure.mid(overlapLastMeasure.count()-(eventsPerMeasure+1));
        accelerationLastMeasure = accelerationLastMeasure.mid(accelerationLastMeasure.count()-(eventsPerMeasure+1));
    }
    result.velocityLastMeasure = getStatisticItem(velocitiesLastMeasure);
    result.speedLastMeasure = getStatisticItem(speedsLastMeasure);
    result.overlapLastMeasure = getStatisticItem(overlapLastMeasure);
    result.accelerationLastMeasure = getStatisticItem(accelerationLastMeasure);

    return result;
}

StatisticCluster StatisticsService::getStatisticCluster(const QList<MidiPairCluster>& mpc, const Fraction& fraction) {
    StatisticCluster result;

    return result;
}

StatisticCluster StatisticsService::getStatisticCluster(const QList<Score>& scores) {
    StatisticCluster result;

    QVector<double> velocities;
    for (Score score : scores) {
        velocities.append(score.midiPair.noteOn.getVelocity());
    }
    result.velocity = getStatisticItem(velocities);

    if (scores.size() >= 2) {
        QVector<double> speeds;

        for (int i = 0; i < scores.count()-1; ++i) {
            if (scores.at(i).status == Status::PLAYED || scores.at(i).status == Status::FAILED) {
                // find next played or wrong note
                Fraction duration = scores.at(i).duration;
                for (int j = i+1; j < scores.count(); ++j) {
                    if (scores.at(j).status == Status::PLAYED || scores.at(j).status == Status::FAILED) {
                        double delta_t = scores.at(j).midiPair.noteOn.getTime()-scores.at(i).midiPair.noteOn.getTime();
                        double speed_bpm = (double)duration/delta_t * 4 * 1000 * 60;
                        speeds.append(speed_bpm);
                        break;
                    } else if (scores.at(j).status == Status::MISSED) {
                        duration += scores.at(j).duration;
                    }
                }
            }
        }

        if ((scores.last().status == Status::PLAYED || scores.last().status == Status::FAILED) && (scores.last().midiPair.noteOn != emptyNoteOnEvent && scores.last().midiPair.noteOff != emptyNoteOffEvent)) {
            double delta_t = scores.last().midiPair.noteOff.getTime()-scores.last().midiPair.noteOn.getTime();
            double speed_bpm = (double)scores.last().duration/delta_t * 4 * 1000 * 60;
            speeds.append(speed_bpm);
        }

        if (speeds.count() >= 1) {
            result.speed = getStatisticItem(speeds);
        }
    }

    return result;
}
