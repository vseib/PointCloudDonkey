/*
 * BSD 3-Clause License
 *
 * Full text: https://opensource.org/licenses/BSD-3-Clause
 *
 * Copyright (c) 2018, Viktor Seib
 * All rights reserved.
 *
 */

#ifndef ISM3D_FEATURESOURCVFH_H
#define ISM3D_FEATURESOURCVFH_H

#include "features.h"

namespace ism3d
{
    /**
     * @brief The FeaturesOURCVFH class
     * Computes features using the Oriented, Unique and Repeatable Clustered Viewpoint Feature Histogram descriptor.
     */
    class FeaturesOURCVFH
            : public Features
    {
    public:
        FeaturesOURCVFH();
        ~FeaturesOURCVFH();

        static std::string getTypeStatic();
        std::string getType() const;

    protected:
        pcl::PointCloud<ISMFeature>::Ptr iComputeDescriptors(pcl::PointCloud<PointT>::ConstPtr,
                                                             pcl::PointCloud<pcl::Normal>::ConstPtr,
                                                             pcl::PointCloud<PointT>::ConstPtr,
                                                             pcl::PointCloud<pcl::Normal>::ConstPtr,
                                                             pcl::PointCloud<pcl::ReferenceFrame>::Ptr,
                                                             pcl::PointCloud<PointT>::Ptr,
                                                             pcl::search::Search<PointT>::Ptr);

    private:
    };
}

#endif // ISM3D_FEATURESOURCVFH_H
