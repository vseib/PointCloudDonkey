/*
 * BSD 3-Clause License
 *
 * Full text: https://opensource.org/licenses/BSD-3-Clause
 *
 * Copyright (c) 2018, Viktor Seib
 * All rights reserved.
 *
 */

#ifndef ISM3D_FEATURESBSHOT_H
#define ISM3D_FEATURESBSHOT_H

#include "features.h"

namespace ism3d
{
    /**
     * @brief The FeaturesBSHOT class
     * Computes features using the binary signature of histograms of orientations descriptor.
     */
    class FeaturesBSHOT
            : public Features
    {
    public:
        FeaturesBSHOT();
        ~FeaturesBSHOT();

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

        std::vector<int> getBinaryVector(const std::vector<float> &vec);

        double m_radius;
    };
}

#endif // ISM3D_FEATURESBSHOT_H
