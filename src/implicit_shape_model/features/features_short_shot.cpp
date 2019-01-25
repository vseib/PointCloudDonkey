/*
 * BSD 3-Clause License
 *
 * Full text: https://opensource.org/licenses/BSD-3-Clause
 *
 * Copyright (c) 2018, Viktor Seib
 * All rights reserved.
 *
 */

#include "features_short_shot.h"

#define PCL_NO_PRECOMPILE
#include <pcl/features/shot_omp.h>
#include <pcl/common/centroid.h>

namespace ism3d
{
    FeaturesSHORTSHOT::FeaturesSHORTSHOT()
    {
        addParameter(m_radius, "Radius", 0.1);
    }

    FeaturesSHORTSHOT::~FeaturesSHORTSHOT()
    {
    }

    pcl::PointCloud<ISMFeature>::Ptr FeaturesSHORTSHOT::iComputeDescriptors(pcl::PointCloud<PointT>::ConstPtr pointCloud,
                                                                         pcl::PointCloud<pcl::Normal>::ConstPtr normals,
                                                                         pcl::PointCloud<PointT>::ConstPtr pointCloudWithoutNaNNormals,
                                                                         pcl::PointCloud<pcl::Normal>::ConstPtr normalsWithoutNaN,
                                                                         pcl::PointCloud<pcl::ReferenceFrame>::Ptr referenceFrames,
                                                                         pcl::PointCloud<PointT>::Ptr keypoints,
                                                                       pcl::search::Search<PointT>::Ptr search)
    {

        pcl::SHOTEstimationOMP<PointT, pcl::Normal, pcl::SHOT352> shotEst;
        Eigen::Vector4d centroid;

        if (pointCloud->isOrganized()) {
            shotEst.setSearchSurface(pointCloud);
            pcl::PointCloud<pcl::Normal>::Ptr fake_normals(new pcl::PointCloud<pcl::Normal>());
            fake_normals->points.resize(normals->size());
            shotEst.setInputNormals(fake_normals);
            pcl::compute3DCentroid(*pointCloud, centroid);
        }
        else {
            shotEst.setSearchSurface(pointCloudWithoutNaNNormals);
            pcl::PointCloud<pcl::Normal>::Ptr fake_normals(new pcl::PointCloud<pcl::Normal>());
            fake_normals->points.resize(normalsWithoutNaN->size());
            shotEst.setInputNormals(fake_normals);
            pcl::compute3DCentroid(*pointCloudWithoutNaNNormals, centroid);
        }

        shotEst.setInputCloud(keypoints);
        shotEst.setInputReferenceFrames(referenceFrames);
        shotEst.setSearchMethod(search);

        // parameters
        shotEst.setRadiusSearch(m_radius);

        // compute features
        pcl::PointCloud<pcl::SHOT352>::Ptr shotFeatures(new pcl::PointCloud<pcl::SHOT352>());
        shotEst.compute(*shotFeatures);

        // create descriptor point cloud
        pcl::PointCloud<ISMFeature>::Ptr features(new pcl::PointCloud<ISMFeature>());
        features->resize(shotFeatures->size());

        for (int i = 0; i < (int)shotFeatures->size(); i++)
        {
            ISMFeature& feature = features->at(i);
            const pcl::SHOT352& shot = shotFeatures->at(i);

            // store the descriptor: only one value per histogram
            feature.descriptor.resize(32);
            for (int j = 0; j < feature.descriptor.size(); j++)
                feature.descriptor[j] = shot.descriptor[5+j*11];

            // store distance to centroid
            PointT keyp = keypoints->at(i);
            feature.centerDist = (Eigen::Vector3d(keyp.x, keyp.y, keyp.z)-Eigen::Vector3d(centroid.x(), centroid.y(), centroid.z())).norm();
        }

        return features;
    }

    std::string FeaturesSHORTSHOT::getTypeStatic()
    {
        return "SHORT_SHOT";
    }

    std::string FeaturesSHORTSHOT::getType() const
    {
        return FeaturesSHORTSHOT::getTypeStatic();
    }
}
