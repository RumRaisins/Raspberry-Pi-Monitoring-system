#!/bin/bash






for i in `cat ./hostname`; do
	echo zy@$i:/home/zy/Project/temp/
	scp -r ./ zy@$i:/home/zy/Project/temp/
	echo 873125305 > 0
done
