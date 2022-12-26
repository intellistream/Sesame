// Copyright (C) 2021 by the IntelliStream team
// (https://github.com/intellistream)

//
// Created by tuidan on 2021/7/22.
//

#include "Algorithm/Algorithm.hpp"

namespace SESAME
{
char const *algo_names[64] = {[BirchType]        = "Birch",
                              [StreamKMeansType] = "StreamKMeans",
                              [CluStreamType]    = "CluStream",
                              [DenStreamType]    = "DenStream",
                              [DBStreamType]     = "DBStream",
                              [EDMStreamType]    = "EDMStream",
                              [DStreamType]      = "DStream",
                              [SLKMeansType]     = "SLKMeans",
                              [8]                = "ERROR8",
                              [9]                = "ERROR9",
                              [10]               = "ERROR10",
                              [V1Stream]         = "V1",
                              [V2Stream]         = "V2",
                              [V3Stream]         = "V3",
                              [V4Stream]         = "V4",
                              [V5Stream]         = "V5",
                              [V6Stream]         = "V6",
                              [V7Stream]         = "V7",
                              [V8Stream]         = "V8",
                              [V9Stream]         = "V9",
                              [Generic]          = "Generic",
                              [G1Stream]         = "G1",
                              [G2Stream]         = "G2",
                              [G3Stream]         = "G3",
                              [G4Stream]         = "G4",
                              [G5Stream]         = "G5",
                              [G6Stream]         = "G6",
                              [G7Stream]         = "G7",
                              [G8Stream]         = "G8",
                              [G9Stream]         = "G9",
                              [V10Stream]        = "G10",
                              [G11Stream]        = "G11",
                              [G12Stream]        = "G12",
                              [G13Stream]        = "G13"};

void Algorithm::Store(std::string output_file, int dim, std::vector<PointPtr> result)
{
    int numberOfCenters = (int)result.size();
    FILE *out           = fopen(output_file.c_str(), "w");
    for (int i = 0; i < numberOfCenters; i++)
    {
        int l;
        fprintf(out, "%f ", result[i]->getWeight());
        for (l = 0; l < dim; l++)
        {
            fprintf(out, "%f ", result[i]->getFeatureItem(l) / result[i]->getWeight());
        }
        fprintf(out, "\n");
    }
    fclose(out);
}

}  // namespace SESAME
