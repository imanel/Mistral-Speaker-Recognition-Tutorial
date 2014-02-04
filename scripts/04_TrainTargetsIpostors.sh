#!/bin/bash

# 4. Impostor GMM model adaptation for score normalization (t-norm)

	./scripts/01_feature_extraction_trainimpostor.sh
	./scripts/02_RUN_spro_front-end.sh trainImpostorData

	echo "Train Impostor dependent GMMs for T-norm"
	bin/TrainTarget --config cfg/TrainImp.cfg &> log/TrainImp.log
	echo "		done, see log/ComputeTest_tnorm.log"