#!/bin/bash

# 2. Speaker GMM model adaptation
	echo "Train Speaker dependent GMMs"
	bin/TrainTarget --config cfg/TrainTarget.cfg &> log/TrainTarget.cfg
	echo "		done, see log/TrainTarget.cfg for details"

# 3. Speaker model comparison
	echo "Compute Likelihood"
	bin/ComputeTest --config cfg/ComputeTest_GMM.cfg &> log/ComputeTest.cfg
	echo "		done, see log/ComputeTest.cfg"
