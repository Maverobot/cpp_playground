#!/usr/bin/env bash
set -e

tarball_name="drake-20220822-$(lsb_release -cs).tar.gz"
cd /tmp && wget https://github.com/RobotLocomotion/drake/releases/download/v1.7.0/${tarball_name}
sudo tar -xvzf ${tarball_name} -C /opt
