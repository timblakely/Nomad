/*
 * NomadRobot.cpp
 *
 *  Created on: June 23, 2020
 *      Author: Quincy Jones
 *
 * Copyright (c) <2020> <Quincy Jones - quincy@implementedrobotics.com/>
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the Software
 * is furnished to do so, subject to the following conditions:
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

// C System Files

// C++ System Files

// Third Party Includes
#include <dart/dynamics/Skeleton.hpp>
#include <dart/utils/urdf/DartLoader.hpp>
#include <dart/dynamics/DegreeOfFreedom.hpp>
#include <dart/dynamics/FreeJoint.hpp>
#include <dart/gui/osg/osg.hpp>

// Project Include Files
#include "../include/NomadRobot.h"

NomadRobot::NomadRobot(const dart::simulation::WorldPtr world)
    : world_(world)
{
    // Reset
    Reset();
}

void NomadRobot::ProcessInputs()
{
}

void NomadRobot::Run(double dt)
{
}

void NomadRobot::SendOutputs()
{
}

void NomadRobot::UpdateState()
{
}
void NomadRobot::Reset()
{
}
Eigen::Quaterniond NomadRobot::GetBodyOrientation() const
{
    dart::dynamics::BodyNodePtr base_link = robot_->getBodyNode("base_link");

    Eigen::Quaterniond body_orientation;
    body_orientation = base_link->getWorldTransform().rotation();
    return body_orientation;
}
Eigen::Vector3d NomadRobot::GetAngularAcceleration() const
{
    dart::dynamics::BodyNodePtr base_link = robot_->getBodyNode("base_link");
    return base_link->getAngularAcceleration(dart::dynamics::Frame::World(), base_link);
}
Eigen::Vector3d NomadRobot::GetLinearAcceleration() const
{
    dart::dynamics::BodyNodePtr base_link = robot_->getBodyNode("base_link");
    return base_link->getLinearAcceleration(dart::dynamics::Frame::World(), base_link);
}

Eigen::Vector3d NomadRobot::GetAngularVelocity() const
{
    dart::dynamics::BodyNodePtr base_link = robot_->getBodyNode("base_link");
    return base_link->getAngularVelocity(dart::dynamics::Frame::World(), base_link);
}

void NomadRobot::LoadFromURDF(const std::string &urdf)
{
    dart::utils::DartLoader loader;
    robot_ = loader.parseSkeleton(urdf);

    // Rename the floating base dofs
    robot_->getDof(0)->setName("theta_x");
    robot_->getDof(1)->setName("theta_y");
    robot_->getDof(2)->setName("theta_z");
    robot_->getDof(3)->setName("base_x");
    robot_->getDof(4)->setName("base_y");
    robot_->getDof(5)->setName("base_z");

    // Set position limits enforcement
    robot_->getJoint("j_kfe_FL")->setPositionLimitEnforced(true);
    robot_->getJoint("j_kfe_FR")->setPositionLimitEnforced(true);
    robot_->getJoint("j_kfe_RL")->setPositionLimitEnforced(true);
    robot_->getJoint("j_kfe_RR")->setPositionLimitEnforced(true);

    //  int i = 0;
    //  for (auto dof : robot_->getDofs())
    //  {
    //      std::cout << "DOF: " << i++ << " : " << dof->getName() << std::endl;
    //  }

    // i = 0;
    // for(auto dof:robot_->getBodyNodes())
    // {
    //     std::cout << "DOF: " << i++ << " : " << dof->getName() << std::endl;
    // }

    // Add to world
    world_->addSkeleton(robot_);

    //std::cout << "Mass Matrix: " << robot_->getMassMatrix() << std::endl;
    //std::cout << "Mass: " << robot_->getMass() << std::endl;
}

void NomadRobot::SetInitialPose()
{

        Eigen::Matrix3d R2;
        R2 = Eigen::AngleAxisd(0, Eigen::Vector3d::UnitZ()) *
                   Eigen::AngleAxisd(M_PI_2, Eigen::Vector3d::UnitY()) *
                   Eigen::AngleAxisd(M_PI_2, Eigen::Vector3d::UnitX());

    Eigen::Isometry3d tf;
    tf.linear() = R2;//Eigen::Matrix3d::Identity();
    Eigen::VectorXd pos = dart::dynamics::FreeJoint::convertToPositions(tf);
    robot_->getRootJoint()->setPositions(pos);
    //robot_->getDof("theta_y")->setPosition(M_PI_2);
    //robot_->getDof("theta_x")->setPosition(M_PI_2);
     //robot_->getDof("omega_y")->setPosition(-.5);
    //robot_->getDof("base_x")->setPosition(0.85);
    robot_->getDof("base_z")->setPosition(0.75);
    // robot_->getDof("j_hfe_FL")->setPosition(-M_PI_2);
    // robot_->getDof("j_hfe_FR")->setPosition(M_PI_2);
    // robot_->getDof("j_hfe_RL")->setPosition(-M_PI_2);
    // robot_->getDof("j_hfe_RR")->setPosition(M_PI_2);

    robot_->getDof("j_hfe_FL")->setPosition(-0);
    robot_->getDof("j_hfe_FR")->setPosition(0);
    robot_->getDof("j_hfe_RL")->setPosition(-0);
    robot_->getDof("j_hfe_RR")->setPosition(0);



    robot_->getDof("j_kfe_FL")->setPositionLimits(-2.2, 0.0);
    robot_->getDof("j_kfe_FR")->setPositionLimits(0.0, 2.2);
    robot_->getDof("j_kfe_RL")->setPositionLimits(-2.2, 0.0);
    robot_->getDof("j_kfe_RR")->setPositionLimits(0.0, 2.2);

    robot_->getDof("j_kfe_RL")->setPosition(-2.2);
    robot_->getDof("j_kfe_FL")->setPosition(-2.2);
    robot_->getDof("j_kfe_RR")->setPosition(2.2);
    robot_->getDof("j_kfe_FR")->setPosition(2.2);
}
