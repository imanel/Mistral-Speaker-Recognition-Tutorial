#!/bin/bash

FEATURE_TYPE="SPro"		# can be SPro or HTK
INPUT_FORMAT="SPH"		# can be SPH or PCM


# If SPro has not been linked to the SPHERE library, convert first the SPHERE files into raw PCM files without header
if [ $INPUT_FORMAT = "PCM" ]; then

	for i in `cat trainWorldData/data.lst`; do

		CMD_DECODE="bin/w_decode -o pcm trainWorldData/sph/$i.sph trainWorldData/pcm/$i.sph"
		echo $CMD_DECODE
		$CMD_DECODE

		CMD_CONVERT="bin/h_strip trainWorldData/pcm/$i.sph trainWorldData/pcm/$i.pcm"
		echo $CMD_CONVERT
		$CMD_CONVERT
	done

	# Remove the temporary SPHERE files
	echo "Remove temporary SPHERE files"
	rm trainWorldData/pcm/*.sph

	# Extract MFCC features with SPro
	for i in `cat trainWorldData/data.lst`;do
        	COMMAND_LINE="bin/sfbcep -m -k 0.97 -p19 -n 24 -r 22 -e -D -A -F PCM16  trainWorldData/pcm/$i.pcm data/prm/$i.tmp.prm"
                echo $COMMAND_LINE
                $COMMAND_LINE
   	done

fi


if [ $INPUT_FORMAT = "SPH" ]; then


	if [ $FEATURE_TYPE = "SPro" ]; then

		# Extract a list of files
		for i in `cat trainWorldData/data.lst`;do
	                COMMAND_LINE="bin/sfbcep -m -k 0.97 -p19 -n 24 -r 22 -e -D -A -F SPHERE  trainWorldData/sph/$i.sph data/prm/$i.tmp.prm"
	 		echo $COMMAND_LINE
	      		$COMMAND_LINE
		done

	else	# Extract features using HTK
	
		# Extract a list of files
		COMMAND_LINE="bin/HCopy -C cfg/hcopy_sph.cfg -T 1 -S trainWorldData/data_htk.scp"
		echo $COMMAND_LINE
		$COMMAND_LINE
	fi
fi

