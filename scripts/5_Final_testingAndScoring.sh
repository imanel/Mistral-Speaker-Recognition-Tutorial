#!/bin/bash

# 5. Compute test for score normalization (z-norm)
	echo "Compute Z-norm scores"
	bin/ComputeTest --config cfg/ComputeTestZNorm.cfg &> log/ComputeTest_znorm.log
	echo "		done, see log/ComputeTest_znorm.log"

# 6. Compute test for score normalization (t-norm)
	echo "Compute T-norm scores"
	bin/ComputeTest --config cfg/ComputeTestTNorm.cfg &> log/ComputeTest_tnorm.log
	echo "		done, see log/ComputeTest_tnorm.log"

# 7. Compute test for scores normalization (zt-norm)
#	echo "Compute ZT-norm scores"
#	bin/ComputeTest --config cfg/ComputeTestZTnorm.cfg &> log/ComputeTestztnorm.log
#	echo "		done, see log/ComputeTest_ztnorm.log"

# 8. Score Normalization: Z-norm
#	echo "Apply Z-norm"
#	bin/ComputeNorm --config cfg/ComputeNorm_znorm.cfg &> log/ComputeNorm_znorm.log
#	echo "		done, see log/ComputeNorm_znorm.log"

# 9. Score Normalization: T-norm
#	echo "Apply T-norm"
#	bin/ComputeNorm --config cfg/ComputeNorm_tnorm.cfg &> log/ComputeNorm_tnorm.log
#	echo "		done, see log/ComputeNorm_tnorm.log"

# Trials:
	bin/ComputeNorm --tnormNistFile res/target-seg_gmm.res --testNistFile res/imp-seg_gmm.res --outputFileBaseName swallow --normType tnorm
	echo "		done, see outputfile"


# 10. Score Normalization: ZT-norm
#
# BEWARE: ZT-norm means t-norm is applied first, then Z-norm follows
#	note that in this configuration, scores after t-norm are also provided as output
#	in a symetric manner, TZ-norm return scores after Z-norm and TZ-norm
#
#	echo "Apply ZT-norm"
#	bin/ComputeNorm --config cfg/ComputeNorm_ztnorm.cfg &> log/ComputeNorm_ztnorm.log
#	echo "		done, see log/ComputeNorm_ztnorm.log"

	
	
	
	
	
	
	
	
	
	
