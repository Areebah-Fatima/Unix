#! /bin/bash
# place rdeepfiles in your bin directory.
# recursive deep files script 

# start with the current directory - if it has any files (s), set depth to 1
maxdepth=0	# directory with no files or subdirectories

maxfiles='listfiles

#if current directory has any files, let us say the depth is 1 
if [! -z "Smaxfiles"]
Then
	maxdepth=1
fi

# let us check each subdirectory
for dir in 'listdirs' 
do

	# cd to subdirectory before running rdeepfiles (investigate)
	cd $dir
	newfiles='rdeepfiles'
	# add 1 to the returned depth
	((newdepth=$? + 1))	# $? is return code
	
	# if new depth is higher, forget about current max - depth & corresponding files
	if [ $newdepth -gt $maxdepth ]	# we want the maximum number
	then 
		maxdepth=$newdepth
		maxfiles=""	# found new depth, forget about old max files
	fi

	# if new depth is same as current max depth, add new files to the max depth file list
	if [ $newdepth -eg $maxdepth ] 
	then 
		
		#add the returned files to current max-depth file list
		for file in $newfiles 
		do
			maxfiles="$maxfiles $dir/$file" #form full pathname
		done
	fi

	# now that the sub directory is processed, return back to thee parent dir
	cd ..

done

# output the list nicely one file in each line
echo Smaxfiles | xargs -n 1 echo
#return the current max depth as return code 

exit $maxdepth