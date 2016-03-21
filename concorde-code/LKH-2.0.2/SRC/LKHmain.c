#include "LKH.h"

/*
 * This file contains the main function of the program.
 */

int main(int argc, char *argv[])
{
    GainType Cost;
    double Time, LastTime = GetTime();

    /* Read the specification of the problem */
    if (argc >= 2)
        ParameterFileName = argv[1];
    ReadParameters();
    MaxMatrixDimension = 10000;
    ReadProblem();

    if (SubproblemSize > 0) {
        if (DelaunayPartitioning)
            SolveDelaunaySubproblems();
        else if (KarpPartitioning)
            SolveKarpSubproblems();
        else if (KMeansPartitioning)
            SolveKMeansSubproblems();
        else if (RohePartitioning)
            SolveRoheSubproblems();
        else if (SierpinskiPartitioning || MoorePartitioning)
            SolveSFCSubproblems();
        else
            SolveTourSegmentSubproblems();
        return EXIT_SUCCESS;
    }
    AllocateStructures();
    CreateCandidateSet();
    InitializeStatistics();

    if (Norm != 0)
        BestCost = PLUS_INFINITY;
    else {
        /* The ascent has solved the problem! */
        Optimum = BestCost = (GainType) LowerBound;
        UpdateStatistics(Optimum, GetTime() - LastTime);
        RecordBetterTour();
        RecordBestTour();
        WriteTour(OutputTourFileName, BestTour, BestCost);
        WriteTour(TourFileName, BestTour, BestCost);
        Runs = 0;
    }

    /* Find a specified number (Runs) of local optima */
    for (Run = 1; Run <= Runs; Run++) {
        LastTime = GetTime();
        Cost = FindTour();      /* using the Lin-Kernighan heuristic */
        if (Run > 1) {
            Cost = MergeBetterTourWithBestTour();
            if (Cost < BestCost)
                RecordBetterTour();
        }
        if (Cost < BestCost) {
            BestCost = Cost;
            RecordBestTour();
            if (MaxTrials == 0)
                WriteTour(OutputTourFileName, BestTour, BestCost);
            WriteTour(TourFileName, BestTour, BestCost);
        }
        if (Cost < Optimum) {
            if (FirstNode->InputSuc) {
                Node *N = FirstNode;
                while ((N = N->InputSuc = N->Suc) != FirstNode);
            }
            Optimum = Cost;
            printff("*** New optimum = " GainFormat " ***\n\n", Optimum);
        }
        Time = fabs(GetTime() - LastTime);
        UpdateStatistics(Cost, Time);
        if (TraceLevel >= 1 && Cost != PLUS_INFINITY) {
            printff("Run %d: Cost = " GainFormat, Run, Cost);
            if (Optimum != MINUS_INFINITY && Optimum != 0)
                printff(", Gap = %0.3f%%",
                        100.0 * (Cost - Optimum) / Optimum);
            printff(", Time = %0.1f sec. %s\n\n", Time,
                    Cost < Optimum ? "<" : Cost == Optimum ? "=" : "");
        }
        SRandom(++Seed);
    }
    PrintStatistics();
    return EXIT_SUCCESS;
}
