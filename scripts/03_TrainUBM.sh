#!/bin/bash

# 1. UBM training
	echo "Train Universal Background Model by EM algorithm"
	bin/TrainWorld --config cfg/TrainWorld.cfg &> log/TrainWorld.log
	echo "		done, see log/TrainWorld.log for details"