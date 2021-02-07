#!/usr/bin/env bash
set -e

cd /tmp
wget https://github.com/RobotLocomotion/drake/releases/download/v0.26.0/drake-20210121-bionic.tar.gz
sudo tar -xvzf drake-20210121-bionic.tar.gz -C /opt
