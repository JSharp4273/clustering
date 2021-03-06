/*
 * Software License Agreement (BSD License)
 *
 *  Copyright (c) 2017, Julien FLEURET.
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *   * Neither the name of the copyright holder nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 *  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 *  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 *
 */

// Created May 2017.
// Author Julien FLEURET, julien.fleuret.1@ulaval.ca

// This code is a direct derivation of the OpenCV's kmeans sample file.
// It has been modify in order to evaluate the dbscan algorithm


#ifndef TEST_CLUSTERING_CPP
#define TEST_CLUSTERING_CPP

#include "tests.h"
#include "clustering.h"

#include <opencv2/core.hpp>

void test_clustering()
{

    const int MAX_CLUSTERS = 5;
//       cv::Scalar colorTab[] =
//       {
//           cv::Scalar(0, 0, 255),
//           cv::Scalar(0,255,0),
//           cv::Scalar(255,100,100),
//           cv::Scalar(255,0,255),
//           cv::Scalar(0,255,255)
//       };

    std::vector<cv::Scalar> _colorTab(1024);
    cv::Scalar* colorTab = _colorTab.data();

    for(int i=0;i<1024;i++)
    {
        cv::Scalar color(cv::theRNG().uniform(0,255), cv::theRNG().uniform(0,255), cv::theRNG().uniform(0,255));

        colorTab[i] = color;
    }

       cv::Mat img(500, 500, CV_8UC3);
       cv::RNG rng(12345);

       for(;;)
       {
           int k, clusterCount = rng.uniform(2, MAX_CLUSTERS+1);
           int i, sampleCount = rng.uniform(100, 1001);
           cv::Mat points(sampleCount, 1, CV_32FC2), labels;

           clusterCount = MIN(clusterCount, sampleCount);
           cv::Mat centers;

           /* generate random sample from multigaussian distribution */
           for( k = 0; k < clusterCount; k++ )
           {
               cv::Point center;
               center.x = rng.uniform(0, img.cols);
               center.y = rng.uniform(0, img.rows);
               cv::Mat pointChunk = points.rowRange(k*sampleCount/clusterCount,
                                                k == clusterCount - 1 ? sampleCount :
                                                (k+1)*sampleCount/clusterCount);
               rng.fill(pointChunk, cv::RNG::NORMAL, cv::Scalar(center.x, center.y), cv::Scalar(img.cols*0.05, img.rows*0.05));
           }

           cv::randShuffle(points, 1, &rng);

//           double compactness = cv::kmeans(points, clusterCount, labels,
//               cv::TermCriteria( cv::TermCriteria::EPS+cv::TermCriteria::COUNT, 10, 1.0),
//                  3, cv::KMEANS_PP_CENTERS, centers);

//           double compactness = clustering::gkmeans(points, clusterCount, labels,
//               cv::TermCriteria( cv::TermCriteria::EPS+cv::TermCriteria::COUNT, 10, 1.0),
//                  3, cv::KMEANS_PP_CENTERS, centers);

//           double compactness = (double)cv::theRNG();

           double compactness = clustering::dbscan(points, rng.uniform(25,45), rng.uniform(1,10), labels, centers);

//           std::cout<<centers<<std::endl;
//           std::cout<<labels<<std::endl;

           img = cv::Scalar::all(0);

           for( i = 0; i < sampleCount; i++ )
           {
               int clusterIdx = labels.at<int>(i);
               cv::Point ipt = points.at<cv::Point2f>(i);
               cv::circle( img, ipt, 2, colorTab[clusterIdx], cv::FILLED, cv::LINE_AA );
           }

           centers = centers.reshape(2);

           for (i = 0; i < centers.rows; ++i)
           {
               cv::Point2f c = centers.at<cv::Point2f>(i);
               cv::circle( img, c, 40, colorTab[i], 1, cv::LINE_AA );
           }

           std::cout << "Compactness: " << compactness << std::endl;

           cv::imshow("clusters", img);

           char key = (char)cv::waitKey();
           if( key == 27 || key == 'q' || key == 'Q' ) // 'ESC'
               break;
       }

}


#endif // TEST_CLUSTERING_CPP
